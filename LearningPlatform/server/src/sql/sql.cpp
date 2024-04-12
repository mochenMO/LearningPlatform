#include "sql.h"

using namespace mochen::sql;


SQLServer::SQLServer(const std::string& _ODBCName, const std::string& _userName, const std::string& _password)
{
	// ���뻷�����
	m_res = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return;
	}
	// ���û������
	m_res = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return;
	}
	// �������ݿ����Ӿ��
	m_res = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return;
	}

	mbstowcs(m_ODBCName, _ODBCName.c_str(), sizeof(m_ODBCName)); // �� char ת�� wchar_t  
	mbstowcs(m_userName, _userName.c_str(), sizeof(m_userName));
	mbstowcs(m_password, _password.c_str(), sizeof(m_password));

	// �������ݿ�
	m_res = SQLConnect(m_hdbc, m_ODBCName, SQL_NTS, m_userName, SQL_NTS, m_password, SQL_NTS);
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return;
	}
}


SQLServer::~SQLServer()
{
	SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);   // �ͷ������
	SQLDisconnect(m_hdbc);                     // �Ͽ�SQL����
	SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);     // �ͷ����Ӿ��
	SQLFreeHandle(SQL_HANDLE_ENV, m_henv);     // �ͷŻ������
}

bool SQLServer::executeCommend(const std::string& _comment)
{
	mbstowcs(m_commend, _comment.c_str(), sizeof(m_commend));         // �� char ת�� wchar_t 
	m_res = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);        // ÿ��ִ��һ���µ� SQL ���ʱ,����Ҫͨ�� SQLAllocHandle() ��������һ���µ������,��Ϊ��ִ����һ�� SQL ����ODBC ���������Զ��������״̬
	m_res = SQLExecDirect(m_hstmt, m_commend, SQL_NTS);               // ִ��SQL��䣬ʹ��ָ�������ݿ�
	memset(m_commend, 0, sizeof(m_commend));                          // �����������
	if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
		printf("logger logger logger\n");
		return false;
	}
	return true;
}

bool SQLServer::openDataBase(const std::string& _DBName)
{
	std::string commend = "use ";   // ע�⣺����и��ո�
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

	m_res = SQLNumResultCols(m_hstmt, &m_numCols);    // �ڻ�ȡ�ý��������

	SQLWCHAR wcolName[SQL_MAX_COLUMN_NAME_LEN] = { 0 };
	SQLSMALLINT colNameLen;
	SQLSMALLINT dataType;
	SQLULEN colSize;
	SQLSMALLINT decimalDigits;
	SQLSMALLINT nullable;
	char colName[SQL_MAX_COLUMN_NAME_LEN] = { 0 };
	
	// ��ȡ�������������͡��д�С������
	for (SQLSMALLINT i = 1; i <= m_numCols; i++) {
		m_res = SQLDescribeCol(m_hstmt, i, wcolName, SQL_MAX_COLUMN_NAME_LEN, &colNameLen, &dataType, &colSize, &decimalDigits, &nullable);
		if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
			printf("logger logger logger\n");
			return data;
		}
		wcstombs(colName, wcolName, sizeof(colName));
		colNameArray.push_back(colName);
	}

	char tempData[1024] = { 0 };                 // ����ÿһ������ݣ������õ�char ���� wchar��    
	int numRow = 0;
	while (SQLFetch(m_hstmt) == SQL_SUCCESS) {      // ��ʼ��ȡ���ݣ�ע��ú�����һ��һ�ж�ȡ�ġ�
		data.push_back({});;
		for (int i = 1; i <= m_numCols; ++i) {
			m_res = SQLGetData(m_hstmt, i, SQL_C_CHAR, tempData, sizeof(tempData), NULL);  // ��ȡ�����е�i������ݣ�i��[1,numCols]��ע������������������Ͷ�ת�� SQLCHAR ����
			if (m_res != SQL_SUCCESS && m_res != SQL_SUCCESS_WITH_INFO) {
				printf("logger logger logger\n");
				return data;
			}
			data[numRow][colNameArray[i - 1]] = tempData;   // ע�⣺colNameArray[i-1]
			memset(tempData, 0, sizeof(tempData));
		}
		++numRow;   // ��ȡ��һ�еĵ�����
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



















