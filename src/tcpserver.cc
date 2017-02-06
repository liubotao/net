#include "tcpserver.h"

void onReadClientHandler(socket_t, short, void* arg)
{
    Context* c = (Context*)arg;
    Buffer* inputBuffer = &c->inputBuffer;
    int saveErrno = 0;
    ssize_t n =  inputBuffer->readFd(&c->clientSocket, &saveErrno);

    if (n > 0) {
        c->server->readingRequest(c);
        c->server->waitRequest(c);
    } else if (n == 0) {
        c->server->closeConnection(c);
    } else {
       errno = saveErrno;
       printf("TcpServer onReadClientHandler Error\n");
    }
}

void onWriteClientHandler(socket_t, short, void* arg)
{
    Context* c = (Context*)arg;
    Buffer* outputBuffer = &c->outputBuffer;
    ssize_t n = c->clientSocket.write(outputBuffer->peek(), outputBuffer->readableBytes());
 
    if (n > 0) {
        outputBuffer->retrieve(n);
        if (outputBuffer->readableBytes() == 0) {
            c->server->writeReplyFinished(c);
        }
    }  else {
        printf("cpServer onReadClientHandler Error\n");
    }
}

void TcpServer::onAcceptHandler(evutil_socket_t sock, short, void* arg)
{
    sockaddr_in clientAddr;
    socklen_t len = sizeof(sockaddr_in);
    socket_t clisock = accept(sock, (sockaddr*)&clientAddr, &len);
    TcpSocket socket(clisock);
    if (socket.isNull()) {
        return;
    }

    socket.setKeepAlive();
    socket.setNonBlocking();
    socket.setNoDelay();
    
    TcpServer* srv = (TcpServer*)arg;
    Context* c = srv->createContextObject();
    if (c != NULL) { 
        c->clientSocket = socket;
        c->clientAddress = HostAddress(clientAddr);
        c->server = srv;
        if (c->eventLoop == NULL) { 
            c->eventLoop = srv->eventLoop();
        }
        srv->clientConnected(c);
        srv->waitRequest(c);
    } else {
        socket.close();
    }
}


TcpServer::TcpServer(void)
{
    loop_ = NULL;
}

TcpServer::~TcpServer(void)
{
    stop();
}

bool TcpServer::run(const HostAddress& addr)
{
    if (!loop_) {
        printf("event loop not set\n");
        return false;
    }

    if (isRunning()) {
        printf("TcpServer::run: server is already running\n");
        return false;
     }    

    TcpSocket tcpSocket = TcpSocket::createTcpSocket();
    if (tcpSocket.isNull()) {
        printf("TcpServer::run: %s", strerror(errno));
        return false;
    }

    tcpSocket.setReuseaddr();
    tcpSocket.setNoDelay();
    tcpSocket.setNonBlocking();

    if (!tcpSocket.bind(addr)) {
        printf("TcpServer::run: listen bind at port %d, %s", addr.port(), strerror(errno));
        tcpSocket.close();
        return false;
    }

    if (!tcpSocket.listen(128)) {
        printf("TcpServer::run: listen failed at port %d, %s", addr.port(), strerror(errno));
        tcpSocket.close();
        return false;
    }

    listener_.set(loop_, tcpSocket.socket(), EV_READ | EV_PERSIST, onAcceptHandler, this);
    listener_.active();
    
    socket_ = tcpSocket;
    addr_ = addr;

    printf("TcpServer::run: listen failed at port %d\n", addr.port());
    return true;
}


bool TcpServer::isRunning(void) const
{
    return !socket_.isNull();
}

void TcpServer::stop(void)
{
    if (isRunning()) {
        listener_.remove();
        socket_.close();
    }
}

Context* TcpServer::createContextObject(void) {
    return new Context;
}

void TcpServer::destoryContextObject(Context* c)
{
    delete c;
}

void TcpServer::closeConnection(Context* c) 
{
    c->clientSocket.close();
    destoryContextObject(c);
}

void TcpServer::clientConnected(Context* c)
{
    printf("client IP:%s\n", c->clientAddress.ip());
}

void TcpServer::waitRequest(Context *c)
{
    c->event.set(c->eventLoop, c->clientSocket.socket(), EV_READ, onReadClientHandler, c);
    c->event.active();
}

void TcpServer::readingRequest(Context*)
{
}

void TcpServer::readRequestFinished(Context*)
{
    printf("readRequestFinished \n");
}

void TcpServer::writeReply(Context* c) 
{
    onWriteClientHandler(0, 0, c);
}

void TcpServer::writeReplyFinished(Context*)
{
}

