
#include "server/src/webserver/webserver.h"
#include "server/src/json/json.h"


using namespace mochen;




int main()
{
	// ���� http://127.123.123.1:8888/path/to/resource?param1=value1&param2=value2&param3=value3#section1   (ע�⣺ê����Ϣ������������ˣ�����������������������)

	json::JsonParser jsonParser{};
	jsonParser.open("server/config/webserver.json");
	json::Json data = jsonParser.parse();

	webserver::WebServer webServer(data["ip"].getString(), data["port"].getInt());
	webServer.getMaxKeepTime() = data["maxWriteTime"].getInt();
	webServer.createSQLServer(data["dataBase"]["ODBCName"].getString(),
		data["dataBase"]["userName"].getString(),
		data["dataBase"]["password"].getString());
	webServer.startup();  // ע�⣺��ȫ��ʼ�� WebServer �ڲ��������Դ������ٵ��� startup ����


	return 0;
}











