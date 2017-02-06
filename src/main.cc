#include "stdio.h"
#include "echoserver.h"
#include <stdlib.h>
#include <sys/resource.h>

EventLoop* loop = NULL;

#define CORE_SIZE   1024 * 1024 * 500

int main(int argc, char* argv[]) {    
    
    struct rlimit rlmt;
    rlmt.rlim_cur = (rlim_t)CORE_SIZE;
    rlmt.rlim_max  = (rlim_t)CORE_SIZE;

    if (setrlimit(RLIMIT_CORE, &rlmt) == -1) {
        return -1; 
    }   

    if (getrlimit(RLIMIT_CORE, &rlmt) == -1) {
        return -1; 
    } 

    if (argc < 2) {
        printf("Usage: ./main threadNum [eg: ./main 19]\n");
        return -1;
    }

    EventLoop listenerLoop;
    EchoServer echoServer;
    echoServer.setEventLoop(&listenerLoop);

    loop = &listenerLoop;
    int port = 9091;
    echoServer.run(HostAddress(port));
 
    int poolNum = atoi(argv[1]);
    EventLoopThreadPool pool;
    pool.start(poolNum);
    echoServer.setEventLoopThreadPool(&pool);

    listenerLoop.exec();
    return 0;
} 

