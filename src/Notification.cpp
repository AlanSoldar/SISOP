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

    //remove end of file markers from the input payload.
    input_message.erase(std::remove(input_message.begin(), input_message.end(), '\0'), input_message.end());
    input_message.erase(std::remove(input_message.begin(), input_message.end(), '\n'), input_message.end());

    uuid_t uuid;
    uuid_generate(uuid);

    this->id = *(uint32_t *)&uuid;
    this->timestamp = time(NULL);
    this->senderId.assign(senderId);
    this->message.assign(input_message);
}

Notification::Notification(uint32_t id, time_t timestamp, uint16_t length, uint16_t pending, string const senderId, string input_message)
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

    //remove end of file markers from the input payload.
    input_message.erase(std::remove(input_message.begin(), input_message.end(), '\0'), input_message.end());
    input_message.erase(std::remove(input_message.begin(), input_message.end(), '\n'), input_message.end());

    this->id = id;
    this->timestamp = timestamp;
    this->length = length;
    this->pending = pending;
    this->senderId.assign(senderId);
    this->message.assign(input_message);
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

time_t Notification::getTimestamp()
{
    return this->timestamp;
}

string Notification::getMessage()
{
    return this->message;
}

void Notification::setTimestamp(time_t timestamp)
{
    this->timestamp = timestamp;
}

void Notification::setPending(uint16_t pending)
{
    this->pending = pending;
}

void Notification::setMessage(string const message)
{

    uint16_t messageSize = message.length();
    if (messageSize > NOTIFICATION_MAX_SIZE)
    {
        std::cout << "ERROR Occured !! The message has exceeded his maximum size\n"
                  << message << std::endl;
        exit(1);
    }

    this->message.assign(message);
    this->length = messageSize;
}

void Notification::setSenderId(string const senderId)
{

    uint16_t senderIdSize = senderId.length();
    if (senderIdSize > SENDER_ID_SIZE)
    {
        std::cout << "ERROR Occured !! The sender id has exceeded his maximum size\n"
                  << senderId << std::endl;
        exit(1);
    }

    this->senderId.assign(senderId);
}

string Notification::toString()
{
    string str_id = to_string(this->getId());
    string str_length = to_string(this->getLength());
    string str_pending = to_string(this->getPending());
    time_t time_input = this->getTimestamp();
    time(&time_input);
    stringstream ss;
    ss << time_input;
    string str_timestamp = ss.str();

    return str_id + ";" + str_timestamp + ";" + str_pending + ";" + this->getSenderId() + ";" + this->getMessage();
}

vector<string> splitNotification(string stringObject, char delimiter)
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

Notification Notification::fromString(string stringObject)
{
    vector<string> results = splitNotification(stringObject, ';');

    uint32_t input_id;
    time_t input_timestamp;
    uint16_t input_pending;
    string input_senderId;
    string input_message;

    //converting numbers from string to correct type.
    input_id = strtoul(results[0].c_str(), NULL, 10);

    //converting timestamp from string to correct type.
    int yy, month, dd, hh, mm, ss;
    struct tm whenStart;
    char const *zStart = results[1].c_str();
    sscanf(zStart, "%d/%d/%d %d:%d:%d", &yy, &month, &dd, &hh, &mm, &ss);
    whenStart.tm_year = yy - 1900;
    whenStart.tm_mon = month - 1;
    whenStart.tm_mday = dd;
    whenStart.tm_hour = hh;
    whenStart.tm_min = mm;
    whenStart.tm_sec = ss;
    whenStart.tm_isdst = -1;

    input_pending = strtoul(results[2].c_str(), NULL, 10);
    input_timestamp = mktime(&whenStart);
    input_senderId.assign(results[3].c_str());
    cout << "tst" << endl;
    input_message.assign(results[4].c_str());
    cout << "tst2" << endl;

    return Notification(input_id, input_timestamp, sizeof(input_message), input_pending, input_senderId, input_message);
}
