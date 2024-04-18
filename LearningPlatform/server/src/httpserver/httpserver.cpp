#include "httpserver.h"

using namespace mochen::httpserver;


// ============================================================================================================
// 全局区


mochen::json::Json* loadContentTypeConfig()
{
	mochen::json::JsonParser jp{};
	jp.open("server/config/mimetypes.json");
	static mochen::json::Json json = jp.parse();
	return &json;
}

mochen::json::Json* getContentTypeConfig()
{
	static mochen::json::Json* json = loadContentTypeConfig();
	return json;
}


// ============================================================================================================
// class HttpServerRequest

HttpServerRequest::HttpServerRequest(HttpRequest& _httpRequest)
	: HttpRequest(_httpRequest),
	m_SQLServer(nullptr),
	m_session(nullptr)
{

}

HttpServerRequest::HttpServerRequest(HttpRequest&& _httpRequest) 
	: HttpRequest(std::move(_httpRequest)),
	m_SQLServer(nullptr),
	m_session(nullptr)
{

}

mochen::session::Session*& HttpServerRequest::getSession()
{
	return m_session;
}


mochen::sql::SQLServer*& HttpServerRequest::getSQLServer()
{
	return m_SQLServer;
}


std::map<std::string, std::string> HttpServerRequest::getCookie()
{
	std::map<std::string, std::string> res{};
	const char* str;

	if (isFindParamter("Cookie") == true) {
		str = m_parameters["Cookie"].c_str();
		res = http::parseStringToKeyValuePairs(str, str + strlen(str), "=", ";");
	}
	else {
		printf("Cookie is not find\n");   // #$#@%@#!$$$$$$$$$$$$$$$$$$$$$$$$$$$%$!%!#@%!$#%!#%!#%!#%!#%!#%!#%!#%
	}
	return res;
}



void HttpServerRequest::addCookie(mochen::http::Cookie& _cookie)
{
	std::string& value = m_parameters["Cookie"];   // 注意：用引用接收返回值
	value += _cookie.getName();
	value += "=";
	value += _cookie.getValue();
	value += ";";
}



void HttpServerRequest::addCookie(std::vector<mochen::http::Cookie>& _cookies)
{
	std::string& value = m_parameters["Cookie"];   // 注意：用引用接收返回值
	for (auto it = _cookies.begin(); it != _cookies.end(); ++it) {
		value += it->getName();
		value += "=";
		value += it->getValue();
		value += ";";
	}
}


// ============================================================================================================
// class HttpServerResqonse


HttpServerResqonse::HttpServerResqonse() : m_filename("")
{
	m_mineTypeConfig = getContentTypeConfig();
}

std::string& HttpServerResqonse::getFilename() 
{
	return m_filename;
}


std::string HttpServerResqonse::getContentType(const std::string& _fileSuffixName)
{
	return (*m_mineTypeConfig)[_fileSuffixName].asString();
}


void HttpServerResqonse::setData(const std::string& _fileSuffixName, const std::string& _data)
{
	std::string contentType = getContentType(_fileSuffixName);
	if (contentType != "") {
		m_parameters["Content-Type"] = contentType;
	}
	else {
		printf("not find contentType logger logger\n"); /////////////////////////////////////////////////////////////////////////
		return;
	}
	m_data = _data;
}


void HttpServerResqonse::addCookie(mochen::http::Cookie& _cookie)
{
	std::string& value = m_parameters["Set-Cookie"];   // 注意：用引用接收返回值
	if (value.empty() == false) {
		value += "\r\nSet-Cookie:";     // 后面的Set - Cookie全部按值处理
	}
	value += _cookie.toString();
}


void HttpServerResqonse::addCookie(std::vector<mochen::http::Cookie>& _cookies)
{
	std::string& value = m_parameters["Set-Cookie"];   // 注意：用引用接收返回值
	int size = _cookies.size();

	for (int i = 0; i < size; ++i) {
		value += _cookies[i].toString();
		if (i + 1 < size) {
			value += "\r\nSet-Cookie:"; // 后面的Set - Cookie全部按值处理
		}
	}
}


void HttpServerResqonse::redirect(const std::string& _urlPath)
{
	m_statusCode = "302";
	m_statusDescription = "Found";
	m_parameters["Location"] = _urlPath;
}
