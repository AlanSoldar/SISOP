#include "../libraries/Socket.hpp"

int Socket::getSocketfd(){
    return this->socketfd;
} 

Socket::~Socket(){
    std::cout << "Closing socketfd...\n";
    close(this->socketfd);
}

Socket::Socket(){
    if ((this->socketfd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        std::cout << "ERROR opening the socket\n" << std::endl;
        exit(1);
    }
}

Socket::Socket(int socketfd){
    this->socketfd = socketfd;
}

// returns a pointer that points to the read Packet or NULL if the connection was closed
Packet* Socket::readPacket(int socketfd){

    Packet* pkt = new Packet();
    memset(pkt, 0, sizeof (Packet));
    int buffer = read(socketfd, pkt, sizeof(Packet));
    
    if (buffer < 0){
        std::cout << "ERROR reading from socket: " << socketfd  << std::endl;
        return NULL;
    }

    else if(buffer == 0){
        std::cout << "Connection closed." << std::endl;
        return NULL;
    }

    return pkt;
}

Packet* Socket::readPacket(){

    Packet* pkt = new Packet();
    memset(pkt, 0, sizeof (Packet));
    int buffer = read(this->socketfd, pkt, sizeof(Packet));

    if (buffer<0){
        std::cout << "ERROR reading from socket: " << this->socketfd  << std::endl;
        return NULL;
    }

    else if(buffer == 0){
        std::cout << "Connection closed." << std::endl;
        return NULL;
    }

    return pkt;
}

// overloading for non-initialized Socket object
int Socket::sendPacket(Packet pkt, int socketfd){
    int buffer = send(socketfd, &pkt, sizeof(pkt), MSG_NOSIGNAL);

    if (buffer < 0) {
        std::cout << "ERROR writing to socket: " << this->socketfd << std::endl;
        std::cout << "Connection closed." << std::endl;
    }
    
    return buffer;
}

// return the buffer value from send primitive
int Socket::sendPacket(Packet pkt){
    int buffer = send(this->socketfd, &pkt, sizeof(pkt), MSG_NOSIGNAL); 

    if (buffer < 0) 
        std::cout << "ERROR writing to socket: " << this->socketfd << std::endl;
    return buffer;
}