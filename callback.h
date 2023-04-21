#ifndef TINY_WEBSERVER_CALLBACK_H_
#define TINY_WEBSERVER_CALLBACK_H_

#include <functional>

// 绑定函数参数的占位符，可以方便定义一些泛型函数或函数对象，使用bind绑定某个函数对象和一些参数。
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace tiny_webserver {
    typedef std::function<void (TcpConnection*)> ConnectionCallback;
    typedef std::function<void (TcpConnection*)> MessageCallback;
    typedef std::function<void ()> ReadCallback;
    typedef std::function<void ()> WriteCallback;
}

#endif