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
#include <stdlib.h>
#include <cerrno>
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

public:
	struct sockaddr_in serv_addr;
	int getSocketfd();
	ClientSocket() = default;
	ClientSocket(const char *serverAddress, int serverPort);

	Packet *readPacket();
	int sendPacket(Packet pkt);
};

class ServerSocket
{

private:
	int socketfd;
	struct sockaddr_in serv_addr;

public:
	int getSocketfd();
	ServerSocket();
	ServerSocket(int port);

	void bindServer();
	Packet *readPacket(sockaddr_in *clientAddress);
	int sendPacket(Packet pkt, sockaddr_in *receiverAddress);
	int sendPacket(Packet pkt, sockaddr_in *receiverAddress, sockaddr_in routerAddress);
};

#endif