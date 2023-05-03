#ifndef TINY_WEBSERVER_HTTPSTATE_H_
#define TINY_WEBSERVER_HTTPSTATE_H_

namespace tiny_webserver {

enum HttpRequestParseState {
  kParseRequestLine,
  kParseHeaders,
  kParseBody,
  kParseGotCompleteRequest,
  kParseErrno,
};

}

#endif
