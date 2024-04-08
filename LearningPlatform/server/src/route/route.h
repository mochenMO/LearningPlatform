#pragma once
#ifndef _MOCHEN_ROUTE_H_
#define _MOCHEN_ROUTE_H_

#include "../session/session.h"
#include "../http/http.h"


namespace mochen
{

namespace route
{


class Route
{
public:
	typedef void (*PDealHttpService)(http::HttpRequest& _httpRequest, http::HttpResponse& _httpResponse);
private:
	std::map<std::string, PDealHttpService> m_routeTree;
	std::string m_staticFilePath;
	session::Session* m_session;
public:
	Route() = default;
	Route(std::map<std::string, PDealHttpService>& _routeTree, const std::string& _staticFilePath);
	~Route() = default;
		
	Route(const Route&) = delete;
	Route(Route&&) noexcept = delete;

	Route& operator=(const Route&) = delete;
	Route& operator = (Route&&) noexcept = delete;
	
	void setRoute(const std::string& _urlPath, PDealHttpService _pDealHttpService);
	bool isFind(std::string _urlPath);
	PDealHttpService getPDealHttpService(const std::string& _urlPath);

	std::string& getStaticFilePath();

	const session::Session& getSession();
};



};

};


#endif // !_MOCHEN_ROUTE_H_














