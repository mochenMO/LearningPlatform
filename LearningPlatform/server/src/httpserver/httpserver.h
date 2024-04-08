#pragma once
#ifndef _MOCHEN_HTTPSERVER_H_
#define _MOCHEN_HTTPSERVER_H_


#include "../http/http.h"
#include "../session/session.h"

namespace mochen
{

namespace httpserver
{




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




class HttpServerResqonse : public http::HttpResponse
{
private:
	std::string m_filename;
public:
	HttpServerResqonse() = default;
	~HttpServerResqonse() = default;

	HttpServerResqonse(const HttpServerRequest&) = delete;
	HttpServerResqonse(HttpServerRequest&&) noexcept = delete;

	HttpServerRequest& operator=(const HttpServerRequest&) = delete;
	HttpServerRequest& operator = (HttpServerRequest&&) noexcept = delete;

	std::string& getFilename();
};












};

};


#endif // !_MOCHEN_HTTPSERVER_H_