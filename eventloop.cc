#include "eventloop.h"
#include "channel.h"

using namespace tiny_webserver;

EventLoop::EventLoop()
    : tid_(pthread_self()),
      epoller_(new Epoller()),
      wakeup_fd_(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
      wakeup_channel_(new Channel(this, wakeup_fd_)),
      calling_functors_(false) {
    // 设置wakeupchannel的回调函数
    wakeup_channel_->SetReadCallback(std::bind(&EventLoop::HandleRead, this));
    // 总是监听wakeupfd的读事件
    wakeup_channel_->EnableReading();
}

EventLoop::~EventLoop() {
    wakeup_channel_->DisableAll();
    Remove(wakeup_channel_.get());
}

void EventLoop::Loop() {
    while(1) {
        epoller_->Poll(active_channels_);
        printf("EventLoop::Loop eventnum: %d\n", active_channels_.size());
        for(const auto& channel : active_channels_) {
            printf("EventLoop::Loop connfd %d\n", channel->fd());
            channel->HandleEvent();
        }
        active_channels_.clear();
        DoToDoList();
    }
}

// wakefd的读回调函数
// 从wakeupFd读取了值之后并未处理，
// 因为我们只是把wakeupFd实际上是eventfd作为等待/通知机制实现，
// 这里是为了当我们向EventLoop的pendingFunctors数组加入任务时，通过eventfd通知IO线程从poll状态退出来执行任务

void EventLoop::HandleRead() {
    uint64_t read_one_byte = 1;
    read(wakeup_fd_, &read_one_byte, sizeof(read_one_byte));
}

// 在I/O线程中调用某个函数，该函数可以跨线程调用
void EventLoop::RunInLoop(BasicFunc func) { 
    if (IsInThreadLoop()) {   
        func();
    } else {
        QueueOneFunc(std::move(func));
    }
}

void EventLoop::QueueOneFunc(BasicFunc func) {
    {  
        MutexLockGuard lock(mutex_);
        to_do_list_.emplace_back(std::move(func));
    }

    if (!IsInThreadLoop() || calling_functors_) {
        uint64_t write_one_byte = 1;  
        ::write(wakeup_fd_, &write_one_byte, sizeof(write_one_byte));
    }
}

// 1. 不是简单的在临界区内依次调用functor，而是把回调列表swap到functors中
// 这一方面减小了临界区的长度，意味着不会阻塞其他线程的queueInLoop()，
// 另一方面也避免了死锁(因为Functor可能再次调用quueInLoop)
// 2. 由于doPendingFunctors()调用的Functor可能再次调用queueInLoop(cb)，
// 这时queueInLoop()就必须wakeup(),否则新增的cb可能就不能及时调用了
// 3. muduo没有反复执行doPendingFunctors()直到pendingFunctors为空，
// 否则I/O线程可能陷入死循环，无法处理I/O事件

void EventLoop::DoToDoList() {
    ToDoList functors;
    calling_functors_ = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(to_do_list_);
    }
    for(const auto& func : functors) {
        func();
    }
    calling_functors_ = false;
}

