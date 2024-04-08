/*// 介绍与说明
* 1.HttpParser内部不会判断，发来的是请求报文还响应报文，请用户先自行判断再调用对应的函数即可。
* 2.使用 parseStringToKeyValuePairs 和 parseStringToArray 一定要保证相关格式的正确性，结尾处尽量也加上 _endString。
* 3.使用 HttpParser 一定要保证 http 报文格式的正确性
* 
* 
*/




#pragma once
#ifndef _MOCHEN_HTTP_H_
#define _MOCHEN_HTTP_H_


#include <sstream>
#include <map>
#include <string>
#include <string>
#include <vector>



namespace mochen
{

namespace http
{

// 声明全局函数
extern std::map<std::string, std::string> parseStringToKeyValuePairs(const char* _startPos, const char* _endPos, const char* _separateString, const char* _endString);   // 注意通常解析类函数的返回值都在值类型。
extern std::string parseKeyValuePairsToString(std::map<std::string, std::string>& _KeyValuePairs, const char* _separateString, const char* _endString);                  // 注意通常解析类函数的返回值都在值类型。
                 
extern std::vector<std::string> parseStringToArray(const char* _startPos, const char* _endPos, const char* _endString);
extern std::string parseArrayToString(std::vector<std::string>& _array, const char* _endString);



class HttpURL   // URL格式是：协议://主机名:端口号/路径?参数#锚点
{
private:
	std::string m_protocols;
	std::string m_domaiName;   // 域名或是IP
	std::string m_port;
	std::string m_path;
	std::map<std::string, std::string> m_parameters;    // 通常如果参数列表的结尾多了一个'&'，浏览器也能正常解析
	std::string m_anchors;    // 注意：锚点信息仅限于，服务器发给浏览器，浏览器不会把它发给服务器
public:
	HttpURL() = default;
	~HttpURL() = default;
	
	HttpURL(const HttpURL& _value) = default;
	HttpURL(HttpURL&& _value) noexcept = default;

	HttpURL& operator=(const HttpURL& _value) = default;
	HttpURL& operator=(HttpURL&& _value) noexcept = default;

	std::string& getProtocols();
	std::string& getDomaiName();
	std::string& getPort();
	std::string& getPath();

	bool isFindParamter(const std::string& _key);
	std::string& getParamter(const std::string& _key);   // 获得URL中的参数
	void setParamter(const std::string& _key, const std::string& _value);
	void setParamterMap(const std::map<std::string, std::string>& _value);
	void setParamterMap(std::map<std::string, std::string>&& _value);

	std::string& getAnchors();
	std::string toString();
};



class HttpRequest
{
private:
	std::string m_method;
	HttpURL m_url;
	std::string m_version;
	std::map<std::string, std::string> m_parameters;
	std::string m_data;
public:
	HttpRequest() = default;
	~HttpRequest() = default;

	HttpRequest(const HttpRequest& _value) = default;
	HttpRequest(HttpRequest&& _value) noexcept = default;

	HttpRequest& operator=(const HttpRequest& _value) = default;
	HttpRequest& operator=(HttpRequest&& _value) noexcept = default;

	std::string& getMethod();
	HttpURL& getUrl();
	std::string& getVersion();

	bool isFindParamter(const std::string& _key);
	std::string& getParamter(const std::string& _key);   // 获得字段值
	void setParamter(const std::string& _key, const std::string& _value);
	void setParamterMap(const std::map<std::string, std::string>& _value);
	void setParamterMap(std::map<std::string, std::string>&& _value);

	std::string& getData();

	std::string headerToString();
public:
	std::map<std::string, std::string> getCookie();
	void setCookie(std::map<std::string, std::string>& _cookie);
};


class HttpResponse
{
private:
	std::string m_version;
	std::string m_statusCode;
	std::string m_statusDescription;
	std::map<std::string, std::string> m_parameters;
	std::string m_data;
public:
	HttpResponse() = default;
	~HttpResponse() = default;

	HttpResponse(const HttpResponse& _value) = default;
	HttpResponse(HttpResponse&& _value) noexcept = default;

	HttpResponse& operator=(const HttpResponse& _value) = default;
	HttpResponse& operator=(HttpResponse&& _value) noexcept = default;

	std::string& getVersion();
	std::string& getStatusCode();
	std::string& getStatusDescription();

	bool isFindParamter(const std::string& _key);
	std::string& getParamter(const std::string& _key);   // 获得字段值
	void setParamter(const std::string& _key, const std::string& _value);
	void setParamterMap(const std::map<std::string, std::string>& _value);
	void setParamterMap(std::map<std::string, std::string>&& _value);

	std::string& getData();

	std::string headerToString();
};



class HttpParser
{
	std::string m_string;			                      // 这里不用char*是因为它不需要向Json解析器一样循环解析。
public:
	HttpParser(const std::string& _string);
	~HttpParser() = default;

	HttpParser(const HttpParser& _value) = delete; 
	HttpParser(HttpParser&& _value) noexcept = delete;

	void operator=(const HttpParser& _value) = delete;
	void operator=(HttpParser&& _value) noexcept = delete;

	HttpURL parseHttpURL(const char* _string);			  // 注意通常解析类函数的返回值都在值类型。
	HttpRequest parseHttpRequest();
	HttpResponse parseHttpResponse();
};











};

};







#endif // !_MOCHEN_HTTP_H_




