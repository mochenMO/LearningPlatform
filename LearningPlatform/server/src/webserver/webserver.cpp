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

	m_maxKeepTime = 15;   // ��󱣳����ӵ�ʱ��Ϊ15s
}

WebServer::~WebServer()
{
	///////////////////////////////////////////////

	m_checkThread.join();
	m_acceptThread.join();

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
			acceptSocket.setNonBlocking();            // ���� acceptsocket Ϊ������
			addAcceptSocket(std::move(acceptSocket));
		}
	}
}


void WebServer::dealData_taskFuntion(AcceptSocket& _acceptSocket, std::string _value)
{
	int len = _value.size();
	char buffer[4094] = { 0 };
	if (len == 4094) {    // ���ڻ������Ĵ�С��˵���������ݣ���ѭ������
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
			if (len < 4094) {   // �����Ѷ�ȡ��
				break;
			}
		}
	}

	std::cout << _value << std::endl;       // @!#@!#@!#@!#@!#@!#@!#@$#!



	http::HttpParser httpParser(_value);
	http::HttpRequest httpRequest = httpParser.parseHttpRequest();
	std::cout << httpRequest.headerToString() << std::endl;    // @!#@!#@!#@!#@!#@!#@!#@$#!


	 
	// �ж��ǲ��Ǵ�����Ǿ�̬���ļ����������Ĭ�ϴ������������ͨ�� routeTree �ַ����û�д�ĺ�˴��봦��
	// ....


	InterlockedExchange((LONG*)&_acceptSocket.getState(), (LONG)AcceptSocket::State::waiting);  // ����Ϊ�ȴ�״̬
	InterlockedExchange64((LONG64*)&_acceptSocket.getWaitingTime(), (LONG64)(0));               // �ȴ�ʱ����Ϊ0

}


void func(int a) {  // ��ȷ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ
	std::cout << a << std::endl;
}




void WebServer::checkClientState_threadFuntion()
{

	// m_threadPool.addTask(&func, 12);  // ��ȷ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ ��ȷ
	//m_threadPool.addTask(&WebServer::acceptConnection_threadFuntion, this);

	int len;
	char buffer[4094] = { 0 };              // ÿ�ν��յĻ�������С��Ϊ4kb
	AcceptSocket* tempSocket;               // ����ȡֵ
	AcceptSocketList* temp = nullptr;       // ���ڱ��� AcceptSocketList
	AcceptSocketList* del = nullptr;        // ����ɾ���ڵ�

	LARGE_INTEGER frequency;                // �������Ƶ��
	LARGE_INTEGER startCount;               // ��¼��ʼʱ�����д���
	LARGE_INTEGER endCount;                 // ��¼����ʱ�����д���
	double time;                            // ����ʱ�䣬��λs��

	QueryPerformanceFrequency(&frequency);  // ��ȡ�������Ƶ��

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
					printf("�ɹ����ӣ�����\n");  // access.log������־��¼ #$@#!$!@###########!@$#$#@!$#!$!@#$#$@#!$!@###########!@$#$#@!$#!$!@#$

					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::working); // ����Ϊ����״̬

					

					m_threadPool.addTask(&WebServer::dealData_taskFuntion, std::ref(*tempSocket), std::string(buffer));   // ע�⣺*tempSocket��ֵ����Ҫ�� std::ref ת����������

					memset(buffer, 0, sizeof(buffer));
				}
				else if (len == 0                                                     // len=0 ��ʾ�ͻ����ѹر�����
					|| (len == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK))  // ���Ƿ�����ģʽ�µĴ���
				{
					InterlockedExchange((LONG*)&tempSocket->getState(), (LONG)AcceptSocket::State::terminated);
				}
			}
			// ��ʱ
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

			// ɾ�� (ͷ�巨��ôɾ�� ���������������������� )
			//if (tempSocket->getState() == AcceptSocket::State::terminated) {
			//	del = temp->next;
			//	InterlockedExchangePointer((PVOID*)temp->next, temp->next->next);
			//	delete(del);    // ���Զ����� AcceptSocket::~AcceptSocket();
			//}

			temp = temp->next;
		}
		

	}

}
