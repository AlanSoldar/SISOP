#pragma once
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <list>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include "Socket.hpp"
#include "Notification.hpp"
#include "Server.hpp"
#include "Client.hpp"
using namespace std;

class Router
{
public:
    Router();

    static void *ClientServerCommunication(void *handlerArgs);
    static void *ServerClientCommunication(void *handlerArgs);
    bool isRouterRunning();

private:
    bool isRunning;
    ServerSocket clientRouterSocket;
    ServerSocket serverRouterSocket;
    list<ClientSocket> serverList;

    void wakeUpServer();
    void sendServerSleepCommand();
    list<ClientSocket> getActiveSockets();
    void processClientPacket(Packet *pkt, sockaddr_in serverAddress, ServerSocket *serverSocket);
    void processServerPacket(Packet *pkt, sockaddr_in serverAddress, ServerSocket *serverSocket);
    static void *clinetPacketHandler(void *handlerArgs);
    static void *serverPacketHandler(void *handlerArgs);
};

struct router_handler_args
{
    string user;
    Router *router;
    Packet *packet;
    sockaddr_in clientAddress;
    sockaddr_in serverAddress;
};