#ifndef BATTLESHIP_TCP_CONNECTOR_H_
#define BATTLESHIP_TCP_CONNECTOR_H_

#include <netinet/in.h>

#include "tcpstream.h"

/* ToDo  document this class */

class tcp_connector {
public:
    tcp_stream* connect(const char* server, int port);
    tcp_stream* connect(const char* server, int port, int timeout);

private:
    int resolveHostName(const char* host, struct in_addr* addr);
};

#endif /* BATTLESHIP_TCP_CONNECTOR_H_ */
