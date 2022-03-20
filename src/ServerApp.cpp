#include "../libraries/Server.hpp"



int main(){

    pthread_t threadConnection;
	int i = 0;

	ServerSocket serverSocket = ServerSocket();
	Server* server = new Server();
	serverSocket.bindServer();

	communiction_handler_args *args = (communiction_handler_args *) calloc(1, sizeof(communiction_handler_args));
    args->connectedSocket = &serverSocket;
    args->server = server;

	pthread_create(&threadConnection, NULL, Server::communicationHandler, (void *)args);
	pthread_join(threadConnection, NULL);

	cout << "test main end" << endl;

	return 0; 
}