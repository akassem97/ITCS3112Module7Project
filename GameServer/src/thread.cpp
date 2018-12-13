// source header
#include "thread.h"

/* ToDo  finish documenting the methods of this class */

/**
 * ToDo  document
 * @param arg
 */
static void* runThread(void* arg) {
    return ((Thread*) arg)->run();
}

/**
 * ToDo  document
 */
Thread::Thread()
    : tid(0), running(0), detached(0)
{
}

/**
 * ToDo  document
 */
Thread::~Thread() {
    if (running == 1 && detached == 0) {
        pthread_detach(tid);
    }

    if (running == 1) {
        pthread_cancel(tid);
    }
}

/**
 * @return 0 if this thread successfully starts, otherwise a non-zero value.
 */
int Thread::start() {
    // create a new thread bound to virtual Thread::run
    int result = pthread_create(&tid, nullptr, runThread, this);

    // signal that this thread is running
    if (result == 0) {
        running = 1;
    }

    return result;
}

/**
 * Blocks the current thread until this thread finishes its execution.
 * @return 0 if this thread joins, otherwise a non-zero value.
 */
int Thread::join() {
    int result = -1;
    if (running == 1) {
        result = pthread_join(tid, nullptr);
        if (result == 0) {
            detached = 0;
        }
    }
    return result;
}

/**
 * Separates the thread of execution from the thread object, allowing execution
 * to continue independently.
 * @return 0 if this thread detaches, otherwise a non-zero value.
 */
int Thread::detach() {
    int result = -1;
    if (running == 1 && detached == 0) {
        result = pthread_detach(tid);
        if (result == 0) {
            detached = 1;
        }
    }
    return result;
}

/**
 * @return this thread's id.
 */
pthread_t Thread::self() {
    return tid;
}
