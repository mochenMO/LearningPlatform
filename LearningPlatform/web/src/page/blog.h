#pragma once
#ifndef _MOCHEN_BLOG_PAGE_H_    // 注意改预处理名！！！！！
#define _MOCHEN_BLOG_PAGE_H_


#include "../../../server/src/route/route.h"
#include "../data/data.h"

#include <iostream>


namespace mochen
{

namespace page
{


inline json::Json toJson(sql::SQLServer::SQLData& _data)
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


inline void blogPageMainFuntion(httpserver::HttpServerRequest& _httpServerRequest, httpserver::HttpServerResqonse& _httpServerResqonse)
{
	http::HttpURL httpUrl = _httpServerRequest.getUrl();

	if (httpUrl.isFindParamter("get") == true) {    // 请求资源

		int page = atoi(httpUrl.getParamter("page").c_str());
		int size = atoi(httpUrl.getParamter("size").c_str());
		int n = (page - 1) * size;

		std::stringstream commend{};
		commend << "select * from blog_tb order by bid offset ";
		commend << n;
		commend << " rows fetch next ";
		commend << size;
		commend << " rows only;";

		sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
		sqlServer->openDataBase("LEARNING");
		sql::SQLServer::SQLData data = sqlServer->select(commend.str());

		std::string temp = toJson(data).toString();

		printf("%s", temp.c_str());

		_httpServerResqonse.setParamter("Content-Type", "application/json; charset=utf-8");
		_httpServerResqonse.getData() = temp;

		// _httpServerResqonse.setData("json", temp);

	}
	else if (httpUrl.isFindParamter("search") == true) {    // 请求资源
		std::stringstream commend{};
		commend << "select * from blog_tb where btitle like '%";
		commend << httpUrl.getParamter("tag");
		commend << "%';";

		sql::SQLServer* sqlServer = _httpServerRequest.getSQLServer();
		sqlServer->openDataBase("LEARNING");
		sql::SQLServer::SQLData data = sqlServer->select(commend.str());


		std::string temp = toJson(data).toString();

		printf("%s", temp.c_str());

		_httpServerResqonse.setParamter("Content-Type", "application/json; charset=utf-8");
		_httpServerResqonse.getData() = temp;

		// select* from blog_tb where btitle like '%algor%'


	}
	
}


};

};


#endif // !_MOCHEN_BLOG_PAGE_H_

