#ifndef UDPTHREAD_H_
#define UDPTHREAD_H_

#define UDP_FILE_LOG_FLAG 0

#include <iostream>     //to use cout
#include <sstream>      //to use parsing
#include <sys/socket.h> // for UDP sockets
#include <arpa/inet.h>  // for UDP sockets
#include <netdb.h>      // for UDP sockets
#include <unistd.h>     // for UDP sockets
#include <netinet/in.h> // for UDP
#include <stdio.h>      // to use the file outputs
#include <sys/time.h>   // to use time measurement

using namespace std;

class UDPThread
{

private:
    FILE *ofp;
    pthread_t _thread;
    int sockfd, n;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char msg[1000];

    struct timeval tvalNow, tvalInit;
    double getTimeElapsed(struct timeval end, struct timeval start);

protected:
    void InternalThreadEntry();

public:
    double sensor_data[4];
    bool StartInternalThread();
    void WaitForInternalThreadToExit();
    static void *InternalThreadEntryFunc(void *This);
    UDPThread();
    ~UDPThread();
};

#endif // UDPTHREAD_H_