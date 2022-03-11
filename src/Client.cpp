#include "Client.hpp"

using namespace std;
Client::Client(string userName, string serverAddress, int serverPort) {
	this->userName = userName;
	this->serverAddress = serverAddress;
	this->serverPort = serverPort;
}

string Client::getUserName() {
	return this->userName;
}

string Client::getServerAdress() {
	return this->serverAddress;
}

int Client::getServerPort() {
	return this->serverPort;
}

void Client::follow(string userName) {
	cout << "following " << userName << "\n";
}

void Client::sendNotification(string message) {
	cout << "you just twetted: " << message << '\n';
}

void *Client::commandThread(void* arg) {
	Client *user = (Client*) arg;

	cout << "user started a command\n";

	//lock mutex for user input
	
	string command = "";
	char input;
	string commandParameter = "";
	do {
		input = getchar();
		command += input;
		// 10 is the code for LF
	} while(input != 10 && input != ' ');

	if(command == "FOLLOW"){
		cin >> commandParameter;
		user->follow(commandParameter);
	} else if(command == "SEND") {
		cin >> commandParameter;
		user->sendNotification(commandParameter);
	} else {
		cout << "This is not a valid command please use <FOLLOW> <userName> || <SEND> <yourMessage>\n";
	}

	//unlock mutex
}