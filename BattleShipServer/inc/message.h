#ifndef BATTLESHIP_MESSAGE_H_
#define BATTLESHIP_MESSAGE_H_

#include <iostream> // DEBUG

#include <string>

namespace battleship {

/**
 * ToDo  documentation
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 12-3-2018
 */
enum class signal
    : char {
        SERVER_FULL = -3,
    CONNECTION_TIMED_OUT = -2,
    CONNECTION_CLOSED = -1,
    MESSAGE_END = 0x00,
    CLIENT_EXIT = 0x01,  //!< CLIENT_EXIT
    CLIENT_JOIN,      //!< CLIENT_JOIN
    CLIENT_LOBBY_LIST,      //!< CLIENT_LOBBY_LIST
    CLIENT_CHALLENGE, //!< CLIENT_CHALLENGE
    CLIENT_BUSY,     //!< CLIENT_BUSY
    CLIENT_ACCEPT,    //!< CLIENT_ACCEPT
    CLIENT_DENY,      //!< CLIENT_DENY
    GAME_COORDINATES,      //!< GAME_COORDINATES
    GAME_HIT,        //!< GAME_HIT
    GAME_MISS,       //!< GAME_MISS
    GAME_WINNER,     //!< GAME_WINNER
    GAME_LOSER,      //!< GAME_LOSER
    GAME_CLIENT_QUIT,      //!< GAME_CLIENT_QUIT
    GAME_CLIENT_DISC,      //!< GAME_CLIENT_DISC
    ERROR = 99,            //!< ERROR
};

/**
 * ToDo  documentation
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 12-3-2018
 */
class message {
private:
    static const char MESSAGE_END = '\0';
    static const int SIGNAL_OFFSET = 0;
    static const int FROM_OFFSET = 1;
    static const int TO_OFFSET = 2;
    static const size_t DEFAULT_CAPACITY = 256;

    char m_msg[DEFAULT_CAPACITY];
    size_t m_length;

    message() = delete;

    /*
     *
     */
    message(battleship::signal sig, const char from, const char to,
        const char* payload =
        nullptr, const size_t payload_length = 0)
        : m_length(payload_length + 4)
    {

        m_msg[0] = static_cast<char>(sig);
        m_msg[1] = from;
        m_msg[2] = to;

        if (payload) {
            for (size_t i = 0; i < payload_length; ++i) {
                m_msg[i + 3] = payload[i];
            }
        }
        m_msg[m_length - 1] = MESSAGE_END;
    }

    /*
     *
     */
    message(const char* input, const size_t length)
        : m_length(length)
    {

        m_msg[SIGNAL_OFFSET] = input[SIGNAL_OFFSET];
        m_msg[FROM_OFFSET] = input[FROM_OFFSET];
        m_msg[TO_OFFSET] = input[TO_OFFSET];

        for (size_t i = 3; i < m_length; ++i) {
            m_msg[i] = input[i];
        }

        m_msg[m_length - 1] = MESSAGE_END;
    }

public:

    /**
     * Default destructor.
     */
    ~message() {
    }

    /**
     * @return the length of this message.
     */
    size_t length() const {
        return m_length;
    }

    /**
     *
     * @return
     */
    battleship::signal signal() const {
        return static_cast<battleship::signal>(m_msg[SIGNAL_OFFSET]);
    }

    /**
     *
     * @return
     */
    char from() const {
        return m_msg[FROM_OFFSET];
    }

    /**
     *
     * @return
     */
    char to() const {
        return m_msg[TO_OFFSET];
    }

    /**
     *
     * @return
     */
    const char* msg() const {
        return m_msg;
    }

    /**
     *
     * @param input
     * @param length
     * @return
     */
    static message build_message(const char* input, const size_t length) {
        return message(input, length);
    }

    /**
     *
     * @param signal
     * @param from
     * @param to
     * @param payload
     * @param payload_length
     * @return
     */
    static message build_message(battleship::signal sig, const char from,
        const char to, const char* payload, const size_t payload_length)
    {
        return message(sig, from, to, payload, payload_length);
    }

    /**
     *
     * @param signal
     * @param from
     * @param to
     * @param payload
     * @return
     */
    static message build_message(battleship::signal sig, const char from,
        const char to, const std::string& payload)
    {
        return build_message(sig, from, to,
            const_cast<char*>(payload.data()), payload.length());
    }

    /**
     *
     * @param signal
     * @param from
     * @param to
     * @return
     */
    static message build_simple_message(battleship::signal sig,
        const char from, const char to)
    {
        return message(sig, from, to);
    }

    /**
     *
     * @param signal
     * @return
     */
    static message build_error_message(battleship::signal sig) {
        return build_simple_message(sig, 0, 0);
    }

};

} // namespace battleship

#endif /* BATTLESHIP_MESSAGE_H_ */
