#ifndef TINY_WEBSERVER_CHANNEL_H_
#define TINY_WEBSERVER_CHANNEL_H_

#include <sys/epoll.h>

#include "eventloop.h"
#include "callback.h"

namespace tiny_webserver {

enum ChannelState {
    kNew,       // 新fd 未添加到epoll关注列表
    kAdded,     // 已经添加
    kDeleted    // 已经删除
};

class Channel
{
public:
    Channel(EventLoop* loop, const int& fd);
    ~Channel();

    void HandleEvent();

    void SetReadCallback(const ReadCallback callback) {
        read_callback_ = std::move(callback);
    }
    
    void SetWriteCallback(const WriteCallback callback) {
        write_callback_ = std::move(callback);
    }

    void EnableReading() {
        events_ |= (EPOLLIN | EPOLLPRI);
        Update();
    }

    void EnableWriting() {
        events_ |= EPOLLOUT;
        Update();
    }
    
    void DisableAll() {
        events_ = 0;
        Update();
    }

    void DisableWriting() {
        events_ &= ~EPOLLOUT;
        Update();  
    }

    void Update() {
        loop_->Update(this);
    }

    void SetReceivedEvents(int revt) {
        recv_events_ = revt;
    }

    void SetChannelState(ChannelState state) {
        state_ = state;
    }

    int fd() const { return fd_; }
    int events() const { return events_; }
    int recv_events() const { return recv_events_; }
    ChannelState state() const { return state_; }

    bool IsWriting() { return events_ & EPOLLOUT; }
    bool IsReading() { return events_ & (EPOLLIN | EPOLLPRI); }

private:
    EventLoop* loop_;
    int fd_;
    int events_;           // fd当前处理（注册）的事件
    int recv_events_;      // poller设置的就绪事件

    ChannelState state_;
    ReadCallback read_callback_;
    WriteCallback write_callback_;
};

}
#endif