#ifndef SOCKET_HEADER
#define SOCKET_HEADER

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include "Packet.hpp"
using namespace std;

typedef struct address
{
	string ipv4;
	int port;

	bool operator<(const address &address) const
	{
		return port < address.port;
	}

	bool operator==(address address) const
	{
		return ipv4 == address.ipv4 && port == address.port;
	}

} host_address;

class ClientSocket
{

private:
	int socketfd;
	struct sockaddr_in serv_addr;

public:
	int getSocketfd();

	~ClientSocket();
	ClientSocket();
	ClientSocket(int socketfd);

	void connectToServer(const char *serverAddress, int serverPort);
	Packet *readPacket();
	int sendPacket(Packet pkt, int socketfd);
	int sendPacket(Packet pkt);

};

class ServerSocket
{

private:
	int socketfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

public:
	ServerSocket();
	~ServerSocket();

	void bindServer();
	void startListener();
	Packet *readPacket();
	int sendPacket(Packet pkt, int socketfd);
	int sendPacket(Packet pkt);
};

#endif