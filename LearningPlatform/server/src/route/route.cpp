#include "route.h"

using namespace mochen::route;


void  Route::setRoute(const std::string& _urlPath, Route::PDealHttpService _pDealHttpService)
{
	m_routeTree[_urlPath] = _pDealHttpService;
}

bool Route::isFindFunction(std::string _urlPath)
{
	return (m_routeTree.find(_urlPath) != m_routeTree.end());
}

Route::PDealHttpService Route::getFunction(const std::string& _urlPath)
{
	return m_routeTree[_urlPath];
}

void Route::setStaticFile(const std::string& _urlPath, const std::string& _filename)
{
	m_staticFileMap[_urlPath] = _filename;
}

bool Route::isFindStaticFile(std::string _urlPath)
{
	return (m_staticFileMap.find(_urlPath) != m_staticFileMap.end());
}

std::string& Route::getStaticFile(const std::string& _urlPath)
{
	return m_staticFileMap[_urlPath];
}

std::string& Route::getDefaultStaticFilePath()
{
	return m_defineStaticFilePath;
}







