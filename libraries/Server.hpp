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

    bool isServerRunning();
    void closeServer();
    static void* communicationHandler(void *handlerArgs);
    static void* commandHandler(void *handlerArgs);
    static void* packetHandler(void *handlerArgs);
    
private: 
    bool isRunning;
    pthread_mutex_t criticalSectionMutex;

    uint32_t notificationIdCounter;

    map<string, sem_t> userSessionsSemaphore;
    map< string, list< host_address > > sessions;
    Database database;

    void manageNotifications(ServerSocket* socket, string sender, Notification notification);
    void sendInitialNotifications(ServerSocket* socket, string sender, sockaddr* senderAddress);
    bool isUserActive(string user);


};


struct communiction_handler_args {
	ServerSocket* connectedSocket;
	string user;
    Server* server;
    Packet* packet;
    sockaddr clientAddress;
};