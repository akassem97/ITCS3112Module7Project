#ifndef BATTLESHIP_MESSAGE_H_
#define BATTLESHIP_MESSAGE_H_

#include <memory>

namespace battleship {

namespace network_message {
/**
 * ToDo  enum class form documentation
 */
enum class type
    : char {
        SERVER = 1, //!< SERVER
    GAME = 2,      //!< GAME
    ERROR = 3,         //!< ERROR
};

/**
 * ToDo  enum class signal documentation
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
    CLIENT_BUSY,     //!< CLIENT_BUSY
    CLIENT_ACCEPT,
    CLIENT_DENY,
    CLIENT_MATCHMAKING,
    GAME_CREATED,
    GAME_READY,
    GAME_COORDINATES,      //!< GAME_COORDINATES
    GAME_HIT,        //!< GAME_HIT
    GAME_MISS,       //!< GAME_MISS
    GAME_WINNER,     //!< GAME_WINNER
    GAME_LOSER,      //!< GAME_LOSER
    GAME_CLIENT_QUIT,      //!< GAME_CLIENT_QUIT
    GAME_CLIENT_DISC,      //!< GAME_CLIENT_DISC
    ERROR,            //!< ERROR
};

/**
 * @brief Base network messaging class.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 12-03-2018
 */
class message {
private:
    // private static members
    static const char MESSAGE_TERMINATOR = '\0'; // messages are null terminated
    static const unsigned char NUM_OFFSETS = 6; // TYPE, SIGNAL, FROM, TO, ...

    // private members
    char* m_msg;
    unsigned char m_len;

    message() = delete; // don't implement default constructor
    message(const message&) = delete; // don't implement copy constructor

protected:
    // protected static members
    static const unsigned char TYPE_OFFSET = 0; // byte offset of type
    static const unsigned char SIGNAL_OFFSET = 1; // byte offset of signal
    static const unsigned char FROM_OFFSET = 2; // byte offset of from
    static const unsigned char TO_OFFSET = 3; // byte offset of to
    static const unsigned char HAS_PAYLOAD_OFFSET = 4; // has payload boolean
    static const unsigned char PAYLOAD_OFFSET = 5; // where the payload begins

    // creates a message from a char buffer and buffer length
    message(const char* msg, const unsigned char len)
        : m_len(len + 1)
    {
        m_msg = new char[m_len]();

        // copy chars from msg to m_msg
        for (unsigned char i = 0; i < len; ++i) {
            m_msg[i] = msg[i];
        }

        // null terminate message
        m_msg[len] = MESSAGE_TERMINATOR;
    }

    // creates a message by placement from the first four parameters, and then
    // copies the chars in the 'msg' parameter
    message(const network_message::type m_type,
        const network_message::signal m_signal, const unsigned char from,
        const unsigned char to, const char* payload = nullptr,
        const unsigned char payload_length = 0)
        : m_len(NUM_OFFSETS + payload_length + 1)
    {
        m_msg = new char[m_len]();

        // set offsets their values
        m_msg[TYPE_OFFSET] = static_cast<char>(m_type);
        m_msg[SIGNAL_OFFSET] = static_cast<char>(m_signal);
        m_msg[FROM_OFFSET] = from;
        m_msg[TO_OFFSET] = to;
        m_msg[HAS_PAYLOAD_OFFSET] = -1;
        m_msg[PAYLOAD_OFFSET] = -1;

        // copy payload if applicable
        if (payload) {
            m_msg[HAS_PAYLOAD_OFFSET] = 1;
            m_msg[PAYLOAD_OFFSET] = NUM_OFFSETS + 1;

            // copy chars from msg to m_msg
            for (size_t i = 0; i < payload_length; ++i) {
                m_msg[NUM_OFFSETS + 1 + i] = payload[i];
            }
        }
        // null terminate message
        m_msg[m_len] = MESSAGE_TERMINATOR;
    }

    // returns '\0', since messages are null terminated
    const char message_terminator() {
        return MESSAGE_TERMINATOR;
    }

    // returns the next available custom assignable offset
    virtual unsigned char next_available_offset() const {
        return NUM_OFFSETS + 1;
    }

    // returns a pointer to the underlying char message
    char* msg() {
        return m_msg;
    }

public:
    /**
     * Deletes this message's underlying {@code char} array.
     */
    virtual ~message() {
        delete[] m_msg;
    }

    /**
     *@return the underlying message {@code char} array.
     */
    const char* msg() const {
        return m_msg;
    }

    /**
     * @return the length of this message.
     */
    const size_t length() const {
        return m_len;
    }

    /**
     * @return the {@code type} of this message.
     */
    const network_message::type type() const {
        return static_cast<network_message::type>(m_msg[TYPE_OFFSET]);
    }

    /**
     * @return what this message is signaling.
     */
    const network_message::signal signal() const {
        return static_cast<network_message::signal>(m_msg[SIGNAL_OFFSET]);
    }

    /**
     * @return the sender ID of this message.
     */
    const unsigned char from() const {
        return m_msg[FROM_OFFSET];
    }

    /**
     * @return the recipient ID of this message.
     */
    const unsigned char to() const {
        return m_msg[TO_OFFSET];
    }

    /**
     * @return true if this message has a payload, otherwise false.
     */
    const bool has_payload() const {
        return m_msg[HAS_PAYLOAD_OFFSET];
    }

    /**
     * @return a pointer to this message's payload.
     */
    const char* payload() const {
        return has_payload() ?
            msg() + static_cast<unsigned char>(msg()[PAYLOAD_OFFSET]) : nullptr;
    }

    /**
     *
     * @return the byte length of this message's payload.
     */
    const unsigned char payload_length() const {
        if (!has_payload()) {
            return 0;
        }

        return length() - static_cast<unsigned char>(msg()[PAYLOAD_OFFSET]);
    }

    /**
     * Parses a message from a passed in {@code char} array of length
     * {@code len}.
     *
     * @param msg - a correctly coded {@code char} array.
     * @param len - the byte length of {@code msg}.
     *
     * @return a smart pointer holding a newly created message from a passed in
     *         {@code char} array of length {@code len}.
     */
    static message* parse_message(const char* msg,
        const unsigned char len)
    {
        return new message(msg, len);
    }

    /**
     * Create a message from the specified arguments.
     *
     * @param m_type - the type of message.
     * @param m_signal - what the message signals.
     * @param from - the ID of the sender.
     * @param to - the ID of the recipient.
     * @param payload - this message's optional payload.
     * @param payload_length - length of optional payload.
     *
     * @return a smart pointer holding this newly created message.
     */
    static message* create_message(
        const network_message::type m_type,
        const network_message::signal m_signal, const unsigned char from,
        const unsigned char to, const char* payload = nullptr,
        const unsigned char payload_length = 0)
    {
        return new message(m_type, m_signal, from, to, payload, payload_length);
    }

    /**
     * Create a message from the specified arguments.
     *
     * @param m_type - the type of message.
     * @param m_signal - what the message signals.
     * @param from - the ID of the sender.
     * @param to - the ID of the recipient.
     * @param payload - the body of this messsage.
     *
     * @return a smart pointer holding this newly created message.
     */
    static message* create_message(
        const network_message::type m_type,
        const network_message::signal m_signal, const unsigned char from,
        const unsigned char to, const std::string payload)
    {
        return
            new message(m_type, m_signal, from, to, payload.data(),
            payload.length());
    }

};
// class message

/**
 * @brief Game network message class.
 *
 * @author Jonathan Henly
 * @author Adham Kassem
 * @version 12-03-2018
 */
class game_message: public message {

private:
    static const unsigned char GAME_ID_OFFSET = 6;
    static const unsigned char NUM_OFFSETS = 7;

    // public create_game_message helper method
    static game_message* create_message_helper(
        const network_message::signal msg_signal, const unsigned char from,
        const unsigned char to, const unsigned char game_id,
        const char* payload, const unsigned char payload_length)
    {
        char tmp[255];

        // set offsets their values
        tmp[TYPE_OFFSET] = static_cast<char>(network_message::type::GAME);
        tmp[SIGNAL_OFFSET] = static_cast<char>(msg_signal);
        tmp[FROM_OFFSET] = from;
        tmp[TO_OFFSET] = to;
        tmp[HAS_PAYLOAD_OFFSET] = -1;
        tmp[PAYLOAD_OFFSET] = -1;
        tmp[GAME_ID_OFFSET] = game_id;

        // copy payload if applicable
        if (payload) {
            tmp[HAS_PAYLOAD_OFFSET] = 1;
            tmp[PAYLOAD_OFFSET] = NUM_OFFSETS + 1;

            // copy chars from msg to m_msg
            for (size_t i = 0; i < payload_length; ++i) {
                tmp[NUM_OFFSETS + 1 + i] = payload[i];
            }
        }
        // null terminate message
        tmp[NUM_OFFSETS + payload_length + 1] = '\0';

        // return newly created game_message
        return new game_message(tmp, NUM_OFFSETS + payload_length);
    }

protected:
    // constructs a game message with a game id
    game_message(const char* msg, const unsigned char len)
        : message(msg, len)
    {
    }

    // returns the next available custom assignable offset
    virtual unsigned char next_available_offset() const {
        return NUM_OFFSETS + 1;
    }

public:
    /**
     * Calls {@code virtual ~message()}.
     */
    virtual ~game_message() {
    }

    /**
     * @return this message's game ID.
     */
    const unsigned char game_id() const {
        return static_cast<unsigned char>(msg()[GAME_ID_OFFSET]);
    }

    /**
     * Parses a message from a passed in {@char array} of length {@code len}.
     *
     * @param msg - a correctly coded {@code char} array.
     * @param len - the byte length of {@code msg}.
     *
     * @return a smart pointer to a new message created from a passed in
     *         {@code char} array of length {@code len}.
     */
    static game_message* parse_message(const char* msg,
        const unsigned char len)
    {
        return new game_message(msg, len);
    }

    /**
     *
     * @param msg_signal -
     * @param from -
     * @param to -
     * @param game_id -
     * @param payload -
     * @param payload_length -
     * @return
     */
    static game_message* create_message(
        const network_message::signal msg_signal,
        const unsigned char from, const unsigned char to,
        const unsigned char game_id,
        const char* payload = nullptr, const unsigned char payload_length = 0)
    {
        return create_message_helper(msg_signal, from, to, game_id, payload,
            payload_length);
    }

};
// class game_message

}// namespace network_message

} // namespace battleship

#endif /* MESSAGE_H_ */
