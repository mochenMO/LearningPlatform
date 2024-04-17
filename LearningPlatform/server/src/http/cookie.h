
/*// ������˵��
* 1.����ֻ��ʵ���˼������õ�Cookie�ֶ�
* 2.ÿ��Cookie��ֻ�ܴ�һ�Լ�ֵ�ԣ����Ҫ�Դ����ö��Cookie���������߷���
*	����һ��д����Set-Cookie�У�����http������std::map���ֵ�Եģ����ܴ�����Set-Cookie����˺����Set-Cookieȫ����ֵ����
*   Set-Cookie: cookie1=value1; Path=/; Max-Age = 3600
*   Set-Cookie: cookie2=value2; Path=/; Max-Age = 3600
*   ��������д��һ��Set-Cookie�У�����","�ָ� (����ֻȡ��һ��Cookieֵ)
*   Set-Cookie: cookie1=value1; Path=/; Max-Age = 3600, cookie2=value2; Path=/; Max-Age = 3600
*/


#pragma once
#ifndef _MOCHEN_COOKIE_H_
#define _MOCHEN_COOKIE_H_


#include <string>

namespace mochen
{

namespace http
{




class Cookie
{
private:
	std::string m_name;                              // �ֶ���
	std::string m_value;                             // �ֶ�ֵ
	std::string m_maxAge;                            // Max-Age=3600 ��λ���롣   ָ��Cookie���ʱ��, Ĭ����һ���Ự�����ر������ʱ���ڡ�Max-Age �� Expires �����ã���Ϊ Expires ��ʽ�����ң�������ͷ�����֮���ʱ�ӿ��ܲ�ͬ������Expires = Weekday, DD Mon YYYY HH:MM:SS GMT��
	std::string m_domain;                            // Domain = abc.com         ָ��Cookie���Է��͵��ĸ�������Ĭ��Ϊ����Cookie��ҳ�����ڵ�������
	std::string m_path;                              // Path = /login            ָ����Cookie��·������ʾ��CookieӦ������Щ·����Ĭ��Ϊ����Ϊ���ո�Cookie��ҳ���·����Path=/ ��ʾ����ҳ��
	std::string m_secure;                            // Secure                   ��ȫ��־����ʾ��Cookieֻ����ͨ��HTTPSЭ�鷢��ʱ�Żᱻ���͵�������
	std::string m_HttpOnly;                          // HttpOnly                 ��ʾ��Cookie����ͨ��HTTP��HTTPSЭ�鷢�͸���������������ͨ���ͻ��˽ű���JS������ʡ�
	std::string m_sameSite;                          // SameSite=None            ָ����Cookie�Ŀ�վ����Ϊ�����ڷ�ֹCSRF����վ����α�죩��������������ΪStrict��Lax��None��Strict��ʾ��ȫ��ֹ������Cookie��Lax��ʾ���ڶ�������ʱ����Cookie��None��ʾ���Ƿ���Cookie����ʹ�ǿ�վ����Ҳ���͡���վ����α�����α��һ������վ�����������վ������վ�ڲ�������վ     
public:
	Cookie() = default;
	Cookie(const std::string& _name, const std::string& _value);
	~Cookie() = default;

	Cookie(const Cookie&) = default;
	Cookie(Cookie&&) noexcept = default;

	Cookie& operator=(const Cookie&) = default;
	Cookie& operator=(Cookie&&) noexcept = default;

	// void parserSetCookie(const std::string& _value);

	std::string& getName();
	std::string& getValue();
	std::string& getMaxAge();
	std::string& getDomain();
	std::string& getPath();
	std::string& getSameSite();

	void setParamter(const std::string& _name, const std::string& _value);

	bool isSetSecure();
	bool isSetHttpOnly();
	void setSecure(bool _value = true);
	void setHttpOnly(bool _value = true);

	std::string toString();
};













};

};




#endif // !_MOCHEN_COOKIE_H_




