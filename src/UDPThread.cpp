#include "../libraries/UDPThread.hpp"
#include "../libraries/Defines.hpp"
#include "strings.h"
#include "string.h"

UDPThread::UDPThread()
{
    // variable initialization
    for (int i = 0; i < 5; i++)
        sensor_data[i] = 0.0;

    // pthread initialization
    _thread = pthread_self();                  // get pthread ID
    pthread_setschedprio(_thread, SCHED_FIFO); // setting priority

    // log file initialization
    if (UDP_FILE_LOG_FLAG == 1)
        ofp = fopen("sensordata.txt", "w");

    // udp initialization
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
}

UDPThread::~UDPThread()
{
    pthread_join(_thread, NULL); // close the thread
    close(sockfd);               // close UDP socket
    fclose(ofp);                 // close log file
}

double UDPThread::getTimeElapsed(struct timeval end, struct timeval start)
{
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.00;
}

// Returns true if the thread was successfully started, or false if there was an error starting the thread
bool UDPThread::StartInternalThread()
{
    return (pthread_create(&_thread, NULL, InternalThreadEntryFunc, this) == 0);
}

// Will not return until the internal thread has exited.
void UDPThread::WaitForInternalThreadToExit()
{
    (void)pthread_join(_thread, NULL);
}

void UDPThread::InternalThreadEntry()
{

    gettimeofday(&tvalInit, NULL);

    for (;;)
    {

        len = sizeof(cliaddr);
        n = recvfrom(sockfd, msg, 1000, 0, (struct sockaddr *)&cliaddr, &len);

        msg[n] = 0;

        string str_sensor_data(msg);
        stringstream stream(str_sensor_data);

        for (int i = 0; i < 5; i++)
        {
            stream >> sensor_data[i];
        }

        if (UDP_FILE_LOG_FLAG == 1)
        {
            gettimeofday(&tvalNow, NULL);
            double time_elapsed = getTimeElapsed(tvalNow, tvalInit);
            fprintf(ofp, "%lf\t%s\n", time_elapsed, msg);
        }
    }
}

void *UDPThread::InternalThreadEntryFunc(void *This)
{
    ((UDPThread *)This)->InternalThreadEntry();
    return NULL;
}