#pragma once
#ifndef _MOCHEN_LOGIN_PAGE_H_    // ע���Ԥ����������������
#define _MOCHEN_LOGIN_PAGE_H_


#include "../../../server/src/route/route.h"

#include <iostream>


namespace mochen
{

namespace page
{



inline void loginPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	std::cout << "loginPageMainFuntion is OK" << std::endl;

}


};

};


#endif // !_MOCHEN_LOGIN_PAGE_H_


