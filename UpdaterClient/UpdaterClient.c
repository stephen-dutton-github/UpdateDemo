//
// Created by sdutton on 29.11.22.
//
#include <arpa/inet.h> // inet_addr()
#include <pthread.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include "Connection.h"
#include "Request.h"
#include "Response.h"
#include "Handler.h"

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define SERVER_EXE "../UpdaterService/UpdaterService &"


typedef struct __threadArgs{
    int* fd;
    pRequest req;
    pResponse rep;
    void* (*heartbeatCallBack)(void*);
} tArgs, *pTArgs;

int initClientConnection(void* data){
    int sfd;
    struct sockaddr_in serverAddress;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    printf("Socket successfully created..\n");
    bzero(&serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(PORT);

    int retry = 0;
    int server = 0;

    if (connect(sfd, (SA *) &serverAddress, sizeof(serverAddress)) != 0)
    {
        printf("Connection failed... \nTrying to start server...\n Please Wait..\n");
        exit(-1);
    }
    printf("connected to the server..\n");

    return sfd;
}
void closeConnection(int fd){
    close(fd);
    printf("server connection closed.. (and any other clear up)\n");
}
void onTrunkRequest(pStateBlock block){

}
void onProgress(int* current, int* target){
    double descr=100.0*((double)*current);
    double denom=100.0*((double)*target);
    double perc= (descr / denom);
    printf(PROGRESS_MASK, perc);
}
void* __heartBeat(void*);
void* __heartBeatCallback(void*);


int runStatus = 1;
pthread_t tid;
pthread_mutex_t lock;

int main()
{
    if(pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init failed\n");
        return -1;
    }

    int sfd=-1;
    pRequest req = malloc(sizeof(Request));
    pResponse rep = malloc(sizeof(Response));
    pTArgs threadArgs = malloc(sizeof(struct __threadArgs));
    bzero(req,sizeof(Request));
    initRequest(req);
    signRequest(req);

    threadArgs->fd = &sfd;
    threadArgs->rep=rep;
    threadArgs->req=req;
    threadArgs->heartbeatCallBack = __heartBeatCallback;

    pthread_create(&tid,NULL,&__heartBeat,(void*)threadArgs);

    pthread_join(tid, NULL);

    /*while(runStatus){
        sfd = initClientConnection(NULL);
        sendRequest(sfd, req, rep);
        callHandler(req, rep, NULL);
    }*/

    //dispose of reserved memory
    pthread_mutex_destroy(&lock);
    free(threadArgs);
    free(req);
    free(rep);
    closeConnection(sfd);
}

void* __heartBeat(void* data){

    pTArgs pArgs = (pTArgs)data;

    while(runStatus){
        //Grab the lock
        pthread_mutex_lock(&lock);
        *pArgs->fd = initClientConnection(NULL);
        sendRequest(*pArgs->fd, pArgs->req, pArgs->rep);
        callHandler(pArgs->req, pArgs->rep, NULL);
        //release access to input function
        pthread_mutex_unlock(&lock);
        sleep(5);
    }

}


void* __hbCallback(void* data){

}