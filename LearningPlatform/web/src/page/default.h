#pragma once
#ifndef _MOCHEN_DEFAULT_PAGE_H_    // ע���Ԥ����������������
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


	// Session �Ƕ��̲߳���ȫ�ģ�ʹ��ʱ�������(Ŀǰ�õ����첽������Ҫ����)
	route::Route* route = _httpServerRequest.getSession()->getParamter<route::Route*>("Route");    // ʵ��ת��
	route->getFunction("/login")(_httpServerRequest, _httpServerResqonse);                // ����һ������ getPDealHttpService ������ֵ�Ƿ����

}


};

};


#endif // !_MOCHEN_DEFAULT_PAGE_H_


