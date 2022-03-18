#include "../libraries/Server.hpp"

using namespace std;

Server::Server()
{
    this->notificationIdCounter = 0;
    mutexSession = PTHREAD_MUTEX_INITIALIZER;
    followMutex = PTHREAD_MUTEX_INITIALIZER;
    followerCountMutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_cond_init(&condNotificationEmpty, NULL);
    pthread_cond_init(&condNotificationFull, NULL);
    pthread_mutex_init(&mutexNotificationSender, NULL);
}

Server::Server(host_address address)
{
    this->notificationIdCounter = 0;
	this->ip = address.ipv4;
	this->port = address.port;

    mutexSession = PTHREAD_MUTEX_INITIALIZER;
    followMutex = PTHREAD_MUTEX_INITIALIZER;
    followerCountMutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_cond_init(&condNotificationEmpty, NULL);
    pthread_cond_init(&condNotificationFull, NULL);
    pthread_mutex_init(&mutexNotificationSender, NULL);
}


// se não tiver um host_address, eu posso fazer e devolver um session_id
bool Server::openSession(string user, host_address address)
{
    pthread_mutex_lock(&mutexSession);

    if(!userExists(user))
    {
        sem_t num_sessions;
        sem_init(&num_sessions, 0, 2);
        userSessionsSemaphore.insert({user, num_sessions}); // user is created with 2 sessions available
        sessions.insert({user, list<host_address>()});
        followers.insert(pair<string, list<string>>(user, list<string>()));
        usersUnreadNotifications.insert({user, list<uint32_t>()});
    } 
    
    int session_started = sem_trywait(&(userSessionsSemaphore[user])); // try to consume a session resource
    if(session_started == 0) // 0 if session started, -1 if not
    { 
        sessions[user].push_back(address);
        active_users_pending_notifications.insert({address, priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>>()});
    }
    pthread_mutex_unlock(&mutexSession); // Fim SC
    return session_started == 0; 
}

// call this function when client presses ctrl+c or ctrl+d
void Server::closeSession(string user, host_address address) 
{
    pthread_mutex_lock(&mutexSession);
    
    list<host_address>::iterator it = find(sessions[user].begin(), sessions[user].end(), address);
    if(it != sessions[user].end()) // remove address from sessions map and < (ip, port), notification to send > 
    {
        sessions[user].erase(it);
        active_users_pending_notifications.erase(address);

        // signal semaphore
        sem_post(&(userSessionsSemaphore[user]));
    }
    pthread_mutex_unlock(&mutexSession);
}

ServerSocket::ServerSocket() : Socket(){
    
    this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(PORT);
	this->serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(this->serv_addr.sin_zero), 8);

}

void ServerSocket::connectNewClient(pthread_t *threadID, Server* server){

    int *newsockfd = (int *) calloc(1, sizeof(int));
    Socket *newClientSocket = (Socket *) calloc(1, sizeof(Socket));
	socklen_t clilen;
	struct sockaddr_in cli_addr;
    host_address client_address;
    string user;
    

    std::cout << "test1" << std::endl;
    // Accepting connection to start communicating
    clilen = sizeof(struct sockaddr_in);
    if ((*newsockfd = accept(this->getSocketfd(), (struct sockaddr *) &cli_addr, &clilen)) == -1) {
        std::cout << "ERROR on accepting client connection" << std::endl;
        return;
    }
        std::cout << "test" << std::endl;
    newClientSocket = new Socket(*newsockfd);

    std::cout << "New connection estabilished on socket: " << *newsockfd << "\n\n";
    
    // Verify if there are free sessions available
    // read client username from socket in 'user' var
    Packet *userPacket = newClientSocket->readPacket();

    if (userPacket == NULL){
        std::cout << "Unable to read user information. Closing connection." << std::endl;
        return;     // destructor automatically closes the socket
    } else 
        user = userPacket->getPayload();
    

    client_address.ipv4 = inet_ntoa(cli_addr.sin_addr);
    client_address.port = ntohs(cli_addr.sin_port);
    bool sessionAvailable = server->openSession(user, client_address);

    
    Packet sessionResultPkt;
    if (!sessionAvailable){
        sessionResultPkt = Packet(OPEN_SESSION_FAIL, "Unable to connect to server: no sessions available.");
        newClientSocket->sendPacket(sessionResultPkt);
        return; // destructor automatically closes the socket
    } else{
        sessionResultPkt = Packet(OPEN_SESSION_SUCCESS, "Connection succeded! Session established.");
        newClientSocket->sendPacket(sessionResultPkt);
    }
    
    // Build args
    communiction_handler_args *args = (communiction_handler_args *) calloc(1, sizeof(communiction_handler_args));
    args->client_address = client_address;
    args->connectedSocket = newClientSocket;
    args->user = user;
    args->server = server;

    pthread_create(threadID, NULL, Server::communicationHandler, (void *)args);
}


void ServerSocket::bindAndListen(){
    
    if (bind(this->getSocketfd(), (struct sockaddr *) &this->serv_addr, sizeof(this->serv_addr)) < 0) {
		cout << "ERROR on binding\n";
        exit(1);
    }
	
	listen(this->getSocketfd(), MAX_TCP_CONNECTIONS);
	std::cout << "Listening..." << "\n\n";
}




void *Server::communicationHandler(void *handlerArgs){

    pthread_t readCommandsT;
    pthread_t sendNotificationsT;

    pthread_create(&readCommandsT, NULL, Server::readCommandsHandler, handlerArgs);

    pthread_join(readCommandsT, NULL);
    pthread_join(sendNotificationsT, NULL);

    return NULL;
}


void *Server::readCommandsHandler(void *handlerArgs){
	struct communiction_handler_args *args = (struct communiction_handler_args *)handlerArgs;

    string userToFollow;
    string response;

    while(1){
        Packet* receivedPacket = args->connectedSocket->readPacket();
        if (receivedPacket == NULL){  // connection closed
            args->server->closeSession(args->user, args->client_address);
            return NULL;
        }
        cout << receivedPacket->getPayload() << "\n\n";

        switch(receivedPacket->getType()){

            case FOLLOW_USER:
                userToFollow = receivedPacket->getPayload();
                response = "Followed "+userToFollow+"!";

                printf("test follow");
                args->connectedSocket->sendPacket(Packet(ERROR, response.c_str()));
                break;

            case SEND_NOTIFICATION:
                printf("test send");
                args->connectedSocket->sendPacket(Packet(ERROR, "Notification sent!"));
                break;

            default:
                break;
        }
    }
}

bool Server::userExists(string user)
{
    return userSessionsSemaphore.find(user) != userSessionsSemaphore.end();
}

bool Server::isUserActive(string user) 
{
    return sessions.find(user) != sessions.end() && !(sessions[user].empty());
}