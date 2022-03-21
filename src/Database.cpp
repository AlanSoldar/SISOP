#include "../libraries/Database.hpp"

using namespace std;

Database::Database()
{
}

Database::Database(string name)
{
    this->name = name;
    this->users = {};
    this->followers = {};
    this->notifications = {};
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

void Database::saveNotification(string senderId, string message)
{
    cout << "message: " << message << endl;
    Notification notification = Notification::fromString(message);

    cout << notification.getId() << endl;
    cout << notification.getMessage() << endl;
    cout << notification.getSenderId() << endl;
    cout << notification.getTimestamp() << endl;

    ofstream notificationFile;
    notificationFile.open("tables/Notification.txt", ios_base::app);
    notificationFile << notification.toString() << endl;
    notificationFile.close();
    cout << senderId << " has posted a new notification: " << message << endl;
}

void Database::loadUsers()
{
    ifstream userFile;
    userFile.open("tables/User.txt");
    string line;
    cout << "loading users:" << endl;
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
    cout << "loading follows:" << endl;
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
    cout << "loading notifications:" << endl;
    while (getline(notificationFile, line))
    {
        Notification notification = Notification::fromString(line);
        cout << line << endl;
        cout << notification.getTimestamp() << endl;
        cout << notification.getMessage() << endl;
    }
}