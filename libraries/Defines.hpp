#ifndef PACKET_TYPES
#define PACKET_TYPES
enum
{
    RECEIVE_NOTIFICATION = uint16_t(100), // Packet type used when an user is recieving a notification
    FOLLOW_USER,                          // Packet type used when the user will follow other user
    SEND_NOTIFICATION,                    // Packet type used when the user wants to send a notification
    OPEN_SESSION_SUCCESS,                 // Packet type used when the server connects the client to a session
    OPEN_SESSION_FAIL,                    // Packet type used when the server couldn't connect the client to a session
    CLOSE_SESSION_SUCCESS,                // Packet type used when the server sucessfully closes user connection
    ERROR,                                // Packet type used when we need to comunicate errors
    USER_CONNECT,                         // Packet type used when we send user name to connect
    USER_CLOSE_CONNECTION                 // Packet type used when we send user name to close connection
};
#endif

#ifndef PORT
#define PORT 4040
#endif

#ifndef PAYLOAD_MAX_SIZE
#define PAYLOAD_MAX_SIZE 512
#endif

#ifndef NOTIFICATION_MAX_SIZE
#define NOTIFICATION_MAX_SIZE 128
#endif

#ifndef BUFFER_HEADER_SIZE
#define BUFFER_HEADER_SIZE 4
#endif

#ifndef MIN_USERID_SIZE
#define MIN_USERID_SIZE 4
#endif

#ifndef MAX_USERID_SIZE
#define MAX_USERID_SIZE 20
#endif

#ifndef SENDER_ID_SIZE
#define SENDER_ID_SIZE 100
#endif

#ifndef CR
#define CR 13
#endif

#ifndef LF
#define LF 10
#endif

#ifndef SERVER_ADDRESS
#define SERVER_ADDRESS "localhost"
#endif