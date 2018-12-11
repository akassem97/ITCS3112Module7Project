#ifndef BATTLESHIP_TCP_ACCEPTOR_H_
#define BATTLESHIP_TCP_ACCEPTOR_H_

#include <string>
#include <netinet/in.h>
#include "tcpstream.h"

// DEBUG  namespace std
using namespace std;

/* ToDo  document this class */

class tcp_acceptor {
private:
    int m_lsd;
    int m_port;
    string m_address;
    bool m_listening;

    // don't implement default constructor
    tcp_acceptor() = delete;
    /* Disallow Copy Operations */
    tcp_acceptor(const tcp_acceptor&) = delete; // don't implement copy constructor
    tcp_acceptor& operator=(const tcp_acceptor&) = delete; // don't implement copy
                                                           // assignment oper.
public:
    //
    tcp_acceptor(int port, const char* address = "");

    //
    ~tcp_acceptor();

    // start listening for connections
    int start();

    // return a connection
    tcp_stream* accept();

};

#endif /* BATTLESHIP_TCP_ACCEPTOR_H_ */
