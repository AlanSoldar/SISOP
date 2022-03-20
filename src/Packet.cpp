#include "../libraries/Packet.hpp"

using namespace std;

Packet::Packet()
{
}

Packet::Packet(uint16_t type, string input_payload)
{

    uint16_t payloadSize = input_payload.length();
    if (payloadSize > PAYLOAD_MAX_SIZE)
    {
        std::cout << "ERROR Occured !! The payload has exceeded his maximum size\n"
                  << payload << std::endl;
        exit(1);
    }

    this->type = type;
    this->seqn = 0;
    this->length = payloadSize;
    this->timestamp = time(NULL);

    input_payload.erase(std::remove(input_payload.begin(), input_payload.end(), '\0'), input_payload.end());
    input_payload.erase(std::remove(input_payload.begin(), input_payload.end(), '\n'), input_payload.end());

    this->payload = input_payload;
}

Packet::Packet(uint16_t type, time_t timestamp, string input_payload)
{

    uint16_t payloadSize = payload.length();
    if (payloadSize > PAYLOAD_MAX_SIZE)
    {
        std::cout << "ERROR Occured !! The payload has exceeded his maximum size\n"
                  << payload << std::endl;
        exit(1);
    }

    this->type = type;
    this->seqn = 0;
    this->length = payloadSize;
    this->timestamp = timestamp;

    input_payload.erase(std::remove(input_payload.begin(), input_payload.end(), '\0'), input_payload.end());
    input_payload.erase(std::remove(input_payload.begin(), input_payload.end(), '\n'), input_payload.end());
    this->payload = input_payload;
}

uint16_t Packet::getType()
{
    return this->type;
}

uint16_t Packet::getSeqn()
{
    return this->seqn;
}

uint16_t Packet::getLength()
{
    return this->length;
}

time_t Packet::getTimestamp()
{
    return this->timestamp;
}

string Packet::getPayload()
{
    return this->payload;
}

void Packet::setType(uint16_t type)
{
    this->type = type;
}

void Packet::setSeqn(uint16_t seqn)
{
    this->seqn = seqn;
}

void Packet::setTimestamp(time_t timestamp)
{
    this->timestamp = timestamp;
}

void Packet::setPayload(string payload)
{

    uint16_t payloadSize = payload.length();
    if (payloadSize > PAYLOAD_MAX_SIZE)
    {
        std::cout << "ERROR Occured !! The payload has exceeded his maximum size\n"
                  << payload << std::endl;
        exit(1);
    }

    this->payload, payload;
    this->length = payloadSize;
}