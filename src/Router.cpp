#include "../libraries/Router.hpp"

using namespace std;

Router::Router()
{
    this->serverSocket = ServerSocket();
    this->sockets = getActiveSockets();
    this->isRunning = true;

    pthread_mutex_init(&criticalSectionMutex, NULL);
}

Router::Router(host_address address)
{
    this->ip = address.ipv4;
    this->port = address.port;
    this->isRunning = true;
    this->serverSocket = ServerSocket();
    this->sockets = getActiveSockets();

    pthread_mutex_init(&criticalSectionMutex, NULL);
}

list<ClientSocket> Router::getActiveSockets()
{
    list<ClientSocket> sockets;

    // populate the list of available sockets accordingly to the maximum server instances available
    for (int i = 1; i <= MAX_SERVER_INSTANCES; i++)
    {
        sockets.push_back(ClientSocket(SERVER_ADDRESS, PORT + i));
    }

    return sockets;
}

bool Router::isRouterRunning()
{
    return this->isRunning;
}

void *Router::RouteCommunication(void *handlerArgs)
{

    struct router_handler_args *args = (struct router_handler_args *)handlerArgs;
    pthread_t t0, t1, t2, t3;
    pthread_t currentThread;
    int threadController = 0;

    cout << "starting to listen to messages" << endl;

    sockaddr_in clientAddress;

    while (args->router->isRouterRunning())
    {
        Packet *receivedPacket = args->connectedSocket->readPacket(&clientAddress);
        
        if (receivedPacket)
        {
            args->packet = receivedPacket;
            args->serverAddress = clientAddress;

            switch (threadController)
            {
            case 0:
                pthread_join(t0, NULL);
                pthread_create(&t0, NULL, Router::packetHandler, (void *)args);
                break;
            case 1:
                pthread_join(t1, NULL);
                pthread_create(&t1, NULL, Router::packetHandler, (void *)args);
                break;
            case 2:
                pthread_join(t2, NULL);
                pthread_create(&t2, NULL, Router::packetHandler, (void *)args);
                break;
            case 3:
                pthread_join(t3, NULL);
                pthread_create(&t3, NULL, Router::packetHandler, (void *)args);
                break;
            default:
                pthread_join(t0, NULL);
                pthread_create(&t0, NULL, Router::packetHandler, (void *)args);
                break;
            }
            threadController++;
            if (threadController > 3)
            {
                threadController = 0;
            }
        }
    }

    return NULL;
}

void *Router::packetHandler(void *handlerArgs)
{
    struct router_handler_args *args = (struct router_handler_args *)handlerArgs;
    Packet *receivedPacket = args->packet;
    sockaddr_in serverAddress = args->router->sockets.front().serv_addr;
    ServerSocket *serverSocket = args->connectedSocket;

    pthread_mutex_lock(&(args->router->criticalSectionMutex));
    args->router->processPacket(receivedPacket, serverAddress, serverSocket);
    pthread_mutex_unlock(&(args->router->criticalSectionMutex));

    return NULL;
}

void Router::processPacket(Packet *pkt, sockaddr_in serverAddress, ServerSocket *serverSocket)
{
    serverSocket->sendPacket(Packet(pkt->getUser(), pkt->getType(), pkt->getPayload()), &serverAddress);
}

// void Router::wakeUpServer()
// {
//     int answer = this->socket.sendPacket(Packet(this->userName, WAKE_UP, to_string(this->socket.getSocketfd())));
//     if (answer < 0)
//     {
//         exit(1);
//     }

//     cout << "setting server as primary" << endl;
// }

// void Router::sendServerSleepCommand()
// {
//     int answer = this->socket.sendPacket(Packet(this->userName, SLEEP, to_string(this->socket.getSocketfd())));
//     if (answer < 0)
//     {
//         exit(1);
//     }

//     cout << "setting server as backup" << endl;
// }