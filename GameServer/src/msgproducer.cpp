#include "msgproducer.h"

/* DEBUG  beg debugout.h */
#include "debugout.h"
using namespace debug_out;
/* DEBUG  end debugout.h */

namespace battleship {

using namespace network_message;

namespace game_server {

// produce the proper message when handling an error message in
// producer loop
bool client_message_producer::handle_receive_error(const message* msg,
    const shared_ptr<client> client_sp)
{
    // handle error message based on its signal
    switch (msg->signal()) {

        // handle losing connection with the client
        case signal::CONNECTION_CLOSED:
            // propagate game_message to notify game_pool and client_pool
            if (client_sp->is_in_game()) {
                m_queue.add(
                    game_message::create_message(signal::CONNECTION_CLOSED,
                        msg->from(), SERVER_ID, client_sp->get_game_id()));
            } else {
                // copy message to consumer queue
                m_queue.add(
                    message::create_message(type::SERVER,
                        signal::CONNECTION_CLOSED, msg->from(), SERVER_ID));
            }

            break;

            // handle connection timing out, usually from non-blocking socket
        case signal::CONNECTION_TIMED_OUT:
            // DEBUG
            println(std::cout, "Thread producer: [ ", client_sp->get_name(),
                " ]'s connection timed out.");

            // only the client can signal a timeout error if they're in game,
            // this is a hard exit
            if (client_sp->is_in_game()) {
                m_queue.add(
                    game_message::create_message(signal::CONNECTION_TIMED_OUT,
                        msg->from(), SERVER_ID, client_sp->get_game_id()));
                break;
            }

            // if client surpasses timeout limit, then add actual timeout
            // error message to message consumer queue
            if (client_sp->timed_out(tout_count_limit)) {

                // add timeout error message to consumer queue
                m_queue.add(
                    message::create_message(type::SERVER,
                        signal::CONNECTION_TIMED_OUT, client_sp->get_id(),
                        SERVER_ID));

                // continue producer loop
                break;
            }

            // unless client surpasses timeout count limit, add them back to the
            // client queue
            return true;

        default:
            // handle other error messages here
            break;
    }

    // don't add client back to message producing queue
    return false;
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

        // block or get client from weak pointer queue, upgrade to shared_ptr
        const std::shared_ptr<client> client_sp = c_queue.remove().lock();

        // if the reference to this client has been deleted, do nothing
        if (!client_sp) {
            continue;
        }

        // DEBUG
        thread_loop_println(std::cout, tid, i, " - Processing client [ ",
            client_sp->get_name(), " ].");

        // get & wrap message from client, with 1 sec. timeout, in smart pointer
        auto msg = std::unique_ptr<message>(client_sp->receive(1));

        // flag for adding client back to client queue
        bool re_add = true;

        // perform action based off message type
        switch (msg->type()) {
            case type::ERROR:
                // handle the received error, returns wether to add client back
                // to queue or not
                re_add = handle_receive_error(msg.get(), client_sp);
                break;

                // handle non-error messages
            case type::GAME:
            case type::SERVER:
                // reset client's timeout count sent, since we received message
                client_sp->reset_timeouts();

                // add msg to consumer thread queue
                m_queue.add(msg.release());
                break;
        }

        if (re_add) {
            // add client back to client queue
            c_queue.add(client_sp);
        }

    }

    // Should never get here
    return nullptr;
}

} // namespace server
} // namespace battleship
