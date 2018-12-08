#ifndef BATTLESHIP_CLIENT_H_
#define BATTLESHIP_CLIENT_H_

#include <message.h> // enum class signal
#include <iostream> // DEBUG
#include <string>

#include "tcpstream.h"

namespace battleship {

namespace server {

/**
 * @brief Represents a client connected to the server.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 11-27-2018
 */
class client {
private:
    // constant private members
    const unsigned char id; // id of this client
    const std::string name; // name of client

    // mutable private members
    mutable tcp_stream* stream; // communication channel
    mutable int tout_count; // number of times client has timed out

    // private members
    unsigned char status; // status of client
    char* l_cache;

    /* Disallow Copy Operations */
    client(const client&) = delete; // don't implement copy constructor
    client& operator=(const client&) = delete; // don't implement copy
                                               // assignment oper.
    client() = delete; // don't implement default constructor

    /*
     *
     */
    inline unsigned char get_status() {
        return status;
    }

    /*
     *
     */
    inline const unsigned char get_status() const {
        return status;
    }

public:

    static const unsigned char STATUS_IDLE = (1u << 0);
    static const unsigned char STATUS_BUSY = (1u << 1);
    static const unsigned char STATUS_WAITING = (1u << 2);
    static const unsigned char STATUS_IN_GAME = (1u << 3);

    explicit client(const unsigned char id, const std::string name,
        tcp_stream* stream)
        : id(id), name(name), stream(stream), tout_count(0),
            status(STATUS_IDLE), l_cache(nullptr)
    {
    }

    /**
     * Default destructor.
     */
    ~client() {
        // DEBUG
        // std::cout << "Deleting client with id [" << id << "]." << std::endl;

        delete stream;
    }

    /**
     *
     * @return
     */
    inline tcp_stream* get_stream() const {
        return stream;
    }

    /**
     *
     * @return
     */
    message receive(int timeout = 0) const {
        char rec_buffer[256];

        // fill buffer with bytes from client
        ssize_t len = stream->receive(rec_buffer, sizeof(rec_buffer) - 1,
            timeout);

        // check for the different errors TCPStream#receive can return
        switch (len) {
            case stream->CONNECTION_CLOSED:
                return message::build_error_message(signal::CONNECTION_CLOSED);

            case stream->CONNECTION_TIMEOUT:
                return message::build_error_message(
                    signal::CONNECTION_TIMED_OUT);

            default: // no errors, return received message
                return message::build_message(rec_buffer, len);
        }

    }

    /**
     *
     * @param message
     * @return
     */
    ssize_t send(const message message) {
        return stream->send(message.msg(), message.length());
    }

    /**
     *
     * @return this clients ID.
     */
    inline char get_id() {
        return id;
    }

    /**
     *
     * @return
     */
    inline const char get_id() const {
        return id;
    }

    /**
     *
     * @return
     */
    inline std::string get_name() {
        return name;
    }

    /**
     *
     * @return
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
     *
     * @return true if the status of this client is in-game, false otherwise.
     */
    inline bool is_in_game() {
        return get_status() & STATUS_IN_GAME;
    }

    /**
     *
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
     * @param max_touts
     * @return
     */
    inline bool timed_out(const int max_touts) {
        tout_count += 1;

        return tout_count >= max_touts;
    }

    /**
     *
     */
    inline void reset_timeouts() {
        tout_count = 0;
    }

    /**
     *
     * @return
     */
    std::string get_listing() {
        if (!l_cache) {
            const size_t listing_length = 2 + get_name().length();
            char listing[listing_length + 1];

            listing[listing_length] = '\0';

            // put id byte in listing first
            listing[0] = id;

            // then put status byte in listing
            listing[1] = status;

            // finally copy name bytes to listing
            const char* tmp_name = get_name().c_str();
            for (size_t i = 2; i < listing_length; ++i) {
                listing[i] = tmp_name[i];
            }

            l_cache = listing;
        } else {
            l_cache[1] = status;
        }

        return std::string(l_cache);
    }

};

} // namespace server

} // namespace battleship

#endif /* BATTLESHIP_CLIENT_H_ */
