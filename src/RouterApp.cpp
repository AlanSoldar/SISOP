#include <cstdio>
#include <unistd.h>
#include "../libraries/Router.hpp"

using namespace std;

int main()
{

	pthread_t clientServetThreadConnection, serverClientThreadConnection, terminalCommandThreadConnection;
	int i = 0;

	Router *router = new Router();

	router_handler_args *args = (router_handler_args *)calloc(1, sizeof(router_handler_args));
	args->router = router;

	pthread_create(&clientServetThreadConnection, NULL, Router::ClientServerCommunication, (void *)args);
	pthread_create(&serverClientThreadConnection, NULL, Router::ServerClientCommunication, (void *)args);
	// pthread_create(&terminalCommandThreadConnection, NULL, Server::terminalCommandHandler, (void *)args);

	pthread_join(clientServetThreadConnection, NULL);
	pthread_join(serverClientThreadConnection, NULL);
	// pthread_join(terminalCommandThreadConnection, NULL);

	cout << "main end" << endl;

	return 0;
}