#include "msgconsumer.h"

/* DEBUG  beg debugout.h */
#include "debugout.h"
using namespace debug_out;
/* DEBUG  end debugout.h */

namespace battleship {

using namespace network_message;

namespace game_server {

/*
 * Server message hellper method.
 */
void message_consumer::handle_server_msg(const message* msg) {
    // DEBUG
    // unsigned long int tid = self();

    // get the sender from the pool
    auto client = c_pool.get(msg->from());

    auto sp_client = client.lock();
    if (!sp_client) {
        client.reset();
        return;
    }

    // choose the appropriate action for the signal
    switch (msg->signal()) {

        case signal::CONNECTION_CLOSED:
            c_pool.remove(sp_client->get_id());
            break;

        case signal::CLIENT_EXIT:
            c_pool.remove(sp_client->get_id());
            break;

        default:
            sp_client->send(msg);
            break;
    }
}

/*
 * Game message helper method.
 */
void message_consumer::handle_game_msg(const message* msg) {
    // convert message msg to game_message
    const game_message* g_msg = dynamic_cast<const game_message*>(msg);

    // get the sender from the client pool
    auto client = c_pool.get(msg->from());
    auto sp_client = client.lock();
    if (!sp_client) {
        client.reset();
        return;
    }

    // client ready signal received, then try to create a game with an existing
    // ready client, otherwise add client to the game pool ready queue
    if (g_msg->signal() == signal::GAME_READY) {
        auto new_game = g_pool.add_ready_client(sp_client);
        auto sp_new_game = new_game.lock();
        if (!sp_new_game) {
            new_game.reset();
            return;
        }

        sp_new_game->broadcast_message(signal::GAME_CREATED);
        return;
    }

    // get existing game
    auto game = g_pool.get(g_msg->game_id());
    auto cur_game = game.lock();
    if (!cur_game) {
        game.reset();
        return;
    }

    // handle the message in the game
    cur_game->handle_message(g_msg);
}

/**
 * Where this {@code message_consumer} thread does all of the work.
 */
void* message_consumer::run() {
    // DEBUG
    unsigned long int tid = self();

    // DEBUG
    thread_println(std::cout, tid, " - consumer ready.");

    // remove 1 message at a time and process it, threads block if no messages
    // are available to process
    for (int i = 0;; i++) {


        // get next item from the queue or block
        auto msg = std::unique_ptr<const message>(
            m_queue.remove());

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

            default:
                // DEBUG
                thread_println(std::cout, tid, "SERIOUS MESSAGE ERROR (",
                    static_cast<int>(msg->type()), ") from client [ ",
                    static_cast<int>(msg->from()), " ] - MESSAGE: ",
                    std::string(msg->msg()));

                std::cerr << "SERIOUS MESSAGE ERROR ("
                    << static_cast<int>(msg->type()) << ") from client [ "
                    << static_cast<int>(msg->from()) << " ] - MESSAGE: "
                    << std::string(msg->msg()) << std::endl;

                break;
        }

    }

    // should never get here
    return nullptr;
}

} // namespace server

} // namespace battleship

