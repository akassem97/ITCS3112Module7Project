// source header
#include "server.h"

// standard library headers
#include <iostream> // std::cerr
#include <memory> // std::shared_ptr

// battleship headers
#include "game.h"
#include "message.h"
#include "msgconsumer.h"
#include "msgproducer.h"
#include "thread.h"

/* DEBUG  beg debugout.h */
#include "debugout.h"
/* DEBUG  end debugout.h*/

namespace battleship {

using namespace network_message;

namespace game_server {

using namespace debug_out;

/**
 *
 * @param maxConnections
 * @param port
 * @param ip
 */
server::server(const unsigned char max_connections, const int port,
    const std::string ip)
    : max_connections(max_connections), port(port), ip(ip), running(false),
        connection_acceptor(nullptr)
{
    // create the client pool
    c_pool = new client_pool(max_connections);
    g_pool = new game_pool(max_connections / 2);
}

/**
 *
 * @return
 */
int server::start() {
    if (is_running()) {
        return SERVER_ALREADY_RUNNING;
    }

    // signal that the server is running
    running = true;

    // ToDo  test if this equation, for thread count, produces optimal results
    const int consumer_count = max_connections / 2;
    const int producer_count = max_connections / 2;

    // create the message consumer threads
    for (int i = 0; i < consumer_count; i++) {
        message_consumer* m_consumer = new message_consumer(msg_queue, *c_pool,
            *g_pool);
        if (!m_consumer) {
            std::cerr << "Could not create ConnectionHandler thread " << i
                << std::endl;
            return COULD_NOT_CREATE_CONSUMER_THREADS;
        }
        m_consumer->start();
    }

    // create the client message producer threads
    for (int i = 0; i < producer_count; i++) {
        client_message_producer* c_m_producer = new client_message_producer(
            c_queue, *c_pool, msg_queue);
        if (!c_m_producer) {
            std::cerr << "Could not create ConnectionHandler thread " << i
                << std::endl;
            return COULD_NOT_CREATE_CONSUMER_THREADS;
        }
        c_m_producer->start();
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

// main server loop
void server::run() {

    // add a client to the pool for each connection, unless pool is full
    while (1) {
        // block until connection attempt
        tcp_stream* connection = connection_acceptor->accept();

        // DEBUG
        thread_println(std::cout, SERVER_TID,
            "Receiving new connection attempt.");

        // check if tcp stream 'connection' is functional
        if (!connection) {
            // DEBUG
            thread_println(std::cout, SERVER_TID,
                "Error accepting connection.");
            std::cerr << "Could not accept a connection.\n" << std::endl;
            continue;
        }

        // DEBUG
        thread_println(std::cout, SERVER_TID, "Connection successful.");

        // if server is full then notify requesting client, clean up the
        // connection and go back to waiting for connection requests
        if (get_connection_count() >= get_max_connections()) {
            // DEBUG
            thread_println(std::cout, SERVER_TID,
                "Server is full, deleting connection.");

            // create server-full message
            std::unique_ptr<const message> server_full(
                message::create_message(type::SERVER, signal::SERVER_FULL,
                    SERVER_ID, 0));
            // send the server-full message to client
            connection->send(server_full->msg(), server_full->length());

            // clean up connection
            delete connection;

            continue;
        }

        // since server isn't full perform client-server handshake procedure
        if (!perform_handshake_procedure(connection)) {
            continue; // on failure go back to listening for connections
        }

        // DEBUG
        thread_println(std::cout, SERVER_TID,
            "Client handshake completed successfully.");
    } // end main server loop

}

/*
 * Private run helper member.
 */
bool server::perform_handshake_procedure(tcp_stream* const connection) {
    /* Client Server Handshake Section */

    // DEBUG
    thread_println(std::cout, SERVER_TID, "Beginning handshake.");

    // create temporary client for the handshake procedure
    auto tmp_handshake_client = c_pool->create_tmp_client(connection);
    // receive client's join request message, including client's name
    std::unique_ptr<message> client_join_request_msg(tmp_handshake_client
        ->receive(1));

    // check if requesting client sent CLIENT_JOIN signal w/ client name payload
    if (client_join_request_msg->signal() != signal::CLIENT_JOIN
        || !client_join_request_msg->has_payload()) {
        // DEBUG
        thread_println(std::cout, SERVER_TID,
            "Client did not send CLIENT_JOIN signal.",
            " Denying client's request.");

        // check for connection error, send message if no error
        switch (client_join_request_msg->type()) {
            case type::ERROR: // do nothing if connection error
                break;
            default:
                // create connection request denial message
                std::unique_ptr<message> denial(
                    message::create_message(type::ERROR, signal::CLIENT_DENY,
                        SERVER_ID, PLACEHOLDER,
                        "Connection denied, did not receive proper CLIENT_JOIN signal."));

                // send connection request denial message to client
                tmp_handshake_client->send(denial.get());

        } // end switch

        // clean up client memory (i.e. delete client's tcp_stream*)
        tmp_handshake_client.reset();

        // go back to waiting for connections
        return false;
    } // client join request conditional

    // we received the proper signal, so store the clients name
    std::string client_name = std::string(client_join_request_msg->payload());

    // DEBUG
    thread_println(std::cout, SERVER_TID,
        "Received CLIENT_JOIN with client name [ ", client_name, " ].");

    // create a client with this connection, through client pool, which also
    // assigns a unique id to the client
    std::shared_ptr<client> added_client = c_pool->add_handshake_client(
        client_name, tmp_handshake_client.get());

    // DEBUG
    thread_println(std::cout, SERVER_TID, "Sending client id [",
        added_client->get_id(), "].");

    // create client ID message to send
    std::unique_ptr<message> client_id_msg(
        message::create_message(type::SERVER, signal::CLIENT_JOIN, SERVER_ID,
            added_client->get_id()));
    // send the client their id
    added_client->send(client_id_msg.get());

    // receive ID confirmation message from client
    std::unique_ptr<message> confirm_id(added_client->receive());

    // check that client received correct id
    if (confirm_id->from() != added_client->get_id()) {
        thread_println(std::cout, SERVER_TID, "Client failed to confirm ID.",
            " Denying client's join request.");

        // check for connection error, send message if no error
        switch (confirm_id->type()) {
            case type::ERROR: // do nothing if connection error
                break;
            default: {
                // create connection request denial message
                std::unique_ptr<message> denial(
                    message::create_message(type::ERROR, signal::CLIENT_DENY,
                        SERVER_ID, PLACEHOLDER,
                        "Connection request denied, client failed to confirm ID."));

                // send connection request denial message to client
                added_client->send(denial.get());
            }
        } // end switch

        // remove client from the pool, which also deletes connection
        c_pool->remove(added_client->get_id());

        // go back to waiting for connections
        return false;
    } // confirm id conditional

    // DEBUG
    thread_println(std::cout, SERVER_TID, "Client [ ", added_client->get_name(),
        " ] confirmed id [ ", static_cast<int>(added_client->get_id()), " ].");

    // create final CLIENT_JOIN message
    std::unique_ptr<message> hshake_complete(
        message::create_message(type::SERVER, signal::CLIENT_JOIN, SERVER_ID,
            added_client->get_id()));

    // send final CLIENT_JOIN message to signal that handshaking is complete
    added_client->send(hshake_complete.get());

    // add weak pointer to client to message producer thread queue
    c_queue.add(std::weak_ptr<client>(added_client));

    // got back to waiting for connections
    return true;
}

/**
 *
 */
void server::shutdown() {

}

} // namespace server

} // namespace battleship
