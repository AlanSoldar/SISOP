using namespace std;


class Client{
public:
	string getUserName();
	string getServerAdress();
	int getServerPort();

	Client(string userName, string serverAddress, int serverPort);

	void Connect(string serverAddress, int serverPort);

	static void *commandThread(void* arg);
	//static void *notifyThread(void* arg);
	

private:
	string userName;
	string serverAddress;
	int serverPort;

	void sendNotification(string message);
	void follow(string message);
};