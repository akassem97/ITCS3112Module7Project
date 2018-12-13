/*
 * gamepool.h
 *
 *  Created on: Dec 11, 2018
 *      Author: henly
 */

#ifndef BATTLESHIP_GAME_POOL_H_
#define BATTLESHIP_GAME_POOL_H_

// standard library headers
#include <atomic>
#include <condition_variable>
#include <list>
#include <mutex>
#include <vector>

// battleship headers
#include "client.h"
#include "game.h"
#include "tsqueue.h"

namespace battleship {

namespace game_server {

/**
 * @brief A thread safe game pool for holding server games.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 11-27-2018
 */
class game_pool {

private:
    std::vector<std::shared_ptr<game>> pool; // holds pointers to games
    std::list<unsigned char> empty_queue; // holds empty pool indexes
    const unsigned char max_game_count; // max number of games in pool
    unsigned char game_count; // number of games in pool

    mutable std::atomic<int> reader_count; // current number of reading threads
    mutable std::mutex writer_mutex; // mutex used to block writing threads
                                     // until another writer thread finishes or
                                     // reader threads finish
    mutable std::condition_variable writer_condv; // condition to signal writing
                                                  // thread that readers are
                                                  // done
    ts_queue<std::weak_ptr<client>>* ready_queue; // queue that holds ready clients

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

    // don't implement default constructor
    game_pool() = delete;
    /* Disallow Copy Operations */
    game_pool(const game_pool&) = delete; // don't implement copy c'tor
    game_pool& operator=(const game_pool&) = delete; // don't implement copy
                                                     // assignment


    std::weak_ptr<game> create_and_add_game(std::weak_ptr<client> c_one,
        std::weak_ptr<client> c_two);

public:

    // create a game pool with a specified max_size
    explicit game_pool(const unsigned char max_size);

    /**
     * Default destructor.
     */
    ~game_pool() {
    }

    //
    std::weak_ptr<game> add_ready_client(std::weak_ptr<client> r_client);


    //
    bool remove(const unsigned char game_id);

    //
    const std::weak_ptr<game> get(const unsigned char game_id) const;

    //
    std::unique_lock<std::mutex> get_lock();

    //
    const std::shared_ptr<game> get_with_lock(
        std::unique_lock<std::mutex>& lock, const unsigned char game_id);

    /**
     * Gets the maximum number of games allowed in this pool instance.
     * @return the max number of games allowed in this pool.
     */
    unsigned char get_max_game_count() const {
        return max_game_count;
    }

    /**
     * Gets the number of games in this pool.
     * @return the number of games in this pool.
     */
    unsigned char size() const {
        // block or acquire lock and increase reader count then release lock
        enter_reader_section();

        unsigned char size = game_count;

        // decrease reader count and notify waiting writer if reader count is 0
        exit_reader_section();

        return size;
    }

};
// class game_pool

}// namespace server

} // namespace battleship

#endif /* BATTLESHIP_GAME_POOL_H_ */
