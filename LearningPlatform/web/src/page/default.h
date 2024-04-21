#pragma once
#ifndef _MOCHEN_DEFAULT_PAGE_H_    // ע���Ԥ����������������
#define _MOCHEN_DEFAULT_PAGE_H_


#include "../../../server/src/route/route.h"
#include "../data/data.h"

#include <iostream>


namespace mochen
{

namespace page
{




inline void defaultPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	std::cout << "defaultPageMainFuntion is OK ============================================" << std::endl;


	
	std::string sessionID;

	if (_httpServerRequest.isFindParamter("Cookie") == true) {
		sessionID = _httpServerRequest.getCookie()["sessionID"];
		if (_httpServerRequest.getSession()->isFind("userMap") == true) {
			webdata::UserMap userMap = _httpServerRequest.getSession()->getParamter<webdata::UserMap>("userMap");
			if (userMap.find(sessionID) != userMap.end()) {
				if (userMap[sessionID].m_isLogin == true) {
					_httpServerResqonse.redirect("/main");
					return;
				}
			}
		}
	}
	// _httpServerResqonse.getFilename() = route->getDefaultStaticFilePath() + "/html/login.html";
	_httpServerResqonse.redirect("/login");

	// ʵ��ת����ע�� Session �Ƕ��̲߳���ȫ�ģ�ʹ��ʱ�������(Ŀǰ�õ����첽������Ҫ����)
	// route::Route* route = _httpServerRequest.getSession()->getParamter<route::Route*>("Route");    
	// route->getFunction("/login")(_httpServerRequest, _httpServerResqonse);                         // ����һ������ getFunction ������ֵ�Ƿ����
	
}


};

};


#endif // !_MOCHEN_DEFAULT_PAGE_H_


