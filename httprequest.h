#ifndef TINY_WEBSERVER_HTTPREQUEST_H_
#define TINY_WEBSERVER_HTTPREQUEST_H_

#include <string>
#include <map>

#include "httpstate.h"

using std::string;

namespace tiny_webserver {

static const char http[] = "HTTP/1.";

enum Method {
    kGet,
    kPost,
    kPut,
    kDelete,
    kTrace,
    kOptions,
    kConnect,
    kPatch
};

enum Version {
    kHttp10,
    kHttp11
};

class HttpRequest {
public:
    HttpRequest();
    ~HttpRequest();

    bool ParseRequestMethod(const char *start, const char *end);

    void ParseRequestLine(const char *start, const char *end,
                          HttpRequestParseState &state);

    void ParseHeaders(const char *start, const char *end,
                      HttpRequestParseState &state);

    void ParseBody(const char *start, const char *end,
                   HttpRequestParseState &state);

    Method method() const { return method_; }
    const string &path() const { return path_; }
    const string &query() const { return query_; }
    Version version() const { return version_; }
    const std::map<string, string> &headers() const { return headers_; }

    string GetHeader(const string &header) const
    {
        auto iter = headers_.find(header);
        if (iter == headers_.end())
        {
            return string();
        }
        else
        {
            return iter->second;
        }
    }

private:
    Method method_;
    string path_;
    string query_;
    Version version_;
    std::map<string, string> headers_;
};

}

#endif