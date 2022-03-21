#include "../libraries/Notification.hpp"

using namespace std;

Notification::Notification()
{
}

Notification::Notification(string const senderId, string input_message)
{

    uint32_t messageSize = input_message.length();
    if (messageSize > NOTIFICATION_MAX_SIZE)
    {
        std::cout << "ERROR Occured !! The message has exceeded his maximum size\n"
                  << input_message << std::endl;
        exit(1);
    }

    uint32_t senderIdSize = senderId.length();
    if (senderIdSize > SENDER_ID_SIZE)
    {
        std::cout << "ERROR Occured !! The sender id has exceeded his maximum size\n"
                  << senderId << std::endl;
        exit(1);
    }

    uuid_t uuid;
    uuid_generate(uuid);

    this->id = *(uint32_t *)&uuid;
    this->senderId.assign(senderId);
    this->message.assign(input_message);

    auto time = chrono::system_clock::now();
    const time_t input_time = chrono::system_clock::to_time_t(time);

    const char *s = ctime(&input_time);
    string str(s);
    this->timestamp.assign(str);
}

Notification::Notification(uint16_t length, uint16_t pending, string senderId, string input_message)
{
    uint32_t messageSize = input_message.length();
    if (messageSize > NOTIFICATION_MAX_SIZE)
    {
        std::cout << "ERROR Occured !! The message has exceeded his maximum size\n"
                  << input_message << std::endl;
        exit(1);
    }

    uint32_t senderIdSize = senderId.length();
    if (senderIdSize > SENDER_ID_SIZE)
    {
        std::cout << "ERROR Occured !! The sender id has exceeded his maximum size\n"
                  << senderId << std::endl;
        exit(1);
    }

    uuid_t uuid;
    uuid_generate(uuid);

    this->id = *(uint32_t *)&uuid;
    this->length = length;
    this->pending = pending;
    this->senderId.assign(senderId);
    this->message.assign(input_message);

    const chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
    time_t input_time = chrono::system_clock::to_time_t(now);

    const char *s = ctime(&input_time);
    string str(s);
    this->timestamp.assign(str);
}

uint32_t Notification::getId()
{
    return this->id;
}

uint16_t Notification::getLength()
{
    return this->length;
}

uint16_t Notification::getPending()
{
    return this->pending;
}

string Notification::getSenderId()
{
    return this->senderId;
}

string Notification::getTimestamp()
{
    return this->timestamp;
}

string Notification::getMessage()
{
    return this->message;
}

void Notification::setTimestamp(string timestamp)
{
    this->timestamp = timestamp;
}

void Notification::setPending(uint16_t pending)
{
    this->pending = pending;
}

void Notification::setMessage(string input_message)
{

    uint16_t messageSize = input_message.length();
    if (messageSize > NOTIFICATION_MAX_SIZE)
    {
        std::cout << "ERROR Occured !! The message has exceeded his maximum size\n"
                  << input_message << std::endl;
        exit(1);
    }

    this->message.assign(input_message);
    this->length = messageSize;
}

void Notification::setSenderId(string input_senderId)
{

    uint16_t senderIdSize = input_senderId.length();
    if (senderIdSize > SENDER_ID_SIZE)
    {
        std::cout << "ERROR Occured !! The sender id has exceeded his maximum size\n"
                  << input_senderId << std::endl;
        exit(1);
    }

    this->senderId.assign(input_senderId);
}

string Notification::toString()
{
    string str_id = to_string(this->getId());
    string str_length = to_string(this->getLength());
    string str_pending = to_string(this->getPending());

    return str_id + "$" + this->getTimestamp() + "$" + str_length + "$" + str_pending + "$" + this->getSenderId() + "$" + this->getMessage() + "$";
}

vector<string> splitNotification(string s, string delimiter)
{
    size_t pos = 0;
    string token;
    vector<string> brokedString;
    
    while ((pos = s.find(delimiter)) != string::npos)
    {
        token = s.substr(0, pos);
        brokedString.push_back(token);
        s.erase(0, pos + 1);
    }

    brokedString.push_back(s);

    return brokedString;
}

Notification Notification::fromString(string stringObject)
{
    vector<string> results = splitNotification(stringObject, "$");

    uint16_t input_length;
    uint16_t input_pending;
    string input_senderId;
    string input_message;

    input_length = strtoul(results[1].c_str(), NULL, 10);
    input_pending = strtoul(results[2].c_str(), NULL, 10);

    input_senderId.assign(results[3]);
    input_message.assign(results[4]);

    return Notification(input_length, input_pending, input_senderId, input_message);
}