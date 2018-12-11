#ifndef BATTLESHIP_THREAD_H_
#define BATTLESHIP_THREAD_H_

#include <pthread.h>

/**
 * @brief A pthread wrapper implementation.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 11-27-2018
 */
class Thread {
private:
    pthread_t tid; // thread's id
    int running; // if this thread is running
    int detached; // if this thread is detached

public:
    Thread();
    virtual ~Thread();

    int start();
    int join();
    int detach();
    pthread_t self();

    virtual void* run() = 0;
};

#endif
