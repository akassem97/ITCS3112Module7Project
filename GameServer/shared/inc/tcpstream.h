#ifndef BATTLESHIP_TCPSTREAM_H_
#define BATTLESHIP_TCPSTREAM_H_

// unix operating system headers
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

// standard C++ headers
#include <string>

// DEBUG  namespace std
using namespace std;

/**
 * The tcp_stream class provides methods to transfer data between peers over a
 * TCP/IP connection.
 *
 * ToDo  give attribution to original source of TCP classes
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 11-25-2018
 */
class tcp_stream {
private:
    // private members
    int m_sd;
    std::string m_peer_ip;
    int m_peer_port;

    // creates this stream's socket descriptor and assigns IP and port
    tcp_stream(int sd, const struct sockaddr_in* address);

    /* Disallow Compiler Generated Constructors */
    tcp_stream() = delete;
    tcp_stream(tcp_stream& stream) = delete;

    /*
     * Used by tcp_stream::receive for non-blocking receives.
     * tout_s - time to wait in seconds.
     * tout_us - time to wait in microseconds.
     */
    bool wait_for_read_event(const int tout_s, const int tout_us = 0);

public:
    // friend classes
    friend class tcp_acceptor;
    friend class tcp_connector;

    // destructor that closes this stream's socket descriptor
    ~tcp_stream();

    // ToDo  document enums
    enum stream_error {
        CONNECTION_CLOSED = 0,    //!< CONNECTION_CLOSED
        CONNECTION_TIMEOUT = -1, //!< CONNECTION_TIMEDOUT
        CONNECTION_RESET = -2     //!< CONNECTION_RESET
    };

    //
    ssize_t send(const char* buffer, const size_t len);

    //
    ssize_t receive(char* buffer, const size_t len, const int tout_s = 0,
        const int tout_us = 0);

    // returns peer ip
    std::string get_peer_ip() const;

    // returns peer port
    int get_peer_port() const;

};

#endif /* BATTLESHIP_TCP_STREAM_H_ */
