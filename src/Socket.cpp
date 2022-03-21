#include "../libraries/Socket.hpp"
#include "../libraries/Defines.hpp"

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

}

int ClientSocket::getSocketfd()
{
    return this->socketfd;
}

// return the buffer value from send primitive
int ClientSocket::sendPacket(Packet pkt)
{
    char buffer[PAYLOAD_MAX_SIZE];
    strcpy(buffer, pkt.toString().c_str());
    int response = sendto(socketfd, buffer, PAYLOAD_MAX_SIZE, 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));

    if (response < 0)
        std::cout << "ERROR writing to socket: " << socketfd << std::endl;

    return response;
}

Packet *ClientSocket::readPacket()
{

    char buf[PAYLOAD_MAX_SIZE];
    socklen_t clilen;
    struct sockaddr_in from;
    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));

    int rsp = recvfrom(socketfd, buf, PAYLOAD_MAX_SIZE, 0, (struct sockaddr *)&from, &clilen);

    cout << "tst" << endl;

    // string response(buf);

    // Packet packet = Packet::fromString(response);

    // pkt = &packet;

    // if (rsp < 0)
    // {
    //     cout << "ERROR reading from server socket: " << socketfd << endl;
    //     return NULL;
    // }

    // else if (rsp == 0)
    // {
    //     std::cout << "Connection closed." << std::endl;
    //     return NULL;
    // }

    return pkt;
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

    char buf[PAYLOAD_MAX_SIZE];
    socklen_t clilen;
    sockaddr_in cli_addrr;
    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));

    recvfrom(socketfd, buf, PAYLOAD_MAX_SIZE, 0, (struct sockaddr *)&cli_addrr, &clilen);
    //cout << cli_addrr << endl;
    cout << sendto(socketfd, "test", 4, 0, (struct sockaddr *)&cli_addrr, sizeof(struct sockaddr_in)) << endl;

    string response(buf);

    Packet packet = Packet::fromString(response);

    pkt = &packet;

    if (packet.getPayload().length() < 0)
    {
        cout << "ERROR reading from server socket: " << socketfd << endl;
        return NULL;
    }

    else if (packet.getPayload().length() == 0)
    {
        std::cout << "Connection closed." << std::endl;
        return NULL;
    }

    return pkt;
}

int ServerSocket::sendPacket(Packet pkt)
{

    cout << "send notifications" << endl;
    char buffer[PAYLOAD_MAX_SIZE];
    strcpy(buffer, pkt.toString().c_str());
    int response = sendto(socketfd, buffer, PAYLOAD_MAX_SIZE, 0, (struct sockaddr *)&this->cli_addr, sizeof(struct sockaddr));

    if (response < -100)
        std::cout << "ERROR writing to socket: " << socketfd << std::endl;

    return response;
}

void ServerSocket::bindServer()
{

    if (bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
    {
        cout << "ERROR on binding" << endl;
        exit(1);
    }
}
