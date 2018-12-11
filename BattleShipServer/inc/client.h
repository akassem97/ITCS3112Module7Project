#ifndef BATTLESHIP_CLIENT_H_
#define BATTLESHIP_CLIENT_H_

#include <iostream> // DEBUG
#include <mutex> // std::mutex, std::lock_gaurd
#include <string>

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
    typedef network_message::message message;
    typedef network_message::game_message game_message;

    static const unsigned char SERVER_ID = 255;
    // static const members
    static const unsigned char STATUS_IDLE = (1u << 0);
    static const unsigned char STATUS_BUSY = (1u << 1);
    static const unsigned char STATUS_WAITING = (1u << 2);
    static const unsigned char STATUS_IN_GAME = (1u << 3);

    // const private members
    const unsigned char id; // id of this client
    const std::string name; // name of client
    // mutable private members
    mutable tcp_stream* stream; // communication channel
    mutable int tout_count; // number of times client has timed out
    // private members
    unsigned char status; // status of client
    char l_cache[32];
    // private synchronization members
    mutable std::mutex stream_mux; // stream ::send and ::receive synchronization

    // returns the status of this client
    inline unsigned char get_status() {
        return status;
    }

    // const version of client::get_status
    inline const unsigned char get_status() const {
        return status;
    }

    // client_pool should be the only class that can create clients
    friend class client_pool;

    /*
     * Used by client_pool to create client instances
     */
    explicit client(const unsigned char id, const std::string name,
        tcp_stream* const stream)
        : id(id), name(name), stream(stream), tout_count(0), status(STATUS_IDLE)
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
    ~client() {
        // DEBUG
        std::cout << "Deleting client with id [" << id << "]." << std::endl;

        delete stream;
    }

    /**
     *
     * @return
     */
    std::unique_ptr<message> receive(const int tout_s = 0,
        const int tout_us = 0) const
    {
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
        }
        // end synchronization critical section

        // check for the different errors TCPStream#receive can return
        switch (len) {
            case stream->CONNECTION_CLOSED:
                return std::unique_ptr<message>(
                    message::create_message(network_message::type::ERROR,
                        network_message::signal::CONNECTION_CLOSED, get_id(),
                        SERVER_ID));

            case stream->CONNECTION_TIMEOUT:
                return std::unique_ptr<message>(
                    message::create_message(network_message::type::ERROR,
                        network_message::signal::CONNECTION_TIMED_OUT, get_id(),
                        SERVER_ID));

            default: // no errors, return received message
                switch (static_cast<network_message::type>(rec_buffer[0])) {
                    default:
                        return std::unique_ptr<message>(
                            message::parse_message(rec_buffer, len));
                        break;

                    case network_message::type::GAME:
                        return std::unique_ptr<game_message>(
                            game_message::parse_message(rec_buffer, len));
                        break;
                }
        }

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
        }
        // end synchronization critical section

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
        }
        // end synchronization critical section

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
        }
        // end synchronization critical section

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
        return get_status() & STATUS_IDLE;
    }

    /**
     * @return true if the status of this client is idle, false otherwise.
     */
    inline const bool is_idle() const {
        return get_status() & STATUS_IDLE;
    }

    /**
     * Sets the status of this client to busy.
     */
    inline void set_busy() {
        status = STATUS_BUSY;
    }

    /**
     * @return true if the status of this client is busy, false otherwise.
     */
    inline bool is_busy() {
        return get_status() & STATUS_BUSY;
    }

    /**
     * @return true if the status of this client is busy, false otherwise.
     */
    inline const bool is_busy() const {
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
