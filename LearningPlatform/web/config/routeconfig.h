#pragma once
#ifndef _MOCHEN_ROUTECONFIG_H_
#define _MOCHEN_ROUTECONFIG_H_


#include "../../server/src/route/route.h"


namespace mochen
{

namespace route
{






Route* createRoute()
{
	Route route{};
	route.getStaticFilePath() = "web/static/";    // 被忘了结尾的'/'

	route.setRoute("/",)




}
	






Route* getRoute()
{
	static Route* route = createRoute();
	return route;
}




};

};


#endif // !_MOCHEN_ROUTECONFIG_H_
