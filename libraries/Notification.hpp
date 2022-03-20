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
#include <chrono>
#include <ctime>
#include "Defines.hpp"

using namespace std;

class Notification
{

private:
    uint32_t id;      // Notification identifier
    string timestamp; // Notification timestamp
    uint16_t length;  // Notification LENGHT
    uint16_t pending; // Number of users pending to receive the notification
    string senderId;  // Sender Id
    string message;   // Content of the notification message

public:
    Notification();
    Notification(string senderId, string message);
    Notification(uint32_t id, string timestamp, uint16_t length, uint16_t pending, string senderId, string message);

    uint32_t getId();
    string getTimestamp();
    uint16_t getLength();
    uint16_t getPending();
    string getSenderId();
    string getMessage();

    void setTimestamp(string timestamp);
    void setSenderId(string senderId);
    void setMessage(string message);
    void setPending(uint16_t pending);
    string toString();
    static Notification fromString(string stringObject);
};

#endif