#include <string>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include "Notification.hpp"

using namespace std;

class Database
{
public:
    Database();

    string getUserByid(string id);
    bool userExists(string userId);

    list<struct sockaddr*> getClientAddressByUserId(string id);
    list<string> getNotificationsByUserId(string id);
    list<string> getFollowersByUserId(string id);
    int getUserSessionCount(string id);
    void addUserSession(string id, struct sockaddr* adrr);

    list<pair<string,struct sockaddr*>> getLoggedUsers();

    int userConnect(string userId, struct sockaddr* sock);
    int userCloseConnection(string userId, struct sockaddr* sock);

    void saveUser(string id);
    void saveNotification(string senderId, Notification notification);
    void saveNewFollow(string followerId, string userId);

private:
    string name;
    list<pair<string,struct sockaddr*>> loggedUserAddresses;
    map<string, string> users;
    map<string, list<string>> followers;
    map<string, list<string>> notifications;

    void loadUsers();
    void loadNotifications();
    void loadFollows();
    vector<string> split(string s, string delimiter);
};