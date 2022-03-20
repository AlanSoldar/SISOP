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

    //remove extra empty space that was being putting at the first place of the input payload.
    input_payload.erase(0, 1);
    //remove end of file markers from the input payload.
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

    //remove extra empty space that was being putting at the first place of the input payload.
    input_payload.erase(0, 1);
    //remove end of file markers from the input payload.
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

string Packet::toString()
{
    string str_type = to_string(this->getType());

    return str_type + ";" + this->getPayload();
}

Packet Packet::fromString(string stringObject)
{
    vector<string> results = split(stringObject, ';');

    uint16_t input_type;
    string input_payload;

    //converting numbers from string to correct type.
    input_type = strtoul(results[0].c_str(), NULL, 10);

    //converting from string to correct type
    input_payload.assign(results[1].c_str());

    return Packet(input_type, input_payload);
}

vector<string> split(string stringObject, char delimiter)
{
    vector<string> brokedString;
    if ((stringObject.find(delimiter) == string::npos) && (stringObject.find_first_not_of(delimiter) == string::npos))
    {
        throw nullptr;
    }

    else if ((stringObject.find(delimiter) == string::npos))
        brokedString.push_back(stringObject);

    else if (stringObject.find_first_not_of(delimiter) == string::npos)
        brokedString.push_back(string(""));

    else
    {
        unsigned i = 0;
        string strstack;

        while (stringObject[0] == delimiter)
        {
            stringObject.erase(0, 1);
        }

        reverse(stringObject.begin(), stringObject.end());

        while (stringObject[0] == delimiter)
        {
            stringObject.erase(0, 1);
        }

        reverse(stringObject.begin(), stringObject.end());

        while (!stringObject.empty())
        {
            brokedString.push_back(stringObject.substr(i, stringObject.find(delimiter)));
            stringObject.erase(0, stringObject.find(delimiter));
            while (stringObject[0] == delimiter)
            {
                stringObject.erase(0, 1);
            }
        }
    }

    return brokedString;
}