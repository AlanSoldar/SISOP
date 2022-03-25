#include "../libraries/Client.hpp"

using namespace std;

Client::Client(string userName, string serverAddress, int serverPort)
{
	this->userName = userName;
	this->serverAddress = serverAddress;
	this->serverPort = serverPort;
	this->socket = ClientSocket(serverAddress.c_str(), serverPort);

	this->connect();

	pthread_mutex_init(&mutex_command, NULL);
	pthread_mutex_init(&mutex_receive_notification, NULL);
	pthread_mutex_init(&mutex_main, NULL);
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

void Client::connect()
{
	Packet *response;
	cout << "Connecting to server..." << endl;
	int answer = socket.sendPacket(Packet(this->getUserName(), USER_CONNECT, "connection request"));
	if (answer < 0)
	{
		cout << "connection failed" << endl;
		exit(1);
	}

	response = socket.readPacket();
	if (response->getType() == OPEN_SESSION_SUCCESS)
	{
		cout << "connection established succesfully" << endl;
		return;
	}
	else
	{
		cout << "to many sessions for this user, try again later" << endl;
		exit(1);
	}
}

void Client::closeConnection()
{
	Packet *response;
	cout << "closing connection to server..." << endl;
	int answer = socket.sendPacket(Packet(this->getUserName(), USER_CLOSE_CONNECTION, "close connection request"));
	if (answer < 0)
	{
		cout << "connection failed to close" << endl;
		exit(1);
	}
}

void Client::follow(string userToFollow)
{
	if (userToFollow.find(' ') != string::npos)
	{
		cout << "Invalid username please remove any whitespace\n";
		return;
	}
	if (userToFollow[0] != '@')
	{
		cout << "Invalid username, note that usernames start with @\n";
		return;
	}
	this->socket.sendPacket(Packet(this->userName, FOLLOW_USER, userToFollow.c_str()));
}

void Client::sendNotification(string message)
{
	Notification notification = Notification(userName, message);
	int answer = this->socket.sendPacket(Packet(this->userName, SEND_NOTIFICATION, notification.toString()));
	if (answer < 0)
	{
		exit(1);
	}

	cout << "Message Sent Successfully!" << endl;
}

void Client::sendCloseConnectionRequest()
{
	int answer = this->socket.sendPacket(Packet(this->userName, USER_CLOSE_CONNECTION, "close connection request"));
	if (answer < 0)
	{
		exit(1);
	}

	cout << "Message Sent Successfully!" << endl;
}

void Client::receiveNotification()
{
	Packet *response;

	response = socket.readPacket();
	cout << "response received: " << response->getPayload() << endl
		 << endl;
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
		string commandParameter;
		cin >> command;

		cout << "command received: " << command << endl;
		if (command == "FOLLOW")
		{
			cout << "Request received for a FOLLOW command" << endl;
			getline(cin, commandParameter);
			user->follow(commandParameter.erase(0, 1));
		}
		else if (command == "SEND")
		{
			cout << "Request received for a SEND command" << endl;
			getline(cin, commandParameter);
			user->sendNotification(commandParameter.erase(0, 1));
		}
		else if (command == "CLOSE")
		{
			cout << "closing connection with server" << endl;
			getline(cin, commandParameter);
			user->sendCloseConnectionRequest();
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
	while (true)
	{
		pthread_mutex_lock(&(user->mutex_receive_notification));

		user->receiveNotification();
		sleep(2);

		pthread_mutex_unlock(&(user->mutex_receive_notification));
	}
}