#ifndef TINY_WEBSERVER_ECHO_H
#define TINY_WEBSERVER_ECHO_H

#include <stdio.h>
#include <string>

#include "tcpserver.h"
#include "tcpconnection.h"

namespace tiny_webserver {

    class Address;
    class EventLoop;

}

class EchoServer {
public:
    EchoServer(tiny_webserver::EventLoop* loop, const tiny_webserver::Address& listen_addr);
    ~EchoServer() {}

    void Start() {
        server_.Start();
    }

    void ConnectionCallback(tiny_webserver::TcpConnection* connection) {
        printf("echo_server has a new connection \n");  
    }

    void MessageCallback(tiny_webserver::TcpConnection* connection) {
        std::string message(connection->Get());
        printf("echo_server get message \n");
        connection->Send(message);
    }

private:
    tiny_webserver::EventLoop* loop_;
    tiny_webserver::TcpServer server_; 
};

#endif