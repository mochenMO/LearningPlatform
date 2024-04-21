#pragma once
#ifndef _MOCHEN_LOGIN_PAGE_H_    // ע���Ԥ����������������
#define _MOCHEN_LOGIN_PAGE_H_


#include "../../../server/src/route/route.h"
#include "../data/data.h"


#include <iostream>


namespace mochen
{

namespace page
{



inline void loginPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	// _httpServerResqonse.getFilename() = "web/static/default.html";

	std::cout << "loginPageMainFuntion is OK =====================================" << std::endl;

	// ��ȡ�û�������˺ź�����
	json::JsonParser jr(_httpServerRequest.getData());
	json::Json js = jr.parse();
	std::string uaccount = js["uaccount"].asString();
	std::cout << uaccount << std::endl; //////////////////////////////////

	// ��ȡSqlServer�е��û�����
	sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
	sqlServer->openDataBase("LEARNING");
	sql::SQLServer::SQLData data = sqlServer->select("select * from user_tb where uaccount =  '" + uaccount + "'");
	sqlServer->print(data);  //////////////////////////////////

	// �ж��Ƿ��¼�ɹ�
	if (data.empty() == false && data[0]["upassword"] == js["upassword"].asString()) {
		webdata::UserMap userMap;
		if (_httpServerRequest.getSession()->isFind("userMap") == false) {   // �ж���û�д��� userSession
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
		userData.m_isLogin = true;   // �����Զ���¼

		_httpServerRequest.getSession()->getParamter<webdata::UserMap>("userMap")[data[0]["uid"]] = userData; 

		// ���� Cookie
		http::Cookie cookie;
		cookie.setParamter("sessionID", data[0]["uid"]); // ���� sessionID = uid
		cookie.getMaxAge() = std::to_string(60 * 60 * 24 * 15);                  // ����ʱ��Ϊ15��            
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


};

};


#endif // !_MOCHEN_LOGIN_PAGE_H_


