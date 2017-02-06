#include <stdio.h>
#include <stdint.h>
#include <endian.h>
#include <vector>
#include <string>
#include <string.h>
#include <algorithm> 
#include <iostream>
#include <assert.h>

#include "tcpsocket.h"
using std::string;

class Buffer
{
public:
    static const size_t prependSize = 8;
    static const size_t initialSize = 1024;

    Buffer(size_t initialSize = initialSize) 
        : buffer_(prependSize + initialSize),
          readerIndex_(prependSize),
          writerIndex_(prependSize)
    {    
        printf("writerIndex_:%zu\n", writerIndex_);
        printf("readerIndex_:%zu\n", readerIndex_);
    }

    void swap(Buffer& rhs) {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIndex_, rhs.readerIndex_);
        std::swap(writerIndex_, rhs.writerIndex_);
    }

    size_t readableBytes() const {
        //printf("writerIndex_:%zu\n", writerIndex_);
        //printf("readerIndex_:%zu\n", readerIndex_);
        return writerIndex_ - readerIndex_;
    }

    size_t writeableBytes() const {
        return buffer_.size() - writerIndex_;
    }

    size_t prependableBytes() const {
        return readerIndex_; 
    }

    const char* peek() const {
        return begin() + readerIndex_;
    }

    const char* findCRLF() const {
        const char* crlf = std::search(peek(), beginWrite(), KCRLF, KCRLF + 2);
        return crlf == beginWrite() ? NULL : crlf;
    }

   const char* findCRLF(const char* start) const {
        const char* crlf = std::search(start, beginWrite(), KCRLF, KCRLF + 2);
        return crlf == beginWrite() ? NULL : crlf;
    }

    const char* findEOL() const {
        const void* eol = memchr(peek(), '\n', readableBytes());
        return static_cast<const char*>(eol);
    }

    const char* EOL(const char* start) const {
        const void* eol = memchr(start, '\n', beginWrite() - start);
        return static_cast<const char*>(eol);
    }

    void retrieve(size_t len) {
        if (len < readableBytes()) {
            readerIndex_ += len;
        } else {
            retrieveAll();
        }
    }

    void retrieveUtil(const char* end) {
        retrieve(end - peek());
    }

    void retrieveInt64() {
        retrieve(sizeof(int64_t));
    }

    void retrieveInt32() {
        retrieve(sizeof(int32_t));
    }

    void retrieveInt16() {
        retrieve(sizeof(int16_t));
    }

    void retrieveInt8() {
        retrieve(sizeof(int8_t));
    }

    void retrieveAll() {
        readerIndex_ = prependSize;
        writerIndex_ = prependSize;
    }

    string retrieveAllAsString() {
        return retrieveAsString(readableBytes());
    }

    string retrieveAsString(size_t len) {
        string result(peek(), len);
        retrieve(len);
        return result;
    }

    void append(const void* data, size_t len) {
        append(static_cast<const char*>(data), len);
    }

    void ensureWriteableBytes(size_t len) {
        if (writeableBytes() < len) {
            makeSpace(len);
        } 
    }

    char* beginWrite() {
        return begin() + writerIndex_;
    }

    const char* beginWrite() const {
        return begin() + writerIndex_;
    }

    void hasWritten(size_t len) {
        writerIndex_ += len;
    }

    void unWriete(size_t len) {
        writerIndex_ -= len;
    }

    void appendInt64(int64_t x) {
       int64_t be64 = htobe64(x);
       append(&be64, sizeof be64);
    }

    void appendInt32(int32_t x) {
        int32_t be32 = htobe32(x);
        append(&be32, sizeof be32);
    }

    int64_t readInt64() {
        int64_t result = peekInt64();
        retrieveInt64();
        return result;
    }
    
    int32_t readInt32() {
        int32_t result = peekInt32();
        retrieveInt32();
        return result;
    }

    int64_t peekInt64() const {
        int64_t be64 = 0;
        ::memcpy(&be64, peek(), sizeof be64);
        return be64toh(be64);
    }

    int32_t peekInt32() const {
        int32_t be32 = 0;
        ::memcpy(&be32, peek(), sizeof be32);
        return be32toh(be32);
    }

    void prependInt64(int64_t x) {
        int64_t be64 = htobe64(x);
        prepend(&be64, sizeof be64);
    }

    void prependInt32(int32_t x)  {
        int32_t be32 = htobe32(x);
        prepend(&be32, sizeof be32);
    }

    void prepend(const void* data, size_t len) {
        readerIndex_ -= len;
        const char* d = static_cast<const char*>(data);
        std::copy(d, d+len, begin() + readerIndex_);
    }

    void shrink(size_t reserve) {
        Buffer other;
        other.ensureWriteableBytes(readableBytes() + reserve);
        //other.append(toStringPiece());
        swap(other);
    }

    size_t internalCapacity() const {
        return buffer_.capacity();
    }

    ssize_t readFd(TcpSocket* clientSocket, int* savedErrno);

private:

  char* begin()
  { return &*buffer_.begin(); }

  const char* begin() const
  { return &*buffer_.begin(); }

   void makeSpace(size_t len) {
        if (writeableBytes() + prependableBytes() < len + prependSize) {
            buffer_.resize(writerIndex_ + len);
        } else {
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_,
                      begin() + writerIndex_,
                      begin() + prependSize);
            readerIndex_ = prependSize;
            writerIndex_ = readerIndex_ + readable;
        }
    }

private:
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
    
    static const char KCRLF[];
};
