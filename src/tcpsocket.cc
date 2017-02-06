#include "tcpsocket.h"

#define closesocket close
#define SOCK_EAGAIN EAGAIN
#define SOCK_EINTR EINTR
#define SOCK_ERRNO (errno)


HostAddress::HostAddress(int port) 
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = 0;    
}

HostAddress::HostAddress(const char* ip, int port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip);
}

HostAddress::HostAddress(const sockaddr_in &addr) {
    addr_ = addr;
}

HostAddress::~HostAddress(void) 
{
}

const char* HostAddress::ip(void) const 
{
    char* s = inet_ntoa(addr_.sin_addr);
    strcpy(ipBuff_, s);
    return ipBuff_;
}

int HostAddress::port(void) const 
{
    return ntohs(addr_.sin_port);
}

TcpSocket::TcpSocket(socket_t sock)
{
    socket_ = sock;
}

TcpSocket::~TcpSocket(void) 
{
}

TcpSocket TcpSocket::createTcpSocket(void)
{
    socket_t sock = ::socket(AF_INET, SOCK_STREAM, 0);
    return TcpSocket(sock);
}

bool TcpSocket::bind(const HostAddress& addr)
{
    if (::bind(socket_, (sockaddr*)addr.sockaddr_(), sizeof(sockaddr_in)) != 0) {
        return false;
    }
    return true;
}

bool TcpSocket::listen(int backlog) 
{
    if(::listen(socket_, backlog) != 0) {
        return false;
    }
    return true;
}

int TcpSocket::getOption(int level, int name,char* val, socklen_t* vallen)
{
    return ::getsockopt(socket_, level, name, val, vallen);
}

int TcpSocket::setOption(int level, int name,const char* val, socklen_t vallen)
{
    return ::setsockopt(socket_, level, name, val, vallen);
}

bool TcpSocket::setNonBlocking(void) 
{
    int flags;
    if ((flags = fcntl(socket_, F_GETFL, NULL)) < 0) {
        return false;
    }

    if (fcntl(socket_, F_SETFL, flags | O_NONBLOCK) == -1) {
        return false;
    }

    return true;
}

bool TcpSocket::setReuseaddr(void)
{
    int reuse;
    socklen_t len;

    reuse = 1;
    len = sizeof(reuse);
    
    return (setOption(SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, len) == 0);
}

bool TcpSocket::setNoDelay(void)
{
    int nodelay;
    socklen_t len;
    
    nodelay = 1;
    len = sizeof(nodelay);

    return (setOption(IPPROTO_TCP, TCP_NODELAY, (char*)&nodelay, len) == 0);
}

bool TcpSocket::setKeepAlive(void)
{
    int val = 1;
    return (setOption(SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(val)) == 0);
}

bool TcpSocket::setSendBufferSize(int size) 
{
    socklen_t len;
    len = sizeof(len);
    return (setOption(SOL_SOCKET, SO_SNDBUF, (char*)&size, len) == 0);
}

bool TcpSocket::setRecvBufferSize(int size)
{
    socklen_t len;
    len = sizeof(size);
    return (setOption(SOL_SOCKET, SO_RCVBUF, (char*)&size, len) == 0);
}

bool TcpSocket::setSendTimeout(int msec)
{
    timeval val;
    val.tv_sec = (msec / 1000);
    val.tv_usec = (msec - val.tv_sec *1000) * 1000;

    return (setOption(SOL_SOCKET, SO_RCVTIMEO, (char*)&val, sizeof(val)) == 0);
}

int TcpSocket::sendBufferSize(void)
{
    int status, size;
    socklen_t len;

    size = 0;
    len = sizeof(size);

    status = getOption(SOL_SOCKET, SO_SNDBUF, (char*)&size, &len);
    if (status < 0) {
        return -1;
    }
    return size;
}

int TcpSocket::recvBufferSize(void)
{
    int status, size;
    socklen_t len;

    size = 0;
    len = sizeof(size);
    
    status = getOption(SOL_SOCKET, SO_RCVBUF, (char*)&size, &len);
    if (status < 0) {
        return -1;
    }

    return size;
}

bool TcpSocket::connect(const HostAddress &addr) 
{
    if (::connect(socket_, (sockaddr*)addr.sockaddr_(), sizeof(sockaddr_in)) != 0) {
        return false;
    }

    return true;
}

void TcpSocket::close(void) {
    if (socket_ != -1) {
        TcpSocket::close(socket_);
        socket_ = -1;
    }
}

bool TcpSocket::isNull(void) const 
{
    return (socket_ < 0);
}

void TcpSocket::close(socket_t sock) 
{
    ::close(sock);

}
