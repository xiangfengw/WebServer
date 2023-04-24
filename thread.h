#ifndef TINY_WEBSERVER_THREAD_H_
#define TINY_WEBSERVER_THREAD_H_

#include <pthread.h>

#include <functional>

#include "latch.h"

namespace tiny_webserver {

class Thread {
public:
    typedef std::function<void ()> ThreadFunc;

    Thread(const ThreadFunc& func);
    ~Thread();

    void StartThread();

private:
    pthread_t pthread_id_;
    ThreadFunc func_;
    Latch latch_;
};


struct ThreadData {  // 封装了线程数据信息，主要用于pthread_create()函数中作为线程传参而使用的

typedef tiny_webserver::Thread::ThreadFunc ThreadFunc;

ThreadFunc func_;
Latch* latch_;

ThreadData(ThreadFunc& func, Latch* latch)
    : func_(func),
      latch_(latch) { }

void RunOneFunc() {
    latch_->CountDown();
    latch_ = nullptr;  
    func_();
}
};

}
#endif