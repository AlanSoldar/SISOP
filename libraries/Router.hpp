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
    Router(host_address addr);
    string ip;
    int port;
    list<ClientSocket> sockets;

    static void *RouteCommunication(void *handlerArgs);
    bool isRouterRunning();

private:
    bool isRunning;
    pthread_mutex_t criticalSectionMutex;

    map<string, sem_t> userSessionsSemaphore;
    ServerSocket serverSocket;

    void wakeUpServer();
    void sendServerSleepCommand();
    list<ClientSocket> getActiveSockets();
    void processPacket(Packet *pkt, sockaddr_in clientAddress, ServerSocket *serverSocket);
    static void *packetHandler(void *handlerArgs);
};

struct router_handler_args
{
    ServerSocket *connectedSocket;
    string user;
    Router *router;
    Packet *packet;
    sockaddr_in serverAddress;
};