#ifndef BATTLESHIP_MESSAGE_PRODUCER_H_
#define BATTLESHIP_MESSAGE_PRODUCER_H_

#include <iostream>
#include <memory>

#include "thread.h"
#include "client.h"
#include "clientpool.h"
#include "message.h"
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
    ts_queue<std::shared_ptr<client>>& c_queue;
    ts_queue<message>& msg_queue;
    client_pool& c_pool;

public:

    /**
     *
     * @param c_queue
     * @param pool
     * @param msg_queue
     */
    client_message_producer(ts_queue<std::shared_ptr<client>>& c_queue,
        client_pool& pool, ts_queue<message>& msg_queue)
        : c_queue(c_queue), msg_queue(msg_queue), c_pool(pool)
    {
    }

    //
    void* run() override;
};
// class client_message_producer


} // namespace server
} // namespace battleship

#endif /* BATTLESHIP_MESSAGE_PRODUCER_H_ */
