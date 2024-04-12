#pragma once
#ifndef _MOCHEN_LOGIN_PAGE_H_    // ע���Ԥ����������������
#define _MOCHEN_LOGIN_PAGE_H_


#include "../../../server/src/route/route.h"

#include <iostream>


namespace mochen
{

namespace page
{



inline void loginPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	// _httpServerResqonse.getFilename() = "web/static/default.html";

	std::cout << "loginPageMainFuntion is OK =====================================" << std::endl;

	std::cout << _httpServerRequest.getData() << std::endl;

	// json::JsonParser jr(std::move(_httpServerRequest.getData()));  // ??????????
	// json::JsonParser jr(_httpServerRequest.getData());             // ??????????
	json::JsonParser jr{};
	jr.loadByString(_httpServerRequest.getData());
	json::Json js = jr.parse();
	std::string username = js["username"].getString();

	std::cout << username << std::endl;

	// SQLServer // Session �Ƕ��̲߳���ȫ�ģ�ʹ��ʱ�������(Ŀǰ�õ����첽������Ҫ����)
	sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
	sqlServer->openDataBase("LEARNING");
	sql::SQLServer::SQLData data = sqlServer->select("select * from user_tb");
	sqlServer->print(data);




}


};

};


#endif // !_MOCHEN_LOGIN_PAGE_H_


