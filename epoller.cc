#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <assert.h>

#include <vector>

#include "epoller.h"
#include "channel.h"

using namespace tiny_webserver;

Epoller::Epoller()
    : epollfd_(epoll_create(kMaxEvents)),
    events_(kMaxEvents),
    channels_() {}

Epoller::~Epoller() {
    close(epollfd_);
}

void Epoller::Poll(Channels &channels) {
    int eventnums = epoll_wait(epollfd_, &*events_.begin(), kMaxEvents, -1);
    FillActiveChannels(eventnums, channels);
}

void Epoller::FillActiveChannels(int eventnums, Channels& channels) {
    for(int i = 0; i < eventnums; ++i) {
        Channel *channel = static_cast<Channel*> (events_[i].data.ptr);
        channel->SetReceivedEvents(events_[i].events);
        channels.emplace_back(channel);
    }
    if(eventnums == static_cast<int>(events_.size())) {
        events_.resize(eventnums * 2);
    }
}

void Epoller::Remove(Channel* channel) {
    int fd = channel->fd();
    ChannelState state = channel->state();
    assert(state == kDeleted || state == kAdded);

    if (state == kAdded) {
        UpdateChannel(EPOLL_CTL_DEL, channel);
    }
    channel->SetChannelState(kNew);
    channels_.erase(fd);
    return;
}

int Epoller::SetNonBlocking(int fd) {
    int old_state = fcntl(fd, F_GETFL);
    int new_state = old_state | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_state);
    return new_state;
}

void Epoller::Update(Channel* channel) {
    int op = 0, events = channel->events();
    ChannelState state = channel->state();
    int fd = channel->fd();

    if(state == kNew || state == kDeleted) {
        if (state == kNew) {
            assert(channels_.find(fd) == channels_.end());
            channels_[fd] = channel;
        } else {
            assert(channels_.find(fd) != channels_.end());
            assert(channels_[fd] == channel);
        }
        channel->SetChannelState(kAdded);
        op = EPOLL_CTL_ADD;
    } else {
        assert(channels_.find(fd) != channels_.end());
        assert(channels_[fd] == channel);
        if(events == 0) {
            op = EPOLL_CTL_DEL;
            channel->SetChannelState(kDeleted);
        } else {
            op = EPOLL_CTL_MOD; 
        }
    }
    
    UpdateChannel(op, channel);
}

void Epoller::UpdateChannel(int operation, Channel* channel) {
    struct epoll_event event;
    memset(&event, '\0', sizeof(event));
    event.events = channel->events();
    event.data.ptr = static_cast<void*>(channel);

    epoll_ctl(epollfd_, operation, channel->fd(), &event);
    return;
}