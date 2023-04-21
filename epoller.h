#ifndef TINY_WEBSERVER_EPOLLER_H_
#define TINY_WEBSERVER_EPOLLER_H_

#include <sys/epoll.h>
#include <vector>

const int kMaxEvents = 8;

namespace tiny_webserver {

class Channel;

class Epoller {
public:
    typedef std::vector<epoll_event> Events;
    typedef std::vector<Channel*> Channels;

    Epoller();

private:
    int epollfd_;
    Events events_;
};

}
#endif