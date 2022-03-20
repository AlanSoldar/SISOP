#include "../libraries/Server.hpp"



int main(){

    pthread_t threadConnections[MAX_TCP_CONNECTIONS];
	int i = 0;

	ServerSocket serverSocket = ServerSocket();
	Server* server = new Server();
	serverSocket.bindServer();
	serverSocket.startListener();

	return 0; 
}