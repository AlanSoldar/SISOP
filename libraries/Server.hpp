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
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Socket.hpp"
#include "Database.hpp"
#include "Notification.hpp"
using namespace std;

class Server
{
public:
    Server();
    Server(host_address addr);
    string ip;
    int port;

    void login(string user);
    static void* communicationHandler(void *handlerArgs);
    static void processPacket(int type, struct communiction_handler_args *args, string user, string payload, Packet *receivedPacket);

private:
    Database database;

    bool userExists(string user);
    bool isUserActive(string user);
    static void* followOperation(void *handlerArgs);
    static void* receiveNotificationOperation(void *handlerArgs);
    static void* sendNotificationOperation(void *handlerArgs);
};

struct communiction_handler_args
{
    ServerSocket *connectedSocket;
    host_address clientAddress;
    string user;
    Server *server;
};

struct func_args
{
    Database db;
    string payload;
    string user;
    ServerSocket *socket;
};