#include "../libraries/Server.hpp"

using namespace std;

pthread_mutex_t sendNotificationMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t followMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t receiveNotificationMutex = PTHREAD_MUTEX_INITIALIZER;

Server::Server()
{
    this->database = Database();
}

Server::Server(host_address address)
{
    this->database = Database();
    this->ip = address.ipv4;
    this->port = address.port;
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
    pthread_t thread1, thread2, thread3;
    int rc1, rc2, rc3;

    cout << "starting to listen to messages" << endl;

    while (1)
    {
        Packet *receivedPacket = args->connectedSocket->readPacket();

        if (receivedPacket)
        {
            func_args *argsParameters = (func_args *) calloc(1, sizeof(func_args));
            argsParameters->socket = args->connectedSocket;
            argsParameters->db = args->server->database;
            argsParameters->payload = receivedPacket->getPayload();
            argsParameters->user = receivedPacket->getUser();
            type = receivedPacket->getType();

            args->server->login(user);

            switch (type)
            {

            case FOLLOW_USER:
                if (rc1 = pthread_create(&thread1, NULL, Server::followOperation, (void *)argsParameters))
                {
                    cout << "Thread Creation Failed: " << rc1 << endl;
                }
                break;

            case SEND_NOTIFICATION:
                if (rc2 = pthread_create(&thread1, NULL, Server::sendNotificationOperation, (void *)argsParameters))
                {
                    cout << "Thread Creation Failed: " << rc2 << endl;
                }
                break;

            case RECEIVE_NOTIFICATION:
                if (rc3 = pthread_create(&thread1, NULL, Server::receiveNotificationOperation, (void *)argsParameters))
                {
                    cout << "Thread Creation Failed: " << rc3 << endl;
                }
                break;

            default:
                cout << "Invalid operation" << endl;
                break;
            }

            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            pthread_join(thread3, NULL);
        }
    }
    return NULL;
}

void* Server::followOperation(void *handlerArgs) //Database db, string user, string payload
{
    struct func_args* args = (struct func_args *)handlerArgs;
    Database db = args->db;
    string user = args->user;
    string payload = args->payload;

    pthread_mutex_lock(&followMutex);
    db.saveNewFollow(user, payload);
    cout << "following new user" << endl;
    pthread_mutex_unlock(&followMutex);
}

void* Server::receiveNotificationOperation(void *handlerArgs) //string user, ServerSocket *socket
{
    struct func_args* args = (struct func_args *)handlerArgs;
    ServerSocket* socket = args->socket;
    string user = args->user;

    pthread_mutex_lock(&receiveNotificationMutex);
    cout << "sending notifications to: " << user << endl;
    socket->sendPacket(Packet("server", SEND_NOTIFICATION, "you have 10 notifications"));
    pthread_mutex_unlock(&receiveNotificationMutex);
}

void* Server::sendNotificationOperation(void *handlerArgs) //Database db, string user, string payload
{
    struct func_args* args = (struct func_args *)handlerArgs;
    Database db = args->db;
    string user = args->user;
    string payload = args->payload;

    pthread_mutex_lock(&sendNotificationMutex);
    cout << "new notification" << endl;
    cout << payload << endl;
    db.saveNotification(user, payload);
    pthread_mutex_unlock(&sendNotificationMutex);
}

void Server::login(string user)
{
    if (!database.userExists(user))
    {
        database.saveUser(user);
    }
}
