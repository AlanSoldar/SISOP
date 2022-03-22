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
        Packet *receivedPacket = args->connectedSocket->readPacket();

        if (receivedPacket)
        {
            user = receivedPacket->getUser();
            type = receivedPacket->getType();
            payload = receivedPacket->getPayload();

            args->server->login(user);


            switch (type)
            {

            case FOLLOW_USER:

                args->server->database.saveNewFollow(user, payload);
                cout << "following new user" << endl;

                break;

            case SEND_NOTIFICATION:
                cout << "new notification" << endl;
                cout << payload << endl;
                args->server->database.saveNotification(user, payload);

                break;

            case RECEIVE_NOTIFICATION:
                cout << "sending notifications to: " << receivedPacket->getUser() << endl;
                args->connectedSocket->sendPacket(Packet("server", SEND_NOTIFICATION, "you have 10 notifications"));
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
