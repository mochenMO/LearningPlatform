#include "httpserver.h"

using namespace mochen::httpserver;


// ============================================================================================================
// È«¾ÖÇø


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

HttpServerRequest::HttpServerRequest(HttpRequest& _httpRequest) : HttpRequest(_httpRequest)
{

}

HttpServerRequest::HttpServerRequest(HttpRequest&& _httpRequest) : HttpRequest(std::move(_httpRequest))
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
	return (*m_mineTypeConfig)[_fileSuffixName].getString();
}