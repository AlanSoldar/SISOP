using namespace std;


class Client{
public:
	string getUserName();
	string getServerAdress();
	int getServerPort();

	Client(string userName, string serverAddress, int serverPort);

	void Connect(string serverAddress, int serverPort);

	}

private:
	string userName;
	string serverAddress;
	int serverPort;
};