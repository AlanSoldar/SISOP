#include "../libraries/Client.hpp"

using namespace std;

Client::Client(string userName, string serverAddress, int serverPort)
{
	this->userName = userName;
	this->serverAddress = serverAddress;
	this->serverPort = serverPort;
	//this->connect();
	this->socket.connectToServer(serverAddress.c_str(), serverPort);
	pthread_mutex_init(&mutex_command, NULL);
	pthread_mutex_init(&mutex_receive_notification, NULL);
	pthread_mutex_init(&mutex_main, NULL);
}

void Client::connect()
{
	cout << "trying to connect" << endl;
	this->socket.connectToServer(this->getServerAddress().c_str(), this->getServerPort());
	// Packet userConnect = Packet(USER_CONNECT, this->getUserName().c_str());
	// this->socket.sendPacket(userConnect);
	// Packet *answer;
	// answer = this->socket.readPacket();

	// if (answer != NULL)
	// {
	// 	cout << answer->getPayload() << "\n";

	// 	if (answer->getType() == OPEN_SESSION_SUCCESS)
	// 		return;
	// 	if (answer->getType() == OPEN_SESSION_FAIL)
	// 		exit(1);
	// }
	// else
	// {
	// 	cout << "No response received from the server"
	// 		 << "\n";
	// 	exit(1);
	// }
}

string Client::getUserName()
{
	return this->userName;
}

string Client::getServerAddress()
{
	return this->serverAddress;
}

int Client::getServerPort()
{
	return this->serverPort;
}

void Client::follow(string userName)
{
	if (userName.find(' ') != string::npos)
	{
		cout << "Invalid username please remove any whitespace\n";
		return;
	}
	if (userName[0] != '@')
	{
		cout << "Invalid username, note that usernames start with @\n";
		return;
	}
	this->socket.sendPacket(Packet(FOLLOW_USER, userName.c_str()));
}

void Client::sendNotification(char *message)
{
	Notification notification = Notification(userName, message);
	int answer = this->socket.sendPacket(Packet(SEND_NOTIFICATION, notification.toString()));
	if (answer < 0)
	{
		cout << "Error trying to send your message, please try again." << endl;
		exit(1);
	}

	cout << "Message Sent Successfully!" << endl;
}

void *Client::mainThread(void *arg)
{
	Client *user = (Client *)arg;

	fd_set rfds, save_rfds;
	struct timeval tv;
	int retval;
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	save_rfds = rfds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	while (true)
	{
		pthread_mutex_lock(&(user->mutex_main));
		rfds = save_rfds;
		retval = select(1, &rfds, NULL, NULL, &tv);

		if (retval)
		{
			pthread_mutex_unlock(&(user->mutex_main));
			pthread_mutex_unlock(&(user->mutex_command));
			sleep(2);

			pthread_mutex_lock(&(user->mutex_main));
		}

		pthread_mutex_unlock(&(user->mutex_main));
	}
}

void *Client::commandThread(void *arg)
{
	Client *user = (Client *)arg;

	cout << "User " + user->userName + " started a list of commands:" << endl;

	while (true)
	{

		pthread_mutex_lock(&(user->mutex_command));
		pthread_mutex_lock(&(user->mutex_main));
		pthread_mutex_lock(&(user->mutex_receive_notification));

		string command = "";
		char commandParameter[128];
		cin >> command;

		if (command == "FOLLOW")
		{
			cout << "Request received for a FOLLOW command:\n" << endl;
			cin.getline(commandParameter,128);
			user->follow(commandParameter);
		}
		else if (command == "SEND")
		{
			cout << "Request received for a SEND command:\n" << endl;
			cin.getline(commandParameter,128);
			user->sendNotification(commandParameter);
		}
		else
		{
			cout << "This is not a valid command please use <FOLLOW> <userName> || <SEND> <yourMessage>" << command << endl;
		}

		pthread_mutex_unlock(&(user->mutex_command));
		pthread_mutex_unlock(&(user->mutex_main));
		pthread_mutex_unlock(&(user->mutex_receive_notification));
	}
}

void *Client::receiveNotificationThread(void *arg)
{
	Client *user = (Client *)arg;
	Packet *notification;
	int i;
	while (true)
	{
		notification = user->socket.readPacket();
		if (notification == NULL)
		{
			exit(1);
		}

		pthread_mutex_lock(&(user->mutex_receive_notification));
		pthread_mutex_unlock(&(user->mutex_receive_notification));
	}
}