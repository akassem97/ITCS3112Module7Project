// source header
#include "tcpacceptor.h"

// standard library header
#include <iostream> // std::err
#include <cstring> // memset

// unix system headers
#include <arpa/inet.h>


/* ToDo  document this class */

/**
 *
 * @param port
 * @param address
 */
tcp_acceptor::tcp_acceptor(int port, const char* address)
    : m_lsd(0), m_port(port), m_address(address), m_listening(false)
{
}

/**
 * Closes any listening sockets.
 */
tcp_acceptor::~tcp_acceptor() {
    // close any listening sockets
    if (m_lsd > 0) {
        close(m_lsd);
    }
}

/**
 * Start listening for connections with this {@code tcp_acceptor} instance.
 *
 * @return 0 on successful start, non-zero if an error occurred.
 */
int tcp_acceptor::start() {
    if (m_listening) {
        return 0;
    }

    m_lsd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;
    address.sin_port = htons(m_port);
    if (m_address.size() > 0) {
        inet_pton(PF_INET, m_address.c_str(), &(address.sin_addr));
    } else {
        address.sin_addr.s_addr = INADDR_ANY;
    }

    int optval = 1;
    setsockopt(m_lsd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    int result = bind(m_lsd, (struct sockaddr*) &address, sizeof(address));
    if (result != 0) {
        std::cerr << "bind() failed" << std::endl;
        return result;
    }

    result = listen(m_lsd, 5);
    if (result != 0) {
        std::cerr << "listen() failed" << std::endl;
        return result;
    }

    // signal that this acceptor is listening
    m_listening = true;

    return result;
}

/**
 * @return a connection accepted by this {@code tcp_acceptor} instance.
 */
tcp_stream* tcp_acceptor::accept() {
    // if not listening then return nullptr
    if (!m_listening) {
        return nullptr;
    }

    struct sockaddr_in address;
    socklen_t len = sizeof(address);
    memset(&address, 0, sizeof(address));
    int sd = ::accept(m_lsd, (struct sockaddr*) &address, &len);
    if (sd < 0) {
        perror("accept() failed");
        return nullptr;
    }
    return new tcp_stream(sd, &address);
}
