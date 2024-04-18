/*// ������˵��
* 1.������ģ�ͣ����¼����첽�ķ�ʽ��������Ŀǰ����������߳�ֻ��һ������������ʵ��һ���������ӵģ�ר�ŷ����������ģ��̻߳����̳߳ء�
* 2.������ģ�ͣ��涨һ��http����ֻ�ܷ�һ���ļ��������Ҫһ��http���ķ�����ļ�����ʹ��ѹ������
* 3.������ģ�ͣ��涨��̬·�ɺͶ�̬·���Ƿֿ������ģ�����̬·��רע�ڴ��侲̬�ļ�������̬·�������߼���������̬·��Ҳ���Դ��ļ���
*   ��̬·�ɺͶ�̬·�ɵ�·�ɵ�ַ����һ������������·�ɳ�ͻ��������涨����ʵ��·�������⣬������̬·�������淶����������Ҫ����"src"��
*	
*/



/*// ����



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
	enum class State : int    // ָ�� enum ��Ԫ�ص����ͣ�����ԭ�Ӳ���
	{
		waiting = 0,
		working,
		terminated
	};
private:
	LONGLONG m_waitingTime;    // �������������д���
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
	LONGLONG                m_maxKeepTime;  // �������������д���
	threadpool::ThreadPool  m_threadPool;
	route::Route           *m_route;        // ����staicȫ�ֱ���������delete
	sql::SQLServer         *m_sqlServer;    // sql::SQLServer�ܴ�����Ҫ��ָ�룬��Լջ�ռ�
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

	void dealData_taskFuntion(AcceptSocket& _acceptSocket, std::string _value);  // ע���������������ͣ�AcceptSocket& �� std::string
	void checkClientState_threadFuntion();
};













};

};




#endif // !_MOCHEN_WEBSERVER_H_











