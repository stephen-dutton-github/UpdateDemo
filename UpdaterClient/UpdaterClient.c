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

int runStatus = 1;

int initClientConnection(void* data){
    int socketFileDesc;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    socketFileDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDesc == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(socketFileDesc, (SA*)&servaddr, sizeof(servaddr))  != 0) {
        printf("connection with update server failed... \nEnsure the server is started.\n");
        exit(0);
    }

    printf("connected to the server..\n");
    return socketFileDesc;
}
void closeConnection(int fd){
    close(fd);
    printf("server connection closed.. (and any other clear up)\n");
}
int main()
{
    int socketFileDesc=-1;
    pRequest req = malloc(sizeof(Request));
    pResponse resp = malloc(sizeof(Response));

    bzero(req,sizeof(Request));

    initRequest(req);
    signRequest(req);
    //socketFileDesc = initClientConnection(NULL);

    while(runStatus){
        //sendRequest(socketFileDesc, req, resp);
        callClientHandler(req, resp);
    }

    //dispose of reserved memory
    free(req);
    free(resp);
    closeConnection(socketFileDesc);

}


