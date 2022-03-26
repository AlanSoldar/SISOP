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
#include <iomanip>
#include <chrono>
#include <ctime>
#include "Defines.hpp"

using namespace std;

class Notification
{

private:
    uint32_t id;      // Notification identifier
    string targetId;  // Target id
    string senderId;  // Sender Id
    string timestamp; // Notification timestamp
    bool pending;     // was message seen by target
    string message;   // Content of the notification message

public:
    Notification();
    Notification(string senderId, string message);

    uint32_t getId();
    string getTargetId();
    string getSenderId();
    string getTimestamp();
    bool getPending();
    string getMessage();

    void setTimestamp(string timestamp);
    void setSenderId(string senderId);
    void setTargetId(string targetId);
    void setMessage(string message);
    void setPending(bool pending);
    string toString();
    static Notification fromString(string stringObject);
    static void printNotification(Notification notification);
};

#endif