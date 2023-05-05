#ifndef TINY_WEBSERVER_TCPCONNECTION_H_
#define TINY_WEBSERVER_TCPCONNECTION_H_

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <string> 
#include <memory>

#include "callback.h"
#include "channel.h"
#include "buffer.h"
#include "httpcontent.h"

using std::string;

namespace tiny_webserver {

class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    enum ConnectionState {
        kConnected,
        kDisconnected
    };

    TcpConnection(EventLoop* loop, int connfd);
    ~TcpConnection();

    void SetConnectionCallback(const ConnectionCallback& callback) { 
        connection_callback_ = callback;
    }

    void SetMessageCallback(const MessageCallback& callback) {
        message_callback_ = callback;
    }

    void SetCloseCallback(CloseCallback callback) {
        close_callback_ = std::move(callback);
    } 

    void ConnectionEstablished() {
        state_ = kConnected;
        channel_->EnableReading();
        connection_callback_(this, &input_buffer_);
    }

    HttpContent* GetHttpContent() {
        return &content_;
    }

    void Shutdown() {
        if(!channel_->IsWriting()) {
            shutdown_ = true;
            shutdown(connfd_, SHUT_WR);
        }
    }

    bool IsShutdown() const { return shutdown_; }
    void ConnectionDestructor();
    void HandleClose();
    void HandleMessage();
    void HandleWrite();
    void Send(Buffer* buffer);
    void Send(const string& str);
    void Send(const char* message, int len);
    void Send(const char* message) { Send(message, strlen(message)); }

    int fd() const { return connfd_; } 
    EventLoop* loop() const { return loop_; }
 
private:
    // 接受connfd_发来的数据到buf_缓冲区
    // int Recv() {
    //     memset(buf_, '\0', sizeof(buf_));
    //     int ret = recv(connfd_, buf_, 1499, 0);
    //     return ret;
    // }

    ConnectionState state_;
    EventLoop* loop_;
    bool shutdown_;
    int connfd_;
    std::unique_ptr<Channel> channel_;
    Buffer input_buffer_;
    Buffer output_buffer_;
    HttpContent content_;
    // char buf_[1500] = {0};

    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
    CloseCallback close_callback_;
};

}  // namespace tiny_webserver
#endif