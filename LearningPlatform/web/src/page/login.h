#pragma once
#ifndef _MOCHEN_LOGIN_PAGE_H_    // 注意改预处理名！！！！！
#define _MOCHEN_LOGIN_PAGE_H_


#include "../../../server/src/route/route.h"

#include <iostream>


namespace mochen
{

namespace page
{


using UserData = std::map<std::string, bool>;


inline void loginPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	// _httpServerResqonse.getFilename() = "web/static/default.html";

	std::cout << "loginPageMainFuntion is OK =====================================" << std::endl;

	// 获取用户输入的账号和密码
	json::JsonParser jr(_httpServerRequest.getData());
	json::Json js = jr.parse();
	std::string uaccount = js["uaccount"].getString();
	std::cout << uaccount << std::endl; //////////////////////////////////

	// 读取SqlServer中的用户数据
	sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
	sqlServer->openDataBase("LEARNING");
	sql::SQLServer::SQLData data = sqlServer->select("select * from user_tb where uaccount =  '" + uaccount + "'");
	sqlServer->print(data);  //////////////////////////////////

	// 判断是否登录成功
	if (data.empty() == false && data[0]["upassword"] == js["upassword"].getString()) {
		UserData userData;
		if (_httpServerRequest.getSession()->isFind("userData") == false) {   // 判断有没有创建 userSession
			_httpServerRequest.getSession()->setParamter<UserData>("userData", userData);
		}
		_httpServerRequest.getSession()->getParamter<UserData>("userData")[data[0]["uid"]] = true;

		std::string cookieData = "sessionID=";
		cookieData += data[0]["uid"];           // 这里 sessionID = uid
		cookieData += ";";                      // 别忘了最后的';'
		cookieData += "Path=/ ;";               // 设置为全局的cookie
		_httpServerResqonse.setParamter("Set-Cookie", cookieData);


		_httpServerResqonse.setParamter("Content-Type", "application/json");
		_httpServerResqonse.getData() = "{\"islogin\":\"true\"}";

		std::cout << "login OK =====================================" << std::endl;
	}
	else {
		_httpServerResqonse.setParamter("Content-Type", "application/json");
		_httpServerResqonse.getData() = "{\"islogin\":\"false\"}";
	}

	
}


};

};


#endif // !_MOCHEN_LOGIN_PAGE_H_


