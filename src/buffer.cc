#include <errno.h>
#include <sys/uio.h>
#include<boost/implicit_cast.hpp>

#include "buffer.h"
#include "tcpsocket.h"

const char Buffer::KCRLF[] = "\r\n";

const size_t Buffer::prependSize;
const size_t Buffer::initialSize;

ssize_t Buffer::readFd(TcpSocket* clientSocket, int* saveErrno) {
    char extrabuf[65536];
    struct iovec vec[2];
    const size_t writeable = writeableBytes();
    vec[0].iov_base = begin()+writerIndex_;
    vec[0].iov_len = writeable;
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovcnt = (writeable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = clientSocket->readv(clientSocket->socket(), vec, iovcnt);
    if (n < 0) {
        *saveErrno = errno;
    } else if (boost::implicit_cast<size_t>(n) <= writeable) {
        writerIndex_ += n;
    } else {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writeable);
    }

    return n;
}


