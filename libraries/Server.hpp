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



typedef struct __notification {

    __notification();
    __notification(uint32_t new_id, string new_author, time_t new_timestamp, string new_body, uint16_t new_length, uint16_t new_pending) :
        id(new_id), author(new_author), timestamp(new_timestamp), body(new_body), length(new_length), pending(new_pending) {}

    uint32_t id; //Identificador da notificação (sugere-se um identificador único)
    string author; 
    time_t timestamp; //Timestamp da notificação
    string body; //Mensagem
    uint16_t length; //Tamanho da mensagem
    uint16_t pending; //Quantidade de leitores pendentes

    bool operator ==(__notification other) const {
		return id == other.id;
	}

} notification;




class Server
{
public:
    Server();
    Server(host_address addr);
    string ip; 
    int port;

    bool openSession(string user, host_address address);
    void closeSession(string user, host_address address);
    
    static void *communicationHandler(void *handlerArgs);
    
private: 
    pthread_mutex_t mutexSession;
    pthread_mutex_t followMutex;
    pthread_mutex_t mutexCommunication;

    pthread_cond_t 	condNotificationEmpty, condNotificationFull;
    pthread_mutex_t mutexNotificationSender;

    uint32_t notificationIdCounter;

    map<string, sem_t> userSessionsSemaphore;
    map< string, list< host_address > > sessions; // {user, [<ip, port>]}
    Database database;

    bool userExists(string user);
    bool isUserActive(string user);


};


struct communiction_handler_args {
	ServerSocket* connectedSocket;
	host_address clientAddress; 
	string user;
    Server* server;
};