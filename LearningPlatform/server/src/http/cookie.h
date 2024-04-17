
/*// 介绍与说明
* 1.该类只是实现了几个常用的Cookie字段
* 2.每个Cookie类只能存一对键值对，如果要以此设置多个Cookie有如下两者方法
*	方法一：写两个Set-Cookie中（由于http报文用std::map存键值对的，不能存两个Set-Cookie，因此后面的Set-Cookie全部按值处理）
*   Set-Cookie: cookie1=value1; Path=/; Max-Age = 3600
*   Set-Cookie: cookie2=value2; Path=/; Max-Age = 3600
*   方法二：写在一个Set-Cookie中，并用","分隔 (可能只取第一个Cookie值)
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
	std::string m_name;                              // 字段名
	std::string m_value;                             // 字段值
	std::string m_maxAge;                            // Max-Age=3600 单位是秒。   指定Cookie存放时间, 默认是一个会话，即关闭浏览器时过期。Max-Age 比 Expires 更常用，因为 Expires 格式复杂且，浏览器和服务器之间的时钟可能不同步。（Expires = Weekday, DD Mon YYYY HH:MM:SS GMT）
	std::string m_domain;                            // Domain = abc.com         指定Cookie可以发送到哪个域名。默认为设置Cookie的页面所在的域名。
	std::string m_path;                              // Path = /login            指定了Cookie的路径，表示该Cookie应用于哪些路径。默认为设置为接收该Cookie的页面的路径。Path=/ 表示所有页面
	std::string m_secure;                            // Secure                   安全标志，表示该Cookie只有在通过HTTPS协议发送时才会被发送到服务器
	std::string m_HttpOnly;                          // HttpOnly                 表示该Cookie仅能通过HTTP或HTTPS协议发送给服务器，而不能通过客户端脚本即JS代码访问。
	std::string m_sameSite;                          // SameSite=None            指定了Cookie的跨站点行为，用于防止CSRF（跨站请求伪造）攻击。可以设置为Strict、Lax或None。Strict表示完全禁止第三方Cookie；Lax表示仅在顶级导航时发送Cookie；None表示总是发送Cookie，即使是跨站请求也发送。跨站请求伪造就是伪造一个假网站，你操作假网站，假网站在操作真网站     
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




