#ifndef TINY_WEBSERVER_CONDITION_H_
#define TINY_WEBSERVER_CONDITION_H_

#include "pthread.h"

#include "mutex.h"

namespace tiny_webserver {

class Condition {
public:
    Condition(MutexLock& mutex) : mutex_(mutex) {
        pthread_cond_init(&cond_, nullptr);
    }
    ~Condition() {
        pthread_cond_destroy(&cond_);
    }

    bool Wait() {
        int ret = 0;
        ret = pthread_cond_wait(&cond_, mutex_.mutex());
        return ret == 0;
    }

    bool Signal() {
        return pthread_cond_signal(&cond_);
    }

    bool BroadCast() {
        return pthread_cond_broadcast(&cond_);
    }    
private:
    MutexLock& mutex_;
    pthread_cond_t cond_;
};

}
#endif