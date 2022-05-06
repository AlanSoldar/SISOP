#include "../libraries/Server.hpp"

int main(){

    pthread_t communicationThreadConnection, terminalCommandThreadConnection;
	int i = 0;

	ServerSocket serverSocket = ServerSocket();
	Server* server = new Server();

	communiction_handler_args *args = (communiction_handler_args *) calloc(1, sizeof(communiction_handler_args));
    args->connectedSocket = &serverSocket;
    args->server = server;

	pthread_create(&communicationThreadConnection, NULL, Server::communicationHandler, (void *)args);
	pthread_create(&terminalCommandThreadConnection, NULL, Server::terminalCommandHandler, (void *)args);

	pthread_join(communicationThreadConnection, NULL);
	pthread_join(terminalCommandThreadConnection, NULL);

	cout << "main end" << endl;

	return 0; 
}