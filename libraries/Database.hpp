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
    void closeDatabase();

    list<struct sockaddr> getClientAddressByUserId(string id);
    list<Notification> getNotificationsByUserId(string id);
    list<string> getFollowersByUserId(string id);

    int getUserSessionCount(string id);
    void addUserSession(string id, struct sockaddr adrr);
    void setNotificationAsSeen(Notification notification);

    list<pair<string,struct sockaddr>> getLoggedUsers();

    int userConnect(string userId, struct sockaddr sock);
    int userCloseConnection(string userId, struct sockaddr sock);

    void saveNotification(Notification notification);
    void saveNewFollow(string followerId, string userId);

private:
    string name;
    list<pair<string,struct sockaddr>> loggedUserAddresses;
    map<string, list<string>> followers;
    map<string, list<Notification>> notifications;

    void loadNotifications();
    void loadFollows();

    void stashNotifications();
    void stashFollows();
    vector<string> split(string s, string delimiter);
};