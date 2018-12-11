// source header
#include "tcpconnector.h"

// standard library headers
#include <cerrno>
#include <cstring> // memset
#include <iostream> // std::err

// unix system headers
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>



/**
 * ToDo  document this method
 *
 * @param server
 * @param port
 * @return
 */
tcp_stream* tcp_connector::connect(const char* server, const int port,
    const int tout_s, const int tout_us)
{
    struct sockaddr_in address;

    // zero out address memory
    memset(&address, 0, sizeof(address));

    // set address attributes
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    //
    if (resolve_host_name(server, &(address.sin_addr)) != 0) {
        inet_pton(PF_INET, server, &(address.sin_addr));
    }

    // create and connect the socket, bail if we fail in either case
    const int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket() failed");
        return nullptr;
    }

    // if timeout specified in seconds or microseconds then create socket in
    // non blocking mode
    if (tout_s != 0 || tout_us != 0) {
        return nb_connect_wtout(sd, address, tout_s, tout_us);
    }

    // since no timeout was specified, create socket in blocking mode
    if (::connect(sd, reinterpret_cast<struct sockaddr*>(&address),
        sizeof(address)) != 0) {
        // on connection failure, output error and close socket
        perror("connect() failed");
        close(sd);
        return nullptr;
    }


    return new tcp_stream(sd, &address);
}

/*
 * non blocking connect with timeout
 */
tcp_stream* tcp_connector::nb_connect_wtout(const int& sd,
    const struct sockaddr_in& address, const int tout_s, const int tout_us)
{
    // set socket to non-blocking
    long arg;
    arg = fcntl(sd, F_GETFL, nullptr);
    arg |= O_NONBLOCK;
    fcntl(sd, F_SETFL, arg);

    // connect with time limit
    int result = -1;
    if ((result = ::connect(sd,
        reinterpret_cast<const struct sockaddr*>(&address),
        sizeof(address))) < 0) {

        if (errno == EINPROGRESS) {
            struct timeval tv;
            tv.tv_sec = tout_s;
            tv.tv_usec = tout_us;

            fd_set sdset;
            FD_ZERO(&sdset);
            FD_SET(sd, &sdset);

            int s = -1;
            do {
                s = select(sd + 1, nullptr, &sdset, nullptr, &tv);
            } while (s == -1 && errno == EINTR);

            if (s > 0) {
                int valopt;
                socklen_t len = sizeof(int);
                getsockopt(sd, SOL_SOCKET, SO_ERROR,
                    static_cast<void*>(&valopt), &len);

                if (valopt) {
                    std::cerr << "connect() error " << valopt << " - "
                        << strerror(valopt) << std::endl;
                } else {
                    // connection established
                    result = 0;
                }
            } else {
                close (sd);
                std::cerr << "connect() timed out" << std::endl;
            }
        } else {
            close (sd);
            std::cerr << "connect() error " << errno << " - " << strerror(errno)
                << std::endl;
        }
    }

    // return socket to blocking mode
    arg = fcntl(sd, F_GETFL, NULL);
    arg &= (~O_NONBLOCK);
    fcntl(sd, F_SETFL, arg);

    // return nullptr if connection did not succeed
    if (result == -1) return nullptr;

    // create and return a TCP stream if connection succeeded
    return new tcp_stream(sd, &address);
}

/*
 * ToDo  document this private method
 */
int tcp_connector::resolve_host_name(const char* hostname, struct in_addr* addr)
{
    struct addrinfo* res; // temporary addrinfo

    // on success, the 'getaddrinfo()' function stores, in the location pointed
    // to by res, a pointer to a linked list of one or more 'addrinfo' structs
    int result = getaddrinfo(hostname, nullptr, nullptr, &res);

    // if 'getaddrinfo(...)' succeeds then copy temp. 'res' to param. 'addr'
    // and clean up 'res's dynamically allocated memory
    if (result == 0) {
        // copy memory from temporary 'res' to parameter 'addr'
        memcpy(addr,
            &((struct sockaddr_in *) /*static_cast<struct sockaddr_in *>*/(res
                ->ai_addr))->sin_addr, sizeof(struct in_addr));

        // clean up dynamically allocated 'addrinfo' memory
        freeaddrinfo(res);
    }

    return result;
}
