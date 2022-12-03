//
// Created by sdutton on 29.11.22.
//

#include "IUpdaterCtr.h"

#ifndef UPDATEDEMO_UPDATEREQUEST_H
#define UPDATEDEMO_UPDATEREQUEST_H

typedef struct Requests{
    Envelope envelope;
    Version version;
    Action cmd;
    char commandAux[MAX_BUFFER];
    char msgBuffer[MAX_MSG_BUFFER];
    char symbolName[MAX_BUFFER];
    char libPath[MAX_PATH];
    char stackCookie[STACK_COOKIE_LEN];
} Request, *pRequest;

void initRequest(pRequest);
void signRequest(pRequest);
int sendRequest(int,pRequest,void*,void (*)(int*,int*));
int checkRequest(pRequest);


#endif //UPDATEDEMO_UPDATEREQUEST_H

