#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "tcpconnection.h"
#include "channel.h"

using namespace tiny_webserver;

TcpConnection::TcpConnection(EventLoop* loop, int connfd)
    : loop_(loop),
      connfd_(connfd),
      channel_(new Channel(loop_, connfd_)) {
    channel_->SetReadCallback(std::bind(&TcpConnection::HandleMessage, this));
}

void TcpConnection::HandleMessage() {
    while(1) {
        int ret = Recv();
        if(ret > 0) {
            message_callback_(this);
        } else if (ret == 0) {
            std::cout << "conn finished" << std::endl;
            // handleClose();
            break;
        } else {
            if(errno == EINTR) {
                continue;
            }
            break;
        }
    }
}

void TcpConnection::Send(const string& message) {
    strcpy(buf_, message.c_str());
    send(connfd_, (const void *)(buf_), sizeof(buf_), 0);
}

std::string TcpConnection::Get() {
    string message(buf_);
    memset(buf_, '\0', sizeof(buf_));
    return message;
}