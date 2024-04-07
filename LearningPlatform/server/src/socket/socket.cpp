#include "socket.h"


using namespace mochen::socket;


// ========================================================================================================
// class WinSockLibrary

WinSockLibrary::WinSockLibrary()
{
	int res = WSAStartup(MAKEWORD(2, 2), &m_wsadata);
	if (res != 0) {
		printf("WinSockLibrary() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

WinSockLibrary::~WinSockLibrary()
{
	WSACleanup();
}


// ========================================================================================================
// class Socket

Socket::Socket(const std::string& _ip, USHORT _port, int _af, int _type, int _protocol)
{
	m_socketFd = ::socket(_af, _type, _protocol);
	if (m_socketFd == INVALID_SOCKET) {
		printf("socket() failed\n");   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
		return;
	}

	m_sockaddr.sin_family = _af;
	m_sockaddr.sin_port = htons(_port);
	int res = inet_pton(_af, _ip.c_str(), &m_sockaddr.sin_addr.s_addr);
	if (res != 1) {
		printf("inet_pton() failed\n");   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}

	m_addrlen = sizeof(m_sockaddr);
}


Socket::~Socket()
{
	clear();
}


Socket::Socket(Socket&& _value) noexcept
{
	m_socketFd = _value.m_socketFd;
	m_sockaddr = _value.m_sockaddr;
	m_addrlen = _value.m_addrlen;
	_value.m_socketFd = INVALID_SOCKET;
}

void Socket::operator=(Socket&& _value) noexcept
{
	clear();
	m_socketFd = _value.m_socketFd;
	m_sockaddr = _value.m_sockaddr;
	m_addrlen = _value.m_addrlen;
	_value.m_socketFd = INVALID_SOCKET;
}


void Socket::clear() 
{
	if (m_socketFd != INVALID_SOCKET) {
		closesocket(m_socketFd);   // closesocket ������� socket ��ֵΪ INVALID_SOCKET
	}
}

SOCKET& Socket::getSocketFd()       // ע�ⷵ�ص�����������
{
	return m_socketFd;
}

sockaddr_in& Socket::getSockaddr()
{
	return m_sockaddr;
}

int& Socket::getAddrlen()
{
	return m_addrlen;
}

int Socket::recv(char* _buffer, int _bufferSize)
{
	return ::recv(m_socketFd, _buffer, _bufferSize, 0);
}

int Socket::send(char* _buffer, int _bufferSize)
{
	return ::send(m_socketFd, _buffer, _bufferSize, 0);
}


void Socket::setNonBlocking()
{
	u_long mode = 1;
	int res = ioctlsocket(  // �� socket ִ�и��ֿ��Ʋ������ɹ��򷵻�0��
		m_socketFd,         // socket �������� 
		FIONBIO,            // Ҫִ�е����FIONBIO ��������socket��IOģʽ��
		&mode);             // ָ���������������ָ�롣��0������ģʽ����0�Ƿ�����ģʽ��
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void Socket::setReuseAddr()
{
	/*// ���� SO_REUSEADDR 
	* 1.ʵ�ֶ˿����ã��ڿ��ٹرշ�����֮�󣬿����������������÷����������󶨵���ͬ�Ķ˿��ϡ���ͨ���������ļ���socket��Ӧ�ô�����
	* 2.���ϣ�https://zhuanlan.zhihu.com/p/79999012
	*/
	int optval = 1;
	int res = setsockopt(     // ���� socket �����ѡ��ɹ��򷵻�0��
		m_socketFd,           // socket �������� 
		SOL_SOCKET,           // ������Э��㣨SOL_SOCKET��ͨ��ѡ�
		SO_REUSEADDR,            // ѡ������
		(const char*)&optval,   // ָ�����ѡ��ֵ�Ļ�������ָ��
		sizeof(optval));        // �������Ĵ�С
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void Socket::setLinger()
{
	/*// ���� SO_LINGER (�����õĶ��ڴ���С������Ч�����ܲ�������)
	* 1.ͨ�������ڷ��ͻ��������������� closesocket �ӳ�ʱ�䣬�����ܵķ��ͻ������еĲ��������ݣ���ߴ���Ŀɿ��ԡ���������������������������رգ����������Ϊ WSAEWOULDBLOCK
	* 2.���� TIME_WAIT ״̬�Ļ��ۺͰ뿪״̬�ķ������Ӷ���߷���˵����ܡ�
	* 3.��ҳ�˵������Զ�����Ϊ����ͨ������Ҫ���� SO_LINGER��SO_LINGER ��Ӧ�����Ӻ͸߿ɿ��Ե����ӡ�
	* 4.���ϣ�https://zhuanlan.zhihu.com/p/612439735 
	*/
	linger lingerOpt;
	lingerOpt.l_onoff = 1;    // ���� SO_LINGER
	lingerOpt.l_linger = 3;   // �����ӳٹر�ʱ��Ϊ3�� (ͨ���ӳ�ʱ��������5������)

	int res = setsockopt(m_socketFd, SOL_SOCKET, SO_LINGER, (char*)&lingerOpt, sizeof(lingerOpt));
	if (res != 0) {
		printf("setsockopt SO_LINGER failed: %ld\n", WSAGetLastError());
	}
}

void  Socket::setKeepalive()
{
	/*// ���� SO_KEEPALIVE
	* 1.����ϵͳ�������Եؼ�鴦�ڿ���״̬�����ӣ���ȷ��������Ȼ���ֻ״̬�������⵽����ʧ�����ȡ��Ӧ�Ĵ�ʩ������ر����ӻ���̽�ⱨ�ģ���������Դ��֪ͨ�Զ������ѶϿ���
	* 2.���ѡ��ͨ���ڳ�ʱ����е�������ʹ�ã���ȷ�����ӵĽ�׳�Ժ��ȶ��ԡ����������ڼ��ͻ��˻����������Ͽ����ӵ������������������Ӧ�ò���������ơ�
	* 3.BS�ܹ�����ÿ�������Ƕ����ģ�ͨ������ʹ�� SO_KEEPALIVE �������ơ������ü�ʱ����
	* 4.���ϣ�https://zhuanlan.zhihu.com/p/79957519
	*/
	int optval = 1;
	int res = setsockopt(     // ���� socket �����ѡ��ɹ��򷵻�0��
		m_socketFd,           // socket �������� 
		SOL_SOCKET,           // ������Э��㣨SOL_SOCKET��ͨ��ѡ�
		SO_KEEPALIVE,            // ѡ������
		(const char*)&optval,   // ָ�����ѡ��ֵ�Ļ�������ָ��
		sizeof(optval));        // �������Ĵ�С
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void Socket::setRecvBuffer(int _size)
{
	int size = _size;
	int res = setsockopt(     // ���� socket �����ѡ��ɹ��򷵻�0��
		m_socketFd,           // socket �������� 
		SOL_SOCKET,           // ������Э��㣨SOL_SOCKET��ͨ��ѡ�
		SO_RCVBUF,            // ѡ������
		(const char*)&size,   // ָ�����ѡ��ֵ�Ļ�������ָ��
		sizeof(size));        // �������Ĵ�С
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void Socket::setSendBuffer(int _size)
{

	int size = _size;
	int res = setsockopt(     // ���� socket �����ѡ��ɹ��򷵻�0��
		m_socketFd,           // socket �������� 
		SOL_SOCKET,           // ������Э��㣨SOL_SOCKET��ͨ��ѡ�
		SO_SNDBUF,            // ѡ������
		(const char*)&size,   // ָ�����ѡ��ֵ�Ļ�������ָ��
		sizeof(size));        // �������Ĵ�С
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}


// ========================================================================================================
// class ServerSocket


ServerSocket::ServerSocket(const std::string& _ip, USHORT _port, int _af, int _type, int _protocol) : Socket(_ip, _port, _af, _type, _protocol)
{

}

ServerSocket::~ServerSocket()
{
	clear();
}

void ServerSocket::bind() 
{
	int res = ::bind(m_socketFd, (struct sockaddr*)&m_sockaddr, m_addrlen);
	if (res != 0) {
		printf("bind() failed\n");   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void ServerSocket::listen(int _backlog)
{
	int res = ::listen(m_socketFd, _backlog);
	if (res != 0) {
		printf("bind() failed\n");   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

SOCKET ServerSocket::accept(struct sockaddr* _sockaddr, int* _addrLen) 
{
	return ::accept(m_socketFd, (struct sockaddr*)&m_sockaddr, &m_addrlen);
}



// ========================================================================================================
// class ClinetSocket

ClinetSocket::ClinetSocket(const std::string& _ip, USHORT _port, int _af, int _type, int _protocol) : Socket(_ip, _port, _af, _type, _protocol)
{

}

ClinetSocket::~ClinetSocket()
{
	clear();
}

void ClinetSocket::connect()
{
	int res = ::connect(m_socketFd, (struct sockaddr*)&m_sockaddr, m_addrlen);
	if (res != 0) {
		printf("connect() failed\n");   // #@%$!#$%^#$!@^$%^%$@%#$%!@$#@%!$#$#$#$#$#$#$#$#$#$#$#%!#$
	}
}


