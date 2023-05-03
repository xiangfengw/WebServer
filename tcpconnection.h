#ifndef TINY_WEBSERVER_TCPCONNECTION_H_
#define TINY_WEBSERVER_TCPCONNECTION_H_

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <string> 

#include "callback.h"
#include "channel.h"
#include "buffer.h"
#include "httpcontent.h"

using std::string;

namespace tiny_webserver {

class EventLoop;

class TcpConnection {
public:
    TcpConnection(EventLoop* loop, int connfd);
    ~TcpConnection();

    void SetConnectionCallback(const ConnectionCallback& callback) { 
        connection_callback_ = callback;
    }

    void SetMessageCallback(const MessageCallback& callback) {
        message_callback_ = callback;
    }

    void ConnectionEstablished() {
        channel_->EnableReading();
        connection_callback_(this, &input_buffer_);
    }

    HttpContent* GetHttpContent() {
        return &content_;
    }

    void Shutdown() {
        if(!channel_->IsWriting()) {
            shutdown_ = true;
            ::shutdown(connfd_, SHUT_WR);
        }
    }

    bool IsShutdown() const { return shutdown_; } 
    void HandleMessage();
    void HandleWrite();
    void Send(Buffer* buffer);
    void Send(const string& str);
    void Send(const char* message, int len);
    void Send(const char* message) { Send(message, strlen(message)); }

    string Get();
 
private:
    // 接受connfd_发来的数据到buf_缓冲区
    // int Recv() {
    //     memset(buf_, '\0', sizeof(buf_));
    //     int ret = recv(connfd_, buf_, 1499, 0);
    //     return ret;
    // }

    EventLoop* loop_;
    bool shutdown_;
    int connfd_;
    Channel* channel_;
    Buffer input_buffer_;
    Buffer output_buffer_;
    HttpContent content_;
    // char buf_[1500] = {0};

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
};

}  // namespace tiny_webserver
#endif