//
// Created by sdutton on 29.11.22.
//
#include <arpa/inet.h> // inet_addr()
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

volatile int runStatus = 1;

pStateBlock sblock;

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

int main()
{
    int sfd=-1;
    pRequest req = malloc(sizeof(Request));
    pResponse resp = malloc(sizeof(Response));
    sblock = malloc(sizeof(struct TrunkStateBlock));
    sblock->trunkHandler = onTrunkRequest;
    sblock->progressHandler = onProgress;

    bzero(req,sizeof(Request));
    initRequest(req);
    signRequest(req);


    while(runStatus){
        sfd = initClientConnection(NULL);
        runStatus = sendRequest(sfd, req, resp, sblock);
        callHandler(req, resp, sblock);
    }

    //dispose of reserved memory
    free(req);
    free(resp);
    closeConnection(sfd);
}


