#ifndef TINY_WEBSERVER_TCPSERVER_H_
#define TINY_WEBSERVER_TCPSERVER_H_

#include <functional>
#include <unordered_map>
#include <memory>

#include "callback.h"
#include "eventloop.h"
#include "acceptor.h"
#include "eventloopthreadpool.h"
#include "tcpconnection.h"

namespace tiny_webserver {

class Address;

class TcpServer {
public:
    TcpServer(EventLoop* loop, const Address& address);
    ~TcpServer();

    void Start() {
        threads_->StartLoop();
        loop_->RunInLoop(std::bind(&Acceptor::Listen, acceptor_.get()));
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

    void HandleNewConnection(int connfd);
    void HandleClose(const TcpConnectionPtr& conn);
    void HandleCloseInLoop(const TcpConnectionPtr& ptr);   
private:
    typedef std::unordered_map<int, TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    std::unique_ptr<EventLoopThreadPool> threads_;
    std::unique_ptr<Acceptor> acceptor_;
  
    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
    ConnectionMap connections_;
};

}// namespace tiny_webserver
#endif