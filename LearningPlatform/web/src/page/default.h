#pragma once
#ifndef _MOCHEN_DEFAULT_PAGE_H_    // 注意改预处理名！！！！！
#define _MOCHEN_DEFAULT_PAGE_H_


#include "../../../server/src/route/route.h"

#include <iostream>


namespace mochen
{

namespace page
{



inline void defaultPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	std::cout << "defaultPageMainFuntion is OK" << std::endl;


	// Session 是多线程不安全的，使用时需加锁。(目前用的是异步，不需要加锁)
	route::Route* route = _httpServerRequest.getSession()->getParamter<route::Route*>("Route");    // 实现转发
	route->getFunction("/login")(_httpServerRequest, _httpServerResqonse);                // 但有一个问题 getPDealHttpService 不会检测值是否存在

}


};

};


#endif // !_MOCHEN_DEFAULT_PAGE_H_


