#ifndef TINY_WEBSERVER_EVENTLOOP_H_
#define TINY_WEBSERVER_EVENTLOOP_H_

#include <stdint.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <pthread.h>

#include <vector>
#include <functional>
#include <memory>

#include "mutex.h"
#include "epoller.h"

namespace tiny_webserver {

class Epoller;
class Channel;

class EventLoop {
public:
    typedef std::vector<Channel*> Channels;
    typedef std::function<void()> BasicFunc;
    typedef std::vector<BasicFunc> ToDoList;

    EventLoop();
    ~EventLoop();

    void Loop();
    void HandleRead();

    void Update(Channel* channel) { epoller_->Update(channel); }
    void Remove(Channel* channel) { epoller_->Remove(channel); }
    
    bool IsInThreadLoop() { return pthread_self() == tid_; }
    void QueueOneFunc(BasicFunc func);
    void RunInLoop(BasicFunc func);
    void DoToDoList();

private:
    pthread_t tid_;
    std::unique_ptr<Epoller> epoller_;
    int wakeup_fd_;              // 用于eventfd 线程间通信
    std::unique_ptr<Channel> wakeup_channel_;    // wakeupfd对应的channel 会纳入epoller管理
    bool calling_functors_;      // 是否正在处理to_do_list
    Channels active_channels_;   // epoller返回的活动通道
    ToDoList to_do_list_;        // 本线程或其他线程使用queueInLoop添加的任务

    MutexLock mutex_;
};

}

#endif