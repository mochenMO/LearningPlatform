#pragma once
#ifndef _MOCHEN_SOCKET_H_
#define _MOCHEN_SOCKET_H_

#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#include <string>


#pragma comment(lib, "Ws2_32.lib")


namespace mochen
{

namespace socket
{


class WinSockLibrary
{
private:
	WSADATA m_wsadata;
public:
	WinSockLibrary();
	~WinSockLibrary();

	WinSockLibrary(const WinSockLibrary&) = delete;
	WinSockLibrary(WinSockLibrary&& _value) noexcept = delete;

	const WinSockLibrary& operator=(const WinSockLibrary&) = delete;
	const WinSockLibrary& operator=(WinSockLibrary&& _value) noexcept = delete;
};


class Socket
{
protected:
	SOCKET m_socketFd;
	sockaddr_in m_sockAddr;
	int m_addrLen;
public:
	Socket() = default;
	Socket(const std::string& _ip, USHORT _port, int _af, int _type, int _protocol);
	~Socket();

	Socket(const Socket&) = delete;
	Socket(Socket&& _value) noexcept;

	void operator=(const Socket&) = delete;
	void operator=(Socket&& _value) noexcept;

	void clear();

	SOCKET& getSocketFd();       // 注意返回的在引用类型
	sockaddr_in& getSockAddr();
	int& getAddrLen();
	
	int recv(char* _buffer, int _bufferSize);
	int send(const char* _buffer, int _bufferSize);

	void setNonBlocking();
	void setReuseAddr();
	void setLinger();
	void setKeepalive();

	void setRecvBuffer(int _size);
	void setSendBuffer(int _size);
};



class ServerSocket : public Socket
{
public:
	ServerSocket(const std::string& _ip, USHORT _port, int _af = AF_INET, int _type = SOCK_STREAM, int _protocol = IPPROTO_TCP);
	~ServerSocket();

	ServerSocket(const ServerSocket&) = delete;
	ServerSocket(ServerSocket&& _value) noexcept = delete;

	const ServerSocket& operator=(const ServerSocket&) = delete;
	const ServerSocket& operator=(ServerSocket&& _value) noexcept = delete;

	void bind();
	void listen(int _backlog = SOMAXCONN);
	SOCKET accept(struct sockaddr* _sockaddr = nullptr, int* _addrLen = nullptr);
};




class ClinetSocket : public Socket
{
public:
	ClinetSocket(const std::string& _ip, USHORT _port, int _af = AF_INET, int _type = SOCK_STREAM, int _protocol = IPPROTO_TCP);
	~ClinetSocket();

	ClinetSocket(const ClinetSocket&) = delete;
	ClinetSocket(ClinetSocket&& _value) noexcept = delete;

	const ClinetSocket& operator=(const ClinetSocket&) = delete;
	const ClinetSocket& operator=(ClinetSocket&& _value) noexcept = delete;

	void connect();
};








};

};






#endif // !_MOCHEN_SOCKET_H_




