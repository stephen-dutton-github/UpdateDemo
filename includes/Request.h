//
// Created by sdutton on 29.11.22.
//

#include "IUpdaterCtr.h"

#ifndef UPDATEDEMO_UPDATEREQUEST_H
#define UPDATEDEMO_UPDATEREQUEST_H

typedef struct Requests{
    Version ver;
    Action cmd;
    char commandAux[MAX_BUFFER];
    char msgBuffer[MAX_MSG_BUFFER];
    char functionName[MAX_BUFFER];
    char libPath[MAX_PATH];
    char stackCookie[STACK_COOKIE_LEN];
} Request, *pRequest;



#endif //UPDATEDEMO_UPDATEREQUEST_H

void initRequest(pRequest req);
void signRequest(pRequest);
int checkRequest(pRequest);
void sendRequest(int fd, pRequest request, void* response);