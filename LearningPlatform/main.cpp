
#include "server/src/webserver/webserver.h"
#include "server/src/json/json.h"


using namespace mochen;




int main()
{
	// 测试 http://127.123.123.1:8888/path/to/resource?param1=value1&param2=value2&param3=value3#section1   (注意：锚点信息仅限于浏览器端，浏览器不会把它发给服务器)
	
	json::JsonParser jsonParser{};
	jsonParser.open("server/config/webserver.json");
	json::Json data = jsonParser.parse();

	webserver::WebServer webServer(data["ip"].asString(), data["port"].asInt());
	webServer.getMaxKeepTime() = data["maxWriteTime"].asInt();
	webServer.createSQLServer(data["dataBase"]["ODBCName"].asString(),
		data["dataBase"]["userName"].asString(),
		data["dataBase"]["password"].asString());

	// 配置 config.js
	FILE* fp = fopen(data["jsConfigPath"].asString().c_str(), "w");
	if (fp == nullptr) {
		printf("fopen() \n");  // !@#$#@$!#$!#$!#@$!#$!#$!#$!#$!#$
		return -1;
	}
	fprintf(fp, "let urlPath = \"http://%s:%d\"", data["ip"].asString().c_str(), data["port"].asInt());   // let urlPath = "http://127.123.123.1:8888"
	fclose(fp);

	webServer.startup();  // 注意：完全初始化 WebServer 内部所需的资源后，最后再调用 startup 函数


	return 0;
}











