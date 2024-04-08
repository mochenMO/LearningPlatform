#include "httpserver.h"

using namespace mochen::httpserver;


// ============================================================================================================
// class HttpServerRequest

HttpServerRequest::HttpServerRequest(HttpRequest& _httpRequest, session::Session* _session)
	: HttpRequest(_httpRequest),
	m_session(_session)
{

}

HttpServerRequest::HttpServerRequest(HttpRequest&& _httpRequest, session::Session* _session) 
	: HttpRequest(std::move(_httpRequest)),
	m_session(_session)
{

}


mochen::session::Session& HttpServerRequest::getSession()
{
	return *m_session;
}


// ============================================================================================================
// class HttpServerResqonse

std::string& HttpServerResqonse::getFilename() 
{
	return m_filename;
}


