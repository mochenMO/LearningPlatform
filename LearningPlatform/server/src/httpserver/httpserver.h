#pragma once
#ifndef _MOCHEN_HTTPSERVER_H_
#define _MOCHEN_HTTPSERVER_H_


#include "../http/http.h"
#include "../session/session.h"
#include "../json/json.h"
#include "../sql/sql.h"

namespace mochen
{

namespace httpserver
{




class HttpServerRequest : public http::HttpRequest
{
private:
	session::Session* m_session;   // 来自WebServer，不用delete
	sql::SQLServer *m_SQLServer;   // 来自WebServer，不用delete
public:
	HttpServerRequest() = default;
	HttpServerRequest(HttpRequest& _httpRequest);
	HttpServerRequest(HttpRequest&& _httpRequest);
	~HttpServerRequest() = default;
		
	HttpServerRequest(const HttpServerRequest&) = delete;
	HttpServerRequest(HttpServerRequest&&) noexcept = delete;

	HttpServerRequest& operator=(const HttpServerRequest&) = delete;
	HttpServerRequest& operator = (HttpServerRequest&&) noexcept = delete;
	
	session::Session*& getSession();
	sql::SQLServer*& getSQLServer();
};




class HttpServerResqonse : public http::HttpResponse
{
private:
	std::string m_filename;
	json::Json* m_mineTypeConfig;
public:
	HttpServerResqonse();
	~HttpServerResqonse() = default;

	HttpServerResqonse(const HttpServerRequest&) = delete;
	HttpServerResqonse(HttpServerRequest&&) noexcept = delete;

	HttpServerRequest& operator=(const HttpServerRequest&) = delete;
	HttpServerRequest& operator = (HttpServerRequest&&) noexcept = delete;

	std::string& getFilename();
	std::string getContentType(const std::string& _fileSuffixName);
};












};

};


#endif // !_MOCHEN_HTTPSERVER_H_