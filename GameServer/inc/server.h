#ifndef BATTLESHIP_SERVER_H_
#define BATTLESHIP_SERVER_H_
// standard library headers
#include <string>

// battleship server headers
#include "client.h"
#include "clientpool.h"
#include "game.h"
#include "gamepool.h"
#include "tcpacceptor.h"
#include "tsqueue.h"


namespace battleship {

namespace game_server {

/**
 * @brief Battleship server implementation that handles multiple client
 * connections and one-on-one games.
 *
 * @author Adham Kassem
 * @author Jonathan Henly
 * @version 11-27-2018
 */
class server {
private:
    const static unsigned char SERVER_ID = 255;
    const static unsigned char PLACEHOLDER = 254;

    const unsigned char max_connections; // max number of client connections
    const int port; // port of this server
    std::string ip; // server's IP, if one is provided
    bool running; // the state of this server
    ts_queue<std::weak_ptr<client>> c_queue; // client message producer thread
                                               // queue
    ts_queue<network_message::message*> msg_queue; // message consumer thread queue
    client_pool* c_pool; // pool of clients
    game_pool* g_pool;
    tcp_acceptor* connection_acceptor; // used to listen for client connections

    // no default constructor
    server() = delete;
    /* Disallow Copy Operations */
    server(const server&) = delete; // don't implement copy c'tor
    server& operator=(const server&) = delete; // don't implement copy
                                                         // assignment

    // main server loop called from start
    void run();

    // performs handshake procedure and returns outcome
    bool perform_handshake_procedure(tcp_stream* const connection);

public:

    // server error enums
    enum {
        SERVER_ALREADY_RUNNING = -1,           //!< SERVER_ALREADY_RUNNING
        COULD_NOT_CREATE_CONSUMER_THREADS = -2, //!< COULD_NOT_CREATE_CONSUMER_THREADS
        COULD_NOT_CREATE_CON_ACCEPTOR = -3 //!< COULD_NOT_CREATE_CON_ACCEPTOR
    };

    //
    explicit server(const unsigned char max_conns, const int port,
        const std::string ip = "");

    ~server() {
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
