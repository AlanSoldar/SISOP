#include <string>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Notification.hpp"

using namespace std;

class Database
{
public:
    Database();
    void saveDatabaseState();
    void loadDatabaseState();

    list<struct sockaddr_in> getClientAddressByUserId(string id);
    list<Notification> getNotificationsByUserId(string id);
    list<string> getFollowersByUserId(string id);

    int getUserSessionCount(string id);
    void addUserSession(string id, struct sockaddr_in adrr);
    void setNotificationAsSeen(Notification notification);

    list<pair<string,struct sockaddr_in>> getLoggedUsers();

    int userConnect(string userId, struct sockaddr_in sock);
    int userCloseConnection(string userId, struct sockaddr_in sock);

    void saveNotification(Notification notification);
    void saveNewFollow(string followerId, string userId);

private:
    string name;
    list<pair<string,struct sockaddr_in>> loggedUserAddresses;
    map<string, list<string>> followers;
    map<string, list<Notification>> notifications;

    void loadUsers();
    void loadFollows();
    void loadNotifications();

    void stashUsers();
    void stashFollows();
    void stashNotifications();
    vector<string> split(string s, string delimiter);
};