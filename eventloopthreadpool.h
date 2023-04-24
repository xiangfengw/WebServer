#ifndef TINY_WEBSERVER_EVENTLOOPTHREADPOOL_H_
#define TINY_WEBSERVER_EVENTLOOPTHREADPOOL_H_

#include <vector>
#include <iostream>

namespace tiny_webserver {

class EventLoopThread;
class EventLoop;

class EventLoopThreadPool {
public:
    typedef std::vector<EventLoopThread*> Threads;
    typedef std::vector<EventLoop*> Loops;

    EventLoopThreadPool(EventLoop* loop);
    ~EventLoopThreadPool();

    void SetThreadNum(int thread_num) {
        thread_num_ = thread_num;
    }

    void StartLoop();
    EventLoop* GetNextLoop();  // round-robin

private:
    EventLoop* base_loop_;    // 主线程的EventLoop
    Threads threads_;         // 线程数组
    Loops loops_;             // EventLoop*数组，每个EventLoopThread线程对应EventLoop

    int thread_num_;
    int next_;
};

}

#endif