/*// 说明与介绍
* 1.请注意编译器的编码格式，是多字节还是宽字节，该实现用的是宽字节。
*
*
*
*/



#pragma once
#ifndef _MOCHEN_SQL_H_
#define _MOCHEN_SQL_H_

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>

#include <stdio.h>

#include <vector>
#include <string>
#include <map>


namespace mochen
{

namespace sql
{



class SQLServer
{
public:
	using SQLData = std::vector<std::map<std::string, std::string>>;
private:
	SQLHENV m_henv;       // SQL环境句柄，用于构建环境
	SQLHDBC m_hdbc;       // 连接句柄，用于连接数据库
	SQLHSTMT m_hstmt;     // 语句句柄，用于运行SQL语句
	SQLRETURN m_res;      // 返回值，用于接收SQL相关函数的返回值

	SQLWCHAR m_ODBCName[64];        // 所用ODBC的连接名（SQLWCHAR 同 wchar_t）
	SQLWCHAR m_userName[64];        // 用于登录SQL的用户名
	SQLWCHAR m_password[64];        // 用户名对应的密码

	SQLWCHAR m_commend[2048];       // 要执行SQL语句
	SQLSMALLINT m_numCols;          // 保存列数

	SQLData m_data;
public:
	SQLServer(const std::string& _ODBCName, const std::string& _userName, const std::string& _password);
	~SQLServer();

	SQLServer(const SQLServer&) = delete;
	SQLServer(SQLServer&&) = delete;

	const SQLServer& operator=(const SQLServer&) = delete;
	const SQLServer& operator=(SQLServer&&) = delete;

	bool openDataBase(const std::string& _DBName);

	bool executeCommend(const std::string& _comment);
	bool add(const std::string& _comment);
	bool del(const std::string& _comment);
	bool update(const std::string& _comment);
	SQLData select(const std::string& _comment);

	void print(SQLData& _SQLData);
};









};

};



#endif // !_MOCHEN_SQL_H_
