#include <stdio.h>
#include <string>

#include "echo.h"
#include "address.h"
#include "eventloop.h"

#include "httpserver.h"
#include "httprequest.h"
#include "httpresponse.h"

using namespace tiny_webserver;

using tiny_webserver::Method;
using tiny_webserver::HttpStatusCode;

void HttpResponseCallback(const HttpRequest& request, HttpResponse& response) {
    if (request.method() != kGet) {
        response.SetStatusCode(k400BadRequest);
        response.SetStatusMessage("Bad Request");
        response.SetCloseConnection(true);
        return;
    }    

    {
        const string& path = request.path(); 
        if (path == "/") {
            response.SetStatusCode(k200OK);
            response.SetBodyType("text/plain");
            response.SetBody("Hello, world!\n");
        } else if (path == "/hello") {
            response.SetStatusCode(k200OK);
            response.SetBodyType("text/plain");
            response.SetBody("Hello, world!\n");
        } else {
            response.SetStatusCode(k404NotFound);
            response.SetStatusMessage("Not Found");
            response.SetBody("Sorry, Not Found\n");
            response.SetCloseConnection(true);
            return;
        }
    }
}

int main(int argc , char* argv[])
{
    if (argc <= 1)
    {
        printf( "Usage: %s portname\n", argv[0] );
        return 0;
    }

    EventLoop loop;
    Address listen_address(argv[1]);    // 8888
    HttpServer server(&loop, listen_address);
    server.SetHttpResponseCallback(HttpResponseCallback);
    server.Start();
    loop.Loop();
  
    return 0;
}

#if 0
int main() {
    Address address("8888");
    EventLoop loop;
    EchoServer server(&loop, address);
    
    server.Start();
    loop.Loop();

    return 0;
}
#endif