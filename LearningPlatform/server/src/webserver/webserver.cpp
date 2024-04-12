#include "webserver.h"

using namespace mochen::webserver;

// ==============================================================================================================
// 全局区

mochen::socket::WinSockLibrary* getWinSockLibrary()
{
	static mochen::socket::WinSockLibrary winSockLibrary{};
	return &winSockLibrary;
}

// ==============================================================================================================
// class AcceptSocket

AcceptSocket::AcceptSocket() : Socket()
{
	m_state = State::waiting;
	m_waitingTime = 0;
}

AcceptSocket::AcceptSocket(AcceptSocket&& _value) noexcept
{
	m_socketFd = _value.m_socketFd;
	m_sockaddr = _value.m_sockaddr;
	m_addrlen = _value.m_addrlen;
	m_waitingTime = _value.m_waitingTime;
	m_state = _value.m_state;
	_value.m_socketFd = INVALID_SOCKET;
}

void AcceptSocket::operator=(AcceptSocket&& _value) noexcept
{
	clear();
	m_socketFd = _value.m_socketFd;
	m_sockaddr = _value.m_sockaddr;
	m_addrlen = _value.m_addrlen;
	m_waitingTime = _value.m_waitingTime;
	m_state = _value.m_state;
	_value.m_socketFd = INVALID_SOCKET;
}

LONGLONG& AcceptSocket::getWaitingTime()
{
	return m_waitingTime;
}

AcceptSocket::State& AcceptSocket::getState()
{
	return m_state;
}


// ==============================================================================================================
// class Client

WebServer::WebServer(const std::string& _ip, USHORT _port, int _af, int _type, int _protocol)
	: m_winSockLibrary(getWinSockLibrary()),
	m_serverSocket(_ip, _port, _af, _type, _protocol),
	m_acceptThread(),
	m_checkThread(),
	m_threadPool(),
	m_session()
{
	m_acceptSocketList = new AcceptSocketList();
	m_acceptSocketList->next = nullptr;
	m_ptrWrite = m_acceptSocketList;

	m_maxKeepTime = 15;

	m_route = mochen::page::getRoute();
	m_session.setParamter<mochen::route::Route*>("Route", m_route);   // 注意这里用的是 Route* 因为 Route 是禁止拷贝和移动的
}

WebServer::~WebServer()
{
	///////////////////////////////////////////////
	// 实现关闭功能...
	
	

	m_checkThread.join();
	m_acceptThread.join();
	
	delete m_SQLServer;  // 注意：最后再delete，因为其他线程可能还要用
}


void WebServer::createSQLServer(const std::string& _ODBCName, const std::string& _userName, const std::string& _password)
{
	m_SQLServer = new sql::SQLServer(_ODBCName, _userName, _password);
}

void WebServer::startup()
{
	m_serverSocket.setReuseAddr();    // 先设置socket相关选项
	// m_serverSocket.setNonBlocking();  // 设置socket为非阻塞 (由于accept有单独的处理线程，因此不用设置为非阻塞)
	m_serverSocket.bind();
	m_serverSocket.listen();

	m_acceptThread = std::thread(&WebServer::acceptConnection_threadFuntion, this);
	m_checkThread = std::thread(&WebServer::checkClientState_threadFuntion, this);

	m_threadPool.startup();
}

LONGLONG& WebServer::getMaxKeepTime()
{
	return m_maxKeepTime;
}


void WebServer::addAcceptSocket(AcceptSocket&& _value)
{
	AcceptSocketList* node = new AcceptSocketList();
	node->next = nullptr;
	
	time::Timer timer{};
	InterlockedExchange64(&_value.getWaitingTime(), timer.getCount());   // 设置起始时间

	node->m_accpetSocket = std::move(_value);
	
	// 尾插法
	InterlockedExchangePointer((PVOID*)&m_ptrWrite->next, node);
	InterlockedExchangePointer((PVOID*)&m_ptrWrite, node);
}

void WebServer::acceptConnection_threadFuntion()
{
	SOCKET tempSocketFd;

	while (1)
	{
		AcceptSocket acceptSocket{};
		tempSocketFd = m_serverSocket.accept();
		if (tempSocketFd == SOCKET_ERROR) {
			printf("accept() failed\n");  // $#@!%$#!%#!@%#%$@#%!#@%!$#%#!@%#!%#!%!#
			closesocket(tempSocketFd);
		}
		else {
			acceptSocket.getSocketFd() = tempSocketFd;
			acceptSocket.setNonBlocking();            // 设置 acceptsocket 为非阻塞
			addAcceptSocket(std::move(acceptSocket));
		}
	}
}


void WebServer::dealData_taskFuntion(AcceptSocket& _acceptSocket, std::string _value)
{
	int len = _value.size();
	char buffer[4096] = { 0 };

	if (len == sizeof(buffer)) {    // 等于缓冲区的大小，说明还有数据，则循环接收
		while (1) {
			int len = _acceptSocket.recv(buffer, 4094);
			if (len > 0) {
				_value.append(buffer, len);
			}
			else if (len == 0                                                     // len=0 表示客户端已关闭连接
				|| (len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))  // 不是非阻塞模式下的错误
			{
				printf("recv()错误！！！\n");  // error.log访问日志记录 #$@#!$!@###########!@$#$#@!$#!$!@#$#$@#!$!@###########!@$#$#@!$#!$!@#$

				InterlockedExchange((LONG*)&_acceptSocket.getState(), (LONG)AcceptSocket::State::terminated);
				return;
			}
			if (len < sizeof(buffer)) {   // 数据已读取完
				break;
			}
		}
	}


	std::cout << _value << std::endl;   /////////////////////////////////////////////////////////////////////////


	http::HttpParser httpParser(_value);
	http::HttpRequest httpRequest = httpParser.parseHttpRequest();
	std::cout << httpRequest.headerToString() << std::endl;    /////////////////////////////////////////////////////////////////////////
	httpserver::HttpServerRequest httpServerRequest(std::move(httpRequest));
	httpServerRequest.getSession() = &m_session;      // 传入 Session
	httpServerRequest.getSQLServer() = m_SQLServer;   // 传入 SQLServer
	httpserver::HttpServerResqonse httpServerResqonse{};
	// httpServerResqonse.setParamter("Connection", "close"); /////////////////////////////////////////////////////////////////////////

	// 根据 url 进行逻辑处理
	std::string urlPath = httpServerRequest.getUrl().getPath();
	std::string filename = "";
	const char* fileSuffixName = nullptr;
	// 处理静态文件请求
	if (m_route->isFindStaticFile(urlPath) == true) {          // 判断是否请求了默认的静态文件
		filename += m_route->getDefaultStaticFilePath();
		filename += m_route->getStaticFile(urlPath);
	}
	else if (strchr(urlPath.c_str(), '.') != nullptr) {        // 在urlPath中如果有文件后缀名，说明请求的是其他静态文件
		filename += m_route->getDefaultStaticFilePath();
		filename += urlPath;	
	}
	if (filename != "") {
		fileSuffixName = strchr(filename.c_str(), '.') + 1;    // 获取文件后缀名 +1 跳过"."
		std::string contentType = httpServerResqonse.getContentType(fileSuffixName);
		if (contentType != "") {
			httpServerResqonse.setParamter("Content-Type", contentType);
		}
		else {
			printf("not find contentType logger logger\n"); /////////////////////////////////////////////////////////////////////////
		}
	}
	// 处理动态请求
	if (m_route->isFindFunction(urlPath) == true){
		m_route->getFunction(urlPath)(httpServerRequest, httpServerResqonse);
	}


	// 开始发送数据
	std::string httpServerResqonseHeader = httpServerResqonse.headerToString();
	std::cout << httpServerResqonse.headerToString() << std::endl; ///////////////////////////////////////////////////////////////////////
	len = send(_acceptSocket.getSocketFd(), httpServerResqonseHeader.c_str(), httpServerResqonseHeader.size(), 0);   // 先发送请求头部
	
	if (filename != "") {
		FILE* fp = fopen(filename.c_str(), "rb");   // 注意要以二进制的形式读取文件
		if (fp != nullptr) {
			while (1) {
				len = fread(buffer, sizeof(char), sizeof(buffer), fp);
				len = send(_acceptSocket.getSocketFd(), buffer, len, 0);
				if (len < sizeof(buffer)) {
					break;
				}
			}
			fclose(fp);
		}
		else {
			printf("文件打开失败！logger\n");  // #@!#@!#@!#@!#!@#!@#!@#!@#!@#!@#
		}
	}


	// 短链接
	InterlockedExchange((LONG*)&_acceptSocket.getState(), (LONG)AcceptSocket::State::working);  // 设置为终止状态
	_acceptSocket.clear();

	// 长连接
	// InterlockedExchange((LONG*)&_acceptSocket.getState(), (LONG)AcceptSocket::State::working);  // 设置为等待状态 
	// time::Timer timer{};
	// InterlockedExchange64(&_acceptSocket.getWaitingTime(), timer.getCount());                   // 重新设置起始时间
}


void WebServer::checkClientState_threadFuntion()
{
	int len;
	char buffer[4094] = { 0 };                        // 每次接收的缓冲区大小设为4kb
	AcceptSocket* tempSocket;                         // 用于取值
	AcceptSocketList* ptrRead = nullptr;              // 用于遍历 AcceptSocketList
	AcceptSocketList* deleteNode = nullptr;           // 用于删除节点
	time::Timer timer{};

	while (1)
	{
		ptrRead = m_acceptSocketList;    // 注意 一开始 ptrRead 可能会变成 nullptr
		while (ptrRead->next != nullptr)
		{
			tempSocket = &ptrRead->next->m_accpetSocket;
			if (tempSocket->getState() == AcceptSocket::State::waiting)
			{
				len = recv(tempSocket->getSocketFd(), buffer, 4094, 0);
				if (len > 0) {
					printf("成功连接！！！\n");  // access.log访问日志记录 #$@#!$!@###########!@$#$#@!$#!$!@#$#$@#!$!@###########!@$#$#@!$#!$!@#$

					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::working); // 设置为工作状态

					m_threadPool.addTask(&WebServer::dealData_taskFuntion, this,std::ref(*tempSocket), std::string(buffer));   // 注意：*tempSocket是值类型要用 std::ref 转成引用类型

					memset(buffer, 0, sizeof(buffer));
				}
				else if (len == 0                                                     // len=0 表示客户端已关闭连接
					|| (len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))  // 不是非阻塞模式下的错误。（通常是网页刷新，传了同一个socket进来，导致又两个一样的socket）
				{
					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::terminated);
				}
			}

			// 计时
			if (tempSocket->getState() == AcceptSocket::State::waiting) {
				if (timer.getCount() - tempSocket->getWaitingTime() > m_maxKeepTime * timer.getFrequency()) {
					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::terminated);
				}
			}

			// 删除已终止的节点，注意不能删除 m_ptrWrite 尾节点
			if (tempSocket->getState() == AcceptSocket::State::terminated) {
				tempSocket->clear();
				if (ptrRead->next != m_ptrWrite) {          // 注意 ptrRead->next != m_ptrWrite 不能删除尾节点
					deleteNode = ptrRead->next;
					ptrRead->next = ptrRead->next->next;    // 不需要原子操作，因为上面已经判断了 ptrRead->next != m_ptrWrite
					ptrRead = ptrRead->next;
					delete(deleteNode);
				}
			}
			else {
				ptrRead = ptrRead->next;
			}
		}
	}

}


