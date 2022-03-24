#include "../libraries/Database.hpp"

using namespace std;

Database::Database()
{
    this->users = {};
    this->followers = {};
    this->notifications = {};
    this->loggedUserAddresses = {};

    loadFollows();
    loadUsers();
    loadNotifications();
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

sockaddr Database::getClientAddressByUserId(string id)
{
    return this->loggedUserAddresses.find(id)->second;
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
    followerFile << followerId << " " << userId << endl;
    followerFile.close();
    cout << followerId << " is now following: " << userId << endl;
}

void Database::saveNotification(string senderId, Notification notification)
{
    ofstream notificationFile;
    notificationFile.open("tables/Notification.txt", ios_base::app);
    notificationFile << notification.toString() << endl;
    notificationFile.close();
    cout << senderId << " has posted a new notification: " << notification.getMessage() << endl;
}

void Database::loadUsers()
{
    ifstream userFile;
    userFile.open("tables/User.txt");
    string line;
    //cout << "loading users:" << endl;
    while (getline(userFile, line))
    {
        users.insert(pair<string, string>(line, line));
    }
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
        cout << follow[0] << " : " << follow[1] << endl;
    }
}

void Database::loadNotifications()
{
    ifstream notificationFile;
    notificationFile.open("tables/Notification.txt");
    string line;
    //cout << "loading notifications:" << endl;
    while (getline(notificationFile, line))
    {
        Notification notification = Notification::fromString(line);
        cout << line << endl;
        cout << notification.getTimestamp() << endl;
        cout << notification.getMessage() << endl;
    }
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