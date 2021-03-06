#include "../libraries/Server.hpp"

using namespace std;

Server::Server()
{
    this->isRunning = true;
    this->isPrimary = false;
    this->database = Database();
    this->notificationIdCounter = 0;
    this->routerAddress;

    routerAddress.sin_family = PF_INET;
    routerAddress.sin_port = htons(ROUTER_INITIAL_PORT + 1);
    routerAddress.sin_addr.s_addr = INADDR_ANY;
    bzero(&(routerAddress.sin_zero), 8);

    pthread_mutex_init(&criticalSectionMutex, NULL);
}

bool Server::isServerRunning()
{
    return this->isRunning;
}

void Server::closeServer()
{
    this->isRunning = false;
}

void *Server::terminalCommandHandler(void *handlerArgs)
{
    struct communiction_handler_args *args = (struct communiction_handler_args *)handlerArgs;

    while (args->server->isServerRunning())
    {
        string command = "";
        cin >> command;

        if (command == "CLOSE" || command == "EXIT" || command == "close" || command == "exit")
        {
            cout << "closing connection with server" << endl;
            args->server->database.saveDatabaseState();
            args->server->closeServer();
        }
        else
        {
            cout << "This is not a valid command please use <EXIT> || <CLOSE>" << endl;
        }
    }
    return NULL;
}

void *Server::communicationHandler(void *handlerArgs)
{
    struct communiction_handler_args *args = (struct communiction_handler_args *)handlerArgs;
    pthread_t t0, t1, t2, t3;
    int threadController = 0;

    cout << "starting to listen to messages" << endl;

    while (args->server->isServerRunning())
    {
        sockaddr_in clientAddress;
        Packet *receivedPacket = args->connectedSocket->readPacket(&clientAddress);

        args->packet = receivedPacket;
        args->clientAddress = clientAddress;

        switch (threadController)
        {
        case 0:
            pthread_join(t0, NULL);
            pthread_create(&t0, NULL, Server::packetHandler, (void *)args);
            break;
        case 1:
            pthread_join(t1, NULL);
            pthread_create(&t1, NULL, Server::packetHandler, (void *)args);
            break;
        case 2:
            pthread_join(t2, NULL);
            pthread_create(&t2, NULL, Server::packetHandler, (void *)args);
            break;
        case 3:
            pthread_join(t3, NULL);
            pthread_create(&t3, NULL, Server::packetHandler, (void *)args);
            break;
        default:
            pthread_join(t0, NULL);
            pthread_create(&t0, NULL, Server::packetHandler, (void *)args);
            break;
        }
        threadController++;
        if (threadController > 3)
        {
            threadController = 0;
        }
    }

    return NULL;
}

void *Server::packetHandler(void *handlerArgs)
{
    
    struct communiction_handler_args *args = (struct communiction_handler_args *)handlerArgs;
    
    Packet *receivedPacket = args->packet;
    
    ServerSocket *serverSocket = args->connectedSocket;

    string user = receivedPacket->getUser();
    int type = receivedPacket->getType();
    string payload = receivedPacket->getPayload();
    sockaddr_in clientAddress = receivedPacket->getSocket();

    if (args->server->isPrimary)
    {
        // locking critical section of the code
        pthread_mutex_lock(&(args->server->criticalSectionMutex));
        args->server->processPacket(user, type, payload, clientAddress, serverSocket);
        pthread_mutex_unlock(&(args->server->criticalSectionMutex));

        args->server->database.saveDatabaseState();
    }
    else if (type == WAKE_UP && receivedPacket->getSocket().sin_port == serverSocket->getServAddr().sin_port)
    {
        args->server->database.loadDatabaseState();
        args->server->isPrimary = true;
        cout << "Server is now active as primary." << endl;
    }

    return NULL;
}

void Server::processPacket(string user, int type, string payload, sockaddr_in clientAddress, ServerSocket *serverSocket)
{
    switch (type)
    {
    case USER_CONNECT:
        if (this->database.userConnect(user, clientAddress) != 0)
        {
            // success
            serverSocket->sendPacket(Packet("server", OPEN_SESSION_SUCCESS, "connection successful"), &this->routerAddress, clientAddress);
            this->sendInitialNotifications(serverSocket, user, clientAddress);
        }
        else
        {
            // failed too many sessions for the user
            serverSocket->sendPacket(Packet("server", OPEN_SESSION_FAIL, "connection failed"), &this->routerAddress, clientAddress);
        }
        break;

    case USER_CLOSE_CONNECTION:
        if (this->database.userCloseConnection(user, clientAddress) != 0)
        {
            // success
            serverSocket->sendPacket(Packet("server", CLOSE_SESSION_SUCCESS, "close connection successful"), &this->routerAddress, clientAddress);
        }
        break;

    case FOLLOW_USER:

        this->database.saveNewFollow(user, payload);

        break;

    case SEND_NOTIFICATION:
        this->manageNotifications(serverSocket, user, Notification::fromString(payload));

        break;

    case SLEEP:
        this->database.saveDatabaseState();
        this->isPrimary = false;
        cout << "Server is now active as backup." << endl;
        break;

    case FAIL:
        serverSocket->sendPacket(Packet("server", FAIL, "failing test"), &this->routerAddress, clientAddress);
        break;

    default:
        cout << "Invalid operation: " << type << endl;
        break;
    }

    // sleep(100);
}

void Server::manageNotifications(ServerSocket *socket, string user, Notification notification)
{
    list<string> followers = this->database.getFollowersByUserId(user);
    // if the user has followers
    if (!followers.empty())
    {
        // for each follower
        for (string follower : followers)
        {
            // create a notification for that follower
            notification.setTargetId(follower);
            this->database.saveNotification(notification);
            // then look if there is any follower logged in
            for (sockaddr_in clientAddress : this->database.getClientAddressByUserId(follower))
            {
                // sends notification immediately
                socket->sendPacket(Packet("server", RECEIVE_NOTIFICATION, notification.toString()), &this->routerAddress, clientAddress);
                this->database.setNotificationAsSeen(notification);
            }
        }
    }
}

void Server::sendInitialNotifications(ServerSocket *socket, string user, sockaddr_in userAddress)
{
    for (Notification notification : this->database.getNotificationsByUserId(user))
    {
        if (notification.getPending())
        {
            socket->sendPacket(Packet("server", RECEIVE_NOTIFICATION, notification.toString()), &this->routerAddress, userAddress);
            this->database.setNotificationAsSeen(notification);
        }
    }
}
