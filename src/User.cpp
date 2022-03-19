#include "../libraries/User.hpp"

using namespace std;

User::User()
{
}

User::User(string const input_userId, string const input_userName)
{

    uint32_t userIdMaxSize = input_userId.length();
    if (userIdMaxSize > MAX_USERID_SIZE)
    {
        std::cout << "ERROR Occured !! The userId has exceeded his maximum size of 20 characters\n"
                  << input_userId << std::endl;
        exit(1);
    }

    uint32_t userIdMinSize = input_userId.length();
    if (userIdMinSize > MIN_USERID_SIZE)
    {
        std::cout << "ERROR Occured !! The userId has less characters than the minimum size of 4\n"
                  << input_userId << std::endl;
        exit(1);
    }


    this->userId.assign(input_userId);
    this->userName.assign(input_userName);
}

string User::getUserId()
{
    return this->userId;
}

string User::getUserName()
{
    return this->userName;
}

void User::setUserId(string const input_userId)
{

    uint32_t userIdMaxSize = input_userId.length();
    if (userIdMaxSize > MAX_USERID_SIZE)
    {
        std::cout << "ERROR Occured !! The userId has exceeded his maximum size of 20 characters\n"
                  << input_userId << std::endl;
        exit(1);
    }

    uint32_t userIdMinSize = input_userId.length();
    if (userIdMinSize > MIN_USERID_SIZE)
    {
        std::cout << "ERROR Occured !! The userId has less characters than the minimum size of 4\n"
                  << input_userId << std::endl;
        exit(1);
    }

    this->userId.assign(input_userId);
}

void User::setUserName(string const input_userName)
{
    this->userName.assign(input_userName);
}

string User::toString()
{
    string str_userId = this->getUserId();
    string str_userName = this->getUserName();

    return str_userId + ";" + str_userName;
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

User User::fromString(string stringObject)
{
    vector<string> results = split(stringObject, ';');

    string input_userId;
    string input_userName;

    input_userId.assign(results[0].c_str());
    input_userName.assign(results[1].c_str());

    return User(input_userId, input_userName);
}

