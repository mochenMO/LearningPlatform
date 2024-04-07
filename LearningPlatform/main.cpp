
#include "server/src/webserver/webserver.h"

using namespace mochen;




int main()
{
	// http://127.123.123.1:8888/path/to/resource?param1=value1&param2=value2&param3=value3#section1   (注意：锚点信息仅限于浏览器端，浏览器不会把它发给服务器)

	webserver::WebServer webServer("127.123.123.1", 8888);
	webServer.startup();


	return 0;
}











