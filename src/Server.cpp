#include "../libraries/Server.hpp"

using namespace std;

Server::Server()
{
    this->database = Database();
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
    this->database = Database();
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
        //followers.insert(pair<string, list<string>>(user, list<string>()));
        //usersUnreadNotifications.insert({user, list<uint32_t>()});
    } 
    
    int session_started = sem_trywait(&(userSessionsSemaphore[user])); // try to consume a session resource
    if(session_started == 0) // 0 if session started, -1 if not
    { 
        sessions[user].push_back(address);
        //active_users_pending_notifications.insert({address, priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>>()});
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
        //active_users_pending_notifications.erase(address);

        // signal semaphore
        sem_post(&(userSessionsSemaphore[user]));
    }
    pthread_mutex_unlock(&mutexSession);
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
    string message;
    string response;

    while(1){
        Packet* receivedPacket = args->connectedSocket->readPacket();
        if (receivedPacket == NULL){  // connection closed
            args->server->closeSession(args->user, args->clientAddress);
            return NULL;
        }
        cout << receivedPacket->getPayload() << receivedPacket->getType() << endl;

        switch(receivedPacket->getType()){

            case FOLLOW_USER:
                userToFollow = receivedPacket->getPayload();
                response = "Followed "+userToFollow+"!";

                args->server->database.saveNewFollow(args->user, userToFollow);

                args->connectedSocket->sendPacket(Packet(ERROR, response.c_str()));
                break;

            case SEND_NOTIFICATION:
                message = receivedPacket->getPayload();
                args->server->database.saveNotification(args->user, message);

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
