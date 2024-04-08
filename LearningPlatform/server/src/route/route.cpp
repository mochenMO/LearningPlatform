#include "route.h"

using namespace mochen::route;


Route::Route(std::map<std::string, PDealHttpService>& _routeTree, const std::string& _staticFilePath)
	: m_routeTree(_routeTree),
	m_staticFilePath(m_staticFilePath),
	m_session(nullptr)
{

}

void  Route::setRoute(const std::string& _urlPath, Route::PDealHttpService _pDealHttpService)
{
	m_routeTree[_urlPath] = _pDealHttpService;
}

bool Route::isFind(std::string _urlPath)
{
	return (m_routeTree.find(_urlPath) != m_routeTree.end());
}


Route::PDealHttpService Route::getPDealHttpService(const std::string& _urlPath)
{
	return m_routeTree[_urlPath];
}


std::string& Route::getStaticFilePath()
{
	return m_staticFilePath;
}

const mochen::session::Session& Route::getSession()
{
	return *m_session;
}





