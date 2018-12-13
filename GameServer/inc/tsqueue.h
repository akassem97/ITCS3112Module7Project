#ifndef BATTLESHIP_TS_QUEUE_H_
#define BATTLESHIP_TS_QUEUE_H_

// standard library headers
#include <condition_variable>
#include <list>
#include <mutex>

namespace battleship {

/**
 * @brief A thread safe queue implementation.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 12-3-2018
 */
template<typename T> class ts_queue {

private:
    std::list<T> queue; // the item queue
    mutable std::mutex queue_mutex; // handles queue mutual exclusion
    mutable std::condition_variable condv; // used to notify waiting threads

public:
    /**
     * Default constructor.
     */
    ts_queue() {
    }

    /**
     * Default destructor.
     */
    ~ts_queue() {
    }

    /**
     * Thread safe method that adds a passed in item to the end of this queue.
     *
     * @param item - the item to add to the end of the queue.
     */
    inline void add(T item) {
        // threads block until they acquire lock
        std::unique_lock<std::mutex> lock(queue_mutex);

        queue.push_back(item);

        // notify waiting threads an item has been added
        condv.notify_one();
    }

    /**
     * Thread safe remove method that pops and returns the front of this queue.
     * If the queue is empty, then this method will block until the queue is not
     * empty.
     *
     * @return the item at the front of the queue.
     */
    inline T remove() {
        // threads block until they acquire lock
        std::unique_lock<std::mutex> lock(queue_mutex);
        // threads wait until notified if queue is empty
        condv.wait(lock, [this]() {return queue.size() > 0;});

        T item = queue.front();
        queue.pop_front();

        return item;
    }

    /**
     * Thread safe method that gets the size of this queue.
     *
     * @return the size of this queue.
     */
    inline size_t size() {
        // threads block until they acquire lock
        std::unique_lock<std::mutex> lock(queue_mutex);

        size_t count = queue.size();

        return count;
    }

};

} // namespace battleship

#endif /* BATTLESHIP_TS_QUEUE_H_ */
