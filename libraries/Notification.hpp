#ifndef NOTIFICATION_HEADER
#define NOTIFICATION_HEADER

#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <uuid/uuid.h>
#include <sstream>
#include "Defines.hpp"

using namespace std;

class Notification
{

private:
    uint32_t id;                         // Notification identifier
    time_t timestamp;                    // Notification timestamp
    uint16_t length;                     // Notification LENGHT
    uint16_t pending;                    // Number of users pending to receive the notification
    char senderId[SENDER_ID_SIZE];       // Sender Id
    char message[NOTIFICATION_MAX_SIZE]; // Content of the notification message

public:
    Notification();
    Notification(char const *senderId, char const *message);
    Notification(uint32_t id, time_t timestamp, uint16_t length, uint16_t pending, char const *senderId, char const *message);

    uint32_t getId();
    time_t getTimestamp();
    uint16_t getLength();
    uint16_t getPending();
    char *getSenderId();
    char *getMessage();

    void setTimestamp(time_t timestamp);
    void setSenderId(char *senderId);
    void setMessage(char *message);
    void setPending(uint16_t pending);
    string toString();
    Notification fromString(string stringObject);
};

#endif