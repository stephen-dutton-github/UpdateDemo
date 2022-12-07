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
    int runStatus;
    int* fd;
    pRequest req;
    pResponse rep;
    void* (*heartBeatCallBack)(void*);
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
void* heartBeat(void*);
void* heartBeatCallback(void*);

int runStatus = 1;
pthread_t tid;
pthread_mutex_t lock, cb_lockl;



int main()
{


    if(pthread_mutex_init(&lock, NULL) != 0){
        printf("server mutex init failed\n");
        return -1;
    }

    if(pthread_mutex_init(&cb_lockl, NULL) != 0){
        printf("callback mutex init failed\n");
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
    threadArgs->heartBeatCallBack = heartBeatCallback;
    {
        pthread_create(&tid, NULL, &heartBeat, (void *) threadArgs);
    }
    //allow for startup server query
    sleep(2);

    while(runStatus){

        pthread_mutex_lock(&lock);
            char inputBuffer[2];
            char previous[1];
            printf("Options: 1-4\n");
            printf("1). Set implementation Version to V1 \n");
            printf("2). Set implementation Version to V2 \n");
            printf("3). Quit the Client Application\n");
            printf("4). Close Server and Client\n");
            while(1)
            {
                ///TODO:Modify buffer for UNICODE standard
                fgets(inputBuffer, sizeof(inputBuffer), stdin);
                if(inputBuffer[0] =='\n')
                    break;
                previous[0] =inputBuffer[0];
            }

            if(atoi(previous) < 1 || atoi(previous) > 4 ){
                inputBuffer[0] = '\0';
                printf("Ignored\n");
                //prevent locking from a synchronized context in case of continuation
                pthread_mutex_unlock(&lock);
                continue;
            }

        printf("Option %d selected please wait...\n",atoi(previous));
            switch(atoi(previous)){
                case 1:
                    threadArgs->req->cmd = Update;
                    threadArgs->req->version = V1;
                    threadArgs->req->version = V1;
                    sprintf(threadArgs->req->libPath,LIB_DEFAULT_PATH,V1,V1);
                    break;
                case 2:
                    threadArgs->req->cmd = Update;
                    threadArgs->req->version = V2;
                    sprintf(threadArgs->req->libPath,LIB_DEFAULT_PATH,V2,V2);
                    break;
                case 3:
                    threadArgs->req->cmd = Shutdown;
                    break;
                case 4:
                    threadArgs->req->cmd = ShutDownServer;
                    break;
            }
        pthread_mutex_unlock(&lock);
        sleep(5);
    }

    pthread_join(tid, NULL);
    //dispose of reserved memory
    pthread_mutex_destroy(&lock);
    free(threadArgs);
    free(req);
    free(rep);
    closeConnection(sfd);
}

void* heartBeat(void* data){
    pTArgs pArgs = (pTArgs)data;

    while(runStatus){
        //Grab the lock
        pthread_mutex_lock(&lock);
        {
            *pArgs->fd = initClientConnection(NULL);
            sendRequest(*pArgs->fd, pArgs->req, pArgs->rep);
            callHandler(pArgs->req, pArgs->rep, NULL);
            pArgs->heartBeatCallBack(pArgs);
        }
        //release access to input function
        pthread_mutex_unlock(&lock);
        sleep(3);
    }

}


void* heartBeatCallback(void* data){
    //
}