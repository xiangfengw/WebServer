#ifndef TINY_WEBSERVER_HTTPSERVER_H_
#define TINY_WEBSERVER_HTTPSERVER_H_

#include <stdio.h>

#include <functional>
#include <utility>

#include "callback.h"
#include "tcpserver.h"
#include "tcpconnection.h"
#include "buffer.h"
#include "httpcontent.h"
#include "httprequest.h"
#include "httpresponse.h"

using tiny_webserver::HttpStatusCode;

namespace tiny_webserver {

static const int kThreadNums = 6;

class EventLoop;

class HttpServer{
typedef std::function<void (const HttpRequest&, HttpResponse&)> HttpResponseCallback;

public:
    HttpServer(EventLoop* loop, const Address& address);
    ~HttpServer();

    void Start() {
        server_.Start();
    }

    void HttpDefaultCallback(const HttpRequest& request, HttpResponse& response) {
        response.SetStatusCode(k404NotFound);
        response.SetStatusMessage("Not Found");
        response.SetCloseConnection(true);
    }

    void ConnectionCallback(TcpConnection* connection) {
        printf("HttpServer has New Conneciton"); 
    }

    void MessageCallback(TcpConnection* connection, Buffer* buffer);
    void SetHttpResponseCallback(const HttpResponseCallback& response_callback) { 
        response_callback_ = std::move(response_callback); 
    }

    void DealWithRequest(const HttpRequest& request, TcpConnection* connection);

private:
    EventLoop* loop_;
    TcpServer server_;

    HttpResponseCallback response_callback_;
};

}

#endif
