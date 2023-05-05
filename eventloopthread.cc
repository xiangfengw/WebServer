#include <pthread.h>
#include <functional>

#include "eventloopthread.h"
#include "mutex.h"
#include "condition.h"
#include "eventloop.h"

using namespace tiny_webserver;

EventLoopThread::EventLoopThread()
    : loop_(nullptr),
      thread_(std::bind(&EventLoopThread::ThreadFunc, this)),
      mutex_(),
      cond_(mutex_) {}

EventLoopThread::~EventLoopThread() {}

// 启动一个EventLoop线程
EventLoop* EventLoopThread::StartLoop() {
    thread_.StartThread();
    EventLoop* loop = nullptr;
    {
        MutexLockGuard lock(mutex_);
        while(loop_ == nullptr) {
            cond_.Wait();
        }
        loop = loop_;
    }
    return loop;
}

/*
* threadFunc()运行的就是实例化一个EventLoop，并让这个EventLoop进入到loop状态。
*/
void EventLoopThread::ThreadFunc() {
    EventLoop loop;
    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.Signal(); // 通知startLoop线程，loop已绑定
    }
    
    loop_->Loop();
    {
        MutexLockGuard lock(mutex_);
        loop_ = nullptr;
    }
}