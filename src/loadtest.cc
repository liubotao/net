#include <stdio.h>
#include <stdlib.h>

#include "eventloop.h"
#include "tcpsocket.h"


static void clientRead(evutil_socket_t fd, short event, void* arg) {
    //printf("clientRead");
}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("Usage:%s host_ip port connections [eg: ./loadtest 127.0.1 9091 100] \n", argv[0]);
        return 1;
    }

    int port = static_cast<int>(atoi(argv[2]));
    HostAddress serverAddress(argv[1], port);
    int connectionNum = atoi(argv[3]);
  
    EventLoop eventLoop;
    Event event; 
    for (int i = 0; i < connectionNum; i++) {
        TcpSocket tcpSocket = TcpSocket::createTcpSocket();        
        tcpSocket.setNoDelay();
        tcpSocket.setNonBlocking();
        tcpSocket.connect(serverAddress);

        int num = 100;
        event.set(&eventLoop, tcpSocket.socket(),EV_READ | EV_PERSIST, clientRead, (void*)&num);
        event.active();
    }

    printf("%d connection success!\n", connectionNum);
    eventLoop.exec();
    return 0;
}


