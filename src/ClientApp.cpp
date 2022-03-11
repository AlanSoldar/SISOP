#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Client.cpp"
using namespace std;
// gcc -pthread -o ClientApp ClientApp.cpp -lstdc++
void* testeThread(void* arg){
	char *letra = (char *) arg;
	for(int i=0; i< 10; i++){
		sleep(1);
		cout << *letra<<'\n';
	}
	return NULL;
}

void* teste2Thread(void* arg){
	char letra;
	for(int i=0; i< 10; i++){
		cin >> letra;
		cout << letra;
	}
	return NULL;
}

int main(int argc, char *argv[]) {

	pthread_t commandThread;
	pthread_t test2;

	if(argc < 4) {
		cout << "Erro: Verifique se inseriu: <username> <server_adress> e <serverport>\n";
		return 0;
	}

	Client *user = (Client *) malloc(sizeof(Client));
	user = new Client(argv[1], argv[2], stoi(argv[3]));

	//cout<< user->getUserName() << ' ' << user->getServerAdress() << ' ' << user->getServerPort() << '\n';
	int i =pthread_create(&commandThread, NULL, Client::commandThread, (void *)user);
	i = pthread_join(commandThread, NULL);
	return 0;
}