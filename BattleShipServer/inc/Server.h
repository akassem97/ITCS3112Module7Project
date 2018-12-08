#ifndef BATTLESHIP_SERVER_H_
#define BATTLESHIP_SERVER_H_

#include <client.h>
#include <cstdint>
#include <clientpool.h>
#include <tsqueue.h>
#include <string>

#include "tcpacceptor.h"

namespace battleship {
namespace server {

class Server {
private:
    ts_queue<std::shared_ptr<client>> c_queue; // the consumer thread queue
    client_pool* c_pool; // pool of clients
    const unsigned char max_connections; // max number of client connections
    const int port; // port of this server
    std::string ip; // server's IP, if one is provided
    tcp_acceptor* connection_acceptor; // used to listen for client connections
    bool running; // the state of this server

    // no default constructor
    Server() = delete;

    // main server loop called from start
    void run();

public:

    // server error enums
    enum {
        SERVER_ALREADY_RUNNING = -1,           //!< SERVER_ALREADY_RUNNING
        COULD_NOT_CREATE_CONSUMER_THREADS = -2, //!< COULD_NOT_CREATE_CONSUMER_THREADS
        COULD_NOT_CREATE_CON_ACCEPTOR = -3 //!< COULD_NOT_CREATE_CON_ACCEPTOR
    };

    //
    explicit Server(const unsigned char max_conns, const int port,
        const std::string ip = "");

    ~Server() {
        /* ToDo
         * Implement an actual strategy or d'tor for use when receiving SIGINT,
         * SIGABORT, etc.
         *
         * Thoughts:
         * Consumer queue should be filled with every client in pool. Then
         * consumer threads notify clients of server shutdown and remove them
         * from the pool, until queue is empty. At which point consumer threads
         * should 'join()' back with main server thread. Main server thread
         * should then iterate over consumer threads container 'delete'ing them
         * (if that is necessary).
         */

    }

    //
    int start();

    //
    void shutdown();

    /**
     * Gets the maximum number of client connections for this server.
     * @return the maximum number of client connections.
     */
    unsigned char get_max_connections() const {
        return max_connections;
    }

    /**
     * Gets the port this server is listening on.
     * @return the port this server is listening on.
     */
    int get_port() const {
        return port;
    }

    /**
     * Gets this server's IP address.
     * @return this server's IP address.
     */
    std::string get_ip() const {
        return ip;
    }

    /**
     * Gets the current number of connected clients.
     * @return the current number of connected clients.
     */
    unsigned char get_connection_count() const {
        return c_pool->size();
    }

    /**
     * Checks if this server is running, that is, checks whether this server is
     * accepting connections or not.
     * @return true if this server is running, otherwise false.
     */
    bool is_running() const {
        return running;
    }

};

} // namespace server
} // namespace battleship

#endif /* BATTLESHIP_SERVER_H_ */
