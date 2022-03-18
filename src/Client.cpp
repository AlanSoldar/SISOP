#include "../libraries/Client.hpp"
#include "../libraries/Defines.hpp"
#include <iostream>

using namespace std;
Client::Client(string userName, string serverAddress, int serverPort) {
	this->userName = userName;
	this->serverAddress = serverAddress;
	this->serverPort = serverPort;
	this->connect();

	pthread_mutex_init(&mutex_command,NULL);

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
	if(userName.find(' ') != string::npos || userName.find('@') == string::npos){
		cout << "invalid userName please remove any whitespace or add a @ at the beginning\n";
	}
}

void Client::sendNotification(string message) {
	int answer = this->socket.sendPacket(Packet(SEND_NOTIFICATION, message.c_str()));
	if(answer<0) {
		exit(1);
	}
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
	} while(input != 10 && input != ' ');
	//remove 'space' from command
	command.pop_back();

	if(command == "FOLLOW"){
		cin >> commandParameter;
		user->follow(commandParameter);
	} else if(command == "SEND") {
		//getline(cin, commandParameter);
		do {
			input = getchar();
			commandParameter += input;
		}while(input != 10);
		user->sendNotification(commandParameter.substr(0,128));
	} else {
		cout << "This is not a valid command please use <FOLLOW> <userName> || <SEND> <yourMessage>\n";
	}

	//unlock mutex
}