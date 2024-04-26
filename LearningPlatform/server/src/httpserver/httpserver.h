#pragma once
#ifndef _MOCHEN_HTTPSERVER_H_
#define _MOCHEN_HTTPSERVER_H_


#include "../http/http.h"
#include "../http/cookie.h"
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
public:
	std::map<std::string, std::string> getCookie();
	void addCookie(http::Cookie& _cookie);
	void addCookie(std::vector<http::Cookie>& _cookies);
};




class HttpServerResqonse : public http::HttpResponse
{
private:
	std::string  m_filename;
	json::Json  *m_mineTypeConfig;
public:
	HttpServerResqonse();
	~HttpServerResqonse() = default;

	HttpServerResqonse(const HttpServerRequest&) = delete;
	HttpServerResqonse(HttpServerRequest&&) noexcept = delete;

	HttpServerRequest& operator=(const HttpServerRequest&) = delete;
	HttpServerRequest& operator = (HttpServerRequest&&) noexcept = delete;

	std::string& getFilename();
	std::string getContentType(const std::string& _fileSuffixName);
public:
	void setData(const std::string& _fileSuffixName, const std::string& _data);

	// std::vector<http::Cookie> getCookie();       // 暂时没有getCookie的需求，其实现需要解析字符串     
	void addCookie(http::Cookie& _cookie);          // 注意该函数内部实现与 HttpServerRequest 中的 getCookie 不一样，这里处理的是Set-Cookie
	void addCookie(std::vector<http::Cookie>& _cookies);

	void redirect(const std::string& _urlPath);     // 临时重定向
};












};

};


#endif // !_MOCHEN_HTTPSERVER_H_