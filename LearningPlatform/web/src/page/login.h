#pragma once
#ifndef _MOCHEN_LOGIN_PAGE_H_    // 注意改预处理名！！！！！
#define _MOCHEN_LOGIN_PAGE_H_


#define _CRT_SECURE_NO_WARNINGS


#include "../../../server/src/route/route.h"
#include "../data/data.h"

#include <time.h>

#include <iostream>


namespace mochen
{

namespace page
{



inline void loginPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	

	http::HttpURL httpUrl = _httpServerRequest.getUrl();

	// 获取用户输入的账号和密码
	json::JsonParser jr(_httpServerRequest.getData());
	json::Json js = jr.parse();
	std::string uaccount = js["uaccount"].asString();
	std::cout << uaccount << std::endl; //////////////////////////////////

	if (httpUrl.getParamter("action") == "login") {

		// 读取SqlServer中的用户数据
		sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
		sqlServer->openDataBase("LEARNING");
		sql::SQLServer::SQLData data = sqlServer->select("select * from user_tb where uaccount =  '" + uaccount + "'");
		sqlServer->print(data);  //////////////////////////////////

		// 判断是否登录成功
		if (data.empty() == false && data[0]["upassword"] == js["upassword"].asString()) {
			webdata::UserMap userMap;
			if (_httpServerRequest.getSession()->isFind("userMap") == false) {   // 判断有没有创建 userSession
				_httpServerRequest.getSession()->setParamter<webdata::UserMap>("userMap", userMap);
			}
			webdata::UserData userData{};
			userData.m_uid = data[0]["uid"];
			userData.m_utype = data[0]["utype"];
			userData.m_uaccount = data[0]["uaccount"];
			userData.m_upassword = data[0]["upassword"];
			if (data[0]["uphone"] != "") {
				userData.m_uphone = data[0]["uphone"];
			}
			if (data[0]["uemail"] != "") {
				userData.m_uemail = data[0]["uemail"];
			}
			if (data[0]["uage"] != "") {
				userData.m_uage = data[0]["uage"];
			}
			userData.m_isLogin = true;   // 开启自动登录

			_httpServerRequest.getSession()->getParamter<webdata::UserMap>("userMap")[data[0]["uid"]] = userData;

			// 设置 Cookie
			http::Cookie cookie;
			cookie.setParamter("sessionID", data[0]["uid"]); // 这里 sessionID = uid
			cookie.getMaxAge() = std::to_string(60 * 60 * 24 * 15);                  // 保存时间为15天            
			cookie.getPath() = "/";
			_httpServerResqonse.addCookie(cookie);

			_httpServerResqonse.setData("json", "{\"islogin\":\"true\"}");

			std::cout << "login OK =====================================" << std::endl;
		}
		else {
			// _httpServerResqonse.setParamter("Content-Type", "application/json");
			// _httpServerResqonse.getData() = "{\"islogin\":\"false\"}";

			_httpServerResqonse.setData("json", "{\"islogin\":\"false\"}");
		}
	}
	else if (httpUrl.getParamter("action") == "enroll") {

		// 读取SqlServer中的用户数据
		sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
		sqlServer->openDataBase("LEARNING");
		sql::SQLServer::SQLData data = sqlServer->select("select * from user_tb where uaccount =  '" + uaccount + "'");
		sqlServer->print(data);  //////////////////////////////////

		if (data.empty() == true) {    // 判断是否以存在
			webdata::UserMap userMap;
			if (_httpServerRequest.getSession()->isFind("userMap") == false) {   // 判断有没有创建 userSession
				_httpServerRequest.getSession()->setParamter<webdata::UserMap>("userMap", userMap);
			}
			webdata::UserData userData{};
			userData.m_uid = std::to_string(std::time(nullptr));
			userData.m_utype = "ordinary";    // privilege  ordinary
			userData.m_uaccount = js["uaccount"].asString();
			userData.m_upassword = js["upassword"].asString();
			userData.m_uphone = "null";
			userData.m_uemail = "null";
			userData.m_uage = "null";
			userData.m_isLogin = true;   // 开启自动登录

			_httpServerRequest.getSession()->getParamter<webdata::UserMap>("userMap")[userData.m_uid] = userData;

			char commend[2048] = { 0 };

			sprintf(commend,
				"insert into user_tb(uid, utype, uaccount, upassword, uphone, uemail, uage) values('%s','%s','%s','%s','%s','%s','%s');",
				userData.m_uid.c_str(),
				userData.m_utype.c_str(),
				userData.m_uaccount.c_str(),
				userData.m_upassword.c_str(),
				userData.m_uphone.c_str(),
				userData.m_uemail.c_str(),
				userData.m_uage.c_str());

			sqlServer->executeCommend(commend);

			// 设置 Cookie
			http::Cookie cookie;
			cookie.setParamter("sessionID", userData.m_uid); // 这里 sessionID = uid
			cookie.getMaxAge() = std::to_string(60 * 60 * 24 * 15);                  // 保存时间为15天            
			cookie.getPath() = "/";
			_httpServerResqonse.addCookie(cookie);

			_httpServerResqonse.setData("json", "{\"islogin\":\"true\"}");

			std::cout << "login OK =====================================" << std::endl;
		}
		else {
			// _httpServerResqonse.setParamter("Content-Type", "application/json");
			// _httpServerResqonse.getData() = "{\"islogin\":\"false\"}";

			_httpServerResqonse.setData("json", "{\"islogin\":\"false\"}");
		}
	}
	else if (httpUrl.getParamter("action") == "backend") {
		if (_httpServerRequest.isFindParamter("Cookie") == true) {
			std::string sessionID = _httpServerRequest.getCookie()["sessionID"];
			if (_httpServerRequest.getSession()->isFind("userMap") == true) {
				webdata::UserMap userMap = _httpServerRequest.getSession()->getParamter<webdata::UserMap>("userMap");
				if (userMap.find(sessionID) != userMap.end()) {
					if (userMap[sessionID].m_utype == "privilege") {
						_httpServerResqonse.setData("json", "{\"islogin\":\"true\"}");
						return;
					}
				}
			}
		}
		_httpServerResqonse.setData("json", "{\"islogin\":\"false\"}");
	}




}















};

};


#endif // !_MOCHEN_LOGIN_PAGE_H_


