#include "httpresponse.h"

#include <string>

#include "buffer.h"

using namespace tiny_webserver;
using std::string;

const string HttpResponse::server_name_ = "Tiny_webserver";
const string HttpResponse::server_http_version_ = "HTTP/1.1";

void HttpResponse::AppendToBuffer(Buffer* buffer) {
    string message;

    message += (server_http_version_ + " " + 
              std::to_string(status_code_) + " " + 
              status_message_ + CRLF);

    if (close_connection_) {
        message += ("Connection: close" + CRLF);
    } else {
        message += ("Content-Length: ");
        message += (std::__cxx11::to_string(body_.size()) + CRLF);
        message += ("Connection: Keep-Alive" + CRLF);
    }
    message += ("Content-Type:" + type_ + CRLF);
    message += ("Server: " + server_name_ + CRLF);

    message += CRLF;
    message += body_;

    buffer->Append(message);
    return;
}
