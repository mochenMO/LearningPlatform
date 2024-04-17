#include "http.h"


using namespace mochen::http;


// =============================================================================================================
// ȫ����    

std::map<std::string, std::string> mochen::http::parseStringToKeyValuePairs(const char* _startPos, const char* _endPos, const char* _separateString, const char* _endString)
{
	// " key1=1; key2 = 2 ; key3 ; key4 =; key5 = 5 ; "
	// " key1%%1** key2 %% 2 ** key3 ** key4 %%** key5 %% 5 ** "
	// �����ֵ�Բ���������ֻ�ᴴ��key��value�ᱻ��Ϊ���ַ���""��

	std::map<std::string, std::string> KeyValuePairs{};
	std::string key;
	std::string value;
	const char* start;    // �����ַ�����ʼλ��
	const char* middle;   // �����ֵ���зָ�������ʼλ��
	const char* end;      // �����ַ�����������

	while (*_startPos == ' ' || *_startPos == '\t') {   // ȥ���ַ�����ͷ�Ŀո�
		++_startPos;
	}

	while (_startPos < _endPos)
	{
		key = "";      // ��� key
		value = "";    // ��� value

		start = _startPos;
		middle = strstr(_startPos, _separateString);
		end = strstr(_startPos, _endString);

		if (end == nullptr) {   // ���βû��_endString
			end = _endPos;
		}
		if (middle > end) {    // û�зָ�����ֻ��keyֵ
			middle = end;
		}
		while (*start != ' ' && *start != '\t' && start < middle) {   // ��ȡkeyֵ
			key += *start;
			++start;
		}

		start = middle + strlen(_separateString);  // ��� middle = end �� start > end

		while (*start == ' ' || *start == '\t') {   // ȥ��value��ͷ�Ŀո�
			++start;
		}

		while (start < end) {
			value += *start;
			++start;
		}

		_startPos = end + strlen(_endString);

		while (*_startPos == ' ' || *_startPos == '\t') {   // �Ƶ���һ���ַ�����ʼ��λ��
			++_startPos;
		}

		KeyValuePairs[key] = value;
	}
	return KeyValuePairs;
}


std::string mochen::http::parseKeyValuePairsToString(std::map<std::string, std::string>& _KeyValuePairs, const char* _separateString, const char* _endString)
{
	std::string res;

	for(auto it = _KeyValuePairs.begin(); it != _KeyValuePairs.end(); ++it){
		res += it->first;
		if (it->second != "") {   // �ж��ǲ���ֻ��key
			res += _separateString;
			res += it->second;
		}
		res += _endString;
	}
	return res;
}

std::vector<std::string> mochen::http::parseStringToArray(const char* _startPos, const char* _endPos, const char* _endString)
{
	// " key1 , , key2,key3, key4, key5 , "      // ע�⣺��β��","������
	// �����ֵ�Բ�������value�ᱻ��Ϊ���ַ���""��

	std::vector<std::string> arr{};
	std::string value;
	const char* start;    // �����ַ�����ʼλ��
	const char* end;      // �����ַ�����������


	while (*_startPos == ' ' || *_startPos == '\t') {   // ȥ���ַ�����ͷ�Ŀո�
		++_startPos;
	}

	while (_startPos < _endPos)
	{
		value = "";    // ��� value

		start = _startPos;
		end = strstr(_startPos, _endString);

		if (end == nullptr) {  // ���βû��_endString
			end = _endPos;
		}

		while (*start != ' ' && *start != '\t' && start < end) {   // ��ȡvalueֵ
			value += *start;
			++start;
		}

		_startPos = end + strlen(_endString);

		while (*_startPos == ' ' || *_startPos == '\t') {   // �Ƶ���һ���ַ�����ʼ��λ��
			++_startPos;
		}

		arr.push_back(value);
	}
	return arr;
}


std::string mochen::http::parseArrayToString(std::vector<std::string>& _array, const char* _endString)
{
	std::string res;

	for (auto it = _array.begin(); it != _array.end(); ++it) {
		res += *it;
		res += _endString;
	}
	return res;
}



// =============================================================================================================
// HttpURL

std::string& HttpURL::getProtocols()
{ 
	return m_protocols; 
}

std::string& HttpURL::getDomaiName()
{ 
	return m_domaiName; 
}

std::string& HttpURL::getPort() 
{ 
	return m_port;
}

std::string& HttpURL::getPath()
{ 
	return m_path;
}

bool HttpURL::isFindParamter(const std::string& _key)
{
	auto it = m_parameters.find(_key);
	return (it != m_parameters.end());
}

std::string& HttpURL::getParamter(const std::string& _key)
{
	return m_parameters[_key];
}

void HttpURL::setParamter(const std::string& _key, const std::string& _value)
{
	m_parameters[_key] = _value;
}

void HttpURL::setParamterMap(const std::map<std::string, std::string>& _value)
{
	m_parameters = _value;
}

void HttpURL::setParamterMap(std::map<std::string, std::string>&& _value)
{
	m_parameters = std::move(_value);
}

std::string& HttpURL::getAnchors()
{
	return m_anchors;
}


std::string HttpURL::toString()
{
	// URL��ʽ�ǣ�Э��://������:�˿ں�/·��?����#ê��

	std::string str = "";

	if (m_protocols != "") {
		str += m_protocols;
		str += "://";
	}
	if (m_domaiName != "") {
		str += m_domaiName;
	}
	if (m_port != "") {
		str += ":";
		str += m_port;
	}
	if (m_path != "") {
		str += m_path;
	}
	if (m_parameters.empty() != true) {
		str += "?";
		str += parseKeyValuePairsToString(m_parameters, "=", "&");
	}
	if (m_anchors != "") {
		str += "#";
		str += m_anchors;
	}

	return str;
}


// =============================================================================================================
// HttpRequest


std::string& HttpRequest::getMethod()
{
	return m_method;
}

HttpURL& HttpRequest::getUrl()
{
	return m_url;
}

std::string& HttpRequest::getVersion()
{
	return m_version;
}



bool HttpRequest::isFindParamter(const std::string& _key)
{
	auto it = m_parameters.find(_key);
	return (it != m_parameters.end());
}

std::string& HttpRequest::getParamter(const std::string& _key)
{
	return m_parameters[_key];
}

void HttpRequest::setParamter(const std::string& _key, const std::string& _value)
{
	m_parameters[_key] = _value;
}

void HttpRequest::setParamterMap(const std::map<std::string, std::string>& _value)
{
	m_parameters = _value;
}

void HttpRequest::setParamterMap(std::map<std::string, std::string>&& _value)
{
	m_parameters = std::move(_value);
}

std::string& HttpRequest::getData()
{
	return m_data;
}

std::string HttpRequest::headerToString()
{
	std::string str;

	str += m_method;
	str += " ";
	str += m_url.toString();
	str += " ";
	str += m_version;
	str += "\r\n";
	str += parseKeyValuePairsToString(m_parameters, ":", "\r\n");
	str += "\r\n";   // ע�⣺�����˽�β��"\n\r"

	return str;
}

//std::map<std::string, std::string> HttpRequest::getCookie()
//{
//	std::map<std::string, std::string> res{};
//	const char* str;
//
//	if (isFindParamter("Cookie") == true) {
//		str = getParamter("Cookie").c_str();
//		res = parseStringToKeyValuePairs(str, str + strlen(str), "=", ";");
//	}
//	else {
//		printf("Cookie is not find\n");   // #$#@%@#!$$$$$$$$$$$$$$$$$$$$$$$$$$$%$!%!#@%!$#%!#%!#%!#%!#%!#%!#%!#%
//	}
//	return res;
//}
//
//
//void HttpRequest::setCookie(std::map<std::string, std::string>& _cookie)
//{
//	std::string res = std::move(parseKeyValuePairsToString(_cookie, "=", ";"));
//	m_parameters["Cookie"] = res;
//}


// =============================================================================================================
// HttpResponse

HttpResponse::HttpResponse() 
{
	m_version = "HTTP/1.1";
	m_statusCode = "200";
	m_statusDescription = "OK";
}


std::string& HttpResponse::getVersion()
{
	return m_version;
}


std::string& HttpResponse::getStatusCode()
{
	return m_statusCode;
}

std::string& HttpResponse::getStatusDescription()
{
	return m_statusDescription;
}

bool HttpResponse::isFindParamter(const std::string& _key)
{
	auto it = m_parameters.find(_key);
	return (it != m_parameters.end());
}

std::string& HttpResponse::getParamter(const std::string& _key)
{
	return m_parameters[_key];
}

void HttpResponse::setParamter(const std::string& _key, const std::string& _value)
{
	m_parameters[_key] = _value;
}

void HttpResponse::setParamterMap(const std::map<std::string, std::string>& _value)
{
	m_parameters = _value;
}

void HttpResponse::setParamterMap(std::map<std::string, std::string>&& _value)
{
	m_parameters = std::move(_value);
}

std::string& HttpResponse::getData()
{
	return m_data;
}

std::string HttpResponse::headerToString()
{
	std::string str;

	str += m_version;
	str += " ";
	str += m_statusCode;
	str += " ";
	str += m_statusDescription;
	str += "\r\n";
	str += parseKeyValuePairsToString(m_parameters, ":", "\r\n");
	str += "\r\n";   // ע�⣺�����˽�β��"\n\r"

	return str;
}



// =============================================================================================================
// HttpParser


HttpParser::HttpParser(const std::string& _string)
{
	m_string = _string;
}


HttpURL HttpParser::parseHttpURL(const char* _string)
{
	// URL��ʽ�ǣ�Э��://������:�˿ں�/·��?����#ê��

	HttpURL res{};
	std::string tempString = "";

	const char* end = strstr(_string, " ");
	
	if (strncmp(_string, "http://", 7) == 0){
		res.getProtocols() = "http";
		_string += 7;
	}

	if (*_string != '/') {            // ������
		while (*_string != ':' && *_string != '/') {
			tempString += *_string;
		}
		res.getDomaiName() = tempString;
		tempString = "";

		if (*_string == ':') {         // �ж˿ں�
			++_string;                 // ����":"
			while (*_string != '/') {
				tempString += *_string;
			}
			res.getPort() = tempString;
			tempString = "";
		}
	}

	while (*_string != '?' && _string < end) {
		tempString += *_string;
		++_string;
	}
	res.getPath() = tempString;
	tempString = "";

	if (*_string == '?') {             // �в���
		++_string;                     // ����"?"
		while (*_string != '#' && _string < end) {
			tempString += *_string;
			++_string;
		}
		res.setParamterMap(parseStringToKeyValuePairs(tempString.c_str(), tempString.c_str() + tempString.size(), "=", "&"));
		tempString = "";
	}

	if (*_string == '#') {            // ��ê��
		++_string;                    // ����"#"
		while (_string < end) {
			tempString += *_string;
		}
		res.getAnchors() = tempString;
	}

	return res;
}

HttpRequest HttpParser::parseHttpRequest()
{
	HttpRequest res;
	const char* start = m_string.c_str();
	const char* temp;

	temp = strstr(start, " ");
	res.getMethod() = std::string(start, temp);
	start = temp + 1;    // +1����" " 


	res.getUrl() = std::move(parseHttpURL(start));
	start = strstr(start, " ");
	start += 1;         //  ����" " 

	temp = strstr(start, "\r\n");
	res.getVersion() = std::string(start, temp);
	start = temp + 2;  //  +2����"\r\n" 


	temp = strstr(start, "\r\n\r\n");
	res.setParamterMap(parseStringToKeyValuePairs(start, temp, ":", "\r\n"));
	start = temp + 4;  //  +2����"\r\n\r\n" 

	res.getData() = std::string(start);

	return res;
}



HttpResponse HttpParser::parseHttpResponse()
{
	HttpResponse res;
	const char* start = m_string.c_str();
	const char* temp;

	temp = strstr(start, " ");
	res.getVersion() = std::string(start, temp - 1);
	start = temp + 1;    // +1����" " 

	temp = strstr(start, " ");
	res.getStatusCode() = std::string(start, temp - 1);
	start = temp + 1;    // +1����" " 

	temp = strstr(start, "\r\n");
	res.getStatusDescription() = std::string(start, temp - 1);
	start = temp + 2;  //  +2����"\r\n" 

	temp = strstr(start, "\r\n\r\n");
	res.setParamterMap(parseStringToKeyValuePairs(start, temp, ":", "\r\n"));
	start = temp + 4;  //  +2����"\r\n\r\n" 

	res.getData() = std::string(start);

	return res;
}
