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
    this->targetId.assign("unasigned");
    this->message.assign(input_message);
    this->pending = false;

    auto time = chrono::system_clock::now();
    const time_t input_time = chrono::system_clock::to_time_t(time);

    const char *s = ctime(&input_time);
    string str(s);
    str.pop_back();
    this->timestamp.assign(str);
}

uint32_t Notification::getId()
{
    return this->id;
}

string Notification::getTargetId()
{
    return this->targetId;
}

bool Notification::getPending()
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

void Notification::setTargetId(string targetId)
{
    this->targetId = targetId;
}

void Notification::setTimestamp(string timestamp)
{
    this->timestamp = timestamp;
}

void Notification::setPending(bool pending)
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
    string str_pending = to_string(this->getPending());

    return str_id + "$" + this->getSenderId() + "$" + targetId + "$" + str_pending + "$" + this->getTimestamp() + "$" + this->getMessage() + "$";
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

    string input_targetId;
    string input_senderId;
    uint16_t input_pending;
    string input_message;
    string input_timestamp;

    input_targetId.assign(results[1]);
    input_senderId.assign(results[2]);
    input_pending = strtoul(results[3].c_str(), NULL, 10);
    input_timestamp.assign(results[4]);
    input_message.assign(results[5]);

    Notification notification = Notification(input_senderId, input_message);
    notification.setTimestamp(input_timestamp);
    notification.setPending(input_pending);
    notification.setTargetId(input_targetId);

    return notification;
}