
#include "server/src/webserver/webserver.h"

using namespace mochen;




int main()
{
	// http://127.123.123.1:8888/path/to/resource?param1=value1&param2=value2&param3=value3#section1   (ע�⣺ê����Ϣ������������ˣ�����������������������)

	webserver::WebServer webServer("127.123.123.1", 8888);
	webServer.startup();


	return 0;
}











