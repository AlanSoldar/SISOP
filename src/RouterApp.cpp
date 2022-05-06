#include <cstdio>
#include <unistd.h>
#include "../libraries/Router.hpp"

using namespace std;

int main(){

    pthread_t communicationThreadConnection, terminalCommandThreadConnection;
	int i = 0;

	ServerSocket serverSocket = ServerSocket();
	Router* router = new Router();

	router_handler_args *args = (router_handler_args *) calloc(1, sizeof(router_handler_args));
    args->connectedSocket = &serverSocket;
    args->router = router;

	pthread_create(&communicationThreadConnection, NULL, Router::RouteCommunication, (void *)args);
	//pthread_create(&terminalCommandThreadConnection, NULL, Server::terminalCommandHandler, (void *)args);

	pthread_join(communicationThreadConnection, NULL);
	//pthread_join(terminalCommandThreadConnection, NULL);

	cout << "main end" << endl;

	return 0; 
}