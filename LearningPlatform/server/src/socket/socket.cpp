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
		closesocket(m_socketFd);   // closesocket 函数会把 socket 赋值为 INVALID_SOCKET
	}
}

SOCKET& Socket::getSocketFd()       // 注意返回的在引用类型
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
	int res = ioctlsocket(  // 对 socket 执行各种控制操作。成功则返回0。
		m_socketFd,         // socket 操作对象 
		FIONBIO,            // 要执行的命令（FIONBIO 用于设置socket的IO模式）
		&mode);             // 指向命令所需参数的指针。（0是阻塞模式，非0是非阻塞模式）
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void Socket::setReuseAddr()
{
	/*// 设置 SO_REUSEADDR 
	* 1.实现端口重用，在快速关闭服务器之后，可以立即重新启动该服务器，并绑定到相同的端口上。（通常服务器的监听socket都应该打开它）
	* 2.资料：https://zhuanlan.zhihu.com/p/79999012
	*/
	int optval = 1;
	int res = setsockopt(     // 设置 socket 的相关选项。成功则返回0。
		m_socketFd,           // socket 操作对象 
		SOL_SOCKET,           // 所属的协议层（SOL_SOCKET是通用选项）
		SO_REUSEADDR,            // 选项名称
		(const char*)&optval,   // 指向存有选项值的缓冲区的指针
		sizeof(optval));        // 缓冲区的大小
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void Socket::setLinger()
{
	/*// 设置 SO_LINGER (该设置的对于处理小量请求，效果可能并不明显)
	* 1.通常作用于发送缓冲区，用于设置 closesocket 延迟时间，尽可能的发送缓冲区中的残留的数据，提高传输的可靠性。如果缓冲区中已无数据则正常关闭，否则错误码为 WSAEWOULDBLOCK
	* 2.避免 TIME_WAIT 状态的积累和半开状态的发生，从而提高服务端的性能。
	* 3.网页端的连接以短连接为主，通常不需要设置 SO_LINGER。SO_LINGER 对应长连接和高可靠性的连接。
	* 4.资料：https://zhuanlan.zhihu.com/p/612439735 
	*/
	linger lingerOpt;
	lingerOpt.l_onoff = 1;    // 启用 SO_LINGER
	lingerOpt.l_linger = 3;   // 设置延迟关闭时间为3秒 (通常延迟时间设置在5秒以内)

	int res = setsockopt(m_socketFd, SOL_SOCKET, SO_LINGER, (char*)&lingerOpt, sizeof(lingerOpt));
	if (res != 0) {
		printf("setsockopt SO_LINGER failed: %ld\n", WSAGetLastError());
	}
}

void  Socket::setKeepalive()
{
	/*// 设置 SO_KEEPALIVE
	* 1.操作系统会周期性地检查处于空闲状态的连接，以确保连接仍然保持活动状态。如果检测到连接失活，则会采取相应的措施，例如关闭连接或发送探测报文，以清理资源并通知对端连接已断开。
	* 2.这个选项通常在长时间空闲的连接上使用，以确保连接的健壮性和稳定性。它可以用于检测客户端或服务器意外断开连接的情况，而不必依赖于应用层的心跳机制。
	* 3.BS架构由于每次请求都是独立的，通常不会使用 SO_KEEPALIVE 心跳机制。而是用计时器。
	* 4.资料：https://zhuanlan.zhihu.com/p/79957519
	*/
	int optval = 1;
	int res = setsockopt(     // 设置 socket 的相关选项。成功则返回0。
		m_socketFd,           // socket 操作对象 
		SOL_SOCKET,           // 所属的协议层（SOL_SOCKET是通用选项）
		SO_KEEPALIVE,            // 选项名称
		(const char*)&optval,   // 指向存有选项值的缓冲区的指针
		sizeof(optval));        // 缓冲区的大小
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void Socket::setRecvBuffer(int _size)
{
	int size = _size;
	int res = setsockopt(     // 设置 socket 的相关选项。成功则返回0。
		m_socketFd,           // socket 操作对象 
		SOL_SOCKET,           // 所属的协议层（SOL_SOCKET是通用选项）
		SO_RCVBUF,            // 选项名称
		(const char*)&size,   // 指向存有选项值的缓冲区的指针
		sizeof(size));        // 缓冲区的大小
	if (res != 0) {
		printf("setNonBlocking() failed. Error codes=%d\n", WSAGetLastError());   // @!#!#@!$#@$#@%%#@$$$$$$$$$$$$$$$$$$$$$!@#!@#@!#!@#@!#@!#
	}
}

void Socket::setSendBuffer(int _size)
{

	int size = _size;
	int res = setsockopt(     // 设置 socket 的相关选项。成功则返回0。
		m_socketFd,           // socket 操作对象 
		SOL_SOCKET,           // 所属的协议层（SOL_SOCKET是通用选项）
		SO_SNDBUF,            // 选项名称
		(const char*)&size,   // 指向存有选项值的缓冲区的指针
		sizeof(size));        // 缓冲区的大小
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


