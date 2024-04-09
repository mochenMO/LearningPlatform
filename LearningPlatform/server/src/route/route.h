#pragma once
#ifndef _MOCHEN_ROUTE_H_
#define _MOCHEN_ROUTE_H_

#include "../httpserver/httpserver.h"


namespace mochen
{

namespace route
{




class Route
{
public:
	typedef void (*PDealHttpService)(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse);
private:
	std::map<std::string, PDealHttpService> m_routeTree;
	std::map<std::string, std::string> m_staticFileMap;
	std::string m_defineStaticFilePath;
public:
	Route() = default;
	~Route() = default;
		
	Route(const Route&) = delete;
	Route(Route&&) noexcept = delete;

	Route& operator=(const Route&) = delete;
	Route& operator = (Route&&) noexcept = delete;
	
	void setRoute(const std::string& _urlPath, PDealHttpService _pDealHttpService);
	bool isFindFunction(std::string _urlPath);
	PDealHttpService getFunction(const std::string& _urlPath);

	void setStaticFile(const std::string& _urlPath, const std::string& _filename);
	bool isFindStaticFile(std::string _urlPath);
	std::string& getStaticFile(const std::string& _urlPath);

	std::string& getDefaultStaticFilePath();
};






};

};


#endif // !_MOCHEN_ROUTE_H_














