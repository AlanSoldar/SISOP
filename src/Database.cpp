#include "../libraries/Database.hpp"

using namespace std;

Database::Database()
{
    this->name = "Database";
    this->followers = {};
    this->notifications = {};
    this->loggedUserAddresses = {};

    loadFollows();
    loadNotifications();
}

void Database::closeDatabase()
{
    stashFollows();
    stashNotifications();
}

list<pair<string, struct sockaddr>> Database::getLoggedUsers()
{
    return this->loggedUserAddresses;
}

int Database::getUserSessionCount(string userId)
{
    list<pair<string, struct sockaddr>> loggedUsers = getLoggedUsers();
    int count = 0;
    for (pair<string, struct sockaddr> user : loggedUsers)
    {
        if (user.first == userId)
        {
            count++;
        }
    }
    return count;
}

void Database::addUserSession(string id, struct sockaddr addr)
{
    pair<string, struct sockaddr> user;
    user.first = id;
    user.second = addr;
    this->loggedUserAddresses.push_back(user);
}

void Database::setNotificationAsSeen(Notification notification)
{
    if (this->notifications.find(notification.getTargetId()) != this->notifications.end())
    {
        list<Notification> newList;
        list<Notification> list = this->notifications.find(notification.getTargetId())->second;
        for (Notification n : list)
        {
            n.setPending(false);
            newList.push_front(n);
        }
        this->notifications[notification.getTargetId()] = newList;
    }
}

int Database::userConnect(string userId, struct sockaddr addr)
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

int Database::userCloseConnection(string userId, sockaddr addr)
{
    pair<string, sockaddr> p = pair<string, sockaddr>(userId, addr);

    for (list<pair<string, sockaddr>>::iterator it = this->loggedUserAddresses.begin(); it != this->loggedUserAddresses.end(); it++)
    {
        // TODO adicionar check de endereco no second
        if (it->first == userId)
        {
            this->loggedUserAddresses.erase(it);
            return 1;
        }
    }

    return 0;
}

list<string> Database::getFollowersByUserId(string id)
{
    if (this->followers.find(id) != this->followers.end())
    {
        return this->followers.find(id)->second;
    }
    return {};
}

list<sockaddr> Database::getClientAddressByUserId(string id)
{
    list<pair<string, sockaddr>> users = this->getLoggedUsers();
    list<sockaddr> addresses;
    cout << "searching logged users" << endl;
    for (pair<string, struct sockaddr> pair : users)
    {
        cout << "looking at: " << pair.first << endl;
        if (pair.first == id)
        {
            addresses.push_back(pair.second);
        }
    }
    return addresses;
}

list<Notification> Database::getNotificationsByUserId(string id)
{
    if (this->notifications.find(id) != this->notifications.end())
    {
        return this->notifications.find(id)->second;
    }
    return {};
}

void Database::saveNewFollow(string followerId, string userId)
{
    ofstream followerFile;
    followerFile.open("tables/Follower.txt", ios_base::app);
    followerFile << userId << " " << followerId << endl;
    followerFile.close();

    followers[userId].push_front(followerId);
    cout << followerId << " is now following: " << userId << endl;
}

void Database::saveNotification(Notification notification)
{
    ofstream notificationFile;
    notificationFile.open("tables/Notification.txt", ios_base::app);
    notificationFile << notification.toString() << endl;
    notificationFile.close();

    notifications[notification.getTargetId()].push_front(notification);
    cout << notification.getTargetId() << " has received a new notification: " << notification.getMessage() << endl;
}

void Database::loadFollows()
{
    followers = {};

    ifstream followFile;
    vector<string> follow;
    list<string> tst;
    list<string>::iterator it;
    followFile.open("tables/Follower.txt");
    string line;
    while (getline(followFile, line))
    {
        follow = split(line, " ");
        followers[follow[0]].push_back(follow[1]);
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
        // cout << "timestamp: " << notification.getTimestamp() << endl;
        // cout << "message: " << notification.getMessage() << endl;
    }
}

void Database::stashFollows()
{
    ofstream followerFile;
    followerFile.open("tables/Follower.txt");

    for (pair<string, list<string>> tuple : followers)
    {
        string userId = tuple.first;
        for (string followerId : tuple.second)
        {
            followerFile << userId << " " << followerId << endl;
        }
    }
    followerFile.close();
}

void Database::stashNotifications()
{
    ofstream notificationFile;
    notificationFile.open("tables/Notification.txt");

    for (pair<string, list<Notification>> tuple : notifications)
    {
        string userId = tuple.first;
        for (Notification notification : tuple.second)
        {
            notificationFile << notification.toString() << endl;
        }
    }
    notificationFile.close();
}

vector<string> Database::split(string s, string delimiter)
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