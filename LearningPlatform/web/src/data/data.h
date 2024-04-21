#pragma once
#ifndef _MOCHEN_DATA_H_
#define _MOCHEN_DATA_H_


#include <string>
#include <map>

namespace mochen
{

namespace webdata
{

	

struct UserData
{
	std::string m_uid;
	std::string m_utype;
	std::string m_uaccount;
	std::string m_upassword;
	std::string m_uphone;
	std::string m_uemail;
	std::string m_uage;
	bool m_isLogin;

	UserData()
	{
		m_uid = "null";
		m_utype = "null";
		m_uaccount = "null";
		m_upassword = "null";
		m_uphone = "null";
		m_uemail = "null";
		m_uage = "null";
		m_isLogin = false;
	}
};


using UserMap = std::map<std::string, webdata::UserData>;


struct BlogData
{
	std::string bid;
	std::string bintroduce;
	std::string bpath;
};






};

};




#endif // !_MOCHEN_USER_H_
