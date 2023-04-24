#include "eventloopthreadpool.h"
#include "eventloopthread.h"
#include <iostream>

using namespace tiny_webserver;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop)
    : base_loop_(loop),
      threads_(),
      loops_(),
      thread_num_(0),
      next_(0) { }

EventLoopThreadPool::~EventLoopThreadPool() { }

// 创建 numThreads_个EventLoopThread对象并运行各个线程
// 并在主线程保存创建的EventLoopThread对象
// 和EventLoopThread线程创建的EventLoop对象
void EventLoopThreadPool::StartLoop() {
    for(int i = 0; i < thread_num_; ++i) {
        EventLoopThread* thread = new EventLoopThread();
        threads_.emplace_back(thread);
        loops_.emplace_back(thread->StartLoop());
    }
}

EventLoop* EventLoopThreadPool::GetNextLoop() {
    EventLoop* ret = base_loop_;
    if(!loops_.empty()) {
        ret = loops_[next_];
        next_ = (next_ + 1) % thread_num_;
    }

    return ret;
}