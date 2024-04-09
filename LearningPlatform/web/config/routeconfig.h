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
	
	route.setStaticFile("/test", "/html/default.html");       // 注意文件路径开头有"/"
	route.setStaticFile("/favicon.ico", "/res/favicon.ico");  

	route.setRoute("/default", &defaultPageMainFuntion);
	route.setRoute("/login", &loginPageMainFuntion);

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
