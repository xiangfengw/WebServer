#ifndef TINY_WEBSERVER_HTTPCONTENT_H_
#define TINY_WEBSERVER_HTTPCONTENT_H_

#include <utility>
#include <algorithm>

#include "buffer.h"
#include "httprequest.h"
#include "httpstate.h"

namespace tiny_webserver {

enum HttpRequestParseLine {
    kLineOK,
    kLineMore,
    kLineErrno
};

class HttpContent {
public:
    HttpContent();
    ~HttpContent();

    void ParseLine(Buffer* buffer);
    bool ParseContent(Buffer* buffer);
    bool GetCompleteRequest() { return parse_state_ == kParseGotCompleteRequest; } 
  
    const HttpRequest& request() const { return request_; }
    void ResetContentState() { 
        parse_state_ = kParseRequestLine; 
        line_state_ = kLineOK; 
    }

private:
    int checked_index_;          // 解析到的字符下标
    HttpRequest request_;
    HttpRequestParseLine line_state_;
    HttpRequestParseState parse_state_;
};

}

#endif