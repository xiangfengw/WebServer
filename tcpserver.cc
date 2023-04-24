#include "tcpserver.h"
#include "acceptor.h"
#include "tcpconnection.h"
#include "eventloopthreadpool.h"

using namespace tiny_webserver;

TcpServer::TcpServer(EventLoop* loop, const Address& address)
    : acceptor_(new Acceptor(loop, address)),
      loop_(loop),
      threads_(new EventLoopThreadPool(loop_)) {
    acceptor_->SetNewConnectionCallback(std::bind(&TcpServer::NewConnection, this, _1));
}

TcpServer::~TcpServer() {
    delete threads_;
    delete acceptor_;
}

void TcpServer::NewConnection(int connfd) {
    EventLoop* loop = threads_->GetNextLoop();
    printf("TcpServer NewConnection Arrive Tid:%ld Manage\n", loop->DebugShowTid());

    TcpConnection* conn = new TcpConnection(loop_, connfd);
    conn->SetConnectionCallback(connection_callback_);
    conn->SetMessageCallback(message_callback_);
    loop_->RunInLoop(std::bind(&TcpConnection::ConnectionEstablished, conn));
}