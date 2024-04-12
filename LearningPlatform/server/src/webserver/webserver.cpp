#include "webserver.h"

using namespace mochen::webserver;

// ==============================================================================================================
// ȫ����

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
	m_session.setParamter<mochen::route::Route*>("Route", m_route);   // ע�������õ��� Route* ��Ϊ Route �ǽ�ֹ�������ƶ���
}

WebServer::~WebServer()
{
	///////////////////////////////////////////////
	// ʵ�ֹرչ���...
	
	

	m_checkThread.join();
	m_acceptThread.join();
	
	delete m_SQLServer;  // ע�⣺�����delete����Ϊ�����߳̿��ܻ�Ҫ��
}


void WebServer::createSQLServer(const std::string& _ODBCName, const std::string& _userName, const std::string& _password)
{
	m_SQLServer = new sql::SQLServer(_ODBCName, _userName, _password);
}

void WebServer::startup()
{
	m_serverSocket.setReuseAddr();    // ������socket���ѡ��
	// m_serverSocket.setNonBlocking();  // ����socketΪ������ (����accept�е����Ĵ����̣߳���˲�������Ϊ������)
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
	InterlockedExchange64(&_value.getWaitingTime(), timer.getCount());   // ������ʼʱ��

	node->m_accpetSocket = std::move(_value);
	
	// β�巨
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
			acceptSocket.setNonBlocking();            // ���� acceptsocket Ϊ������
			addAcceptSocket(std::move(acceptSocket));
		}
	}
}


void WebServer::dealData_taskFuntion(AcceptSocket& _acceptSocket, std::string _value)
{
	int len = _value.size();
	char buffer[4096] = { 0 };

	if (len == sizeof(buffer)) {    // ���ڻ������Ĵ�С��˵���������ݣ���ѭ������
		while (1) {
			int len = _acceptSocket.recv(buffer, 4094);
			if (len > 0) {
				_value.append(buffer, len);
			}
			else if (len == 0                                                     // len=0 ��ʾ�ͻ����ѹر�����
				|| (len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))  // ���Ƿ�����ģʽ�µĴ���
			{
				printf("recv()���󣡣���\n");  // error.log������־��¼ #$@#!$!@###########!@$#$#@!$#!$!@#$#$@#!$!@###########!@$#$#@!$#!$!@#$

				InterlockedExchange((LONG*)&_acceptSocket.getState(), (LONG)AcceptSocket::State::terminated);
				return;
			}
			if (len < sizeof(buffer)) {   // �����Ѷ�ȡ��
				break;
			}
		}
	}


	std::cout << _value << std::endl;   /////////////////////////////////////////////////////////////////////////


	http::HttpParser httpParser(_value);
	http::HttpRequest httpRequest = httpParser.parseHttpRequest();
	std::cout << httpRequest.headerToString() << std::endl;    /////////////////////////////////////////////////////////////////////////
	httpserver::HttpServerRequest httpServerRequest(std::move(httpRequest));
	httpServerRequest.getSession() = &m_session;      // ���� Session
	httpServerRequest.getSQLServer() = m_SQLServer;   // ���� SQLServer
	httpserver::HttpServerResqonse httpServerResqonse{};
	// httpServerResqonse.setParamter("Connection", "close"); /////////////////////////////////////////////////////////////////////////

	// ���� url �����߼�����
	std::string urlPath = httpServerRequest.getUrl().getPath();
	std::string filename = "";
	const char* fileSuffixName = nullptr;
	// ����̬�ļ�����
	if (m_route->isFindStaticFile(urlPath) == true) {          // �ж��Ƿ�������Ĭ�ϵľ�̬�ļ�
		filename += m_route->getDefaultStaticFilePath();
		filename += m_route->getStaticFile(urlPath);
	}
	else if (strchr(urlPath.c_str(), '.') != nullptr) {        // ��urlPath��������ļ���׺����˵���������������̬�ļ�
		filename += m_route->getDefaultStaticFilePath();
		filename += urlPath;	
	}
	if (filename != "") {
		fileSuffixName = strchr(filename.c_str(), '.') + 1;    // ��ȡ�ļ���׺�� +1 ����"."
		std::string contentType = httpServerResqonse.getContentType(fileSuffixName);
		if (contentType != "") {
			httpServerResqonse.setParamter("Content-Type", contentType);
		}
		else {
			printf("not find contentType logger logger\n"); /////////////////////////////////////////////////////////////////////////
		}
	}
	// ����̬����
	if (m_route->isFindFunction(urlPath) == true){
		m_route->getFunction(urlPath)(httpServerRequest, httpServerResqonse);
	}


	// ��ʼ��������
	std::string httpServerResqonseHeader = httpServerResqonse.headerToString();
	std::cout << httpServerResqonse.headerToString() << std::endl; ///////////////////////////////////////////////////////////////////////
	len = send(_acceptSocket.getSocketFd(), httpServerResqonseHeader.c_str(), httpServerResqonseHeader.size(), 0);   // �ȷ�������ͷ��
	
	if (filename != "") {
		FILE* fp = fopen(filename.c_str(), "rb");   // ע��Ҫ�Զ����Ƶ���ʽ��ȡ�ļ�
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
			printf("�ļ���ʧ�ܣ�logger\n");  // #@!#@!#@!#@!#!@#!@#!@#!@#!@#!@#
		}
	}


	// ������
	InterlockedExchange((LONG*)&_acceptSocket.getState(), (LONG)AcceptSocket::State::working);  // ����Ϊ��ֹ״̬
	_acceptSocket.clear();

	// ������
	// InterlockedExchange((LONG*)&_acceptSocket.getState(), (LONG)AcceptSocket::State::working);  // ����Ϊ�ȴ�״̬ 
	// time::Timer timer{};
	// InterlockedExchange64(&_acceptSocket.getWaitingTime(), timer.getCount());                   // ����������ʼʱ��
}


void WebServer::checkClientState_threadFuntion()
{
	int len;
	char buffer[4094] = { 0 };                        // ÿ�ν��յĻ�������С��Ϊ4kb
	AcceptSocket* tempSocket;                         // ����ȡֵ
	AcceptSocketList* ptrRead = nullptr;              // ���ڱ��� AcceptSocketList
	AcceptSocketList* deleteNode = nullptr;           // ����ɾ���ڵ�
	time::Timer timer{};

	while (1)
	{
		ptrRead = m_acceptSocketList;    // ע�� һ��ʼ ptrRead ���ܻ��� nullptr
		while (ptrRead->next != nullptr)
		{
			tempSocket = &ptrRead->next->m_accpetSocket;
			if (tempSocket->getState() == AcceptSocket::State::waiting)
			{
				len = recv(tempSocket->getSocketFd(), buffer, 4094, 0);
				if (len > 0) {
					printf("�ɹ����ӣ�����\n");  // access.log������־��¼ #$@#!$!@###########!@$#$#@!$#!$!@#$#$@#!$!@###########!@$#$#@!$#!$!@#$

					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::working); // ����Ϊ����״̬

					m_threadPool.addTask(&WebServer::dealData_taskFuntion, this,std::ref(*tempSocket), std::string(buffer));   // ע�⣺*tempSocket��ֵ����Ҫ�� std::ref ת����������

					memset(buffer, 0, sizeof(buffer));
				}
				else if (len == 0                                                     // len=0 ��ʾ�ͻ����ѹر�����
					|| (len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))  // ���Ƿ�����ģʽ�µĴ��󡣣�ͨ������ҳˢ�£�����ͬһ��socket����������������һ����socket��
				{
					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::terminated);
				}
			}

			// ��ʱ
			if (tempSocket->getState() == AcceptSocket::State::waiting) {
				if (timer.getCount() - tempSocket->getWaitingTime() > m_maxKeepTime * timer.getFrequency()) {
					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::terminated);
				}
			}

			// ɾ������ֹ�Ľڵ㣬ע�ⲻ��ɾ�� m_ptrWrite β�ڵ�
			if (tempSocket->getState() == AcceptSocket::State::terminated) {
				tempSocket->clear();
				if (ptrRead->next != m_ptrWrite) {          // ע�� ptrRead->next != m_ptrWrite ����ɾ��β�ڵ�
					deleteNode = ptrRead->next;
					ptrRead->next = ptrRead->next->next;    // ����Ҫԭ�Ӳ�������Ϊ�����Ѿ��ж��� ptrRead->next != m_ptrWrite
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


