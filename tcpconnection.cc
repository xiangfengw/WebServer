#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>


#include "tcpconnection.h"
#include "channel.h"
#include "buffer.h"

using namespace tiny_webserver;

TcpConnection::TcpConnection(EventLoop* loop, int connfd)
    : loop_(loop),
      connfd_(connfd),
      state_(kDisconnected),
      shutdown_(false),
      channel_(new Channel(loop_, connfd_)) {
    channel_->SetReadCallback(std::bind(&TcpConnection::HandleMessage, this));
    channel_->SetWriteCallback(std::bind(&TcpConnection::HandleWrite, this));
}

TcpConnection::~TcpConnection() {
  //printf("TcpConnection::~TcpConnection destructor\n");
  ::close(connfd_);
}

void TcpConnection::ConnectionDestructor() {
    if (state_ == kConnected) {
        state_ = kDisconnected;
        channel_->DisableAll();
    }
    loop_->Remove(channel_.get());
}

void TcpConnection::HandleClose() {
    state_ = kDisconnected;
    channel_->DisableAll();

    TcpConnectionPtr guard(shared_from_this());
    close_callback_(guard);
}

void TcpConnection::HandleMessage() {
    
    int read_size = input_buffer_.ReadFd(connfd_);
    if(read_size > 0) {
        message_callback_(this, &input_buffer_);
    } else if(read_size == 0) {
        HandleClose();
    }
    // while(1) {
        // int ret = Recv();
        // if(ret > 0) {
        //     message_callback_(this);
        // } else if (ret == 0) {
        //     std::cout << "conn finished" << std::endl;
        //     // handleClose();
        //     break;
        // } else {
        //     if(errno == EINTR) {
        //         continue;
        //     }
        //     break;
        // }
    // }
}

void TcpConnection::HandleWrite() {
    int len = output_buffer_.readablebytes();
    int remaining = len;
    int send_size = write(connfd_, output_buffer_.Peek(), remaining);
    remaining -= send_size;
    output_buffer_.Retrieve(send_size);

    assert(remaining <= len);
    if (!remaining) {
        channel_->DisableWriting();
    }
}

void TcpConnection::Send(const char* message, int len) {
    int remaining = len;
    int send_size = 0;
    if (!channel_->IsWriting() && output_buffer_.readablebytes() == 0) {
      send_size = send(connfd_, message, len, 0);
      remaining -= send_size; 
    }

    assert(remaining <= len);
    if (remaining > 0) {
      output_buffer_.Append(message + send_size, remaining);
      if (!channel_->IsWriting()) {
        channel_->EnableWriting(); 
      }
    }
}

void TcpConnection::Send(Buffer* buffer) {
    Send(buffer->Peek(), buffer->readablebytes());
    buffer->RetrieveAll();
}

void TcpConnection::Send(const string& message) {
    Send(message.data(), sizeof(message.size()));
}

// void TcpConnection::Send(const string& message) {
//     strcpy(buf_, message.c_str());
//     send(connfd_, (const void *)(buf_), sizeof(buf_), 0);
// }

// std::string TcpConnection::Get() {
//     string message(buf_);
//     memset(buf_, '\0', sizeof(buf_));
//     return message;
// }