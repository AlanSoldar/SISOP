#ifndef PACKET_HEADER
#define PACKET_HEADER

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <time.h>
#include <sstream>
#include <chrono>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Defines.hpp"


using namespace std;

class Packet
{

private:
    uint16_t type;    // Possible types of package declared at Defines.hpp
    string user;      // Logged user sending the packet
    uint16_t seqn;    // Sequence number
    uint16_t length;  // Payload length
    string timestamp; // Data timestamp
    string payload;   // Content included at the payload
    sockaddr_in socket;

public:
    Packet();
    Packet(string user, uint16_t type, string payload);

    uint16_t getType();
    string getUser();
    uint16_t getSeqn();
    uint16_t getLength();
    string getTimestamp();
    string getPayload();
    sockaddr_in getSocket();

    void setSocket(sockaddr_in socket);
    void setType(uint16_t type);
    void setUser(string user);
    void setSeqn(uint16_t seqn);
    void setTimestamp(string timestamp);
    void setPayload(string payload);
    string toString();
    static Packet* fromString(string stringObject);
};
#endif