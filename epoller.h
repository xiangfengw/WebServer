#ifndef TINY_WEBSERVER_EPOLLER_H_
#define TINY_WEBSERVER_EPOLLER_H_

#include <sys/epoll.h>
#include <vector>
#include <unordered_map>

const int kMaxEvents = 16;

namespace tiny_webserver {

class Channel;

class Epoller {
public:
    typedef std::vector<epoll_event> Events;
    typedef std::vector<Channel*> Channels;

    Epoller();
    ~Epoller();

    void Poll(Channels &channels);
    int SetNonBlocking(int fd);
    void FillActiveChannels(int eventnums, Channels &channels);
    void Remove(Channel* channel);
    void Update(Channel* channel);
    /// Changes the interested I/O events.
    /// Must be called in the loop thread.
    void UpdateChannel(int operation, Channel* channel);

private:
    typedef std::unordered_map<int, Channel*> ChannelMap;

    int epollfd_;
    Events events_; // 存放发生事件的数组
    ChannelMap channels_;
};

};
#endif