#include "../libraries/Notification.hpp"
#include <uuid/uuid.h>
#include <iostream>

Notification::Notification(){
    
}

Notification::Notification(char const *senderId, char const *message){

    uint32_t messageSize = strlen(message);
    if (messageSize > NOTIFICATION_MAX_SIZE) {
        std::cout << "ERROR Occured !! The message has exceeded his maximum size\n" << message << std::endl;
        exit(1);
    }

    uuid_t uuid;
    uuid_generate(uuid);

    this->id = *(uint32_t *)&uuid;
    this->timestamp = time(NULL);
    strcpy(this->senderId, senderId);
    strcpy(this->message, message);
}

uint32_t Notification::getId(){
    return this->id;
}

uint16_t Notification::getLength(){
    return this->length;
}

uint16_t Notification::getPending(){
    return this->pending;
}

char* Notification::getSenderId(){
    return this->senderId;
}

time_t Notification::getTimestamp(){
    return this->timestamp;
}

char* Notification::getMessage(){
    return this->message;
}

void Notification::setTimestamp(time_t timestamp){
    this->timestamp = timestamp;
}

void Notification::setPending(uint16_t pending){
    this->pending = pending;
}

void Notification::setMessage(char* const message){

    uint16_t messageSize = strlen(message);
    if (messageSize > NOTIFICATION_MAX_SIZE) {
        std::cout << "ERROR Occured !! The message has exceeded his maximum size\n" << message << std::endl;
        exit(1);
    } 

    strcpy(this->message, message);
    this->length = messageSize;
}

void Notification::setSenderId(char* const senderId){

    uint16_t senderIdSize = strlen(senderId);
    if (senderIdSize > SENDER_ID_SIZE) {
        std::cout << "ERROR Occured !! The sender id has exceeded his maximum size\n" << senderId << std::endl;
        exit(1);
    } 

    strcpy(this->senderId, senderId);
}