#pragma once
#ifndef _MOCHEN_HTTPSERVER_H_
#define _MOCHEN_HTTPSERVER_H_


#include "../http/http.h"
#include "../session/session.h"

namespace mochen
{

namespace httpserver
{

using HttpServerResqonse = http::HttpResponse;


class HttpServerRequest : public http::HttpRequest
{
private:
	session::Session* m_session;
public:
	HttpServerRequest() = default;
	HttpServerRequest(HttpRequest& _httpRequest, session::Session* _session);
	HttpServerRequest(HttpRequest&& _httpRequest, session::Session* _session);
	~HttpServerRequest() = default;
		
	HttpServerRequest(const HttpServerRequest&) = delete;
	HttpServerRequest(HttpServerRequest&&) noexcept = delete;

	HttpServerRequest& operator=(const HttpServerRequest&) = delete;
	HttpServerRequest& operator = (HttpServerRequest&&) noexcept = delete;
	
	session::Session& getSession();
};





};

};


#endif // !_MOCHEN_HTTPSERVER_H_