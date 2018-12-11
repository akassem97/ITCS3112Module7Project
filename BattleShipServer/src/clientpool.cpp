// source header
#include "clientpool.h"


namespace battleship {

namespace game_server {

// private const static member
const std::string client_pool::EMPTY_STRING = "";

/**
 * Creates a client pool with a maximum number of allowed clients.
 *
 * @param max_size - the maximum number of clients allowed in this pool.
 */
client_pool::client_pool(const unsigned char max_size)
    : max_client_count(max_size), client_count(0)
{
    // request that pool preallocate enough memory for 'size + 1' clients, '+ 1'
    // because pool is 1 indexed, not 0 indexed -- explanation in comment below
    pool.reserve(max_client_count + 1);

    // fill empty queue with empty pool indexes, skip 0 to avoid client ID being
    // '\0' - the terminating char in most, basic string representations
    for (unsigned char i = 1; i < max_client_count + 1; i++) {
        empty_queue.push_back(i);
        cl_cache.push_back("");
    }

    cl_cache.push_back("");
}

//
client* client_pool::create_tmp_client(tcp_stream* const connection) {
    return new client(0, EMPTY_STRING, connection);
}

/**
 * Creates a {@code client} with a unique id and a passed in {@code TCPStream*}.
 * The created {@code client} is then wrapped in an
 * {@code std::shared_ptr<client>} smart pointer and added to the pool.
 *
 * @param connection - the TCP stream of the new client.
 * @return a smart pointer to the created client, or {@code nullptr} if this
 *         pool is full or an exception is thrown.
 */
std::shared_ptr<client> client_pool::create_and_add_client(
    const std::string client_name, tcp_stream* const connection)
{
    // writer threads wait for reader threads to finish then acquire lock
    std::unique_lock<std::mutex> write_lock(writer_mutex);
    writer_condv.wait(write_lock, [this]() {return reader_count == 0;});

    // signal server that it's reached max clients
    if (client_count == max_client_count) {
        return nullptr;
    }

    // assign an empty pool index as the client's id
    const int client_id = empty_queue.front();
    empty_queue.pop_front();

    // create the client in a shared pointer to store and return
    std::shared_ptr<client> new_client = std::shared_ptr<client>(
        new client(client_id, client_name, connection));
    //std::make_shared<client>(client_id,
    //client_name, connection);

    // put the client in the empty pool index
    pool[client_id] = new_client;
    client_count += 1;

    cout << "This far!" << endl;
    // cache new client's listing
    cl_cache[client_id] = new_client->get_listing();
    cout << "MOBY DICK!" << endl;
    // unlock not needed, writerLock 'unlocks' when it goes out of scope
    // writeLock.unlock();

    return new_client;
}

/**
 * ToDo finish documentation
 *
 * Finally, it makes the specified client ID available for re-use and decreases
 * the client count by one.
 *
 * @param clientId - the ID of the client to remove.
 * @return true if the client with matching specified ID was removed from this
 *         pool, otherwise false.
 */
bool client_pool::remove(const unsigned char client_id) {
    // bounds checking on clientId, account for 1, not 0, indexed
    if (client_id < 1 || client_id > max_client_count + 1) {
        return false;
    }

    // writer threads acquire lock then wait for reader threads to finish
    std::unique_lock<std::mutex> write_lock(writer_mutex);
    writer_condv.wait(write_lock, [this]() {return reader_count == 0;});

    // client pointer used for return
    std::shared_ptr<client> removed = pool[client_id];

    // if client_id is not in pool then return false
    if (!removed) {
        return false;
    }

    // remove client from pool
    pool[client_id] = nullptr;
    client_count -= 1;

    // remove client listing from cache
    cl_cache[client_id] = EMPTY_STRING;

    // decrease the reference count of the shared pointer by one, hopefully
    // leading to the 'delete'ion of the raw client pointer
    removed.reset();

    // recycle client_id
    empty_queue.push_back(client_id);

    // unlock not needed, writerLock "unlocks" when it goes out of scope
    // writeLock.unlock();

    return true;
}

/**
 * Gets the client, whose ID matches the passed in ID, from this client pool
 * instance.
 *
 * This method is the reader version of {@code client_pool::get_with_lock}.
 *
 * Note: this method will return {@code nullptr} if the specified client ID is
 * not found in this client pool.
 *
 * @param clientId - the ID of the client to retrieve.
 * @return the client whose ID is {@code clientId} or {@code nullptr}.
 */
const std::shared_ptr<client> client_pool::get(
    const unsigned char client_id) const
{
    // bounds checking on clientId, account for 1, not 0, indexed
    if (client_id < 1 || client_id > max_client_count + 1) {
        return nullptr;
    }

    // block or acquire lock and increase reader count then release lock
    enter_reader_section();

    // get client or nullptr from pool
    std::shared_ptr<client> element = pool[client_id];

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
 * @see client_pool::get_with_lock
 */
std::unique_lock<std::mutex> client_pool::get_lock() {
    // returns when this writer thread acquires lock
    return std::unique_lock<std::mutex>(writer_mutex);
}

/**
 * Gets the client with specified client ID, from this client pool instance,
 * while owning this pool's writer mutex lock.
 *
 * This method is the writer version of {@code client_pool::get}. It is used in
 * conjunction with {@code client_pool::get_lock} to safely update the state of
 * a client.
 *
 * Note: this method will return {@code nullptr} if the specified client ID is
 * not found in this client pool.
 *
 * @param lock - the writer lock obtained from {@code client_pool::get_lock}.
 * @param clientId - the ID of the client to get.
 *
 * @return a safe pointer with the client matching the specified client ID, or
 *         {@code nullptr}.
 *
 * @see client_pool::get_lock
 * @see client_pool::get
 */
const std::shared_ptr<client> client_pool::get_with_lock(
    std::unique_lock<std::mutex>& lock, const unsigned char client_id)
{
    // bounds checking on clientId, account for 1, not 0, indexed
    if (client_id < 1 || client_id > max_client_count + 1) {
        return nullptr;
    }
    // wait for any reader threads to finish
    writer_condv.wait(lock, [this]() {return reader_count == 0;});

    std::shared_ptr<client> client = pool[client_id];

    return client;
}

/**
 * Returns a string listing of the client IDs, names and status of the clients
 * in this pool.
 *
 * The listing contains a comma separated entry for each client, with each
 * client entry taking the form {@code <id> + <status> + <client name> + ','}.
 * Where {@code 'i'} is of type {@code unsigned char}, {@code 's'} is of type
 * {@code char} and {@code <client name>} is of type {@code std::string}.
 *
 * @return a comma separated listing of the clients, and their attributes, in
 *         this pool.
 */
std::string client_pool::get_client_listings() {
    // block until lock acquired, then increase reader count and release lock
    enter_reader_section();

    // ToDo implement this method
    std::string listings;
    for (auto cl_iter = cl_cache.begin(); cl_iter != cl_cache.end();
        ++cl_iter) {
        if (*cl_iter == EMPTY_STRING) continue;

        listings += *cl_iter + ',';
    }

    // decrease reader count and possibly notify waiting writer
    exit_reader_section();

    return listings;
}

} // namespace server

} // namespace battleship
