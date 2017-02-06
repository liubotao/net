
#include "echoserver.h"
#include <iostream>

EchoServer::EchoServer(void) {
    next_ = 0;
}

EchoServer::~EchoServer(void){}

Context* EchoServer::createContextObject(void) {
    Context* context = new Context;
    EventLoop* loop;
    if (eventLoopThreadPool_) {
        int threadCount = eventLoopThreadPool_->size();
        EventLoopThread* loopThread = eventLoopThreadPool_->thread(next_ % threadCount);
        ++next_;
        if (next_ >= threadCount) {
            next_ = 0;
        }
        loop = loopThread->eventLoop();
    } else {
        loop = eventLoop();
    }
    context->eventLoop = loop;
    return context;
}

void EchoServer::destroyContextObject(Context* c) {
    delete c;
}

void EchoServer::clientConnected(Context* c)
{ 
    printf("EchoServer client[%s]  Connected Success\n", c->clientAddress.ip());
}

void EchoServer::closeConnection(Context* c){
     //printf("EchoServer client[%s]  closed Success\n", c->clientAddress.ip());
     TcpServer::closeConnection(c);
}

void EchoServer::readingRequest(Context* c){ 
    std::cout << c->inputBuffer.retrieveAllAsString();
    //return ;
}

void EchoServer::readRequestFinished(Context* c){
    printf("EchoServer readRequestFinished \n");
}

void EchoServer::writeReply(Context* c){
    printf("EchoServer writeReply \n");
}


void EchoServer::writeReplyFinished(Context* c){
  waitRequest(c);
}




