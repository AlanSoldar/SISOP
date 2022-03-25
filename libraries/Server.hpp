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

    void login(string user);
    static void *communicationHandler(void *handlerArgs);
    
private: 
    pthread_mutex_t mutexSession;
    pthread_mutex_t followMutex;
    pthread_mutex_t mutexCommunication;

    pthread_cond_t 	condNotificationEmpty, condNotificationFull;
    pthread_mutex_t mutexNotificationSender;

    uint32_t notificationIdCounter;

    map<string, sem_t> userSessionsSemaphore;
    map< string, list< host_address > > sessions;
    Database database;

    void manageNotifications(ServerSocket* socket, string sender, Notification notification);
    bool userExists(string user);
    bool isUserActive(string user);


};


struct communiction_handler_args {
	ServerSocket* connectedSocket;
	host_address clientAddress; 
	string user;
    Server* server;
};