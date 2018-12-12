#include "msgproducer.h"

/* DEBUG  beg debugout.h */
#include "debugout.h"
using namespace debug_out;
/* DEBUG  end debugout.h */

namespace battleship {

using namespace network_message;

namespace game_server {

//
void client_message_producer::handle_error_msg(const message* msg,
    const weak_ptr<client> msg_client)
{
    // upgrade weak_ptr to shared_ptr to get reference to client
    auto client = msg_client.lock();

    // handle error message based on its signal
    switch (msg->signal()) {

        // handle losing connection with the client
        case signal::CONNECTION_CLOSED:
            if (client->is_in_game()) {
                m_queue.add(
                    game_message::create_message(signal::CONNECTION_TIMED_OUT,
                        msg->from(), SERVER_ID, client->get_game_id()));
            }
            break;

            // handle connection timing out, usually from non-blocking socket
        case signal::CONNECTION_TIMED_OUT:
            // DEBUG
            thread_println(std::cout, tid, "[ ", client->get_name(),
                " ]'s connection timed out.");

            // only the client can signal a timeout error if they're in game
            if (client->is_in_game()) {
                m_queue.add(
                    game_message::create_message(signal::CONNECTION_TIMED_OUT,
                        msg->from(), SERVER_ID, client->get_game_id()));
                break;
            }

            // if client surpasses timeout limit, then add actual timeout
            // error message to message consumer queue
            if (client->timed_out(tout_count_limit)) {

                // add timeout error message to consumer queue
                m_queue.add(
                    message::create_message(type::ERROR,
                        signal::CONNECTION_TIMED_OUT, client->get_id(),
                        SERVER_ID));

                // continue producer loop
                break;
            }

            // unless client surpasses timeout count limit, add them back to the
            // client queue
            c_queue.add(client);
            break;

        default:
            // handle other error messages here
            break;
    }
}

/**
 *
 */
void* client_message_producer::run() {
    unsigned long int tid = self();

    // remove 1 item at a time and process it. Blocks if no items are
    // available to process
    for (int i = 0;; i++) {

        // DEBUG
        thread_loop_println(std::cout, tid, i, " - idle.");

        // get next item from the queue or block
        const std::shared_ptr<client> client = c_queue.remove();

        // DEBUG
        thread_loop_println(std::cout, tid, i, " - Processing client [ ",
            client->get_name(), " ].");

        // get & wrap message from client, with 1 sec. timeout, in smart pointer
        auto msg = std::unique_ptr<message>(client->receive(1));

        // add client back to client queue flag
        bool no_re_add = false;

        // perform action based off message type
        switch (msg->type()) {
            case type::ERROR:
                handle_error_msg(msg.get());
                continue;

                // handle non-error messages
            default:

                // perform action based off message signal
                switch (msg->signal()) {

                    // non-error timeout, message from non-blocking receive
                    case signal::CONNECTION_TIMED_OUT:

                        c_queue.add(client);
                        continue;

                    case signal::CLIENT_EXIT:
                        no_re_add = true;

                        // DEBUG
                        thread_println(std::cout, tid, "[ ", client->get_name(),
                            " ] has exited.");

                        // DEBUG
                        println(std::cout, "\tNumClients: ",
                            (int) c_pool.size());

                        c_pool.remove(client->get_id());

                        // DEBUG
                        println(std::cout, "\tNumClients: ",
                            (int) c_pool.size());

                        continue;

                    default:

                        // DEBUG
                        thread_println(std::cout, tid, "Received signal (",
                            static_cast<int>(msg->signal()), ") from client [ ",
                            client->get_name(), " ].");

                        break;
                }
        }

        if (!no_re_add) {
            client->reset_timeouts();

            // add client back to client queue
            c_queue.add(client);
        }

    }

    // Should never get here
    return nullptr;
}

} // namespace server
} // namespace battleship
