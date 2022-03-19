#include "Socket.hpp"
#include <pthread.h>

using namespace std;

class ClientSocket : public Socket
{
public:
	//void connectToServer();
	void connectToServer(const char *serverAddress, int serverPort);
};

class Client
{
public:
	string getUserName();
	string getServerAddress();
	int getServerPort();
	ClientSocket socket;

	Client(string userName, string serverAddress, int serverPort);

	static void *commandThread(void *arg);
	static void *receiveNotificationThread(void *arg);
	static void *mainThread(void *arg);

	pthread_mutex_t mutex_command;
	pthread_mutex_t mutex_receive_notification;
	pthread_mutex_t mutex_main;

private:
	string userName;
	string serverAddress;
	int serverPort;

	void connect();
	void sendNotification(string message);
	void follow(string userName);
};