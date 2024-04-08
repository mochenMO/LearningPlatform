#include "httpserver.h"

using namespace mochen::httpserver;

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


