#include <cstdio>
#include <unistd.h>
#include "../libraries/Client.hpp"

using namespace std;

int main(int argc, char *argv[])
{

	pthread_t commandThread, receiveNotificationThread;

	if (argc < 4)
	{
		cout << "Erro: Verifique se inseriu: <username> <server_adress> e <serverport>" << endl;
		return 0;
	}

	Client *user = (Client *)malloc(sizeof(Client));
	user = new Client(argv[1], argv[2], stoi(argv[3]));

	pthread_create(&commandThread, NULL, Client::commandThread, (void *)user);
	pthread_create(&receiveNotificationThread, NULL, Client::receiveNotificationThread, (void *)user);

	pthread_join(commandThread, NULL);
	pthread_join(receiveNotificationThread, NULL);

	return 0;
}