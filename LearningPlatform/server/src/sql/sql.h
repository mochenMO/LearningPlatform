/*// ˵�������
* 1.��ע��������ı����ʽ���Ƕ��ֽڻ��ǿ��ֽڣ���ʵ���õ��ǿ��ֽڡ�
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
	SQLHENV m_henv;       // SQL������������ڹ�������
	SQLHDBC m_hdbc;       // ���Ӿ���������������ݿ�
	SQLHSTMT m_hstmt;     // ���������������SQL���
	SQLRETURN m_res;      // ����ֵ�����ڽ���SQL��غ����ķ���ֵ

	SQLWCHAR m_ODBCName[64];        // ����ODBC����������SQLWCHAR ͬ wchar_t��
	SQLWCHAR m_userName[64];        // ���ڵ�¼SQL���û���
	SQLWCHAR m_password[64];        // �û�����Ӧ������

	SQLWCHAR m_commend[2048];       // Ҫִ��SQL���
	SQLSMALLINT m_numCols;          // ��������

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
