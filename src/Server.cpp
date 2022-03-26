#include "../libraries/Server.hpp"

using namespace std;

Server::Server()
{
    this->database = Database();
    this->notificationIdCounter = 0;
    mutexSession = PTHREAD_MUTEX_INITIALIZER;
    followMutex = PTHREAD_MUTEX_INITIALIZER;
    mutexCommunication = PTHREAD_MUTEX_INITIALIZER;

    pthread_cond_init(&condNotificationEmpty, NULL);
    pthread_cond_init(&condNotificationFull, NULL);
    pthread_mutex_init(&mutexNotificationSender, NULL);
}

Server::Server(host_address address)
{
    this->database = Database();
    this->notificationIdCounter = 0;
    this->ip = address.ipv4;
    this->port = address.port;

    mutexSession = PTHREAD_MUTEX_INITIALIZER;
    followMutex = PTHREAD_MUTEX_INITIALIZER;
    mutexCommunication = PTHREAD_MUTEX_INITIALIZER;

    pthread_cond_init(&condNotificationEmpty, NULL);
    pthread_cond_init(&condNotificationFull, NULL);
    pthread_mutex_init(&mutexNotificationSender, NULL);
}

void *Server::communicationHandler(void *handlerArgs)
{
    struct communiction_handler_args *args = (struct communiction_handler_args *)handlerArgs;

    string userToFollow;
    string message;
    string response;
    string user;
    int type;
    string payload;

    cout << "starting to listen to messages" << endl;

    while (1)
    {
        sockaddr clientAddress;
        Packet *receivedPacket = args->connectedSocket->readPacket(&clientAddress);

        if (receivedPacket)
        {
            user = receivedPacket->getUser();
            type = receivedPacket->getType();
            payload = receivedPacket->getPayload();

            args->server->login(user);

            switch (type)
            {
            case USER_CONNECT:
                if (args->server->database.userConnect(user, clientAddress) != 0)
                {
                    // success
                    args->connectedSocket->sendPacket(Packet("server", OPEN_SESSION_SUCCESS, "connection successful"), &clientAddress);
                }
                else
                {
                    // failed too many sessions for the user
                    args->connectedSocket->sendPacket(Packet("server", OPEN_SESSION_FAIL, "connection failed"), &clientAddress);
                }
                break;

            case USER_CLOSE_CONNECTION:
                if (args->server->database.userCloseConnection(user, clientAddress) != 0)
                {
                    // success
                    args->connectedSocket->sendPacket(Packet("server", CLOSE_SESSION_SUCCESS, "close connection successful"), &clientAddress);
                }
                break;

            case FOLLOW_USER:

                args->server->database.saveNewFollow(user, payload);
                cout << "following new user" << endl;

                break;

            case SEND_NOTIFICATION:
                args->server->manageNotifications(args->connectedSocket, user, Notification::fromString(payload));

                break;

            default:
                cout << "Invalid operation" << endl;
                break;
            }
        }
    }
    return NULL;
}

void Server::login(string user)
{
    if (!database.userExists(user))
    {
        database.saveUser(user);
    }
}

void Server::manageNotifications(ServerSocket* socket, string user, Notification notification)
{
    this->database.saveNotification(user, notification);
    list<string> followers = this->database.getFollowersByUserId(user);
    if (!followers.empty())
    {
        cout << "followers are not empty" << endl;
        for (string follower : followers)
        {
            cout << "looking at follower: " << follower << endl;
            for (sockaddr clientAddress : this->database.getClientAddressByUserId(follower))
            {
                socket->sendPacket(Packet("server", RECEIVE_NOTIFICATION, notification.toString()), &clientAddress);
            }
        }
    }
}
