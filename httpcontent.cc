#include "httpcontent.h"

#include "httprequest.h"
#include "httpstate.h"

using namespace tiny_webserver;

HttpContent::HttpContent() : checked_index_(0),
                             parse_state_(kParseRequestLine) {
}

HttpContent::~HttpContent() {}

void HttpContent::ParseLine(Buffer* buffer) {
    line_state_ = kLineMore;
    if (buffer->readablebytes() == 0) return;

    int readable_index = buffer->readablebytes();
    const char* buf = buffer->beginread();

    for (; checked_index_ < readable_index; ++checked_index_) {
        char ch = buf[checked_index_]; 
        if (ch == '\n') {
            if (checked_index_ && buf[checked_index_ - 1] == '\r') {
                checked_index_ = checked_index_ + 1;
                line_state_ = kLineOK;
            } else {
            line_state_ = kLineErrno;
            }
            return;
        } else if (ch == '\r') {
            if (checked_index_ == readable_index - 1) continue;
            if (buf[checked_index_ + 1] == '\n') {
                checked_index_ = checked_index_ + 2;
                line_state_ = kLineOK;
            } else { 
                line_state_ = kLineErrno;
            }
            return;
        } else {
            continue;
        }
    }

    return;
}

bool HttpContent::ParseContent(Buffer* buffer) {
    while (parse_state_ != kParseErrno) {
        ParseLine(buffer);

        if (line_state_ == kLineMore || line_state_ == kLineErrno) { 
            if (line_state_ == kLineErrno) {
                parse_state_ = kParseErrno;
                checked_index_ = 0;
            }
            break;
        }

        const char* start = buffer->beginread();
        const char* end = start + (checked_index_ - 2);

        if (parse_state_ == kParseRequestLine) {
            request_.ParseRequestLine(start, end, parse_state_);
        } else if (parse_state_ == kParseHeaders) {
            request_.ParseHeaders(start, end, parse_state_);
        } else if (parse_state_ == kParseBody) {
            request_.ParseBody(start, end, parse_state_);
        } else if (parse_state_ == kParseGotCompleteRequest) {
            break;
        }
        buffer->RetrieveUntilIndex(start + checked_index_);
        checked_index_ = 0;
    }

    return parse_state_ != kParseErrno;
}
