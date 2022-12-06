//
// Created by sdutton on 01.12.22.
//

#include <stdio.h>
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include "Request.h"
#include "Response.h"

void initRequest(pRequest req)
{
    req->envelope = REQUEST;
    req->version = V1;
    req->cmd =WhatIsTheCurrentVersion;
    strcpy(req->symbolName, SYM_DEFAULT_NAME);
    sprintf(req->libPath, LIB_DEFAULT_PATH, req->version, req->version);
    printf("Enforce Default values for Demo Startup: %s; %s \n", req->libPath, req->symbolName);
}

void signRequest(pRequest req){
    //digest authentication on the struct
    bzero(req->stackCookie, sizeof(char) * STACK_COOKIE_LEN);
    strcpy(req->stackCookie,STACK_COOKIE_FAKE);
}
int checkRequest(pRequest req){
    //Some hashing logic check
    return 1;
}

int sendRequest(int fd, pRequest request, void* response) {
    //write request to socket

    int lastValue = 0;
    bzero(response, sizeof(Response));

    printf("Sending request...\n");
    lastValue = write(fd,request, sizeof(Request));
    if(lastValue < 0){
        printf("Error sending the packet\n");
        return -1;
    }

    lastValue = read(fd, response, sizeof(Response));
    printf("Response received...\n");

    if(lastValue < 0){
        printf("Error reading the packet\n");
        return -1;
    }

    return 0;
}
