#ifndef TINY_WEBSERVER_TCPSERVER_H_
#define TINY_WEBSERVER_TCPSERVER_H_

#include <functional>

#include "callback.h"
#include "eventloop.h"
#include "acceptor.h"

namespace tiny_webserver {

class Address;
class EventLoopThreadPoll;

class TcpServer {
public:
    TcpServer(EventLoop* loop, const Address& address);
    ~TcpServer();

    void Start() {
        //threads_.Start();
        loop_->RunOneFunc(std::bind(&Acceptor::Listen, acceptor_));
    }

    void SetConnectionCallback(const ConnectionCallback& callback) { 
        connection_callback_ = callback;
    }

    void SetMessageCallback(const MessageCallback& callback) {
        message_callback_ = callback;
    }

    void NewConnection(int connfd);
private:
  EventLoop* loop_;
  EventLoopThreadPoll* threads_;
  Acceptor* acceptor_;
  
  ConnectionCallback connection_callback_;
  MessageCallback message_callback_;
};

}// namespace tiny_webserver
#endif

