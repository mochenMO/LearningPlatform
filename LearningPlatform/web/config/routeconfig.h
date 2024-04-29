#pragma once
#ifndef _MOCHEN_ROUTECONFIG_H_
#define _MOCHEN_ROUTECONFIG_H_


#include "../src/page/default.h"
#include "../src/page/login.h"
#include "../src/page/home.h"
#include "../src/page/blog.h"
#include "../src/page/cv.h"
#include "../src/page/backend.h"

namespace mochen
{

namespace page
{






inline route::Route* createRoute()
{
	static route::Route route{};
	route.getDefaultStaticFilePath() = "web/static";
	
	// ���þ�̬·��
	route.setStaticFile("/favicon.ico", "/img/favicon.ico");  // ע���ļ�·����ͷ��"/"
	route.setStaticFile("/login", "/html/login.html");
	route.setStaticFile("/main", "/html/main.html");
	route.setStaticFile("/cv", "/html/cv.html");
	route.setStaticFile("/backend", "/html/backend.html");
	route.setStaticFile("/error", "/html/error.html");


	// ���ö�̬·��
	route.setRoute("/", &defaultPageMainFuntion);      // defaultPageMainFuntion ʵ��"/"�ض�����ҳ�棬�Ӷ�ʵ���Զ���¼��ע���䲻����ֹ�û�ǿ����ת����¼ҳ��
	route.setRoute("/login/src", &loginPageMainFuntion);
	route.setRoute("/main/src", &mainPageMainFuntion);
	route.setRoute("/blog/src", &blogPageMainFuntion);
	route.setRoute("/cv/src", &cvPageMainFuntion);
	route.setRoute("/backend/src", &backendPageMainFuntion);

	return &route;
}
	


inline route::Route* getRoute()
{
	static route::Route* route = createRoute();
	return route;
}




};

};


#endif // !_MOCHEN_ROUTECONFIG_H_
