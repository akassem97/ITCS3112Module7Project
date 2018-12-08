// unix operating system header
#include <arpa/inet.h>

#include "tcpstream.h"

tcp_stream::tcp_stream(int sd, const struct sockaddr_in* address)
    : m_sd(sd)
{
    char ip[50]; // ip address buffer

    // parses an IP address in network format (big-endian) to presentation form,
    // i.e. C string (a null terminated char array)
    inet_ntop(PF_INET, (struct in_addr*) &(address->sin_addr.s_addr), ip,
        sizeof(ip) - 1);
    m_peer_ip = ip;

    // parses port using ntohs (network to host short), i.e. parses big-endian
    // short (2 byte integer with MSB in 1st byte) to host's endianess
    m_peer_port = ntohs(address->sin_port);
}

/**
 * Destructor that closes the socket descriptor associated with this tcp stream.
 */
tcp_stream::~tcp_stream() {
    close(m_sd);
}

/**
 *
 * @param buffer
 * @param len
 * @return
 */
ssize_t tcp_stream::send(const char* buffer, size_t len) {
    return write(m_sd, buffer, len);
}

/**
 *
 * @param buffer - a pointer to a char arrays to store the read in bytes.
 * @param len - the number of bytes to read into buffer.
 * @param tout_s - timeout in seconds, default is 0.
 * @param tout_us - timeout in microseconds, default is 0.
 * @return
 */
ssize_t tcp_stream::receive(char* buffer, const size_t len, const int tout_s,
    const int tout_us)
{

    // if no timeout specified then block until data has been received
    if (tout_s <= 0 && tout_us <= 0) {
        return read(m_sd, buffer, len);
    }

    // if timeout specified then poll for timeout length and return read data if
    // available, otherwise return that the connection exceeded the timeout
    if (wait_for_read_event(tout_s, tout_us)) {
        return read(m_sd, buffer, len);
    }

    return stream_error::CONNECTION_TIMEOUT;
}

/**
 * @return the IP address this stream is connected to.
 */
std::string tcp_stream::get_peer_ip() const {
    return m_peer_ip;
}

/**
 * @return the port this stream is connected to.
 */
int tcp_stream::get_peer_port() const {
    return m_peer_port;
}

/*
 *
 */
bool tcp_stream::wait_for_read_event(const int tout_s, const int tout_us) {
    fd_set sdset; // socket descriptor
    struct timeval tv; // timeout timeval

    // init. timeval struct with seconds and microseconds
    tv.tv_sec = tout_s;
    tv.tv_usec = tout_us;

    // setup the socket descriptor
    FD_ZERO(&sdset);
    FD_SET(m_sd, &sdset);

    // if data has been received before timeval has elapsed then return true
    if (select(m_sd + 1, &sdset, NULL, NULL, &tv) > 0) {
        return true;
    }

    // still waiting for data to be received, i.e. a timeout has occurred
    return false;
}
