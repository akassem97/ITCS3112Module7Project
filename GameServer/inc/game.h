/*
 * game.h
 *
 *  Created on: Dec 9, 2018
 *      Author: henly
 */

#ifndef BATTLESHIP_GAME_H_
#define BATTLESHIP_GAME_H_

// standard library headers
#include <memory>
#include "message.h"

// battleship headers
#include "client.h"

namespace battleship {

namespace game_server {

using namespace network_message;

/**
 * @brief Represents a game of battleship between two clients.
 *
 * @author Adham Kassem
 * @author Jonathan Henly
 * @version 12-4-2018
 */
class game {
private:
    // private static members
    const static unsigned char SERVER_ID = 255;
    const static ssize_t CLIENT_REFERENCE_EMPTY = -1;
    const static size_t NUM_PLAYERS = 2;
    const static size_t BOARD_SIZE = 101;
    const static unsigned char BOARD_INDEX_OUT_OF_BOUNDS = 255;
    const static unsigned char CLIENT_TO_PLAYER_MAPPING_NOT_FOUND = 254;
    const static unsigned char WATER = 1;
    const static unsigned char SHIP = 2;

    // private members
    const unsigned char g_id;
    std::weak_ptr<client> p_one; // player one
    std::weak_ptr<client> p_two; // player two
    unsigned char map_c_to_p[NUM_PLAYERS]; // maps player id to client id
    char player_boards[NUM_PLAYERS][BOARD_SIZE]; // plyer board squares
    unsigned char curr_p; // current player

    mutable std::mutex game_mutex;

    /*// might not need this method
     static void to_rc_coords(unsigned char coords, unsigned char& row,
     unsigned char& col)
     {

     }
     */

    /*
     * Gets the value of a square at a specified index, in a specified player's
     * board.
     *
     * Note: this method returns BOARD_INDEX_OUT_OF_BOUNDS (255) if the
     * specified index is out of bounds.
     *
     * @param which - which player (0 or 1) to get the square value of.
     * @param index - the index of the square whose value to return.
     * @return the value of the specified square.
     */
    unsigned char get_square(unsigned char which, unsigned char index) const {
        if (index < 1 || index >= BOARD_SIZE) {
            return BOARD_INDEX_OUT_OF_BOUNDS;
        }

        return player_boards[which][index];
    }

    /*
     * Sets the value of a square at a specified index, in a specified player's
     * board, to a specified value.
     *
     * Note: this method returns BOARD_INDEX_OUT_OF_BOUNDS (255) if the
     * specified index is out of bounds.
     *
     * @param which - which player (0 or 1) to get the square value of.
     * @param index - the index of the square whose value to return.
     * @param value - the new value of the square.
     * @return 0 on success, BOARD_INDEX_OUT_OF_BOUNDS otherwise.
     */
    unsigned char set_square(unsigned char which, unsigned char index,
        unsigned char value)
    {
        if (index < 1 || index >= BOARD_SIZE) {
            return BOARD_INDEX_OUT_OF_BOUNDS;
        }

        player_boards[which][index] = value;

        return 0;
    }

    // initializes a players board
    void fill_player_board(const unsigned char which, const char* board) {
        for (size_t i = 0; i < BOARD_SIZE; ++i) {
            set_square(which, i, board[i]);
        }
    }

    // returns the player id to weak_ptr<client> mapping
    std::weak_ptr<client> p_to_c(const char p_id) {
        return p_id == 0 ?
            p_one : p_two;
    }

    // returns client id to player id mapping
    char c_to_p(const unsigned char c_id) {
        if (map_c_to_p[0] == c_id) {
            return 0;
        } else if (map_c_to_p[1] == c_id) {
            return 1;
        }

        return CLIENT_TO_PLAYER_MAPPING_NOT_FOUND;
    }

    // sends a game message to the specified client
    ssize_t send_game_message(std::weak_ptr<client> client,
        network_message::signal sig, const char* payload = nullptr,
        const unsigned char pload_len = 0)
    {
        // upgrade client weak pointer to shared pointer
        auto to_client = client.lock();
        // clean up weak pointer and return error if shared pointer is not valid
        if (!to_client) {
            client.reset();
            return CLIENT_REFERENCE_EMPTY;
        }

        // create the game message to send
        auto g_msg = std::unique_ptr<game_message>(
            game_message::create_message(sig, SERVER_ID, to_client->get_id(),
                get_game_id(), payload, pload_len));

        ssize_t result = to_client->send(g_msg.get());

        return result;
    }

    // sends a game message to the specified client
    ssize_t send_game_message(std::weak_ptr<client> client,
        const game_message msg)
    {
        // upgrade client weak pointer to shared pointer
        auto to_client = client.lock();
        // clean up weak pointer and return error if shared pointer is not valid
        if (!to_client) {
            client.reset();
            return CLIENT_REFERENCE_EMPTY;
        }

        ssize_t result = to_client->send(msg);

        return result;
    }

    // no default constructor
    game() = delete;

    /* Disallow Copy Operations */
    game(const game&) = delete; // don't implement copy c'tor
    game& operator=(const game& rhs) = delete; // don't implement copy
                                               // assignment

public:
    /**
     * Constructs a battleship game between two passed in weak pointers to
     * clients.
     *
     * @param one - a weak pointer to the first client.
     * @param two - a weak pointer to the second client.
     */
    game(const unsigned char game_id, const std::weak_ptr<client> one,
        const std::weak_ptr<client> two)
        : g_id(game_id), p_one(one), p_two(two), curr_p(0)
    {
        map_c_to_p[0] = one.lock()->get_id();
        map_c_to_p[1] = two.lock()->get_id();
    }

    /**
     * Broadcasts a game message to both clients.
     */
    void broadcast_message(game_message msg) {
        // upgrade client weak pointer to shared pointer
        auto c_one = p_one.lock();
        auto c_two = p_two.lock();

        if (!c_one) {
            p_one.reset();
        }

        if (!c_two) {
            p_two.reset();
        }

        c_one->send(msg);
        c_two->send(msg);
    }

    /**
     * Broadcasts a game message to both clients.
     */
    void broadcast_message(network_message::signal sig, const char* payload =
        nullptr, const unsigned char pload_len = 0)
    {
        // upgrade client weak pointer to shared pointer
        auto c_one = p_one.lock();
        auto c_two = p_two.lock();

        if (!c_one) {
            p_one.reset();
        }

        if (!c_two) {
            p_two.reset();
        }

        auto g_one_msg = std::unique_ptr<game_message>(
            game_message::create_message(sig, SERVER_ID, c_one->get_id(),
                get_game_id(), payload, pload_len));

        auto g_two_msg = std::unique_ptr<game_message>(
            game_message::create_message(sig, SERVER_ID, c_two->get_id(),
                get_game_id(), payload, pload_len));

        c_one->send(g_one_msg.get());
        c_two->send(g_two_msg.get());
    }

    /**
     * Resets the both client weak pointers.
     */
    virtual ~game() {
        p_one.reset();
        p_two.reset();
    }

    /**
     * @return the unique ID of this game instance.
     */
    const unsigned char get_game_id() {
        return g_id;
    }

    /**
     * Handles network message producing and receiving between this game
     * instance's two clients.
     *
     * @param msg - the network message to handle.
     */
    void handle_message(const network_message::game_message* msg) {
        // threads block until they acquire lock
        std::lock_guard<std::mutex> lock(game_mutex);
        ssize_t result = 1;

        if (msg->type() == type::ERROR) {
            handle_error_message(msg);
            return;
        }

        switch (msg->signal()) {

            // handle game creation and player turn order
            case signal::GAME_CREATED: {
                if (!msg->has_payload()) {
                    break;
                }
                fill_player_board(c_to_p(msg->from()), msg->payload());

                if (c_to_p(msg->from()) == 0) {
                    send_game_message(p_to_c(0), signal::GAME_PLAYER_ONE);
                } else {
                    send_game_message(p_to_c(1), signal::GAME_PLAYER_TWO);
                }
            }
                break;

            case signal::GAME_COORDINATES: {
                if (!msg->has_payload()) {
                    break;
                }
                unsigned char index = *msg->payload();
                signal sig;
                const unsigned char square = get_square(c_to_p(msg->from()),
                    index);

                if (square == WATER) {
                    sig = signal::GAME_MISS;
                } else {
                    sig = signal::GAME_HIT;
                }
                const char opp = 1 - c_to_p(msg->from());
                send_game_message(p_to_c(msg->from()), sig);

                send_game_message(p_to_c(opp), sig);
                send_game_message(p_to_c(opp), signal::GAME_COORDINATES,
                    msg->payload(), msg->payload_length());

            }
                break;
                /* fall through these signals */
            case signal::GAME_CLIENT_QUIT:
                // handle client quit
                break;
            case signal::GAME_CLIENT_DISC:
                // handle client disconnect
                break;
                // should not handle this in game
            case signal::GAME_READY:
                // should not receive this from client
            case signal::GAME_HIT:
                // should not receive this from client
            case signal::GAME_MISS:
                // should not receive this from client
            case signal::GAME_WINNER:
                // should not receive this from client
            case signal::GAME_LOSER:
                // should not receive this from client
                break;

            default:
                break;
        }

        if (result < 1) {

        }

    }

    /**
     * Handles network message producing and receiving between this game
     * instance's two clients.
     *
     * @param msg - the network message to handle.
     */
    void handle_error_message(const network_message::game_message* msg) {
        ssize_t result = 1;

        switch (msg->signal()) {
            default:
                break;
        }

        if (result < 1) {

        }

    }

};
// class game

}// namespace battleship

} // namespace server

#endif /* BATTLESHIP_GAME_H_ */
