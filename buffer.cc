#include "buffer.h"

#include <sys/uio.h>

using namespace tiny_webserver;

// 从指定连接对应fd读取数据，当读取的数据超过内部缓冲区writable空间大小时，
// 采用的策略是先用一个64K栈缓存extrabuf临时存储，
// 然后根据需要合并prependable空间到writable空间，或者resize buffer_大小。
int Buffer::ReadFd(int fd) {
    char extrabuf[65536] = {0};
    struct iovec iv[2];
    const int writable = writablebytes();
    iv[0].iov_base = beginwrite();
    iv[0].iov_len = writable;
    iv[1].iov_base = extrabuf;
    iv[1].iov_len = sizeof(extrabuf);
  
    const int iovcnt = (writable < static_cast<int>(sizeof(extrabuf)) ? 2 : 1);
    int readn = readv(fd, iv, iovcnt);

    if (readn < 0) {
        assert(readn >= 0);
    }
    else if (readn <= writable) {
        write_index_ += readn;
    } else {
        write_index_ = buffer_.size();
        Append(extrabuf, readn - writable);
    }

    return readn;
}