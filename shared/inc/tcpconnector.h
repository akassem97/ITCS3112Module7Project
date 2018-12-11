#ifndef BATTLESHIP_TCP_CONNECTOR_H_
#define BATTLESHIP_TCP_CONNECTOR_H_

// unix operating system headers
#include <netinet/in.h>

// project specific headers
#include "tcpstream.h"

/**
 * @brief  ToDo  document this class
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 11-27-2018
 */
class tcp_connector {
public:
    // attempt establishing a connection, if tout_s or tout_us are set then non
    // the attempt will be made with a non blocking socket within the specified
    // time, otherwise the attempt will be made with a blocking socket
    tcp_stream* connect(const char* server, const int port,
        const int tout_s = 0, const int tout_us = 0);

private:

    // non blocking connect with timeout
    tcp_stream* nb_connect_wtout(const int& sd,
        const struct sockaddr_in& address, const int tout_s, const int tout_us);
    // attempt to resolve a hostname, if successful then populate 'addr' with
    // hostname info
    int resolve_host_name(const char* host, struct in_addr* addr);
};

#endif /* BATTLESHIP_TCP_CONNECTOR_H_ */
