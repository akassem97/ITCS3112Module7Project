#include "msgconsumer.h"

/* DEBUG  beg debugout.h */
#include "debugout.h"
using namespace debug_out;
/* DEBUG  end debugout.h */

namespace battleship {

namespace game_server {

using namespace network_message;

/*
 *
 */
void message_consumer::handle_server_msg(const message* msg) {
    // DEBUG
    unsigned long int tid = self();

    // get the sender from the pool
    auto client = c_pool.get(msg->from());

    // choose the appropiate action for the signal
    switch (msg->signal()) {
        default:
            client->send(msg);
            break;
        case signal::CONNECTION_CLOSED:
            break;
        case signal::CLIENT_EXIT:
            break;
        case signal::CLIENT_LOBBY_LIST:
            // DEBUG
            thread_println(std::cout, tid, "[ ", client->get_name(),
                " ] has requested the client lobby list.");

            auto reply = message::create_message(type::SERVER,
                signal::CLIENT_LOBBY_LIST, 255, client->get_id(),
                c_pool.get_client_listings());

            // DEBUG
            thread_println(std::cout, tid, "Sending message [",
                static_cast<int>(msg->signal()), static_cast<int>(msg->from()),
                static_cast<int>(msg->to()), msg->payload(), "] to client [ ",
                client->get_name(), " ].");

            client->send(msg);
            break;
    }
}

/*
 *
 */
void message_consumer::handle_game_msg(const message* msg) {
    // DEBUG
    // unsigned long int tid = self();

    // convert message msg to game_message
    const game_message* g_msg = dynamic_cast<const game_message*>(msg);

    // get sending client from client pool
    auto from_client = c_pool.get(g_msg->from());
    // get the proper game context

    // handle the passed in message based on its signal

}

/*
 *
 */
void message_consumer::handle_error_msg(const message* msg) {

}

/**
 * Where this {@code message_consumer} thread does all of the work.
 */
void* message_consumer::run() {
    // DEBUG
    unsigned long int tid = self();

    // remove 1 message at a time and process it, threads block if no messages
    // are available to process
    for (int i = 0;; i++) {

        // DEBUG
        thread_loop_println(std::cout, tid, i, " - consumer idle.");

        // get next item from the queue or block
        std::unique_ptr<const message> msg = m_queue.remove();

        // DEBUG
        thread_loop_println(std::cout, tid, i, " - Consuming message [ ",
            std::string(msg->msg()), " ].");

        switch (msg->type()) {
            case type::SERVER:
                handle_server_msg(msg.get());
                continue;

            case type::GAME:
                handle_game_msg(msg.get());
                continue;

            case type::ERROR:
                handle_error_msg(msg.get());
                continue;

            default:
                // DEBUG
                thread_println(std::cout, tid, "SERIOUS MESSAGE ERROR (",
                    static_cast<int>(msg->type()), ") from client [ ",
                    static_cast<int>(msg->from()), " ] - MESSAGE: ",
                    std::string(msg->msg()));

                break;
        }

    }

    // should never get here
    return nullptr;
}

} // namespace server

} // namespace battleship

