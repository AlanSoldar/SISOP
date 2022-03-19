#include "../libraries/Database.hpp"


using namespace std;

Database::Database()
{
    this->name = "database";
    this->users = {};
    this->followers = {};
    this->notifications = {};

    this->loadUsers();
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
    return this->users.find(id)->second;
}

list<string> Database::getFollowersByUserId(string id)
{
    return this->followers.find(id)->second;
}

list<string> Database::getNotificationsByUserId(string id)
{
    return this->notifications.find(id)->second;
}

void Database::saveUser(string id)
{
    ofstream userFile;
    userFile.open("tables/User.txt");
    //userFile << followerId << " " << userId;
    userFile.close();
    //cout << followerId << " is now following: " << userId << endl;
}

void Database::saveNewFollow(string followerId, string userId)
{
    ofstream followerFile;
    followerFile.open("tables/Follower.txt");
    followerFile << followerId << " " << userId;
    followerFile.close();
    cout << followerId << " is now following: " << userId << endl;

}

void Database::saveNotification(string senderId, string message)
{
    ofstream notificationFile;
    notificationFile.open("tables/Notification.txt");
    //notificationFile << followerId << " " << userId;
    notificationFile.close();
    //cout << followerId << " is now following: " << userId << endl;
}


void Database::loadUsers() {
    ifstream userFile;
    userFile.open("tables/User.txt");
    string line;
    while(getline(userFile, line)) {
        cout << line << endl;
    }
}

void Database::loadFollows() {
    ifstream followFile;
    followFile.open("tables/Follower.txt");
    string line;
    while(getline(followFile, line)) {
        cout << line << endl;
    }
}

void Database::loadNotifications() {
    ifstream notificationFile;
    notificationFile.open("tables/Notification.txt");
    string line;
    while(getline(notificationFile, line)) {
        cout << line << endl;
    }
}