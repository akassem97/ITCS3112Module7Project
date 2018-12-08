/*
 client.cpp

 Test client for Battleship server.
 */

#include <message.h>
#include <iostream>
#include <string>

#include "tcpconnector.h"

using namespace std;

using namespace battleship;

typedef battleship::message message;

namespace client {

void outputSending(message& msg) {
    cout << "Sending message [" << static_cast<int>(msg.signal())
        << static_cast<int>(msg.from()) << static_cast<int>(msg.to())
        << msg.msg() + 3 << "]." << endl;
}

void outputReceived(char* line, ssize_t len) {
    cout << "Received: [";

    if (len <= 0) {
        cout << "Error: " << static_cast<int>(len) << "]." << endl;
        return;
    }

    line[len] = '\0'; // c_string, i.e. null terminated
    for (ssize_t i = 0; i < len - 1; i++) {
        if (i < 3) {
            cout << static_cast<int>(line[i]);
        } else {
            cout << line[i];
        }
    }
    cout << "]." << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }

    cout << "Starting client." << endl;
    cout << "Connecting to server." << endl;

    // TCP variables
    tcp_connector* connector = new tcp_connector();
    tcp_stream* stream = connector->connect(argv[2], atoi(argv[1]));

    // message variables
    ssize_t len;
    string message;
    char line[256];

    cout << "Enter your username:" << endl;

    cin >> message;
    battleship::message msg = message::build_message(signal::CLIENT_JOIN, -1,
        -1, message);

    // output message to send
    outputSending(msg);
    stream->send(msg.msg(), msg.length());

    // output received message from server
    len = stream->receive(line, sizeof(line));
    outputReceived(line, len);

    // exit if server is full
    if (static_cast<signal>(line[0]) == signal::SERVER_FULL) {
        cout << "The server is full." << endl;
        return 0;
    }

    int id = line[2];

    // build name message
    msg = message::build_simple_message(signal::CLIENT_JOIN, id, -1);

    // send id confirmation message
    outputSending(msg);
    stream->send(msg.msg(), msg.length());

    // get final handshaking message
    len = stream->receive(line, sizeof(line));
    outputReceived(line, len);

    while (message != "quit") {
        // build message to send
        msg = message::build_simple_message(signal::CLIENT_BUSY, id, -1);

        // output sending message
        outputSending(msg);
        stream->send(msg.msg(), msg.length());

        // output received message from server
        len = stream->receive(line, sizeof(line));
        outputReceived(line, len);

        // build message to send
        msg = message::build_simple_message(signal::CLIENT_CHALLENGE, id, -1);

        // output message to send
        outputSending(msg);
        stream->send(msg.msg(), msg.length());

        // output received message from server
        len = stream->receive(line, sizeof(line));
        outputReceived(line, len);

        msg = message::build_simple_message(signal::CLIENT_LOBBY_LIST, id, -1);
        // output message to send
        outputSending(msg);
        stream->send(msg.msg(), msg.length());

        // output received message from server
        len = stream->receive(line, sizeof(line));
        outputReceived(line, len);

        // PAUSE EXECUTION
        std::cin >> message;

        if (message == "quit") {
            // build quit message
            msg = message::build_simple_message(signal::CLIENT_EXIT, id, -1);

            // output message to send
            outputSending(msg);
            stream->send(msg.msg(), msg.length());
        }
    }

    cout << "Ending client." << endl;

    if (stream) {
        delete stream;
    }

    exit(0);
}

}
