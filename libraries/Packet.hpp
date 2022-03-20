#ifndef PACKET_HEADER
#define PACKET_HEADER

#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <time.h>
#include <sstream>
#include "Defines.hpp"

using namespace std;

class Packet
{

private:
    uint16_t type;    // Possible types of package declared at Defines.hpp
    uint16_t seqn;    // Sequence number
    uint16_t length;  // Payload length
    time_t timestamp; // Data timestamp
    string payload;   // Content included at the payload

public:
    Packet();
    Packet(uint16_t type, string payload);
    Packet(uint16_t type, time_t timestamp, string payload);

    uint16_t getType();
    uint16_t getSeqn();
    uint16_t getLength();
    time_t getTimestamp();
    string getPayload();

    void setType(uint16_t type);
    void setSeqn(uint16_t seqn);
    void setTimestamp(time_t timestamp);
    void setPayload(string payload);
    string toString();
    static Packet fromString(string stringObject);
};
#endif