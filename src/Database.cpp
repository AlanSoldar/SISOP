#include "../libraries/Database.hpp"

using namespace std;

Database::Database()
{
    this->name = "Database";
    this->users = {};
    this->followers = {};
    this->notifications = {};
    this->loggedUserAddresses = {};

    loadUsers();
    loadFollows();
    loadNotifications();
}

Database::Database(string name)
{
    this->name = name;
    this->users = {};
    this->followers = {};
    this->notifications = {};
    this->loggedUserAddresses = {};

    loadUsers();
    loadFollows();
    loadNotifications();
}

list<pair<string, struct sockaddr *>> Database::getLoggedUsers()
{
    return this->loggedUserAddresses;
}

int Database::getUserSessionCount(string userId)
{
    list<pair<string, struct sockaddr *>> loggedUsers = this->getLoggedUsers();
    int count = 0;
    for (pair<string, struct sockaddr *> user : loggedUsers)
    {
        if (user.first == userId)
        {
            count++;
        }
    }
    return count;
}

void Database::addUserSession(string id, struct sockaddr *addr)
{
    pair<string, struct sockaddr *> user;
    user.first = id;
    user.second = addr;
    this->loggedUserAddresses.push_back(user);
}

int Database::userConnect(string userId, struct sockaddr *addr)
{
    if (this->getUserSessionCount(userId) < 2)
    {
        this->addUserSession(userId, addr);
        return 1;
    }
    else
    {
        return 0;
    }
}

int Database::userCloseConnection(string userId, struct sockaddr *addr)
{
    pair<string, sockaddr *> p = pair<string, sockaddr *>(userId, addr);

    for (list<pair<string, struct sockaddr *>>::iterator it = this->loggedUserAddresses.begin(); it != this->loggedUserAddresses.end(); it++)
    {
        if (it->first == userId && it->second == addr)
        {
            this->loggedUserAddresses.erase(it);
            return 1;
        }
    }

    return 0;
}

string Database::getUserByid(string id)
{
    loadUsers();
    return this->users.find(id)->second;
}

bool Database::userExists(string userId)
{
    loadUsers();
    map<string, string>::iterator it = users.find(userId);
    return it != users.end();
}

list<string> Database::getFollowersByUserId(string id)
{
    loadFollows();
    return this->followers.find(id)->second;
}

list<struct sockaddr *> Database::getClientAddressByUserId(string id)
{
    list<pair<string, struct sockaddr *>> users = this->getLoggedUsers();
    list<struct sockaddr *> addresses;
    for (pair<string, struct sockaddr *> pair : users)
    {
        if (pair.first == id)
        {
            addresses.push_back(pair.second);
        }
    }
    return addresses;
}

list<string> Database::getNotificationsByUserId(string id)
{
    loadNotifications();
    return this->notifications.find(id)->second;
}

void Database::saveUser(string id)
{
    ofstream userFile;
    userFile.open("tables/User.txt", ios_base::app);
    userFile << id << endl;
    userFile.close();
    cout << id << " was created as a new user " << endl;
}

void Database::saveNewFollow(string followerId, string userId)
{
    ofstream followerFile;
    followerFile.open("tables/Follower.txt", ios_base::app);
    followerFile << followerId << " " << userId;
    followerFile.close();
    cout << followerId << " is now following: " << userId << endl;
}

void Database::saveNotification(string senderId, string payload)
{
    //Notification notification = Notification::fromString(payload);
    ofstream notificationFile;
    notificationFile.open("tables/Notification.txt", ios_base::app);
    notificationFile << payload << endl;
    notificationFile.close();
    cout << senderId << " has posted a new notification: " << payload << endl;
}

void Database::loadUsers()
{
    ifstream userFile;
    userFile.open("tables/User.txt");
    string line;
    // cout << "loading users:" << endl;
    while (getline(userFile, line))
    {
        users.insert(pair<string, string>(line, line));
    }
}

void Database::loadFollows()
{
    ifstream followFile;
    followFile.open("tables/Follower.txt");
    string line;
    // cout << "loading follows:" << endl;
    while (getline(followFile, line))
    {
        cout << line << endl;
    }
}

void Database::loadNotifications()
{
    ifstream notificationFile;
    notificationFile.open("tables/Notification.txt");
    string line;
    while (getline(notificationFile, line))
    {
        Notification notification = Notification::fromString(line);
        cout << "timestamp: " << notification.getTimestamp() << endl;
        cout << "message: " << notification.getMessage() << endl;
    }
}