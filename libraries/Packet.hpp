#ifndef PACKET_HEADER
#define PACKET_HEADER

#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
#include "Defines.hpp"


class Packet {

    private:
        uint16_t type;                       // Possible types of package declared at Defines.hpp
        uint16_t seqn;                       // Sequence number
        uint16_t length;                     // Payload length
        time_t timestamp;                    // Data timestamp
        char payload[PAYLOAD_MAX_SIZE];      // Content included at the payload

	public:
        Packet();
        Packet(uint16_t type, char const *payload);
        Packet(uint16_t type, time_t timestamp, char const *payload);   

		uint16_t getType();
		uint16_t getSeqn();
		uint16_t getLength();
		time_t getTimestamp();
        char* getPayload();
        
        void setType(uint16_t type);
        void setSeqn(uint16_t seqn);
        void setTimestamp(time_t timestamp);
        void setPayload(char* payload);
};
#endif