#ifndef TINY_MUDUO_LATCH_H_
#define TINY_MUDUO_LATCH_H_

#include "mutex.h"
#include "condition.h"

namespace tiny_webserver {

class Latch {
public:
    Latch(int count) : count_(count), mutex_(), cond_(mutex_) {}

    void CountDown() {
        MutexLockGuard lock(mutex_);
        --count_;
        if (count_ == 0) {
            cond_.BroadCast();
        }
    }
    
    void Wait() {
        MutexLockGuard lock(mutex_);
        while (count_ > 0) {
            cond_.Wait();
        }
    }

private:
    int count_;
    MutexLock mutex_;
    Condition cond_;
};

}
#endif
