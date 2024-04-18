/*// 介绍与说明
* 1.该网络模型，用事件和异步的方式处理请求，目前处理请求的线程只有一条。后续可能实现一个处理长连接的，专门发送数据流的，线程或者线程池。
* 2.该网络模型，规定一个http报文只能发一个文件，如果需要一个http报文发多个文件，请使用压缩包。
* 3.该网络模型，规定静态路由和动态路由是分开工作的，即静态路由专注于传输静态文件，而动态路由则是逻辑处理。（动态路由也可以传文件）
*   静态路由和动态路由的路由地址不能一样，避免引起路由冲突。（这里规定处了实现路由守卫外，其他动态路由命名规范，即名称中要带有"src"）
*	
*/



/*// 问题



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
	std::thread             m_acceptThread;
	std::thread             m_checkThread;
	AcceptSocketList       *m_acceptSocketList;
	AcceptSocketList       *m_ptrWrite;
	LONGLONG                m_maxKeepTime;  // 保存计算机的运行次数
	threadpool::ThreadPool  m_threadPool;
	route::Route           *m_route;        // 来自staic全局变量，不用delete
	sql::SQLServer         *m_sqlServer;    // sql::SQLServer很大所以要存指针，节约栈空间
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











