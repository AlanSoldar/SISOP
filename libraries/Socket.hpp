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

typedef struct address {
	string ipv4;
	int port;

    bool operator < (const address& address) const {
		return port < address.port;
	}

	bool operator == (address address) const {
		return ipv4 == address.ipv4 && port == address.port;
	}
    
} host_address;


class Socket
{
	private:
		int socketfd;

	public:
		int getSocketfd();
		
		~Socket();
		Socket();
		Socket(int socketfd);

		Packet* readPacket();
		static Packet* readPacket(int socketfd);
        int sendPacket(Packet packet);
		int sendPacket(Packet pkt, int socketfd);
};
#endif