#ifndef BATTLESHIP_MESSAGE_PRODUCER_H_
#define BATTLESHIP_MESSAGE_PRODUCER_H_

// standard library headers
#include <iostream> // DEBUG
#include <memory>

// battleship headers
#include "client.h"
#include "clientpool.h"
#include "message.h"
#include "thread.h"
#include "tsqueue.h"

namespace battleship {

namespace game_server {

/**
 * @brief Handles the connections between clients and the server.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 12-4-2018
 */
class client_message_producer: public Thread {
private:
    // private typedefs
    typedef network_message::message message;

    // private static const members
    static const unsigned char SERVER_ID = 255;

    // private member variables
    ts_queue<std::shared_ptr<client>>& c_queue;
    ts_queue<message*>& m_queue;
    client_pool& c_pool;
    const unsigned int tout_count_limit;

    /* Message Proucer message-type handler helper methods. */
    void handle_error_msg(const message* msg,
        const weak_ptr<client> msg_client);

public:

    /**
     *
     * @param c_queue
     * @param pool
     * @param msg_queue
     */
    client_message_producer(ts_queue<std::shared_ptr<client>>& c_queue,
        client_pool& c_pool, ts_queue<message*>& m_queue,
        const unsigned int num_timeout_limit = 30000)
        : c_queue(c_queue), m_queue(m_queue), c_pool(c_pool),
            tout_count_limit(num_timeout_limit)
    {
    }

    //
    void* run() override;
};
// class client_message_producer


} // namespace server

} // namespace battleship

#endif /* BATTLESHIP_MESSAGE_PRODUCER_H_ */
