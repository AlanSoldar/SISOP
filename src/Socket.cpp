#include "../libraries/Socket.hpp"
#include "../libraries/Defines.hpp"

ClientSocket::ClientSocket(const char *serverAddress, int serverPort)
{
    int sockfd, n;
	unsigned int length;
	struct sockaddr_in serv_addr, from;
	struct hostent *server;
	
	server = gethostbyname("localhost");

	
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");
	
	serv_addr.sin_family = PF_INET;     
	serv_addr.sin_port = htons(PORT);    
	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(serv_addr.sin_zero), 8); 
    

    this->socketfd = sockfd;
    this->serv_addr = serv_addr;

}

int ClientSocket::getSocketfd()
{
    return this->socketfd;
}

int ClientSocket::sendPacket(Packet pkt)
{
    char buffer[256];

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

    int rsp = recvfrom(socketfd, buf, PAYLOAD_MAX_SIZE, 0, NULL, NULL);

    string response(buf);

    pkt = Packet::fromString(response);

    if (rsp < 0)
    {
        cout << "ERROR reading from server socket: " << socketfd << endl;
        return NULL;
    }

    else if (rsp == 0)
    {
        std::cout << "Connection closed." << std::endl;
        return NULL;
    }

    return pkt;
}

ServerSocket::ServerSocket()
{
    int sockfd, n;
    struct sockaddr_in serv_addr, cli_addr;

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
        cout << "ERROR opening socket" << endl;

    serv_addr.sin_family = PF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_addr.sin_zero), 8);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
        cout << "ERROR on binding" << endl;

    this->socketfd = sockfd;
}

Packet *ServerSocket::readPacket()
{

    int respCode;
    char buf[PAYLOAD_MAX_SIZE];
    socklen_t clilen;
    Packet *pkt = new Packet();
    memset(pkt, 0, sizeof(Packet));

    clilen = sizeof(struct sockaddr_in);

    recvfrom(socketfd, buf, PAYLOAD_MAX_SIZE, 0, (struct sockaddr *)&cli_addr, &clilen);

    string response(buf);

    pkt = Packet::fromString(response);

    if (pkt->getPayload().length() < 0)
    {
        cout << "ERROR reading from server socket: " << socketfd << endl;
        return NULL;
    }

    else if (pkt->getPayload().length() == 0)
    {
        std::cout << "Connection closed." << std::endl;
        return NULL;
    }

    return pkt;
}

int ServerSocket::sendPacket(Packet pkt)
{

    char buffer[PAYLOAD_MAX_SIZE];
    strcpy(buffer, pkt.toString().c_str());
    int response = sendto(socketfd, buffer, PAYLOAD_MAX_SIZE, 0, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr));

    if (response < 0)
        std::cout << "ERROR writing to socket: " << socketfd << std::endl;

    return response;
}
