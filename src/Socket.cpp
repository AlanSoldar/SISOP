#include "../libraries/Socket.hpp"
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

// returns a pointer that points to the read Packet or NULL if the connection was closed
Packet *ClientSocket::readPacket(int socketfd)
{

    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));
    int buffer = read(socketfd, pkt, sizeof(Packet));

    if (buffer < 0)
    {
        std::cout << "ERROR reading from socket: " << socketfd << std::endl;
        return NULL;
    }

    else if (buffer == 0)
    {
        std::cout << "Connection closed." << std::endl;
        return NULL;
    }

    return pkt;
}

Packet *ClientSocket::readPacket()
{

    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));
    int buffer = read(this->socketfd, pkt, sizeof(Packet));

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

// overloading for non-initialized Socket object
int ClientSocket::sendPacket(Packet pkt, int socketfd)
{
    int buffer = send(socketfd, &pkt, sizeof(pkt), MSG_NOSIGNAL);

    if (buffer < 0)
    {
        std::cout << "ERROR writing to socket: " << this->socketfd << std::endl;
        std::cout << "Connection closed." << std::endl;
    }

    return buffer;
}

// return the buffer value from send primitive
int ClientSocket::sendPacket(Packet pkt)
{
    char buffer[256];
    //int buffer = send(this->socketfd, &pkt, sizeof(pkt), MSG_NOSIGNAL);
    int response = sendto(socketfd, "test", sizeof("test"), MSG_NOSIGNAL, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));

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

int ServerSocket::getSocketfd()
{
    return this->socketfd;
}

// void ServerSocket::connectNewClient(pthread_t *threadID, Server* server){

// 	socklen_t clientLen;
// 	struct sockaddr_in clientAddressIn;
//     host_address clientAddress;
//     string user;

//     // Wait for connection to socket
//     clientLen = sizeof(struct sockaddr_in);
//     if ((*newsockfd = accept(this->getSocketfd(), (struct sockaddr *) &clientAddressIn, &clientLen)) == -1) {
//         std::cout << "Could not complete connection" << std::endl;
//         return;
//     }

//     std::cout << "Connectet to socket: " << *newsockfd << std::endl;

//     Packet *userPacket = newClientSocket->readPacket();

//     if (userPacket == NULL){
//         std::cout << "Could not receive user information. Closing connection." << std::endl;
//         return;
//     } else
//         user = userPacket->getPayload();

//     clientAddress.ipv4 = inet_ntoa(clientAddressIn.sin_addr);
//     clientAddress.port = ntohs(clientAddressIn.sin_port);
//     bool sessionAvailable = server->openSession(user, clientAddress);

//     Packet resultPacket;
//     if (!sessionAvailable){
//         resultPacket = Packet(OPEN_SESSION_FAIL, "Could not connect to server.");
//         newClientSocket->sendPacket(resultPacket);
//         return; // destructor automatically closes the socket
//     } else{
//         resultPacket = Packet(OPEN_SESSION_SUCCESS, "Connection Successful.");
//         newClientSocket->sendPacket(resultPacket);
//     }

//     communiction_handler_args *args = (communiction_handler_args *) calloc(1, sizeof(communiction_handler_args));
//     args->clientAddress = clientAddress;
//     args->connectedSocket = newClientSocket;
//     args->user = user;
//     args->server = server;

//     pthread_create(threadID, NULL, Server::communicationHandler, (void *)args);
// }

Packet *ServerSocket::readPacket()
{

    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));
    int buffer = read(this->socketfd, pkt, sizeof(Packet));

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

Packet *ServerSocket::readPacket(int socketfd)
{

    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));
    int buffer = read(socketfd, pkt, sizeof(Packet));

    if (buffer < 0)
    {
        std::cout << "ERROR reading from socket: " << socketfd << std::endl;
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
    //int buffer = send(this->socketfd, &pkt, sizeof(pkt), MSG_NOSIGNAL);
    int response = sendto(this->socketfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));

    if (response < 0)
        std::cout << "ERROR writing to socket: " << this->socketfd << std::endl;
    return response;
}

void ServerSocket::bindAndListen()
{

    int n;
    char buf[256];
    socklen_t clilen;

    if (bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
    {
        cout << "ERROR on binding" << endl;
        exit(1);
    }

    cout << "test" << endl; 

    while (1)
    {   
        cout << "starting to receive pack" << endl; 
        /* receive from socket */
        n = recvfrom(socketfd, buf, 256, 0, (struct sockaddr *)&cli_addr, &clilen);
        if (n < 0)
            cout << "ERROR on recvfrom" << endl;
        cout << "Received a datagram" << endl;

        /* send to socket */
        n = sendto(socketfd, "Got your message\n", 17, 0, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr));
        if (n < 0)
            cout << "ERROR on sendto" << endl;
    }

    //listen(this->getSocketfd(), MAX_TCP_CONNECTIONS);
    //std::cout << "Server is running" << endl ;
}
