#include "sql.h"

using namespace mochen::sql;


SQLServer::SQLServer(const std::string& _ODBCName, const std::string& _userName, const std::string& _password)
{
	// 申请环境句柄
	m_res = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return;
	}
	// 设置环境句柄
	m_res = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return;
	}
	// 申请数据库连接句柄
	m_res = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return;
	}

	mbstowcs(m_ODBCName, _ODBCName.c_str(), sizeof(m_ODBCName)); // 将 char 转成 wchar_t  
	mbstowcs(m_userName, _userName.c_str(), sizeof(m_userName));
	mbstowcs(m_password, _password.c_str(), sizeof(m_password));

	// 连接数据库
	m_res = SQLConnect(m_hdbc, m_ODBCName, SQL_NTS, m_userName, SQL_NTS, m_password, SQL_NTS);
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return;
	}
}


SQLServer::~SQLServer()
{
	SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);   // 释放语句句柄
	SQLDisconnect(m_hdbc);                     // 断开SQL连接
	SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);     // 释放连接句柄
	SQLFreeHandle(SQL_HANDLE_ENV, m_henv);     // 释放环境句柄
}

bool SQLServer::executeCommend(const std::string& _comment)
{
	mbstowcs(m_commend, _comment.c_str(), sizeof(m_commend));         // 将 char 转成 wchar_t 
	m_res = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);        // 每次执行一条新的 SQL 语句时,都需要通过 SQLAllocHandle() 函数分配一个新的语句句柄,因为当执行完一条 SQL 语句后，ODBC 驱动程序将自动清除语句的状态
	m_res = SQLExecDirect(m_hstmt, m_commend, SQL_NTS);               // 执行SQL语句，使用指定的数据库
	memset(m_commend, 0, sizeof(m_commend));                          // 清空命令数据
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return false;
	}
	return true;
}

bool SQLServer::openDataBase(const std::string& _DBName)
{
	std::string commend = "use ";   // 注意：最后有个空格
	commend += _DBName;
	return executeCommend(commend);
}

bool SQLServer::add(const std::string& _comment)
{
	return executeCommend(_comment);
}

bool SQLServer::del(const std::string& _comment)
{
	return executeCommend(_comment);
}

bool SQLServer::update(const std::string& _comment)
{
	return executeCommend(_comment);
}

SQLServer::SQLData SQLServer::select(const std::string& _comment)
{
	SQLServer::SQLData data{};
	std::vector<std::string> colNameArray{};

	if (executeCommend(_comment) == false) {
		printf("logger logger logger\n");
		return data;
	}

	m_res = SQLNumResultCols(m_hstmt, &m_numCols);    // 于获取该结果的列数

	SQLWCHAR wcolName[SQL_MAX_COLUMN_NAME_LEN] = { 0 };
	SQLSMALLINT colNameLen;
	SQLSMALLINT dataType;
	SQLULEN colSize;
	SQLSMALLINT decimalDigits;
	SQLSMALLINT nullable;
	char colName[SQL_MAX_COLUMN_NAME_LEN] = { 0 };
	
	// 获取列名、数据类型、列大小等数据
	for (SQLSMALLINT i = 1; i <= m_numCols; i++) {
		m_res = SQLDescribeCol(m_hstmt, i, wcolName, SQL_MAX_COLUMN_NAME_LEN, &colNameLen, &dataType, &colSize, &decimalDigits, &nullable);
		if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
			printf("logger logger logger\n");
			return data;
		}
		wcstombs(colName, wcolName, sizeof(colName));
		colNameArray.push_back(colName);
	}

	char tempData[1024] = { 0 };                 // 保存每一格的数据（这里用的char 不是 wchar）    
	int numRow = 0;
	while (SQLFetch(m_hstmt) == SQL_SUCCESS) {      // 开始获取数据，注意该函数是一行一行读取的。
		data.push_back({});;
		for (int i = 1; i <= m_numCols; ++i) {
			m_res = SQLGetData(m_hstmt, i, SQL_C_CHAR, tempData, sizeof(tempData), NULL);  // 获取该行中第i格的数据，i∈[1,numCols]。注意他会把其他数据类型都转成 SQLCHAR 类型
			if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
				printf("logger logger logger\n");
				return data;
			}
			data[numRow][colNameArray[i - 1]] = tempData;   // 注意：colNameArray[i-1]
			memset(tempData, 0, sizeof(tempData));
		}
		++numRow;   // 获取下一行的的数据
	}
	return data;
}


void SQLServer::print(SQLServer::SQLData& _SQLData)
{
	for (auto itMap = _SQLData.begin(); itMap != _SQLData.end(); ++itMap) {
		for (auto it = itMap->begin(); it != itMap->end(); ++it) {
			printf("%s=>%s  ", it->first.c_str(),it->second.c_str());
		}
		printf("\n");
	}
}



















