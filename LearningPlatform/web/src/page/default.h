#pragma once
#ifndef _MOCHEN_DEFAULT_PAGE_H_    // ע���Ԥ����������������
#define _MOCHEN_DEFAULT_PAGE_H_


#include "../../../server/src/route/route.h"

#include <iostream>


namespace mochen
{

namespace page
{


using UserData = std::map<std::string, bool>;


inline void defaultPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	std::cout << "defaultPageMainFuntion is OK ============================================" << std::endl;


	route::Route* route = _httpServerRequest.getSession()->getParamter<route::Route*>("Route");
	std::string sessionID{};
	UserData userData{};

	if (_httpServerRequest.isFindParamter("Cookie") == true) {
		sessionID = _httpServerRequest.getCookie()["sessionID"];
		if (_httpServerRequest.getSession()->isFind("userData") == true) {
			userData = _httpServerRequest.getSession()->getParamter<UserData>("userData");
			if (userData.find(sessionID) != userData.end()) {
				if (userData[sessionID] == true) {
					_httpServerResqonse.getFilename() = route->getDefaultStaticFilePath() + "/html/default.html";
					return;
				}
			}
		}
	}
	_httpServerResqonse.getFilename() = route->getDefaultStaticFilePath() + "/html/login.html";
	



	// ʵ��ת����ע�� Session �Ƕ��̲߳���ȫ�ģ�ʹ��ʱ�������(Ŀǰ�õ����첽������Ҫ����)
	// route::Route* route = _httpServerRequest.getSession()->getParamter<route::Route*>("Route");    
	// route->getFunction("/login")(_httpServerRequest, _httpServerResqonse);                         // ����һ������ getFunction ������ֵ�Ƿ����

}


};

};


#endif // !_MOCHEN_DEFAULT_PAGE_H_


