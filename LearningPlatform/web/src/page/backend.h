#pragma once
#ifndef _MOCHEN_BACKEND_PAGE_H_    // 注意改预处理名！！！！！
#define _MOCHEN_BACKEND_PAGE_H_


#include "../../../server/src/route/route.h"
#include "../data/data.h"

#include <iostream>


namespace mochen
{

namespace page
{


inline json::Json SQLDataToJson(sql::SQLServer::SQLData& _data)
{
	json::Json res(json::Json::Type::json_array);
	json::Json temp;

	for (auto itVec = _data.begin(); itVec != _data.end();++itVec) {
		for(auto itMap = itVec->begin(); itMap != itVec->end(); ++itMap){
			temp[itMap->first] = itMap->second;
		}
		res.append(temp);
		temp.clear();
	}
	
	return res;
}


inline std::string getSelectComment(const std::string& _tableName, const std::string& _listName, int _start, int _size)
{
	std::stringstream commend{};
	commend << "select * from ";
	commend << _tableName;
	commend << " order by ";
	commend << _listName;
	commend <<" offset ";
	commend << _start;
	commend << " rows fetch next ";
	commend << _size;
	commend << " rows only;";
	return commend.str();
}


inline std::string getSearchComment(const std::string& _tableName, const std::string& _listName, const std::string& _tag)
{
	std::stringstream commend{};
	commend << "select * from ";
	commend << _tableName;
	commend << " where " << _listName << " like";
	commend << "'%" << _tag << "%';";
	return commend.str();
}


inline std::string getUpdateUserDataComment(const std::string& _tableName, json::Json _json)
{
	std::stringstream commend{};
	commend << "update " << _tableName << " set ";
	commend << "utype = " << "'" << _json["utype"].asString() << "',";
	commend << "uaccount = " << "'" << _json["uaccount"].asString() << "',";
	commend << "upassword = " << "'" << _json["upassword"].asString() << "',";
	commend << "uphone = " << "'" << _json["uphone"].asString() << "',";
	commend << "uemail = " << "'" << _json["uemail"].asString() << "',";
	commend << "uage = " << "'" << _json["uage"].asString() << "' ";
	commend << "where uid = "  << "'" << _json["uid"].asString() << "';";
	return commend.str();
}


inline void backendPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	http::HttpURL httpUrl = _httpServerRequest.getUrl();

	if (httpUrl.isFindParamter("get") == true) {    // 请求资源

		int page = atoi(httpUrl.getParamter("page").c_str());
		int size = atoi(httpUrl.getParamter("size").c_str());
		int n = (page - 1) * size;

		std::string commend;
		if (httpUrl.getParamter("get") == "userData") {
			commend = getSelectComment("user_tb", "uid", n, size);
		}


		sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
		sqlServer->openDataBase("LEARNING");
		sql::SQLServer::SQLData data = sqlServer->select(commend);

		std::string temp = SQLDataToJson(data).toString();

		printf("%s", temp.c_str());

		// _httpServerResqonse.setParamter("Content-Type", "application/json; charset=utf-8");
		// _httpServerResqonse.getData() = temp;

		_httpServerResqonse.setData("json", temp);

	}
	else if (httpUrl.isFindParamter("search") == true) {    

		std::string commend;
		if (httpUrl.getParamter("search") == "userData") {
			commend = getSearchComment("user_tb", "uaccount", httpUrl.getParamter("tag"));
		}

		sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
		sqlServer->openDataBase("LEARNING");
		sql::SQLServer::SQLData data = sqlServer->select(commend);


		std::string temp = SQLDataToJson(data).toString();

		printf("%s", temp.c_str());

		//_httpServerResqonse.setParamter("Content-Type", "application/json; charset=utf-8");
		//_httpServerResqonse.getData() = temp;

		_httpServerResqonse.setData("json", temp);
	}
	else if (httpUrl.isFindParamter("update") == true) {

		json::JsonParser jp(_httpServerRequest.getData());
		json::Json json = jp.parse();

		std::string commend;
		if (httpUrl.getParamter("update") == "userData") {
			commend = getUpdateUserDataComment("user_tb", json);
		}

		sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
		sqlServer->openDataBase("LEARNING");
		sqlServer->update(commend);
	}
	else if (httpUrl.isFindParamter("delete") == true) {


		char commend[2048] = { 0 };

		json::JsonParser jp(_httpServerRequest.getData());

		
		if (httpUrl.getParamter("delete") == "userData") {
			sprintf(commend, "delete from user_tb where uid = '%s';", jp.parse()["uid"].asString().c_str());
		}

		sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
		sqlServer->openDataBase("LEARNING");
		sqlServer->update(commend);

	}

}


};

};


#endif // !_MOCHEN_BACKEND_PAGE_H_

