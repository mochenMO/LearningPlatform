#pragma once
#ifndef _MOCHEN_MAIN_PAGE_H_    // 注意改预处理名！！！！！
#define _MOCHEN_MAIN_PAGE_H_


#include "../../../server/src/route/route.h"
#include "../data/data.h"

#include <iostream>


namespace mochen
{

namespace page
{





inline void mainPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	http::HttpURL httpUrl = _httpServerRequest.getUrl();
	std::string sessionID;
	webdata::UserMap userMap;

	if (_httpServerRequest.isFindParamter("Cookie") == true) {
		sessionID = _httpServerRequest.getCookie()["sessionID"];
		if (_httpServerRequest.getSession()->isFind("userMap") == true) {
			userMap = _httpServerRequest.getSession()->getParamter<webdata::UserMap>("userMap");
			if (userMap.find(sessionID) != userMap.end()) {
				goto anchors01;
			}
		}
	}
	return;

anchors01:
	if (httpUrl.isFindParamter("get") == true) {    // 请求资源
		if (httpUrl.getParamter("get") == "userData") {
			json::Json json{};
			webdata::UserData userData = userMap[sessionID];
			json["uaccount"] = userData.m_uaccount;
			json["upassword"] = userData.m_upassword;
			json["uphone"] = userData.m_uphone;
			json["uemail"] = userData.m_uemail;
			json["uage"] = userData.m_uage;
			_httpServerResqonse.setData("json", json.toString());
		}
	}
	else if (httpUrl.isFindParamter("save") == true) {    // 写入资源
		if (httpUrl.getParamter("save") == "userData") {		
			json::JsonParser jp(_httpServerRequest.getData());
			json::Json js = jp.parse();
	
			std::string commend = "";
			sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
			sqlServer->openDataBase("LEARNING");

			commend += "update user_tb set ";
			commend += "uaccount='" + js["uaccount"].asString() + "',";
			commend += "upassword='" + js["upassword"].asString() + "',";
			commend += "uphone='" + js["uphone"].asString() + "',";
			commend += "uemail='" + js["uemail"].asString() + "',";
			commend += "uage='" + js["uage"].asString() + "'";
			commend += "where uid='" + sessionID + "'";   // 注意 sessionID = uid 
			 
			sqlServer->update(commend);
		}
	}


	
}


};

};


#endif // !_MOCHEN_MAIN_PAGE_H_