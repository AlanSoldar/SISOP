#include "../libraries/Router.hpp"

using namespace std;

Router::Router()
{
    this->clientRouterSocket = ServerSocket(ROUTER_INITIAL_PORT);
    this->serverRouterSocket = ServerSocket(ROUTER_INITIAL_PORT + 1);
    this->serverList = getActiveSockets();
    this->isRunning = true;
    this->activeServerIndex = 0;
    wakeUpServer();
}

list<ClientSocket> Router::getActiveSockets()
{
    list<ClientSocket> sockets;

    // populate the list of available sockets accordingly to the maximum server instances available
    for (int i = 0; i < MAX_SERVER_INSTANCES; i++)
    {
        sockets.push_back(ClientSocket(SERVER_ADDRESS, SERVER_INITIAL_PORT + i));
    }

    return sockets;
}

bool Router::isRouterRunning()
{
    return this->isRunning;
}

void *Router::ClientServerCommunication(void *handlerArgs)
{

    struct router_handler_args *args = (struct router_handler_args *)handlerArgs;
    pthread_t t0, t1, t2, t3;
    pthread_t currentThread;
    int threadController = 0;
    Router *router = args->router;

    cout << "starting to listen to client messages" << endl;

    sockaddr_in clientAddress;

    while (router->isRouterRunning())
    {
        Packet *receivedPacket = router->clientRouterSocket.readPacket(&clientAddress);

        cout << "message received from client" << endl;

        if (receivedPacket)
        {
            receivedPacket->setSocket(clientAddress);
            args->packet = receivedPacket;

            switch (threadController)
            {
            case 0:
                pthread_join(t0, NULL);
                pthread_create(&t0, NULL, Router::clinetPacketHandler, (void *)args);
                break;
            case 1:
                pthread_join(t1, NULL);
                pthread_create(&t1, NULL, Router::clinetPacketHandler, (void *)args);
                break;
            case 2:
                pthread_join(t2, NULL);
                pthread_create(&t2, NULL, Router::clinetPacketHandler, (void *)args);
                break;
            case 3:
                pthread_join(t3, NULL);
                pthread_create(&t3, NULL, Router::clinetPacketHandler, (void *)args);
                break;
            default:
                pthread_join(t0, NULL);
                pthread_create(&t0, NULL, Router::clinetPacketHandler, (void *)args);
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

void *Router::clinetPacketHandler(void *handlerArgs)
{
    struct router_handler_args *args = (struct router_handler_args *)handlerArgs;
    Router *router = args->router;
    Packet *receivedPacket = args->packet;
    sockaddr_in serverAddress = router->getActiveServer();

    args->router->processClientPacket(receivedPacket, serverAddress, &router->clientRouterSocket);

    return NULL;
}

void Router::processClientPacket(Packet *packetPointer, sockaddr_in serverAddress, ServerSocket *routerSocket)
{
    Packet pkt = Packet(packetPointer->getUser(), packetPointer->getType(), packetPointer->getPayload());
    pkt.setSocket(packetPointer->getSocket());
    int attemptNum = 0;
    int response;
    do
    {
        response = routerSocket->sendPacket(pkt, &serverAddress);
        if (response < 0)
        {
            switchActiveServer();
        }
    } while (response < 0 && attemptNum < MAX_SERVER_INSTANCES);

    if (response < 0)
    {
        cout << "communication with server failed" << endl;
    }
}

void *Router::ServerClientCommunication(void *handlerArgs)
{
    struct router_handler_args *args = (struct router_handler_args *)handlerArgs;
    pthread_t t0, t1, t2, t3;
    pthread_t currentThread;
    int threadController = 0;
    Router *router = args->router;

    cout << "starting to listen to server messages" << endl;

    sockaddr_in serverAddress;

    while (router->isRouterRunning())
    {
        Packet *receivedPacket = router->serverRouterSocket.readPacket(&serverAddress);

        cout << "message received from server" << endl;

        if (receivedPacket)
        {
            args->packet = receivedPacket;
            args->serverAddress = serverAddress;

            switch (threadController)
            {
            case 0:
                pthread_join(t0, NULL);
                pthread_create(&t0, NULL, Router::serverPacketHandler, (void *)args);
                break;
            case 1:
                pthread_join(t1, NULL);
                pthread_create(&t1, NULL, Router::serverPacketHandler, (void *)args);
                break;
            case 2:
                pthread_join(t2, NULL);
                pthread_create(&t2, NULL, Router::serverPacketHandler, (void *)args);
                break;
            case 3:
                pthread_join(t3, NULL);
                pthread_create(&t3, NULL, Router::serverPacketHandler, (void *)args);
                break;
            default:
                pthread_join(t0, NULL);
                pthread_create(&t0, NULL, Router::serverPacketHandler, (void *)args);
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

void *Router::serverPacketHandler(void *handlerArgs)
{
    struct router_handler_args *args = (struct router_handler_args *)handlerArgs;
    Router *router = args->router;
    Packet *receivedPacket = args->packet;
    sockaddr_in serverAddress = router->getActiveServer();

    if (receivedPacket->getType() == FAIL)
    {
        router->switchActiveServer();
    }
    else
    {
        router->processServerPacket(receivedPacket, serverAddress, &router->serverRouterSocket);
    }

    return NULL;
}

void Router::processServerPacket(Packet *packetPointer, sockaddr_in serverAddress, ServerSocket *routerSocket)
{
    Packet pkt = Packet(packetPointer->getUser(), packetPointer->getType(), packetPointer->getPayload());
    pkt.setSocket(packetPointer->getSocket());
    sockaddr_in userAddress = pkt.getSocket();
    routerSocket->sendPacket(pkt, &userAddress);
}

void Router::switchActiveServer()
{
    sendServerSleepCommand();
    this->activeServerIndex++;
    if (activeServerIndex >= MAX_SERVER_INSTANCES)
        this->activeServerIndex = 0;
    wakeUpServer();
}

sockaddr_in Router::getActiveServer()
{
    list<ClientSocket>::iterator it = this->serverList.begin();
    advance(it, this->activeServerIndex);

    return it->serv_addr;
}

void Router::wakeUpServer()
{
    sockaddr_in serverAddress = getActiveServer();
    cout << "wake port " << serverAddress.sin_port << endl;
    Packet pkt = Packet("router", WAKE_UP, "time to wake up");
    pkt.setSocket(this->getActiveServer());
    int answer = this->clientRouterSocket.sendPacket(pkt, &serverAddress);
    cout << "setting server as primary" << endl;

    if (answer < 0)
    {
        exit(1);
    }
}

void Router::sendServerSleepCommand()
{
    sockaddr_in serverAddress = getActiveServer();
    int answer = this->clientRouterSocket.sendPacket(Packet("router", SLEEP, "time to sleep"), &serverAddress);
    cout << "setting server as backup" << endl;

    if (answer < 0)
    {
        exit(1);
    }
}
