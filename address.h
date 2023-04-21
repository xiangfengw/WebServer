#ifndef TINY_WEBSERVER_ADDRESS_H_
#define TINY_WEBSERVER_ADDRESS_H_

#include <string>
#include <cstring>

namespace tiny_webserver {

class Address {
private:
    const char* ip_;
    const int port_;

public:
    Address(const char* port = "8888") : ip_("0.0.0.0"), port_(atoi(port)) {}

    const char* ip() const { return ip_; }
    const int port() const { return port_; }
};

}
#endif

