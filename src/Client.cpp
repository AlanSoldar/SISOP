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