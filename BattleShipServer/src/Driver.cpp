/*
 * Main.cpp
 *
 *  Created on: Dec 1, 2018
 *      Author: henly
 */


#include <server.h>
#include <iostream>

using namespace std;

using namespace battleship;

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

    std::cout << "Creating server with a maximum of " << maxConnections
        << " connections." << std::endl;

    game_server::server server(maxConnections, port, ip);

    std::cout << "Starting Server ..." << std::endl;

    server.start();
}
