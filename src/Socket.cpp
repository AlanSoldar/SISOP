#include "../libraries/Socket.hpp"
#include "../libraries/Defines.hpp"
#include <stdlib.h>

ClientSocket::~ClientSocket()
{
    std::cout << "Closing socketfd...\n";
    close(socketfd);
}

ClientSocket::ClientSocket()
{
    if ((this->socketfd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
    {
        std::cout << "ERROR opening the socket\n"
                  << std::endl;
        exit(1);
    }
}

ClientSocket::ClientSocket(int socketfd)
{
    this->socketfd = socketfd;
}

void ClientSocket::connectToServer(const char *serverAddress, int serverPort)
{
    struct hostent *server;
    int sockfd;
    server = gethostbyname(serverAddress);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPort);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero), 8);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        ("ERROR on tentative to stablish a connection with server.\n");
        exit(1);
    }

    // if (connect(this->getSocketfd(), (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    // {
    // 	cout <<
    // 	exit(1);
    // }
}

int ClientSocket::getSocketfd()
{
    return this->socketfd;
}

Packet *ClientSocket::readPacket()
{

    char buf[256];
    socklen_t clilen;
    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));

    int buffer = recvfrom(socketfd, buf, 256, 0, (struct sockaddr *)&serv_addr, &clilen);

    if (buffer < 0)
    {
        std::cout << "ERROR reading from socket: " << this->socketfd << std::endl;
        return NULL;
    }

    else if (buffer == 0)
    {
        std::cout << "Connection closed." << std::endl;
        return NULL;
    }

    return pkt;
}

// return the buffer value from send primitive
int ClientSocket::sendPacket(Packet pkt)
{
    char buffer[PAYLOAD_MAX_SIZE];
    strcpy(buffer, pkt.getPayload().c_str());
    int response = sendto(socketfd, buffer, strlen(buffer), MSG_NOSIGNAL, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));

    if (response < 0)
        std::cout << "ERROR writing to socket: " << socketfd << std::endl;

    return response;
}

ServerSocket::ServerSocket()
{
    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
    {
        std::cout << "ERROR opening the socket\n"
                  << std::endl;
        exit(1);
    }

    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = htons(PORT);
    this->serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(this->serv_addr.sin_zero), 8);
}

ServerSocket::~ServerSocket()
{
    std::cout << "\nClosing socketfd...\n";
    close(socketfd);
}

Packet *ServerSocket::readPacket()
{

    char buf[256];
    socklen_t clilen;
    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));

    int buffer = recvfrom(socketfd, buf, 256, 0, (struct sockaddr *)&cli_addr, &clilen);
    buffer = recvfrom(socketfd, buf, 256, 0, (struct sockaddr *)&cli_addr, &clilen);

    if (buffer < 0)
    {
        //cout << "ERROR reading from socket: " << socketfd << endl;
        return NULL;
    }

    else if (buffer == 0)
    {
        std::cout << "Connection closed." << std::endl;
        return NULL;
    }

    return pkt;
}

int ServerSocket::sendPacket(Packet pkt)
{
    char buffer[256];
    int response = sendto(this->socketfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));

    if (response < 0)
        std::cout << "ERROR writing to socket: " << this->socketfd << std::endl;
    return response;
}

void ServerSocket::bindServer()
{
    int n;
    char buf[256];
    socklen_t clilen;

    if (bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
    {
        cout << "ERROR on binding" << endl;
        exit(1);
    }
}

void ServerSocket::startListener()
{ 
    cout << "Starting to receive messages:" << endl;
    while (1)
    {
        readPacket();
    }
}
