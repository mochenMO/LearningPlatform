#pragma once
#ifndef _MOCHEN_ROUTECONFIG_H_
#define _MOCHEN_ROUTECONFIG_H_


#include "../src/page/default.h"
#include "../src/page/login.h"

namespace mochen
{

namespace page
{






inline route::Route* createRoute()
{
	static route::Route route{};
	route.getDefaultStaticFilePath() = "web/static";
	
	// 设置静态路由
	route.setStaticFile("/favicon.ico", "/img/favicon.ico");  // 注意文件路径开头有"/"
	route.setStaticFile("/test", "/html/default.html");       ////////////// 



	//route.setStaticFile("/", "/html/login.html");
	//route.setStaticFile("/login", "/html/login.html");
	// route.setRoute("/default", &defaultPageMainFuntion); //////////////

	// 设置动态路由
	route.setRoute("/", &defaultPageMainFuntion);      // defaultPageMainFuntion 用于实现自动登录
	route.setRoute("/login", &defaultPageMainFuntion);   

	route.setRoute("/login/src", &loginPageMainFuntion);



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
