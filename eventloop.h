#ifndef TINY_WEBSERVER_EVENTLOOP_H_
#define TINY_WEBSERVER_EVENTLOOP_H_

#include <vector>
#include <functional>

#include "epoller.h"

namespace tiny_webserver {

class Epoller;
class Channel;

class EventLoop {
public:
    typedef std::vector<Channel*> Channels;
    typedef std::function<void()> BasicFunc;\

    EventLoop();
    ~EventLoop();

    void Loop();
    void Update(Channel* channel) { epoller_->Update(channel); }
    

private:
    Epoller* epoller_;
    int wakeup_fd_;
    Channels active_channels_;

}

}

#endif