
#include "tcpserver.h"

class EchoServer : public TcpServer 
{
public:
    EchoServer(void);
    ~EchoServer(void);

public:

    void setEventLoopThreadPool(EventLoopThreadPool* pool) {
        eventLoopThreadPool_ = pool;
    }
    
    EventLoopThreadPool* eventLoopThreadPool(void) {
        return eventLoopThreadPool_;
    }

    virtual Context* createContextObject(void);
    virtual void destroyContextObject(Context* c);
    virtual void closeConnection(Context* c);
    virtual void clientConnected(Context* c);
    virtual void  readingRequest(Context* c);
    virtual void readRequestFinished(Context* c);
    virtual void writeReply(Context* c);
    virtual void writeReplyFinished(Context* c);

private:
    EchoServer(const EchoServer&);
    EchoServer& operator =(const EchoServer&);

private:
    EventLoopThreadPool* eventLoopThreadPool_;
    int next_;
};
