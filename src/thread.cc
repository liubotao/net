
#include <memory.h>
#include <unistd.h>
#include <pthread.h>

#include "thread.h"

class ThreadPrivate
{
public:
    ThreadPrivate(Thread* thread) :
        isRunning_(false),
        thread_(thread) {}

    virtual ~ThreadPrivate(void) {
        if (isRunning_) {
            terminate();
        }
    }

    virtual void start(void) {}
    virtual void terminate(void) {}
    bool isRunning(void) const { return isRunning_;}
    
    bool isRunning_;
    Thread* thread_;
};


class UnixThread : public ThreadPrivate
{
public:
    UnixThread(Thread* thread) :
        ThreadPrivate(thread) {}
    ~UnixThread(void) {}

    virtual void start(void) {
        pthread_create(&thread_id_, NULL, UnixThreadEntry, thread_);
    }

    virtual void terminate(void) {
        pthread_cancel(thread_id_);
    }

    static void* UnixThreadEntry(void* lp) {
        Thread* thread = (Thread*)lp;
        thread->priv_->isRunning_ = true;
        thread->run();
        thread->priv_->isRunning_ = false;
        return NULL;
    }

private:
    pthread_t thread_id_;
};


Thread::Thread(void) : priv_(NULL) 
{
    priv_ = new UnixThread(this);
}

Thread::~Thread(void) 
{
    delete priv_;
}

void Thread::start(void)
{
    if (!isRunning()) {
        priv_->start();
    }
}

void Thread::terminate(void) 
{
    if (isRunning()) {
        priv_->terminate();
    }
}

bool Thread::isRunning(void) const 
{
    return priv_->isRunning();
}

void Thread::sleep(int msec)
{
    usleep(msec * 1000);
}

Thread::tid_t Thread::currentThreadId(void)
{
    pthread_t tid = pthread_self();
    tid_t thread_id = 0;
    memcpy(&thread_id, &tid, sizeof(tid_t));
    return thread_id;
}


