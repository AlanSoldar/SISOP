#include "../libraries/Client.hpp"

using namespace std;

Client::Client(string userName, string serverAddress, int serverPort)
{
	this->userName = userName;
	this->serverAddress = serverAddress;
	this->serverPort = serverPort;
	this->socket = ClientSocket(serverAddress.c_str(), serverPort);

	this->connect();
	this->isConnected = true;

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
		cout << "connection established succesfully" << endl
			 << "Welcome " << userName << endl
			 << endl;
		return;
	}
	else
	{
		cout << "too many sessions for this user, try again later" << endl;
		exit(1);
	}
}

void Client::closeConnection()
{
	int answer = this->socket.sendPacket(Packet(this->userName, USER_CLOSE_CONNECTION, "close connection request"));
	if (answer < 0)
	{
		exit(1);
	}
	cout << "Connection closed!" << endl;
	this->isConnected = false;
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
	int answer = this->socket.sendPacket(Packet(this->userName, FOLLOW_USER, userToFollow.c_str()));
	if (answer < 0)
	{
		exit(1);
	}
	cout << "Now you are following" << userToFollow << endl;
}

void Client::wakeUpServer()
{
	int answer = this->socket.sendPacket(Packet(this->userName, WAKE_UP, to_string(this->socket.getSocketfd())));
	if (answer < 0)
	{
		exit(1);
	}

	cout << "setting server as primary" << endl;
}

void Client::sendServerSleepCommand()
{
	int answer = this->socket.sendPacket(Packet(this->userName, SLEEP, to_string(this->socket.getSocketfd())));
	if (answer < 0)
	{
		exit(1);
	}

	cout << "setting server as backup" << endl;
}

void Client::sendNotification(string message)
{
	Notification notification = Notification(userName, message);
	int answer = this->socket.sendPacket(Packet(this->userName, SEND_NOTIFICATION, notification.toString()));
	if (answer < 0)
	{
		exit(1);
	}

	cout << "New notification sent to your followers!" << endl;
}

void Client::receiveNotification()
{
	Packet *response;
	response = socket.readPacket();
	if (response->getType() == RECEIVE_NOTIFICATION)
	{
		Notification notification = Notification::fromString(response->getPayload());
		cout << "you have a new notification: " << endl;
		Notification::printNotification(notification);
	}
}

void *Client::commandThread(void *arg)
{
	Client *user = (Client *)arg;

	while (user->isConnected)
	{
		pthread_mutex_lock(&(user->mutex_command));

		cout << "Type your command:" << endl;

		string command = "";
		string commandParameter;
		cin >> command;

		if (command == "FOLLOW" || command == "follow")
		{
			getline(cin, commandParameter);
			user->follow(commandParameter.erase(0, 1));
		}
		else if (command == "SEND" || command == "send")
		{
			getline(cin, commandParameter);
			user->sendNotification(commandParameter.erase(0, 1));
		}
		else if (command == "WAKE" || command == "wake")
		{
			user->wakeUpServer();
		}
		else if (command == "SLEEP" || command == "sleep")
		{
			user->sendServerSleepCommand();
		}
		else if (command == "CLOSE" || command == "EXIT" || command == "close" || command == "exit")
		{
			cout << "closing connection with server" << endl;
			user->closeConnection();
		}
		else
		{
			cout << "This is not a valid command please use <FOLLOW> <userName> || <SEND> <yourMessage>" << command << endl;
		}

		pthread_mutex_unlock(&(user->mutex_command));
	}
	return NULL;
}

void *Client::receiveNotificationThread(void *arg)
{
	Client *user = (Client *)arg;
	Packet *notification;
	while (user->isConnected)
	{
		pthread_mutex_lock(&(user->mutex_receive_notification));

		user->receiveNotification();
		sleep(2);

		pthread_mutex_unlock(&(user->mutex_receive_notification));
	}
	return NULL;
}