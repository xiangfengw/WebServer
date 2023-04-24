#ifndef TINY_WEBSERVER_TCPSERVER_H_
#define TINY_WEBSERVER_TCPSERVER_H_

#include <functional>

#include "callback.h"
#include "eventloop.h"
#include "acceptor.h"
#include "eventloopthreadpool.h"

namespace tiny_webserver {

class Address;

class TcpServer {
public:
    TcpServer(EventLoop* loop, const Address& address);
    ~TcpServer();

    void Start() {
        threads_->StartLoop();
        loop_->RunInLoop(std::bind(&Acceptor::Listen, acceptor_));
    }

    void SetConnectionCallback(const ConnectionCallback& callback) { 
        connection_callback_ = callback;
    }

    void SetMessageCallback(const MessageCallback& callback) {
        message_callback_ = callback;
    }

    void SetThreadNum(int thread_num) {
        threads_->SetThreadNum(thread_num);
    }

    void NewConnection(int connfd);
private:
  EventLoop* loop_;
  EventLoopThreadPool* threads_;
  Acceptor* acceptor_;
  
  ConnectionCallback connection_callback_;
  MessageCallback message_callback_;
};

}// namespace tiny_webserver
#endif

