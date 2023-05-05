#include "tcpserver.h"
#include "acceptor.h"
#include "tcpconnection.h"
#include "eventloopthreadpool.h"

using namespace tiny_webserver;

TcpServer::TcpServer(EventLoop* loop, const Address& address)
    : acceptor_(new Acceptor(loop, address)),
      loop_(loop),
      threads_(new EventLoopThreadPool(loop_)) {
    acceptor_->SetNewConnectionCallback(std::bind(&TcpServer::HandleNewConnection, this, _1));
}

TcpServer::~TcpServer() {
    for (auto& pair : connections_) {
        TcpConnectionPtr ptr(pair.second);
        pair.second.reset();
        ptr->loop()->RunInLoop(std::bind(&TcpConnection::ConnectionDestructor, ptr));
    }
}

void TcpServer::HandleClose(const TcpConnectionPtr& ptr) {
    loop_->QueueOneFunc(std::bind(&TcpServer::HandleCloseInLoop, this, ptr));
}

void TcpServer::HandleCloseInLoop(const TcpConnectionPtr& ptr) {
    assert(connections_.find(ptr->fd()) != connections_.end());
    connections_.erase(connections_.find(ptr->fd()));
    EventLoop* loop = ptr->loop(); 
    loop->QueueOneFunc(std::bind(&TcpConnection::ConnectionDestructor, ptr));
}

void TcpServer::HandleNewConnection(int connfd) {
    EventLoop* loop = threads_->GetNextLoop(); 

    TcpConnectionPtr ptr(new TcpConnection(loop, connfd));
    connections_[connfd] = ptr;

    ptr->SetConnectionCallback(connection_callback_);
    ptr->SetMessageCallback(message_callback_);
    ptr->SetCloseCallback(std::bind(&TcpServer::HandleClose, this, _1));
    loop->RunInLoop(std::bind(&TcpConnection::ConnectionEstablished, ptr));
}