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
#include "Request.h"
#include "Response.h"
#include "Handler.h"

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int runStatus = 1;

void getCurrentVersion(int sockfd, pRequest request, pResponse response)
{
    //write request to socket
    bzero(response, sizeof(Response));
    write(sockfd, request, sizeof(Request));
    read(sockfd, response, sizeof(Response));
    printf("From Server : %s", updateReq);

}



int main()
{

    int sockfd, connfd;

    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("connection with update server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for update Request


    pRequest req = malloc(sizeof(Request));
    pResponse resp = malloc(sizeof(Response));

    bzero(req,sizeof(Request));
    bzero(resp,sizeof(Response));


    strcpy(req->stackCookie, STACK_COOKIE_FAKE);
    strcpy(req->functionName, "getVersionMessage");
    strcpy(req->libPath, "./libmessageV1.so");
    printf("Enforce Default values for Demo Startup: %s; %s \n", req->libPath, req->functionName);

    while(runStatus){
        sendRequest(sockfd, req, resp);
        callClientHandler(req, resp);

    }


    // close the socket
    free(req);
    free(resp);
    close(sockfd);
}