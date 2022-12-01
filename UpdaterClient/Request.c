//
// Created by sdutton on 01.12.22.
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


void initRequest(pRequest req)
{
    req->ver = V1;
    req->cmd =WhatIsTheCurrentVersion;
    strcpy(req->functionName, "getVersionMessage");
    sprintf(req->libPath,SYMBOL_NAME_DEFAULT,req->ver,req->ver);
    printf("Enforce Default values for Demo Startup: %s; %s \n", req->libPath, req->functionName);
}


void signRequest(pRequest req){
    //digest authentication on the struct
    bzero(req->stackCookie, sizeof(char) * STACK_COOKIE_LEN);
    strcpy(req->stackCookie,STACK_COOKIE_FAKE);
}

int checkRequest(pRequest){
    //Some hashing logic check
    return 1;
}

void sendRequest(int fd, pRequest request, void* response)
{
    //write request to socket
    bzero(response, sizeof(Response));
    write(fd, request, sizeof(Request));
    read(fd, response, sizeof(Response));
}
