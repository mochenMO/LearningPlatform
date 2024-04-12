/*// 问题
1.AcceptSocketList 应该用尾插法而不是头插法


*/



/*// 配置文件信息
1.ip
2.port
3.maxKeepTime
4.static静态文件的路径


*/



#pragma once
#ifndef _MOCHEN_WEBSERVER_H_
#define _MOCHEN_WEBSERVER_H_

#include "../socket/socket.h"
#include "../threadpool/threadpool.h"
#include "../time/time.h"
#include "../../../web/config/routeconfig.h"


#include <iostream>       // @!#!@$#@@@@@@@@$!@#$@!#!@#!@%$%!%!#$%#!$%#$!%@#!%$!@#%#$%!#$%#$!



namespace mochen
{

namespace webserver
{


class AcceptSocket : public socket::Socket
{
public:
	enum class State : int    // 指定 enum 中元素的类型，便于原子操作
	{
		waiting = 0,
		working,
		terminated
	};
private:
	LONGLONG m_waitingTime;    // 保存计算机的运行次数
	State m_state;
public:
	AcceptSocket();
	~AcceptSocket() = default;

	AcceptSocket(const AcceptSocket&) = delete;
	AcceptSocket(AcceptSocket&& _value) noexcept;

	void operator=(const AcceptSocket&) = delete;
	void operator=(AcceptSocket&& _value) noexcept;

	LONGLONG& getWaitingTime();
	State& getState();
};


class WebServer
{
private:
	struct AcceptSocketList
	{
		AcceptSocket m_accpetSocket;
		AcceptSocketList* next;
	};
private:
	socket::WinSockLibrary *m_winSockLibrary;
	socket::ServerSocket    m_serverSocket;
	std::thread			    m_acceptThread;
	std::thread			    m_checkThread;
	AcceptSocketList       *m_acceptSocketList;
	AcceptSocketList       *m_ptrWrite;
	LONGLONG                m_maxKeepTime;  // 保存计算机的运行次数
	threadpool::ThreadPool  m_threadPool;
	route::Route           *m_route;        // 来自staic全局变量，不用delete
	sql::SQLServer         *m_SQLServer;    // sql::SQLServer很大所以要存指针，节约栈空间
	session::Session        m_session;
public:
	WebServer(const std::string& _ip, USHORT _port, int _af = AF_INET, int _type = SOCK_STREAM, int _protocol = IPPROTO_TCP);
	~WebServer();

	WebServer(const WebServer&) = delete;
	WebServer(WebServer&& _value) noexcept = delete;

	const WebServer& operator=(const WebServer&) = delete;
	const WebServer& operator=(WebServer&& _value) noexcept = delete;
	
	void createSQLServer(const std::string& _ODBCName, const std::string& _userName, const std::string& _password);
	void startup();

	LONGLONG& getMaxKeepTime();

	void addAcceptSocket(AcceptSocket&& _value);
	void acceptConnection_threadFuntion();

	void dealData_taskFuntion(AcceptSocket& _acceptSocket, std::string _value);  // 注意两个参数的类型，AcceptSocket& 和 std::string
	void checkClientState_threadFuntion();
};













};

};




#endif // !_MOCHEN_WEBSERVER_H_











