
#include "server/src/webserver/webserver.h"
#include "server/src/json/json.h"


using namespace mochen;




int main()
{
	// ���� http://127.123.123.1:8888/path/to/resource?param1=value1&param2=value2&param3=value3#section1   (ע�⣺ê����Ϣ������������ˣ�����������������������)
	
	json::JsonParser jsonParser{};
	jsonParser.open("server/config/webserver.json");
	json::Json data = jsonParser.parse();

	webserver::WebServer webServer(data["ip"].asString(), data["port"].asInt());
	webServer.getMaxKeepTime() = data["maxWriteTime"].asInt();
	webServer.createSQLServer(data["dataBase"]["ODBCName"].asString(),
		data["dataBase"]["userName"].asString(),
		data["dataBase"]["password"].asString());

	// ���� config.js
	FILE* fp = fopen(data["jsConfigPath"].asString().c_str(), "w");
	if (fp == nullptr) {
		printf("fopen() \n");  // !@#$#@$!#$!#$!#@$!#$!#$!#$!#$!#$
		return -1;
	}
	fprintf(fp, "let urlPath = \"http://%s:%d\"", data["ip"].asString().c_str(), data["port"].asInt());   // let urlPath = "http://127.123.123.1:8888"
	fclose(fp);

	webServer.startup();  // ע�⣺��ȫ��ʼ�� WebServer �ڲ��������Դ������ٵ��� startup ����


	return 0;
}











