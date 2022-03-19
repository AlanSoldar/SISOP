#ifndef USER_HEADER
#define USER_HEADER

#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sstream>
#include "Defines.hpp"

using namespace std;

class User
{

private:
    string userId;   // User identifier (@ of the user)
    string userName; // Name of the user (anything that the users wants to use)

public:
    User();
    User(string const userId, string const userName);

    string getUserId();
    string getUserName();

    void setUserId(string senderId);
    void setUserName(string message);
    string toString();
    static User fromString(string stringObject);
};

#endif