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

    // private members
    const unsigned char g_id;
    std::weak_ptr<client> p_one; // player one
    std::weak_ptr<client> p_two; // player two
    unsigned char map_c_to_p[NUM_PLAYERS]; // maps player id to client id
    char player_boards[NUM_PLAYERS][BOARD_SIZE]; // plyer board squares
    unsigned char curr_p; // current player

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

        auto g_msg = std::unique_ptr<game_message>(
            game_message::create_message(sig, SERVER_ID, to_client->get_id(),
                get_game_id(), payload, pload_len));

        ssize_t result = to_client->send(g_msg.get());

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
     * Constructs a battleship game between two passed in clients.
     *
     * @param one - a shared pointer to the first client.
     * @param two - a shared pointer to the second client.
     */
    game(const unsigned char game_id, const std::shared_ptr<client> one,
        const std::shared_ptr<client> two)
        : g_id(game_id), p_one(one), p_two(two), curr_p(0)
    {
        map_c_to_p[0] = one->get_id();
        map_c_to_p[1] = two->get_id();
    }

    /**
     *
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
        ssize_t result = 1;

        switch (msg->signal()) {
            default:
                break;

                // handle game creation and player turn order
            case signal::GAME_CREATED: {
                const char pload[1] = { c_to_p(msg->from()) };
                result = send_game_message(p_to_c(pload[0]),
                    signal::GAME_CREATED);

            }
                break;
            case signal::GAME_READY: // handle client game board init.
                if (!msg->has_payload()) {

                    break;
                }
                fill_player_board(c_to_p(msg->from()), msg->payload());

                break;
            case signal::GAME_COORDINATES:
                // handle client move

                break;
                /* fall through these signals */
            case signal::GAME_HIT:
                // should not receive this from client
            case signal::GAME_MISS:
                // should not receive this from client
            case signal::GAME_WINNER:
                // should not receive this from client
            case signal::GAME_LOSER:
                // should not receive this from client
                break;
            case signal::GAME_CLIENT_QUIT:
                // handle client quit

                break;
            case signal::GAME_CLIENT_DISC:
                // handle client disconnect

                break;
        }

        if (result < 1) {

        }

    }

}
;
// class game

}// namespace battleship

} // namespace server

#endif /* BATTLESHIP_GAME_H_ */
