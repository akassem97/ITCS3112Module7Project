#ifndef BATTLESHIP_CLIENT_H_
#define BATTLESHIP_CLIENT_H_

// standard library headers
#include <atomic>
#include <iostream> // DEBUG
#include <mutex> // std::mutex, std::lock_gaurd
#include <string>

// battleship headers
#include "message.h" // enum class signal
#include "tcpstream.h"

namespace battleship {

namespace game_server {

/**
 * @brief Represents a client connected to the server.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 11-27-2018
 */
class client {
private:
    // private typedefs
    typedef network_message::message message;
    typedef network_message::game_message game_message;

    // private static const members
    static const unsigned char SERVER_ID = 255;
    // client states
    static const unsigned char STATUS_IDLE = (1u << 0);
    static const unsigned char STATUS_WAITING = (1u << 1);
    static const unsigned char STATUS_IN_GAME = (1u << 2);
    static const unsigned char STATUS_BUSY = (1u << 3);

    // const private members
    const unsigned char id; // id of this client
    const std::string name; // name of client
    // mutable private members
    mutable tcp_stream* stream; // communication channel
    mutable int tout_count; // number of times client has timed out
    // private members
    std::atomic_uchar status; // status of client
    unsigned char game_id;
    char l_cache[32];
    // private synchronization members
    mutable std::mutex stream_mux; // stream ::send and ::receive synchronization

    // returns the status of this client
    inline unsigned char get_status() {
        return status.load();
    }

    // const version of client::get_status
    inline const unsigned char get_status() const {
        return status;
    }

    inline void set_game_id(const unsigned char g_id) {
        if (g_id == 0) {
            status.store(STATUS_IDLE);
            game_id = 0;
        } else {
            status.store(STATUS_IN_GAME);
            game_id = g_id;
        }
    }

    // client_pool should be the only class that can create clients
    friend class client_pool;

    /*
     * Client_pool needs access to temporary handshake client's tcp_stream when
     * adding a client from server handshake procedure.
     *
     * Note: calling this method leaves this client instance in a valid, but
     * non-functional state. Calling '~client()` will result in 'delete nullptr'
     * which is a no-op.
     */
    tcp_stream* move_stream() {
        tcp_stream* tmp;

        // begin synchronization critical section
        {
            // threads block until lock acquired
            std::lock_guard<std::mutex> stream_lock(stream_mux);

            // move ownership of tcp_stream to tmp
            tmp = std::move(stream);
            // leave this client in a valid state, i.e. assign this client's
            // tcp_stream to nullptr
            stream = nullptr;

            // stream lock unlocks
        } // end synchronization critical section

        return tmp;
    }

    // used by client_pool to create client instances
    explicit client(const unsigned char id, const std::string name,
        tcp_stream* const stream)
        : id(id), name(name), stream(stream), tout_count(0),
            status(STATUS_IDLE), game_id(0)
    {
        l_cache[0] = '\0';
    }

    // don't implement default constructor
    client() = delete;
    /* Disallow Copy Operations */
    client(const client&) = delete; // don't implement copy constructor
    client& operator=(const client&) = delete; // don't implement copy
                                               // assignment oper.

public:

    /**
     * Destructor that deletes the {@code tcp_stream} connection associated with
     * this server client.
     */
    virtual ~client() {
        // DEBUG
        std::cout << "Deleting client with id [" << id << "]." << std::endl;

        delete stream;
    }

    /**
     * Returns a pointer to a message that was created from the bytes sent by
     * this client.
     *
     * Note: if both {@code tout_s} and {@code tout_us} are 0, the this method
     * will block until data is received.
     *
     * @param tout_s - the time to wait for data in seconds.
     * @param tout_us - the time to wait for data in microseconds.
     * @return a pointer to a message constructed from the bytes this client
     *         sent.
     */
    message* receive(const int tout_s = 0, const int tout_us = 0) const {
        char rec_buffer[255]; // receiving by buffer
        ssize_t len; // holds bytes sent or errors

        // begin synchronization critical section
        {
            // threads block until lock acquired
            std::lock_guard<std::mutex> stream_lock(stream_mux);

            // fill buffer with bytes from client
            len = stream->receive(rec_buffer, sizeof(rec_buffer) - 1, tout_s,
                tout_us);

            // stream lock unlocks
        } // end synchronization critical section

        // check for the different errors tcp_Stream#receive can return
        switch (len) {

            // lost connection with the client
            case stream->CONNECTION_CLOSED:
                return message::create_message(network_message::type::ERROR,
                    network_message::signal::CONNECTION_CLOSED, get_id(),
                    SERVER_ID);

                // no data was received within specified time limit
            case stream->CONNECTION_TIMEOUT:
                return message::create_message(network_message::type::ERROR,
                    network_message::signal::CONNECTION_TIMED_OUT, get_id(),
                    SERVER_ID);

                // no errors, parse and return received message
            default:

                // parse message based on first byte being network_message::type
                switch (static_cast<network_message::type>(rec_buffer[0])) {

                    // received a base class network_message::message
                    default:
                        return message::parse_message(rec_buffer, len);
                        break;

                        // received a network_message::game_message derived
                        // from network_message::message
                    case network_message::type::GAME:
                        return game_message::parse_message(rec_buffer, len);
                        break;

                } // switch -- network_message::type

        } // switch -- receiving error or message

    }

    /**
     * Sends a message over the network to a client.
     *
     * @param msg - a pointer to the message to send to the client.
     *
     * @return the number of bytes sent to the client, or if negative, an error
     *         value.
     */
    ssize_t send(const network_message::message* msg) {
        ssize_t len; // holds bytes sent or errors

        // begin synchronization critical section
        {
            // threads block until lock acquired
            std::lock_guard<std::mutex> stream_lock(stream_mux);

            // fill buffer with bytes from client
            len = stream->send(msg->msg(), msg->length());

            // stream lock unlocks
        } // end synchronization critical section

        return len;
    }

    /**
     * Sends a message over the network to a client.
     *
     * @param msg - the message to send to the client.
     *
     * @return the number of bytes sent to the client, or if negative, an error
     *         value.
     */
    ssize_t send(const network_message::message&& msg) {
        ssize_t len; // holds bytes sent or errors

        // begin synchronization critical section
        {
            // threads block until lock acquired
            std::lock_guard<std::mutex> stream_lock(stream_mux);

            // fill buffer with bytes from client
            len = stream->send(msg.msg(), msg.length());

            // stream lock unlocks
        } // end synchronization critical section

        return len;
    }

    /**
     * Sends a message over the network to a client.
     *
     * @param msg - the message to send to the client.
     *
     * @return the number of bytes sent to the client, or if negative, an error
     *         value.
     */
    ssize_t send(const network_message::message& msg) {
        ssize_t len; // holds bytes sent or errors

        // begin synchronization critical section
        {
            // threads block until lock acquired
            std::lock_guard<std::mutex> stream_lock(stream_mux);

            // fill buffer with bytes from client
            len = stream->send(msg.msg(), msg.length());

            // stream lock unlocks
        } // end synchronization critical section

        return len;
    }

    /**
     * @return this client's ID.
     */
    inline unsigned char get_id() {
        return id;
    }

    /**
     * @return this client's ID.
     */
    inline const unsigned char get_id() const {
        return id;
    }

    /**
     * @return this client's name.
     */
    inline std::string get_name() {
        return name;
    }

    /**
     * @return this client's name.
     */
    inline const std::string get_name() const {
        return name;
    }

    /**
     *
     */
    inline unsigned char get_game_id() {
        return game_id;
    }

    /**
     *
     */
    inline const unsigned char get_game_id() const {
        return game_id;
    }

    /**
     * Sets the status of this client to in-game.
     */
    inline void set_in_game() {

        status = STATUS_IN_GAME;
    }

    /**
     * @return true if the status of this client is in-game, false otherwise.
     */
    inline bool is_in_game() {
        return get_status() & STATUS_IN_GAME;
    }

    /**
     * @return true if the status of this client is in-game, false otherwise.
     */
    inline const bool is_in_game() const {
        return get_status() & STATUS_IN_GAME;
    }

    /**
     * Sets the status of this client to idle.
     */
    inline void set_idle() {
        status = STATUS_IDLE;
    }

    /**
     * @return true if the status of this client is idle, false otherwise.
     */
    inline bool is_idle() {
        return status.load(memory_order::memory_order_relaxed) & STATUS_IDLE;
    }

    /**
     * @return true if the status of this client is idle, false otherwise.
     */
    inline const bool is_idle() const {
        return status.load(memory_order::memory_order_relaxed) & STATUS_IDLE;
    }

    /**
     * Sets the status of this client to busy.
     */
    inline void set_searching() {
        status = STATUS_BUSY;
    }

    /**
     * @return true if the status of this client is busy, false otherwise.
     */
    inline bool is_searching() {
        return get_status() & STATUS_BUSY;
    }

    /**
     * @return true if the status of this client is busy, false otherwise.
     */
    inline const bool is_searching() const {
        return get_status() & STATUS_IDLE;
    }

    /**
     *
     * @param max_touts - the number of allowed timeouts before action should be
     * taken.
     * @return true if this client's timeout count is greater than
     *         {@code max_touts}, otherwise false.
     */
    inline bool timed_out(const int max_touts) {
        tout_count += 1;

        return tout_count >= max_touts;
    }

    /**
     * Resets this client's timeout count to zero.
     */
    inline void reset_timeouts() {
        tout_count = 0;
    }

    /**
     * Returns this client's listing as an {@code std::string}. The first
     * {@code char} is the client's ID, the second is the client's status and
     *
     * @return
     */
    std::string get_listing() {
        const int L_CACHE_STATUS_OFFSET = 1;

        // create client listing on first call to get_listing, all subsequent
        // calls just update the listing with the client's status
        if (l_cache[0] == '\0') {
            const int L_CACHE_ID_OFFSET = 0;

            // listing bytes: 1 byte(<client id>) + 1 byte(<client status>) +
            // [n < 30] bytes(<client name>)
            const size_t listing_length = 2 + get_name().length();
            l_cache[listing_length] = '\0';
            l_cache[L_CACHE_ID_OFFSET] = id;
            l_cache[L_CACHE_STATUS_OFFSET] = status;

            // finally copy name bytes to listing
            const char* tmp_name = get_name().c_str();
            for (size_t i = 0; i < listing_length; ++i) {
                l_cache[i + 2] = tmp_name[i];
            }
        } else {
            l_cache[L_CACHE_STATUS_OFFSET] = status;
        }

        return std::string(l_cache);
    }

}
;

} // namespace server

} // namespace battleship

#endif /* BATTLESHIP_CLIENT_H_ */
