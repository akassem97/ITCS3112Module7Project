/*
 * Server.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: henly
 */

#include <message.h> // enum class Signal
#include "Server.h"

#include <iostream> // std::cerr
#include <memory> // std::shared_ptr

#include "thread.h"

/* DEBUG  includes */
#include <mutex> // DEBUG  std::mutex, std::lock_guard
/* DEBUG  end includes */

using namespace battleship;

using namespace server;

/* DEBUG  Thread Safe Variadic Output Functions */
const unsigned long SERVER_TID = 0UL;
std::mutex ostream_mutex;

template<typename Arg, typename ... Args>
void thread_loop_println(std::ostream& out, unsigned long int tid, int i,
    Arg&& arg, Args&&... args)
{
    std::lock_guard<std::mutex> lock(ostream_mutex); // unlocks once scope ends

    out << "Thread [" << tid << "]: Loop " << i;
    out << std::forward<Arg>(arg);
    using expander = int[];
    (void) expander { 0, (void(out << std::forward<Args>(args)), 0)... };
    out << std::endl;

    // make sure out's entire buffer is written
    out.flush();
}

template<typename Arg, typename ... Args>
void thread_println(std::ostream& out, unsigned long int tid, Arg&& arg,
    Args&&... args)
{
    std::lock_guard<std::mutex> lock(ostream_mutex); // unlocks once scope ends

    out << "Thread [" << tid << "]: ";
    out << std::forward<Arg>(arg);
    using expander = int[];
    (void) expander { 0, (void(out << std::forward<Args>(args)), 0)... };
    out << std::endl;

    // make sure out's entire buffer is written
    out.flush();
}

template<typename Arg, typename ... Args>
void println(std::ostream& out, Arg&& arg, Args&&... args) {
    std::lock_guard<std::mutex> lock(ostream_mutex); // unlocks once scope ends

    out << std::forward<Arg>(arg);
    using expander = int[];
    (void) expander { 0, (void(out << std::forward<Args>(args)), 0)... };
    out << std::endl;

    // make sure out's entire buffer is written
    out.flush();
}
/* DEBUG  End Thread Safe Variadic Output Functions */

/**
 * @brief Handles the connections between clients and the server.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 12-4-2018
 */
class connection_handler: public Thread {
    ts_queue<std::shared_ptr<client>>& c_queue;
    client_pool& c_pool;

public:

    /**
     *
     * @param queue
     * @param pool
     */
    connection_handler(ts_queue<std::shared_ptr<client>>& queue,
        client_pool& pool)
        : c_queue(queue), c_pool(pool)
    {
    }

    void* run() {
        // Remove 1 item at a time and process it. Blocks if no items are
        // available to process.
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
            message msg = client->receive(1);

            bool no_re_add = false;
            switch (msg.signal()) {
                case signal::CONNECTION_TIMED_OUT:

                    if (client->timed_out(30)) {
                        no_re_add = true;
                        // DEBUG
                        thread_println(std::cout, tid, "[ ", client->get_name(),
                            " ]'s connection timed out.");

                        // DEBUG
                        println(std::cout, "\tNumClients: ",
                            (int) c_pool.size());

                        c_pool.remove(client->get_id());

                        // DEBUG
                        println(std::cout, "\tNumClients: ",
                            (int) c_pool.size());
                        continue;
                    }

                    break;

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
                    println(std::cout, "\tNumClients: ", c_pool.size());

                    c_pool.remove(client->get_id());

                    // DEBUG
                    println(std::cout, "\tNumClients: ", c_pool.size());

                    continue;

                case signal::CLIENT_LOBBY_LIST:
                    no_re_add = true;

                    // DEBUG
                    thread_println(std::cout, tid, "[ ", client->get_name(),
                        " ] has requested the client lobby list.");

                    cout << "CLIENT_LISTINGS(): "
                        << c_pool.get_client_listings() << endl;

                    msg = message::build_message(signal::CLIENT_LOBBY_LIST, 255,
                        client->get_id(), c_pool.get_client_listings());

                    // DEBUG
                    thread_println(std::cout, tid, "Sending message [",
                        static_cast<int>(msg.signal()),
                        static_cast<int>(msg.from()),
                        static_cast<int>(msg.to()), msg.msg() + 3,
                        "] to client [ ", client->get_name(), " ].");

                    client->send(msg);

                    // add client back to queue
                    c_queue.add(client);
                    continue;

                default:

                    // DEBUG
                    thread_println(std::cout, tid, "Received signal (",
                        static_cast<int>(msg.signal()), ") from client [ ",
                        client->get_name(), " ].");

                    break;
            }

            if (!no_re_add) {
                message msg_to_send = message::build_message(
                    signal::CLIENT_BUSY, 255, client->get_id(),
                    std::string("Received message."));

                // DEBUG
                thread_println(std::cout, tid, "Sending message [",
                    static_cast<int>(msg_to_send.signal()),
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
};

/**
 *
 * @param maxConnections
 * @param port
 * @param ip
 */
Server::Server(const unsigned char maxConnections, const int port,
    const std::string ip)
    : max_connections(maxConnections), port(port), ip(ip),
        connection_acceptor(nullptr), running(false)
{
    // create the client pool
    c_pool = new client_pool(maxConnections);
}

/**
 *
 * @return
 */
int Server::start() {
    if (is_running()) {
        return SERVER_ALREADY_RUNNING;
    }

    // ToDo  test if this equation, for thread count, produces optimal results
    const int consumer_count = max_connections / 2;

    // create the consumer (worker) threads
    for (int i = 0; i < consumer_count; i++) {
        connection_handler* handler = new connection_handler(c_queue, *c_pool);
        if (!handler) {
            std::cerr << "Could not create ConnectionHandler thread " << i
                << std::endl;
            return COULD_NOT_CREATE_CONSUMER_THREADS;
        }
        handler->start();
    }

    // create a TCP connection acceptor to listen for client connections
    if (get_ip().length() > 0) {
        connection_acceptor = new tcp_acceptor(get_port(),
            (char*) get_ip().c_str());
    } else {
        connection_acceptor = new tcp_acceptor(get_port());
    }

    // check if the connection acceptor is functional
    if (!connection_acceptor || connection_acceptor->start() != 0) {
        std::cerr << "Could not create a connection acceptor.\n" << std::endl;
        return COULD_NOT_CREATE_CON_ACCEPTOR;
    }

    // enter the main server loop
    this->run();

    // signal that the server is no longer running
    running = false;

    return 0;
}

/*
 *
 */
void Server::run() {
    // signal that the server is running
    running = true;

    // add a client to the pool for each connection, unless pool is full
    while (1) {
        // block until connection attempt
        tcp_stream* connection = connection_acceptor->accept();

        // DEBUG
        thread_println(std::cout, SERVER_TID,
            "Receiving new connection attempt.");

        // check if connection is functioning
        if (!connection) {
            // DEBUG
            thread_println(std::cout, SERVER_TID,
                "Error accepting connection.");
            std::cerr << "Could not accept a connection.\n" << std::endl;
            continue;
        }

        // DEBUG
        thread_println(std::cout, SERVER_TID, "Connection successful.");

        // if server isn't full then being handshaking, otherwise notify client
        if (get_connection_count() < get_max_connections()) {
            // DEBUG
            thread_println(std::cout, SERVER_TID, "Beginning handshaking.");

            // create temporary client for the handshake procedure
            client* tmp_handshake_client = new client(255, "", connection);
            // receive client's join request message including client's name
            message client_request_join_msg = tmp_handshake_client->receive(5);

            // check if requesting client sent CLIENT_JOIN signal
            if (client_request_join_msg.signal() != signal::CLIENT_JOIN) {
                // DEBUG
                thread_println(std::cout, SERVER_TID,
                    "Client did not send CLIENT_JOIN signal.",
                    " Denying client's request.");

                // check for connection error, send message if no error
                switch (client_request_join_msg.signal()) {
                    case signal::CONNECTION_TIMED_OUT:
                    case signal::CONNECTION_CLOSED:
                        // do nothing if connection error
                        break;
                    default:
                        // send connection request denial message to client
                        tmp_handshake_client->send(
                            message::build_message(signal::CLIENT_DENY, 255,
                                255,
                                "Connection request denied: Did not receive proper CLIENT_JOIN signal."));

                } // end switch

                // clean up memory
                delete connection;

                // go back to waiting for connections
                continue;
            } // client join request conditional

            // store the clients name
            std::string client_name = std::string(
                client_request_join_msg.msg() + 3);

            // DEBUG
            thread_println(std::cout, SERVER_TID,
                "Received CLIENT_JOIN with client name [ ", client_name, " ].");

            /* ADD CLIENT TO POOL */
            std::shared_ptr<client> client = c_pool->create_and_add_client(
                client_name, std::move(tmp_handshake_client->get_stream()));

            // DEBUG
            thread_println(std::cout, SERVER_TID, "Sending client id [",
                client->get_id(), "].");

            // send the client their id
            message id_msg = message::build_simple_message(signal::CLIENT_JOIN,
                255, client->get_id());
            client->send(id_msg);

            // check that client received correct id
            message confirm_id = client->receive();

            if (confirm_id.from() != client->get_id()) {
                thread_println(std::cout, SERVER_TID,
                    "Client failed to confirm ID.",
                    " Denying client's join request.");

                // check for connection error, send message if no error
                switch (confirm_id.signal()) {
                    case signal::CONNECTION_TIMED_OUT:
                    case signal::CONNECTION_CLOSED:
                        // do nothing if connection error
                        break;
                    default:
                        // send connection request denial message to client
                        client->send(
                            message::build_message(signal::CLIENT_DENY, 255,
                                255,
                                "Connection request denied: Failed to confirm ID."));

                } // end switch

                // remove client from the pool, which also deletes connection
                c_pool->remove(client->get_id());

                // go back to waiting for connections
                continue;
            } // confirm id conditional

            // DEBUG
            thread_println(std::cout, SERVER_TID, "Client [ ",
                client->get_name(), " ] confirmed id [ ",
                static_cast<int>(client->get_id()), " ].");

            // send final CLIENT_JOIN to signal that handshaking is complete
            message handshaking_complete_msg = message::build_simple_message(
                signal::CLIENT_JOIN, 255, client->get_id());
            client->send(handshaking_complete_msg);

            // add client to worker thread queue
            c_queue.add(client);

            // DEBUG
            thread_println(std::cout, SERVER_TID, "Handshaking complete.");

        } else {
            // DEBUG
            thread_println(std::cout, SERVER_TID,
                "Server is full, deleting connection.");

            // send server-full message to client
            message server_full = message::build_error_message(
                signal::SERVER_FULL);
            connection->send(server_full.msg(), server_full.length());

            // clean up connection
            delete connection;
        }
    } // end main server loop

}

void Server::shutdown() {

}

