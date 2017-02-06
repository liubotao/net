#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "buffer.h"
#include "eventloop.h"
#include "tcpsocket.h"

class TcpServer;
class Context
{
public:
    Context(void) {
        server = NULL;
        eventLoop = NULL;
    }

    virtual ~Context(void) {}

    TcpSocket clientSocket;
    HostAddress clientAddress;
    TcpServer* server; 
    Buffer inputBuffer;
    Buffer outputBuffer;
    EventLoop* eventLoop;
    Event event;
};


class TcpServer
{
public:
    enum ReadStatus {
        ReadFinished = 0,
        ReadIncomplete = 1,
        ReadError = 2
    };

    TcpServer(void);
    virtual ~TcpServer(void);

    EventLoop* eventLoop(void) { 
        return loop_;
    }
    
    void setEventLoop(EventLoop* loop) {
        loop_ = loop;
    }

    const HostAddress& address(void) const {
        return addr_;
    }

    bool run(const HostAddress& addr);
    bool isRunning(void) const;
    void stop(void);

    virtual Context* createContextObject(void);
    virtual void destoryContextObject(Context* c);
    virtual void closeConnection(Context* c);
    virtual void clientConnected(Context* c);  
    virtual void waitRequest(Context* c);
    virtual void readingRequest(Context* c);
    virtual void readRequestFinished(Context* c);
    virtual void writeReply(Context* c);
    virtual void writeReplyFinished(Context* c);

protected:
    static void onAcceptHandler(evutil_socket_t sock, short, void* arg);

private:
    HostAddress addr_;
    Event listener_;
    EventLoop* loop_;
    TcpSocket socket_;

private:
    TcpServer(const TcpServer&);
    TcpServer& operator=(const TcpServer&);
};

#endif
