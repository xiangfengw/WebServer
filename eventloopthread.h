#ifndef TINY_WEBSERVER_EVENTLOOPTHREAD_H
#define TINY_WEBSERVER_EVENTLOOPTHREAD_H

#include "thread.h"
#include "mutex.h"
#include "condition.h"

namespace tiny_webserver {

class EventLoop;

class EventLoopThread {
public:
    EventLoopThread();
    ~EventLoopThread();

    void ThreadFunc();
    EventLoop* StartLoop();
private:
    EventLoop* loop_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};

}
#endif
