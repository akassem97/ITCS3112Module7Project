#ifndef BATTLESHIP_MESSAGE_CONSUMER_H_
#define BATTLESHIP_MESSAGE_CONSUMER_H_

// standard library headers
#include <iostream>
#include <memory>

// battleship headers
#include "client.h"
#include "clientpool.h"
#include "game.h"
#include "gamepool.h"
#include "message.h"
#include "thread.h"
#include "tsqueue.h"

namespace battleship {

namespace game_server {

/**
 * @brief A message consuming thread.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 12-01-2018
 */
class message_consumer: public Thread {
private:
    typedef network_message::message message; // namespace network_message from
                                              // "message.h"

    ts_queue<message*>& m_queue;
    client_pool& c_pool;
    game_pool& g_pool;

    /* Message Consumer message-type handler helper methods. */
    void handle_server_msg(const message* msg);
    void handle_game_msg(const message* msg);
    void handle_error_msg(const message* msg);

public:
    /**
     *
     * @param queue - the thread safe message queue.
     * @param pool - the thread safe pool of clients.
     * @param games - the thread safe pool of games.
     */
    message_consumer(ts_queue<message*>& m_queue,
        client_pool& c_pool,
        game_pool& g_pool)
        : m_queue(m_queue), c_pool(c_pool), g_pool(g_pool)
    {
    }

    // where this {@code message_consumer} thread does all of the work
    void* run() override;

};
// class message_consumer

}// namespace server
} // namespace battleship

#endif /* BATTLESHIP_MESSAGE_CONSUMER_H_ */
