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

int sendRequest(int fd, pRequest request, void* response, void (*pgHandler)(int*,int*))
{
    //write request to socket

    int pSizeRequest = sizeof(Request);
    int pSizeResponse = sizeof(Response);
    int progress = 0, lastValue;
    bzero(response, pSizeResponse);

    printf("Sending request...\n");
    while(pSizeRequest > progress) {
        progress += write(fd, request, pSizeRequest);
        if(pgHandler != NULL) {
            pgHandler(&progress, &pSizeRequest);
        }
    }

    printf("Await Response...\n");
    progress= lastValue =0;
    while(pSizeResponse > progress) {
        progress += lastValue = read(fd, request, pSizeResponse);
        if(lastValue != 0) {
            if (pgHandler != NULL) {
                pgHandler(&progress, &pSizeResponse);
            }
        }
    }

    while
    read(fd, response, pSizeResponse);
    return 0;
}
