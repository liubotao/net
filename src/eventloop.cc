#include "eventloop.h"

Event::Event(void) {}
Event::~Event(void){}

void Event::set(EventLoop* loop, evutil_socket_t sock, short flags, event_callback_fn fn, void *arg) {
    event_assign(&event_, loop->event_loop_, sock, flags, fn, arg);
}

void Event::setTimer(EventLoop *loop, event_callback_fn fn, void *arg)
{
    evtimer_assign(&event_, loop->event_loop_, fn, arg);
}

void Event::active(int timeout)
{
    if (timeout != -1) {
        timeval val;
        val.tv_sec = (timeout / 1000);
        val.tv_usec = (timeout - val.tv_sec * 1000) * 1000;
        event_add(&event_, &val);
    } else {
        event_add(&event_, NULL);
    }
}

void Event::remove(void)
{
    event_del(&event_);
}

EventLoop::EventLoop(void)
{
    evthread_use_pthreads();
    event_loop_ = event_base_new();
}

EventLoop::~EventLoop(void)
{
    if (event_loop_) {
        event_base_free(event_loop_);
    }
}

void EventLoop::exec(void)
{
    if (event_loop_) {
        if (event_base_dispatch(event_loop_) < 0) {
            printf("EventLoop::exec event_base_dispatch() failed\n");
        }
    }
}

void EventLoop::exit(int timeout)
{
    event_base_loopbreak(event_loop_);
    return;
}

void emptyCallBack(evutil_socket_t, short, void*) {
}

EventLoopThread::EventLoopThread(void)
{}

EventLoopThread::~EventLoopThread(void)
{}

void EventLoopThread::exit(void)
{
    event_.remove();
    loop_.exit();
}

void EventLoopThread::run(void) 
{
    event_.set(&loop_, -1, EV_PERSIST | EV_TIMEOUT, emptyCallBack, this);
    event_.active(60000);
    loop_.exec();
}

EventLoopThreadPool::EventLoopThreadPool(void)
{
    size_ = 0;
    threads_ = NULL;
}

EventLoopThreadPool::~EventLoopThreadPool(void) 
{
    stop();
}

void EventLoopThreadPool::start(int size)
{
    stop();
    
    size_ = size;
    if (size_ <= 0 || size_ > MaxThreadCount) {
        printf("EventLoopThreadPool::start size out of range, use default size\n");
        size_ = DefaultThreadCount;
    }

    printf("create the thread pool\n");
    threads_ = new EventLoopThread[size_];
    for (int i = 0; i < size_; ++i) {
        threads_[i].start();
    }

    Thread::sleep(100);
    printf("Thread Pool create, size:%d\n", size_);
}

void EventLoopThreadPool::stop(void)
{
    if (threads_) {
        printf("Destory thread pool\n");
        for (int i = 0; i < size_; ++i) {
            threads_[i].exit();
        }

        delete []threads_;
        threads_ = NULL;
        size_ = 0;
    }
}

EventLoopThread *EventLoopThreadPool::thread(int index) const
{
    if ((index >= 0) && (index < size_)) {
        return &threads_[index];
    }

    return NULL;
}

