#include <string>
#include <map>
#include <list>
#include <iostream>
#include <fstream>

using namespace std;

class Database
{
public:
    Database();
    Database(string name);

    string getUserByid(string id);
    list<string> getNotificationsByUserId(string id);
    list<string> getFollowersByUserId(string id);

    void saveUser(string id);
    void saveNotification(string senderId, string message);
    void saveNewFollow(string followerId, string userId);

private:
    string name;
    map<string, string> users;
    map<string, list<string>> followers;
    map<string, list<string>> notifications;

    void loadUsers();
    void loadNotifications();
    void loadFollows();
};