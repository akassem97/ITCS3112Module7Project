#ifndef BATTLESHIP_CLIENT_POOL_H_
#define BATTLESHIP_CLIENT_POOL_H_

#include <client.h>
#include <atomic>
#include <condition_variable>
#include <cstdint> // std::unsigned char
#include <list>
#include <mutex>
#include <vector>


namespace battleship {

namespace server {

/**
 * @brief A thread safe client pool for holding server clients.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 11-27-2018
 */
class client_pool {

private:
    std::vector<std::shared_ptr<client>> pool; // holds pointers to clients
    std::list<unsigned char> empty_queue; // holds empty pool indexes
    const unsigned char max_client_count; // max number of clients in pool
    unsigned char client_count; // number of clients in pool
    std::vector<std::string> cl_cache; // client listing cache

    mutable std::atomic<int> reader_count; // current number of reading threads
    mutable std::mutex writer_mutex; // mutex used to block writing threads
                                     // until another writer thread finishes or
                                     // reader threads finish
    mutable std::condition_variable writer_condv; // condition to signal writing
                                                  // thread that readers are
                                                  // done

    /*
     * Reader threads wait if pool is being written. Once a reader thread
     * acquires the lock, it then increases readerCount in order to make
     * writer threads wait. The reader thread then releases the lock for
     * another reader thread or a writer thread to acquire it.
     */
    void enter_reader_section() const {
        std::unique_lock<std::mutex> writeLock(writer_mutex);
        reader_count += 1;
    }

    /*
     * Decrease readerCount to allow writer threads to run, notify a possibly
     * waiting writer thread to stop waiting.
     */
    void exit_reader_section() const {
        reader_count -= 1;

        if (reader_count == 0) {
            writer_condv.notify_one();
        }
    }

    /* Disallow Copy Operations */
    client_pool(const client_pool&) = delete; // don't implement copy c'tor
    client_pool& operator=(const client_pool& rhs) = delete; // don't implement
                                                             // copy assignment

public:

    //
    explicit client_pool(const unsigned char max_size);

    /**
     * Default destructor.
     */
    ~client_pool() {
    }

    //
    std::shared_ptr<client> create_and_add_client(const std::string client_name,
        tcp_stream* connection);

    //
    bool remove(const unsigned char clientId);

    //
    const std::shared_ptr<client> get(const unsigned char clientId) const;

    //
    std::unique_lock<std::mutex> get_lock();

    //
    const std::shared_ptr<client> get_with_lock(
        std::unique_lock<std::mutex>& lock, const unsigned char clientId);

    //
    std::string get_client_listings();

    /**
     * Gets the maximum number of clients allowed in this pool instance.
     * @return the max number of clients allowed in this pool.
     */
    unsigned char get_max_client_count() const {
        return max_client_count;
    }

    /**
     * Gets the number of clients in this pool.
     * @return the number of clients in this pool.
     */
    unsigned char size() const {
        // block or acquire lock and increase reader count then release lock
        enter_reader_section();

        unsigned char size = client_count;

        // decrease reader count and notify waiting writer if reader count is 0
        exit_reader_section();

        return size;
    }

};

} // namespace server

} // namespace battleship

#endif /* BATTLESHIP_CLIENT_POOL_H_ */
