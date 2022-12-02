//
// Created by sdutton on 29.11.22.
//

#include "Connection.h"
#include "Request.h"
#include "Response.h"
#include "Handler.h"

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int runStatus = 1;

// Driver function
int main()
{

    struct sockaddr_in addressLocal, addressClient;
    socklen_t addressLen;
    int cfd=0;  //Connection file descriptor
    int sfd=0;  //socket file descriptor

    pRequest req = malloc(sizeof(Request));
    pResponse resp = malloc(sizeof(Response));

    sfd = initServerConnection(NULL);

    addressLen = sizeof(addressClient);
    cfd = accept(sfd, (SA*)&addressClient, &addressLen);
    if (cfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }

    while(1){
        read(sfd, req,sizeof(Request));
        callServerHandler(req,resp);
    }

    printf("Server listening..\n");




    closeConnection(sfd);
}

void closeConnection(int fd){
    close(fd);
    printf("server connection closed.. (and any other clear up)\n");
}

int initServerConnection(int* pConnectionFileDesc){

    struct sockaddr_in addressLocal;
    socklen_t len;
    int sfd = 0;
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    bzero(&addressLocal, sizeof(addressLocal));
    addressLocal.sin_family = AF_INET;
    addressLocal.sin_addr.s_addr = htonl(INADDR_ANY);
    addressLocal.sin_port = htons(PORT);

    if ((bind(sfd, (SA*)&addressLocal, sizeof(addressLocal))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }

    // Now server is ready to listen and verification
    if ((listen(sfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }

    return sfd;
}