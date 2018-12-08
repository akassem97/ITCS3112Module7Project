/*
 * Main.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: henly
 */


#include <iostream>
#include "Server.h"

using namespace std;

using namespace battleship::server;

int main(int argc, char* argv[]) {
    // process command line arguments
    if (argc < 3 || argc > 4) {
        cout << "usage:" << argv[0] << "<max connections> <port> <ip>"
             << std::endl;
        return -1;
    }

    int maxConnections = atoi(argv[1]);
    int port = atoi(argv[2]);
    string ip;
    if (argc == 4) {
        ip = argv[3];
    }

    std::cout << "Creating server with " << maxConnections
              << " max connections." << std::endl;

    Server server(maxConnections, port, ip);

    std::cout << "Starting Server ..." << std::endl;

    server.start();
}
