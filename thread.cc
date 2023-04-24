#include <pthread.h>

#include "thread.h"
#include "latch.h"

using namespace tiny_webserver;

static void* ThreadRun(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    data->RunOneFunc();
    delete data;
    return nullptr;
}

Thread::Thread(const ThreadFunc& func) 
    : pthread_id_(-1),
      func_(func),
      latch_(1) {}

Thread::~Thread() {
    pthread_detach(pthread_id_);
}

void Thread::StartThread() {
    ThreadData* data = new ThreadData(func_, &latch_);
    pthread_create(&pthread_id_, nullptr, ThreadRun, data);
    latch_.Wait();
}
