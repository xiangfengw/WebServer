#ifndef TINY_MUDUO_BUFFER_H_
#define TINY_MUDUO_BUFFER_H_

#include <vector>
#include <algorithm>
#include <string>
#include <assert.h>

using std::string;

namespace tiny_webserver {

static const int kPrePendIndex = 8;

class Buffer {
public:
    Buffer() : buffer_(1024), read_index_(kPrePendIndex), write_index_(kPrePendIndex) {}
    ~Buffer() {}

    int ReadFd(int fd);
  
    char* begin() { return &*buffer_.begin(); };
    const char* begin() const { return &*buffer_.begin(); };

    char* beginread() { return begin() + read_index_; } 
    const char* beginread() const { return begin() + read_index_; }

    char* beginwrite() { return begin() + write_index_; }
    const char* beginwrite() const { return begin() + write_index_; }

    void Append(const char* message, int len) {
        MakeSureEnoughStorage(len);
        std::copy(message, message + len, beginwrite());
        write_index_ += len;
    }

    void Append(const string& message) {
        Append(message.data(), message.size()); 
    }

    // retrieve 从readable空间取走数据，之关系移动移动readableIndex_，改变readable空间大小，
    // 通常不关心读取数据具体内容，除非有指定具体的返回值，如retrieveAsString。
    
    // 改变readable空间大小，但通常不改变writable空间大小，除非retrieveAll取完所有readable空间数据，
    // readable空间回合并到writable
    void Retrieve(int len) {
        if (len + read_index_ != write_index_) {
            read_index_ = read_index_ + len;
        } else {
            RetrieveAll();
        }
    }

    // 从readable取走[peek(), index] peek是readable空间首地址
    void RetrieveUntilIndex(const char* index) {
        assert(beginwrite() >= index);
        read_index_ += index - beginread();
    }

    void RetrieveAll() {
        write_index_ = kPrePendIndex;
        read_index_ = write_index_;
    }

    // 从readable空间头部取走len长的数据，转换为字符串返回
    string RetrieveAsString(int len) {
        assert(read_index_ + len <= write_index_);
        // string ret = std::move(PeekAsString(len));
        string ret(Peek(), len);
        Retrieve(len); 
        return ret;
    }

    string RetrieveAllAsString() {
        string ret = std::move(PeekAllAsString());
        RetrieveAll();
        return ret;
    }

    // peek只读不动指针
    const char* Peek() const {
        return beginread();
    }

    char* Peek() {
        return beginread();
    }

    string PeekAsString(int len) {
        return string(beginread(), beginread() + len);
    }

    string PeekAllAsString() {
        return string(beginread(), beginwrite()); 
    }
  
    int readablebytes() const { return write_index_ - read_index_; }
    int writablebytes() const { return buffer_.capacity() - write_index_; } 
    int prependablebytes() const { return read_index_; }

    void MakeSureEnoughStorage(int len) {
        if (writablebytes() >= len) return;
        if (writablebytes() + prependablebytes() >= kPrePendIndex + len) {
            std::copy(beginread(), beginwrite(), begin() + kPrePendIndex);
            write_index_ = 8 + readablebytes();
            read_index_ = 8;
        } else {
            buffer_.resize(buffer_.size() + len);
        }
    }

private:
    std::vector<char> buffer_;
    int read_index_;
    int write_index_;
};

}
#endif
