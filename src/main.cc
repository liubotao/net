#include "stdio.h"
#include "echoserver.h"
#include "config.h"
#include <stdlib.h>
#include "logger.h"
#include <sys/resource.h>

EventLoop* loop = NULL;

#define CORE_SIZE   1024 * 1024 * 500

int main(int argc, char* argv[]) {    
    
    const char *logFile = "pin.log";
    FileLogger fileLogger;
    fileLogger.setFileName(logFile);
    
    Logger::setDefaultLogger(&fileLogger);

    Logger::error("Echo Server running...");
    /*
    const char *path = "/home/parallels/Desktop/Parallels\ Shared\ Folders/Home/ClionProjects/room/demo.conf";
    Config config;
    config.initConfig(path);
    char packageName[64];
    config.readItem("Server", "PackageName", packageName, sizeof(packageName));
    printf("port:%s\n", packageName);
    */
    return 0;
    /*struct rlimit rlmt;
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
    return 0; */
} 

