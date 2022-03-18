#include "Socket.hpp"
#include <pthread.h>

using namespace std;

class ClientSocket : public Socket {
	public:
		void connectToServer();
		void connectToServer(const char* serverAddress, int serverPort);
};

class Client{
public:
	string getUserName();
	string getServerAddress();
	int getServerPort();
	ClientSocket socket;

	Client(string userName, string serverAddress, int serverPort);



	static void *commandThread(void* arg);
	//static void *notifyThread(void* arg);

	pthread_mutex_t mutex_command;
	

private:
	string userName;
	string serverAddress;
	int serverPort;

		void connect();
	void sendNotification(string message);
	void follow(string message);
};