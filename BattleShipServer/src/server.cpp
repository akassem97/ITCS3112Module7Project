/*
 * Server.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: henly
 */

// source header
#include <server.h>
#include <iostream> // std::cerr
#include <memory> // std::shared_ptr

#include "game.h"
#include "message.h" // enum class Signal
#include "msgconsumer.h"
#include "msgproducer.h"
#include "thread.h"

/* DEBUG  beg debugout.h */
#include "debugout.h"
using namespace debug_out;
/* DEBUG  end debugout.h*/

namespace battleship {

namespace game_server {

using namespace network_message;

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
    games.reserve(max_connections / 2);
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
            games);
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

/*
 *
 */
void server::run() {

    // add a client to the pool for each connection, unless pool is full
    while (1) {
        // block until connection attempt
        tcp_stream* connection = connection_acceptor->accept();

        // DEBUG
        thread_println(std::cout, SERVER_TID,
            "Receiving new connection attempt.");

        // check if connection is functional
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

            // send server-full message to client
            auto server_full = message::create_message(type::SERVER,
                signal::SERVER_FULL, SERVER_ID, 0);
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
    client* tmp_handshake_client = c_pool->create_tmp_client(connection);
    // receive client's join request message, including client's name
    auto client_join_request_msg = tmp_handshake_client->receive(5);

    // check if requesting client did not send CLIENT_JOIN signal
    if (client_join_request_msg->signal() != signal::CLIENT_JOIN) {
        // DEBUG
        thread_println(std::cout, SERVER_TID,
            "Client did not send CLIENT_JOIN signal.",
            " Denying client's request.");

        // check for connection error, send message if no error
        switch (client_join_request_msg->type()) {
            case type::ERROR: // do nothing if connection error
                break;
            default:
                // send connection request denial message to client
                tmp_handshake_client->send(
                    message::create_message(type::ERROR, signal::CLIENT_DENY,
                        SERVER_ID, 0,
                        "Connection denied, did not receive proper CLIENT_JOIN signal."));

        } // end switch

        // clean up memory
        delete connection;

        // go back to waiting for connections
        return false;
    } // client join request conditional

    // store the clients name
    std::string client_name = std::string(client_join_request_msg->payload());

    // DEBUG
    thread_println(std::cout, SERVER_TID,
        "Received CLIENT_JOIN with client name [ ", client_name, " ].");

    // create a client with this connection, through client pool, which also
    // assigns a unique id to the client
    std::shared_ptr<client> client = c_pool->create_and_add_client(client_name,
        std::move(tmp_handshake_client->get_stream()));

    // DEBUG
    thread_println(std::cout, SERVER_TID, "Sending client id [",
        client->get_id(), "].");

    // send the client their id
    client->send(
        message::create_message(type::SERVER, signal::CLIENT_JOIN, SERVER_ID,
            client->get_id()));

    // check that client received correct id
    auto confirm_id = client->receive();
    if (confirm_id->from() != client->get_id()) {
        thread_println(std::cout, SERVER_TID, "Client failed to confirm ID.",
            " Denying client's join request.");

        // check for connection error, send message if no error
        switch (confirm_id->type()) {
            case type::ERROR: // do nothing if connection error
                break;
            default:
                // send connection request denial message to client
                client->send(
                    message::create_message(type::ERROR, signal::CLIENT_DENY,
                        SERVER_ID, PLACEHOLDER,
                        "Connection request denied, client failed to confirm ID."));

        } // end switch

        // remove client from the pool, which also deletes connection
        c_pool->remove(client->get_id());

        // go back to waiting for connections
        return false;
    } // confirm id conditional

    // DEBUG
    thread_println(std::cout, SERVER_TID, "Client [ ", client->get_name(),
        " ] confirmed id [ ", static_cast<int>(client->get_id()), " ].");

    // send final CLIENT_JOIN to signal that handshaking is complete
    client->send(
        message::create_message(type::SERVER, signal::CLIENT_JOIN, SERVER_ID,
            client->get_id()));

    // add client to worker thread queue
    c_queue.add(client);

    return true;
}

/**
 *
 */
void server::shutdown() {

}

} // namespace server

} // namespace battleship
