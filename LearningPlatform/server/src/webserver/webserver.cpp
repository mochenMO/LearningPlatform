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
	m_waitingTime = 0.0;
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

double& AcceptSocket::getWaitingTime()
{
	return m_waitingTime;
}

AcceptSocket::State& AcceptSocket::getState()
{
	return m_state;
}


// ==============================================================================================================
// class Client

//class WebServer
//{
//private:
//	struct ClientList
//	{
//		Client* m_client;
//		ClientList* next;
//	};
//private:
//	socket::ServerSocket m_serverSocket;
//	std::thread m_acceptThread;
//	std::thread m_checkThread;
//	ClientList* m_acceptSocketList;
//	double m_maxWaitingTime;
//public:


WebServer::WebServer(const std::string& _ip, USHORT _port, int _af, int _type, int _protocol)
	: m_winSockLibrary(getWinSockLibrary()),
	m_serverSocket(_ip, _port, _af, _type, _protocol),
	m_acceptThread(),
	m_checkThread(),
	m_threadPool()
{
	m_acceptSocketList = new AcceptSocketList();
	m_acceptSocketList->next = nullptr;

	m_maxKeepTime = 15;   // 最大保持连接的时间为15s
}

WebServer::~WebServer()
{
	///////////////////////////////////////////////

	m_checkThread.join();
	m_acceptThread.join();

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

double& WebServer::getMaxKeepTime()
{
	return m_maxKeepTime;
}


void WebServer::addAcceptSocket(AcceptSocket&& _value)
{
	AcceptSocketList* data = new AcceptSocketList();
	data->m_accpetSocket = std::move(_value);
	while(1) {
		data->next = m_acceptSocketList->next;
		if (InterlockedCompareExchangePointer((PVOID*)&m_acceptSocketList->next, data, data->next) == data->next) {
			break;
		}
	}
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
	char buffer[4094] = { 0 };
	if (len == 4094) {    // 等于缓冲区的大小，说明还有数据，则循环接收
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
			if (len < 4094) {   // 数据已读取完
				break;
			}
		}
	}

	std::cout << _value << std::endl;       // @!#@!#@!#@!#@!#@!#@!#@$#!



	http::HttpParser httpParser(_value);
	http::HttpRequest httpRequest = httpParser.parseHttpRequest();
	std::cout << httpRequest.headerToString() << std::endl;    // @!#@!#@!#@!#@!#@!#@!#@$#!


	 
	// 判断是不是处理的是静态的文件，如果是则默认处理，如果不是则通过 routeTree 分发给用户写的后端代码处理
	// ....


	InterlockedExchange((LONG*)&_acceptSocket.getState(), (LONG)AcceptSocket::State::waiting);  // 设置为等待状态
	InterlockedExchange64((LONG64*)&_acceptSocket.getWaitingTime(), (LONG64)(0));               // 等待时间设为0

}


void func(int a) {  // 正确正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确
	std::cout << a << std::endl;
}




void WebServer::checkClientState_threadFuntion()
{

	// m_threadPool.addTask(&func, 12);  // 正确正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确 正确
	//m_threadPool.addTask(&WebServer::acceptConnection_threadFuntion, this);

	int len;
	char buffer[4094] = { 0 };              // 每次接收的缓冲区大小设为4kb
	AcceptSocket* tempSocket;               // 用于取值
	AcceptSocketList* temp = nullptr;       // 用于遍历 AcceptSocketList
	AcceptSocketList* del = nullptr;        // 用于删除节点

	LARGE_INTEGER frequency;                // 计算机的频率
	LARGE_INTEGER startCount;               // 记录开始时的运行次数
	LARGE_INTEGER endCount;                 // 记录结束时的运行次数
	double time;                            // 保存时间，单位s秒

	QueryPerformanceFrequency(&frequency);  // 获取计算机的频率

	while (1)
	{
		temp = m_acceptSocketList;
		QueryPerformanceCounter(&startCount);

		while (temp->next != nullptr)
		{
			tempSocket = &temp->next->m_accpetSocket;
			if (tempSocket->getState() == AcceptSocket::State::waiting)
			{
				len = recv(tempSocket->getSocketFd(), buffer, 4094, 0);
				if (len > 0) {
					printf("成功连接！！！\n");  // access.log访问日志记录 #$@#!$!@###########!@$#$#@!$#!$!@#$#$@#!$!@###########!@$#$#@!$#!$!@#$

					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::working); // 设置为工作状态

					

					m_threadPool.addTask(&WebServer::dealData_taskFuntion, std::ref(*tempSocket), std::string(buffer));   // 注意：*tempSocket是值类型要用 std::ref 转成引用类型

					memset(buffer, 0, sizeof(buffer));
				}
				else if (len == 0                                                     // len=0 表示客户端已关闭连接
					|| (len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))  // 不是非阻塞模式下的错误
				{
					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::terminated);
				}
			}
			// 计时
			if (tempSocket->getState() == AcceptSocket::State::waiting) {
				QueryPerformanceCounter(&endCount);
				time = (endCount.QuadPart - startCount.QuadPart) / (double)frequency.QuadPart;
				time += tempSocket->getWaitingTime();
				
				if (time < m_maxKeepTime) {
					InterlockedExchange64((LONG64*)&tempSocket->getWaitingTime(), (LONG64)(time));
				}
				else {
					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::terminated);
				}
			}

			// 删除 (头插法怎么删除 ？？？？？？？？？？？ )
			//if (tempSocket->getState() == AcceptSocket::State::terminated) {
			//	del = temp->next;
			//	InterlockedExchangePointer((PVOID*)temp->next, temp->next->next);
			//	delete(del);    // 会自动调用 AcceptSocket::~AcceptSocket();
			//}

			temp = temp->next;
		}
		

	}

}
