// source header
#include "gamepool.h"

namespace battleship {

namespace game_server {

/**
 * Creates a client pool with a maximum number of allowed clients.
 *
 * @param max_size - the maximum number of clients allowed in this pool.
 */
game_pool::game_pool(const unsigned char max_size)
    : max_game_count(max_size), game_count(0)
{
    // preallocate enough memory for 'size + 1' games, '+ 1' because pool is 1
    // indexed, not 0 indexed -- explanation in comment below
    pool.reserve(max_game_count + 1);

    // fill empty queue with empty pool indexes, skip 0 to avoid game ID being
    // '\0' - the terminating char in most, basic string representations
    for (unsigned char i = 1; i < max_game_count + 1; ++i) {
        empty_queue.push_back(i);
    }

    // create the client ready queue
    ready_queue = new ts_queue<std::weak_ptr<client>>();
}

/**
 *
 * @param client_id
 * @return
 */
std::weak_ptr<game> game_pool::add_ready_client(std::weak_ptr<client> r_client)
{
    // writer threads wait for reader threads to finish then acquire lock
    std::unique_lock<std::mutex> write_lock(writer_mutex);
    writer_condv.wait(write_lock, [this]() {return reader_count == 0;});

    // check if ready client is still valid
    auto s_r_client = r_client.lock();
    if (!s_r_client) {
        return std::weak_ptr<game>();
    }

    // check if there is a waiting client in ready queue
    if (ready_queue->size() < 1) {
        s_r_client->set_searching();

        ready_queue->add(r_client);

        return std::weak_ptr<game>();
    }

    // get the weak ptr to waiting client
    weak_ptr<client> other = ready_queue->remove();

    // check if the waiting client is still valid
    if (!other.lock()) {
        ready_queue->add(r_client);
        return std::weak_ptr<game>();
    }

    // return a new game with both clients
    return create_and_add_game(r_client, other);
}

/*
 * Creates a {@code game} with a unique id and two passed in {@code client}
 * smart pointers.
 *
 * The created {@code game} is then wrapped in an
 * {@code std::shared_ptr<game>} and added to the pool.
 *
 * @param c_one - the first {@code client} smart pointer.
 * @param c_two - the second {@code client} smart pointer.
 * @return a smart pointer to the created {@code game}, or {@code nullptr} if
 *         this pool is full or an exception is thrown.
 */
std::weak_ptr<game> game_pool::create_and_add_game(
    std::weak_ptr<client> c_one,
    std::weak_ptr<client> c_two)
{
    // signal server that it's reached max games
    if (game_count >= max_game_count) {
        // return an empty weak_ptr
        return std::weak_ptr<game>();
    }

    // upgrade to shared pointers
    std::shared_ptr<client> s_c_one = c_one.lock();
    std::shared_ptr<client> s_c_two = c_two.lock();

    // check for valid clients
    if (!s_c_one) {
        if (!s_c_two) {
            return std::weak_ptr<game>();
        }
        // s_c_two is still valid so put back in ready queue
        ready_queue->add(c_two);
        return std::weak_ptr<game>();
    }

    // check for valid client
    if (!c_two.lock()) {
        ready_queue->add(c_one);
        return std::weak_ptr<game>();
    }

    // assign an empty pool index as the game's id
    const int game_id = empty_queue.front();
    empty_queue.pop_front();

    // create a shared ptr with the game and put it in the empty pool index
    std::shared_ptr<game> shared_game = std::make_shared<game>(game_id, c_one,
        c_two);
    game_count += 1;

    s_c_one->set_in_game(game_id);
    s_c_two->set_in_game(game_id);

    // store the shared_ptr in pool
    pool[game_id] = shared_game;

    // return a weak pointer to store
    std::weak_ptr<game> new_game(shared_game);

    // unlock not needed, writerLock 'unlocks' when it goes out of scope
    // writeLock.unlock();

    return new_game;
}

/**
 * ToDo finish documentation
 *
 * Finally, it makes the specified client ID available for re-use and decreases
 * the game count by one.
 *
 * @param gameId - the ID of the game to remove.
 * @return true if the game with matching specified ID was removed from this
 *         pool, otherwise false.
 */
bool game_pool::remove(const unsigned char game_id) {
    // bounds checking on game_id, account for 1, not 0, indexed
    if (game_id < 1 || game_id > max_game_count + 1) {
        return false;
    }

    // writer threads acquire lock then wait for reader threads to finish
    std::unique_lock<std::mutex> write_lock(writer_mutex);
    writer_condv.wait(write_lock, [this]() {return reader_count == 0;});

    // game pointer used for return
    std::shared_ptr<game> removed = pool[game_id];

    // if game_id is not in pool then return false
    if (!removed) {
        return false;
    }

    // remove game from pool
    pool[game_id] = nullptr;
    game_count -= 1;

    // decrease the reference count of the shared pointer by one, hopefully
    // leading to the 'delete'ion of the raw game pointer
    removed.reset();

    // recycle game_id
    empty_queue.push_back(game_id);

    // unlock not needed, writerLock "unlocks" when it goes out of scope
    // writeLock.unlock();

    return true;
}

/**
 * Gets the game, whose ID matches the passed in ID, from this game pool
 * instance.
 *
 * This method is the reader version of {@code game_pool::get_with_lock}.
 *
 * Note: this method will return an empty {@code std::weak_ptr<game>} if the
 * specified game ID is not found in this game pool.
 *
 * @param game_id - the ID of the game to retrieve.
 * @return the game whose ID is {@code game_id} or {@code nullptr}.
 */
const std::weak_ptr<game> game_pool::get(const unsigned char game_id) const
{
    // bounds checking on clientId, account for 1, not 0, indexed
    if (game_id < 1 || game_id > max_game_count + 1) {
        //
        std::weak_ptr<game>();
    }

    // block or acquire lock and increase reader count then release lock
    enter_reader_section();

    // get client or nullptr from pool
    std::weak_ptr<game> element = pool[game_id];

    // decrease reader count and notify waiting writer if reader count is 0
    exit_reader_section();

    return element;
}

/**
 * Acquires and returns a lock on this pool's mutex. This method blocks until
 * the lock can be acquired.
 *
 * Note: if not manually released, the returned lock will release once the
 * calling site goes out of scope.
 *
 * @return a lock on this client pool's mutex.
 *
 * @see game_pool::get_with_lock
 */
std::unique_lock<std::mutex> game_pool::get_lock() {
    // returns when this writer thread acquires lock
    return std::unique_lock<std::mutex>(writer_mutex);
}

/**
 * Gets the client with specified client ID, from this client pool instance,
 * while owning this pool's writer mutex lock.
 *
 * This method is the writer version of {@code game_pool::get}. It is used in
 * conjunction with {@code game_pool::get_lock} to safely update the state of
 * a client.
 *
 * Note: this method will return {@code nullptr} if the specified client ID is
 * not found in this client pool.
 *
 * @param lock - the writer lock obtained from {@code game_pool::get_lock}.
 * @param clientId - the ID of the client to get.
 *
 * @return a safe pointer with the client matching the specified client ID, or
 *         {@code nullptr}.
 *
 * @see game_pool::get_lock
 * @see game_pool::get
 */
const std::shared_ptr<game> game_pool::get_with_lock(
    std::unique_lock<std::mutex>& lock, const unsigned char game_id)
{
    // bounds checking on clientId, account for 1, not 0, indexed
    if (game_id < 1 || game_id > max_game_count + 1) {
        return nullptr;
    }
    // wait for any reader threads to finish
    writer_condv.wait(lock, [this]() {return reader_count == 0;});

    auto game = pool[game_id];

    return game;
}

} // namespace server

} // namespace battleship
