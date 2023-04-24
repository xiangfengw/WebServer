#include "echo.h"
#include "address.h"
#include "eventloop.h"

using namespace tiny_webserver;

int main() {
    Address address("8888");
    EventLoop loop;
    EchoServer server(&loop, address);
    
    server.Start();
    loop.Loop();

    return 0;
}