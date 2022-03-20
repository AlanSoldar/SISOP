#include "../libraries/Server.hpp"

using namespace std;

Server::Server()
{
    this->database = Database();
    this->notificationIdCounter = 0;
    mutexSession = PTHREAD_MUTEX_INITIALIZER;
    followMutex = PTHREAD_MUTEX_INITIALIZER;
    mutexCommunication = PTHREAD_MUTEX_INITIALIZER;

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
    mutexCommunication = PTHREAD_MUTEX_INITIALIZER;

    pthread_cond_init(&condNotificationEmpty, NULL);
    pthread_cond_init(&condNotificationFull, NULL);
    pthread_mutex_init(&mutexNotificationSender, NULL);
}

bool Server::openSession(string user, host_address address)
{
    pthread_mutex_lock(&mutexSession);

    if (!userExists(user))
    {
        sem_t num_sessions;
        sem_init(&num_sessions, 0, 2);
        userSessionsSemaphore.insert({user, num_sessions}); // user is created with 2 sessions available
        sessions.insert({user, list<host_address>()});
        //followers.insert(pair<string, list<string>>(user, list<string>()));
        //usersUnreadNotifications.insert({user, list<uint32_t>()});
    }

    int session_started = sem_trywait(&(userSessionsSemaphore[user])); // try to consume a session resource
    if (session_started == 0)                                          // 0 if session started, -1 if not
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
    if (it != sessions[user].end()) // remove address from sessions map and < (ip, port), notification to send >
    {
        sessions[user].erase(it);
        //active_users_pending_notifications.erase(address);

        // signal semaphore
        sem_post(&(userSessionsSemaphore[user]));
    }
    pthread_mutex_unlock(&mutexSession);
}

void *Server::communicationHandler(void *handlerArgs)
{
    struct communiction_handler_args *args = (struct communiction_handler_args *)handlerArgs;

    string userToFollow;
    string message;
    string response;

    cout << "starting to listen to messages" << endl;

    while (1)
    {
        Packet *receivedPacket = args->connectedSocket->readPacket();

        if (receivedPacket)
        {
            int type = receivedPacket->getType();
            switch (type)
            {

            case FOLLOW_USER:
                //userToFollow = receivedPacket->getPayload();
                //response = "Followed " + userToFollow + "!";

                args->server->database.saveNewFollow(receivedPacket->getUser(), receivedPacket->getPayload());
                cout << "following new user" << endl;

                //args->connectedSocket->sendPacket(Packet(ERROR, response.c_str()));
                break;

            case SEND_NOTIFICATION:
                //message = receivedPacket->getPayload();
                cout << "new notification" << endl;
                args->server->database.saveNotification(receivedPacket->getUser(), receivedPacket->getPayload());

                //args->connectedSocket->sendPacket(Packet(RECEIVE_NOTIFICATION, "Notification sent!"));
                break;

            default:
                break;
            }
        }
    }
    return NULL;
}

bool Server::userExists(string user)
{
    return userSessionsSemaphore.find(user) != userSessionsSemaphore.end();
}

bool Server::isUserActive(string user)
{
    return sessions.find(user) != sessions.end() && !(sessions[user].empty());
}
