#ifndef SOCKET_H
#define SOCKET_H

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#include <string.h>

typedef int socket_t;

class HostAddress 
{
public:
    HostAddress(int port = 0);
    HostAddress(const char* ip, int port);
    HostAddress(const sockaddr_in& addr);
    ~HostAddress(void);

    const char* ip(void) const;
    int port(void) const;
    
    sockaddr_in* sockaddr_(void) const {
        return (sockaddr_in*)&addr_;
    }
private:
    mutable char ipBuff_[32];
    sockaddr_in addr_;
};

class TcpSocket
{
public:
    enum {
        IOAgain = -1,
        IOError = -2
    };

    TcpSocket(socket_t sock = -1);  
    ~TcpSocket(void);

    static TcpSocket createTcpSocket(void);
    
    socket_t socket(void) const { return socket_;}

    bool bind(const HostAddress& addr);
    bool listen(int backlog);
    
    int getOption(int level, int name,char* val, socklen_t* vallen);
    int setOption(int level, int name, const char* val, socklen_t vallen);

    bool setNonBlocking(void);
    bool setReuseaddr(void);
    bool setNoDelay(void);
    bool setKeepAlive(void);
    bool setSendBufferSize(int size);
    bool setRecvBufferSize(int size);
    bool setSendTimeout(int msec);
    bool setRecvTimeout(int msec);
    
    int sendBufferSize(void);
    int recvBufferSize(void);

    bool connect(const HostAddress& addr);
    
    int write(const char* buf, size_t count) {
        return ::write(socket_, buf, count);
    }

    int read(socket_t socket, void *buf, size_t count) {
        return ::read(socket, buf, count);
    }

    ssize_t readv(socket_t socket,const struct iovec *iov, int iovcnt) {
        return ::readv(socket, iov, iovcnt);
    }

    void close(void);
    bool isNull(void) const;

    static void close(socket_t sock);

private:
    socket_t socket_;
};

#endif
