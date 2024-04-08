/*// ������˵��
* 1.HttpParser�ڲ������жϣ��������������Ļ���Ӧ���ģ����û��������ж��ٵ��ö�Ӧ�ĺ������ɡ�
* 2.ʹ�� parseStringToKeyValuePairs �� parseStringToArray һ��Ҫ��֤��ظ�ʽ����ȷ�ԣ���β������Ҳ���� _endString��
* 3.ʹ�� HttpParser һ��Ҫ��֤ http ���ĸ�ʽ����ȷ��
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

// ����ȫ�ֺ���
extern std::map<std::string, std::string> parseStringToKeyValuePairs(const char* _startPos, const char* _endPos, const char* _separateString, const char* _endString);   // ע��ͨ�������ຯ���ķ���ֵ����ֵ���͡�
extern std::string parseKeyValuePairsToString(std::map<std::string, std::string>& _KeyValuePairs, const char* _separateString, const char* _endString);                  // ע��ͨ�������ຯ���ķ���ֵ����ֵ���͡�
                 
extern std::vector<std::string> parseStringToArray(const char* _startPos, const char* _endPos, const char* _endString);
extern std::string parseArrayToString(std::vector<std::string>& _array, const char* _endString);



class HttpURL   // URL��ʽ�ǣ�Э��://������:�˿ں�/·��?����#ê��
{
private:
	std::string m_protocols;
	std::string m_domaiName;   // ��������IP
	std::string m_port;
	std::string m_path;
	std::map<std::string, std::string> m_parameters;    // ͨ����������б�Ľ�β����һ��'&'�������Ҳ����������
	std::string m_anchors;    // ע�⣺ê����Ϣ�����ڣ����������������������������������������
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
	std::string& getParamter(const std::string& _key);   // ���URL�еĲ���
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
	std::string& getParamter(const std::string& _key);   // ����ֶ�ֵ
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
	std::string& getParamter(const std::string& _key);   // ����ֶ�ֵ
	void setParamter(const std::string& _key, const std::string& _value);
	void setParamterMap(const std::map<std::string, std::string>& _value);
	void setParamterMap(std::map<std::string, std::string>&& _value);

	std::string& getData();

	std::string headerToString();
};



class HttpParser
{
	std::string m_string;			                      // ���ﲻ��char*����Ϊ������Ҫ��Json������һ��ѭ��������
public:
	HttpParser(const std::string& _string);
	~HttpParser() = default;

	HttpParser(const HttpParser& _value) = delete; 
	HttpParser(HttpParser&& _value) noexcept = delete;

	void operator=(const HttpParser& _value) = delete;
	void operator=(HttpParser&& _value) noexcept = delete;

	HttpURL parseHttpURL(const char* _string);			  // ע��ͨ�������ຯ���ķ���ֵ����ֵ���͡�
	HttpRequest parseHttpRequest();
	HttpResponse parseHttpResponse();
};











};

};







#endif // !_MOCHEN_HTTP_H_




