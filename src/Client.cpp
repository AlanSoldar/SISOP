#include "../libraries/Client.hpp"
#include "../libraries/Defines.hpp"
#include <iostream>
#include <string.h>

using namespace std;

void ClientSocket::connectToServer(const char* serverAddress, int serverPort) {
	struct sockaddr_in serv_addr;
	struct hostent *server;
	server = gethostbyname(serverAddress);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serverPort);
	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

	if(connect(this->getSocketfd(),(struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0) {
		cout << ("ERROR on connection\n");
		exit(1);
	}
}

Client::Client(string userName, string serverAddress, int serverPort) {
	this->userName = userName;
	this->serverAddress = serverAddress;
	this->serverPort = serverPort;
	this->connect();
	pthread_mutex_init(&mutex_command,NULL);
	pthread_mutex_init(&mutex_receive_notification,NULL);
	pthread_mutex_init(&mutex_main,NULL);
}

void Client::connect() {
	this->socket.connectToServer(this->getServerAddress().c_str(), this->getServerPort());
	Packet userConnect = Packet(USER_CONNECT, this->getUserName().c_str());
	this->socket.sendPacket(userConnect);

	Packet *answer;
	answer = this->socket.readPacket();

	if(answer != NULL) {
		cout << answer->getPayload() << "\n";

		if(answer->getType() == OPEN_SESSION_SUCCESS)
			return;
		if(answer->getType() == OPEN_SESSION_FAIL)
			exit(1);
	}
	else {
		cout << "no response from the server" << "\n";
		exit(1);
	}
}

string Client::getUserName() {
	return this->userName;
}

string Client::getServerAddress() {
	return this->serverAddress;
}

int Client::getServerPort() {
	return this->serverPort;
}

void Client::follow(string userName) {
	if(userName.find(' ') != string::npos){
		cout << "invalid username please remove any whitespace\n";
		return;
	}
	if(userName[0] != '@') {
		cout << "invalid username, note that usernames start with @\n";
		return;
	}
	this->socket.sendPacket(Packet(FOLLOW_USER, userName.c_str()));
}

void Client::sendNotification(string message) {
	int answer = this->socket.sendPacket(Packet(SEND_NOTIFICATION, message.c_str()));
	if(answer<0) {
		exit(1);
	}
}

void *Client::mainThread(void* arg) {
	Client *user = (Client*) arg;

	fd_set rfds, save_rfds;
	struct timeval tv;
	int retval;
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	save_rfds = rfds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	while(true) {
		pthread_mutex_lock(&(user->mutex_main));
		rfds = save_rfds;
		retval = select(1, &rfds, NULL, NULL, &tv);

		if(retval) {
			pthread_mutex_unlock(&(user->mutex_main));
			pthread_mutex_unlock(&(user->mutex_command));
			sleep(2);

			pthread_mutex_lock(&(user->mutex_main));
		}

		pthread_mutex_unlock(&(user->mutex_main));
	}

}

void *Client::commandThread(void* arg) {
	Client *user = (Client*) arg;

	cout << "user started a command\n";

	while(true) {

		pthread_mutex_lock(&(user->mutex_command));
		pthread_mutex_lock(&(user->mutex_main));
		pthread_mutex_lock(&(user->mutex_receive_notification));

		string command = "";
		string commandParameter = "";

		cin >> command;
		//remove 'space' from command
		//command.pop_back();

		if(command == "FOLLOW"){
			cin >> commandParameter;
			user->follow(commandParameter);
		} else if(command == "SEND") {
			cin >> commandParameter;
			user->sendNotification(commandParameter.substr(0,128));
		} else {
			cout << "This is not a valid command please use <FOLLOW> <userName> || <SEND> <yourMessage>" << command << endl;
		}

		pthread_mutex_unlock(&(user->mutex_command));
		pthread_mutex_unlock(&(user->mutex_main));
		pthread_mutex_unlock(&(user->mutex_receive_notification));
	}
}

void *Client::receiveNotificationThread(void* arg) {
	Client *user = (Client*) arg;
	Packet* notification;
	int i;
	while(true) {
		notification = user->socket.readPacket();
		if(notification == NULL) {
			exit(1);
		}

		pthread_mutex_lock(&(user->mutex_receive_notification));
		pthread_mutex_unlock(&(user->mutex_receive_notification));
	}
}