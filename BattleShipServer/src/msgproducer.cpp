#include "msgproducer.h"

/* DEBUG  beg debugout.h */
#include "debugout.h"
using namespace debug_out;
/* DEBUG  end debugout.h */


namespace battleship {

namespace game_server {

/**
 *
 */
void* client_message_producer::run() {
    // remove 1 item at a time and process it. Blocks if no items are
    // available to process
    for (int i = 0;; i++) {
        unsigned long int tid = self();

        // DEBUG
        thread_loop_println(std::cout, tid, i, " - idle.");

        // get next item from the queue or block
        const std::shared_ptr<client> client = c_queue.remove();

        // DEBUG
        thread_loop_println(std::cout, tid, i, " - Processing client [ ",
            client->get_name(), " ].");

        // get message from client with a 1 second timeout
        message msg = client->receive(0, 100);

        bool no_re_add = false;
        switch (msg.sig()) {
            case signal::CONNECTION_TIMED_OUT:

                if (client->timed_out(30000)) {
                    no_re_add = true;
                    // DEBUG
                    thread_println(std::cout, tid, "[ ", client->get_name(),
                        " ]'s connection timed out.");

                    // DEBUG
                    println(std::cout, "\tNumClients: ", (int) c_pool.size());

                    c_pool.remove(client->get_id());

                    // DEBUG
                    println(std::cout, "\tNumClients: ", (int) c_pool.size());
                    continue;
                }

                c_queue.add(client);
                continue;

            case signal::CONNECTION_CLOSED:
                no_re_add = true;

                // DEBUG
                thread_println(std::cout, tid, "[ ", client->get_name(),
                    " ]'s connection was closed.");

                // DEBUG
                println(std::cout, "\tNumClients: ", (int) c_pool.size());

                c_pool.remove(client->get_id());

                // DEBUG
                println(std::cout, "\tNumClients: ", (int) c_pool.size());

                continue;

            case signal::CLIENT_EXIT:
                no_re_add = true;

                // DEBUG
                thread_println(std::cout, tid, "[ ", client->get_name(),
                    " ] has exited.");

                // DEBUG
                println(std::cout, "\tNumClients: ", (int) c_pool.size());

                c_pool.remove(client->get_id());

                // DEBUG
                println(std::cout, "\tNumClients: ", (int) c_pool.size());

                continue;

            case signal::CLIENT_LOBBY_LIST:
                no_re_add = true;

                // DEBUG
                thread_println(std::cout, tid, "[ ", client->get_name(),
                    " ] has requested the client lobby list.");

                cout << "CLIENT_LISTINGS(): " << c_pool.get_client_listings()
                    << endl;

                msg = message::build_message(form::SERVER,
                    signal::CLIENT_LOBBY_LIST, 255, client->get_id(),
                    c_pool.get_client_listings());

                // DEBUG
                thread_println(std::cout, tid, "Sending message [",
                    static_cast<int>(msg.sig()), static_cast<int>(msg.from()),
                    static_cast<int>(msg.to()), msg.msg() + 3, "] to client [ ",
                    client->get_name(), " ].");

                client->send(msg);

                // add client back to queue
                c_queue.add(client);
                continue;

            default:

                // DEBUG
                thread_println(std::cout, tid, "Received signal (",
                    static_cast<int>(msg.sig()), ") from client [ ",
                    client->get_name(), " ].");

                break;
        }

        if (!no_re_add) {
            client->reset_timeouts();

            message msg_to_send = message::build_message(form::SERVER,
                signal::CLIENT_BUSY, 255, client->get_id(),
                std::string("Received message."));

            // DEBUG
            thread_println(std::cout, tid, "Sending message [",
                static_cast<int>(msg_to_send.sig()),
                static_cast<int>(msg_to_send.from()),
                static_cast<int>(msg_to_send.to()), msg_to_send.msg() + 3,
                "] to client [ ", client->get_name(), " ].");

            client->send(msg_to_send);

            // add client back to queue
            c_queue.add(client);
        }

    }

    // Should never get here
    return nullptr;
}


} // namespace server
} // namespace battleship
