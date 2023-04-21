#include <sys/types.h>
#include <sys/socket.h>

#include "tcpconnection.h"
#include "channel.h"

using namespace tiny_webserver;

TcpConnection::TcpConnection(EventLoop* loop, int connfd)
    :loop_(loop),
    connfd_(connfd),
    channel(new Channel(loop_, connfd_)) {
    channel_
}