#ifndef BATTLESHIP_TCP_ACCEPTOR_H_
#define BATTLESHIP_TCP_ACCEPTOR_H_

#include <string>
#include <netinet/in.h>
#include "tcpstream.h"

// DEBUG  namespace std
using namespace std;

/* ToDo  document this class */

class tcp_acceptor {
    int m_lsd;
    int m_port;
    string m_address;
    bool m_listening;

public:
    tcp_acceptor(int port, const char* address = "");
    ~tcp_acceptor();

    int start();
    tcp_stream* accept();

private:
    tcp_acceptor() = delete;
};

#endif /* BATTLESHIP_TCP_ACCEPTOR_H_ */
