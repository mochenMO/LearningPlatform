#include "cookie.h"


using namespace mochen::http;

Cookie::Cookie(const std::string& _name, const std::string& _value)
	: m_name(_name),
	m_value(_value),
	m_maxAge(""),
	m_domain(""),
	m_path(""),
	m_secure(""),
	m_httpOnly(""),
	m_sameSite("")
{

}


std::string& Cookie::getName() 
{
	return m_name;
}

std::string& Cookie::getValue()
{
	return m_value;
}

std::string& Cookie::getMaxAge()
{
	return m_maxAge;
}

std::string& Cookie::getDomain()
{
	return m_domain;
}

std::string& Cookie::getPath()
{
	return m_path;
}

std::string& Cookie::getSameSite()
{
	return m_sameSite;
}

void Cookie::setParamter(const std::string& _name, const std::string& _value)
{
	m_name = _name;
	m_value = _value;
}

bool Cookie::isSetSecure()
{
	return (m_secure != "");
}

bool Cookie::isSetHttpOnly()
{
	return (m_httpOnly != "");
}

void Cookie::setSecure(bool _value)
{
	if (isSetSecure() == _value) {
		return;
	}
	m_secure = _value == true ? "Secure" : "";
}


void Cookie::setHttpOnly(bool _value)
{
	if (isSetHttpOnly() == _value) {
		return;
	}
	m_httpOnly = _value == true ? "Secure" : "";
}


std::string Cookie::toString()
{
	std::string res;

	if (m_name == "") {
		printf("Logger Logger Logger\n");
		return "";
	}
	if (m_value == "") {
		printf("Logger Logger Logger\n");
		return "";
	}
	res += m_name;
	res += "=";
	res += m_value;
	res += ";";

	if (m_maxAge != "") {
		res += "Max-Age";
		res += "=";
		res += m_maxAge;
		res += ";";
	}
	if (m_domain != "") {
		res += "Domain";
		res += "=";
		res += m_domain;
		res += ";";
	}
	if (m_path != "") {
		res += "Path";
		res += "=";
		res += m_path;
		res += ";";
	}
	if (m_secure != "") {
		res += "Secure;";
	}
	
	if (m_httpOnly != "") {
		res += "HttpOnly;";
	}

	if (m_sameSite != "") {
		if (m_sameSite == "Strict" || m_sameSite == "Lax" || m_sameSite == "None") {
			res += "SameSite";
			res += "=";
			res += m_sameSite;
			res += ";";
		}
		else {
			printf("Logger Logger Logger\n");
			return "";
		}
	}
	return res;
}



