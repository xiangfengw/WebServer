#ifndef TINY_WEBSERVER_CHANNEL_H_
#define TINY_WEBSERVER_CHANNEL_H_

#include <sys/epoll.h>

#include "eventloop.h"
#include "callback.h"

namespace tiny_webserver {

class Channel
{
public:
    Channel(EventLoop* loop, const int fd);
    ~Channel();

    void HandleEvent();

    void SetReadCallBack(const ReadCallback& callback) {
        read_callback_ = callback;
    }
    
    void SetWriteCallBack(const WriteCallback& callback) {
        write_callback_ = callback;
    }

    void EnableReading() {
        events_ |= EPOLLIN;
        Update();
    }

    void EnableReading() {
        events_ |= EPOLLOUT;
        Update();
    }

    void Update() {
        loop_->Update(this);
    }

    void SetReceivedEvents(int revt) {
        recv_events_ = revt;
    }

    int fd() { return fd_; }
    int events() { return events_; }
    int recv_events() { return recv_events_; }

private:
    EventLoop* loop_;
    int fd_;
    int events_;           // fd当前处理（注册）的事件
    int recv_events_;      // poller设置的就绪事件
    
    ReadCallback read_callback_;
    WriteCallback write_callback_;
};

}
#endif