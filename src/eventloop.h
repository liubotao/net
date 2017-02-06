
#ifndef EVENTLOOPPOOL_H
#define EVENTLOOPPOOL_H

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/event_compat.h>
#include <event2/thread.h>

#include "thread.h"

class EventLoop;
class Event
{
public:
    Event(void);
    ~Event(void);

    void set(EventLoop* loop, evutil_socket_t sock, short flags, event_callback_fn fn, void* arg);

    void setTimer(EventLoop* loop, event_callback_fn fn, void* arg);

    void active(int timeout_msec = -1);

    void remove(void);

private:
    event event_;
    friend class EventLoop;
};

class EventLoop
{
public:
    EventLoop(void);
    ~EventLoop(void);

    void exec(void);
    void exit(int timeout = -1);

private:
    event_base* event_loop_;
    friend class Event;
    EventLoop(const EventLoop&);
    EventLoop& operator=(const EventLoop&);
};

class EventLoopThread : public Thread
{
public:
    EventLoopThread(void);
    ~EventLoopThread(void);

    EventLoop* eventLoop(void) { return &loop_;}
    void exit(void);

protected:
    virtual void run(void);

protected:
    Event event_;
    EventLoop loop_;
};

class EventLoopThreadPool
{
public:
    enum {  
        DefaultThreadCount = 4,
        MaxThreadCount = 32
    };

    EventLoopThreadPool(void);
    ~EventLoopThreadPool(void);

    void start(int size = DefaultThreadCount);
    void stop(void);
    
    int size(void) const { return size_;}
    EventLoopThread* thread(int index) const;

private:
    int size_;
    EventLoopThread* threads_;
    EventLoopThreadPool(const EventLoopThreadPool&);
    EventLoopThreadPool& operator=(const EventLoopThreadPool&);
};

#endif
