//
// Created by sdutton on 29.11.22.
//

#include "IUpdaterCtr.h"

#ifndef UPDATEDEMO_UPDATERESPONSE_H
#define UPDATEDEMO_UPDATERESPONSE_H

typedef struct Responses{
    Envelope envelope;
    Version version;
    Action responseTo;
    Status status;
    char functionName[MAX_BUFFER];
    char libPath[MAX_PATH];
    char data[MAX_DATA];
    char currentFunctionName[MAX_BUFFER];
    char stackCookie[STACK_COOKIE_LEN];
} Response, *pResponse;

void initResponse(pResponse);
void signResponse(pResponse);
int checkResponse(pResponse);
void sendResponse(int fd, void* request, pResponse response, void (*)(int*,int*));

#endif //UPDATEDEMO_UPDATERESPONSE_H

