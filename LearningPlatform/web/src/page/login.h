#pragma once
#ifndef _MOCHEN_LOGIN_PAGE_H_    // 注意改预处理名！！！！！
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

	// SQLServer // Session 是多线程不安全的，使用时需加锁。(目前用的是异步，不需要加锁)
	sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
	sqlServer->openDataBase("LEARNING");
	sql::SQLServer::SQLData data = sqlServer->select("select * from user_tb");
	sqlServer->print(data);




}


};

};


#endif // !_MOCHEN_LOGIN_PAGE_H_


