//
// Created by sdutton on 02.12.22.
//

#include <stdio.h>
#include "Response.h"
#include "string.h"

void initResponse(pResponse);
void signResponse(pResponse);
int checkResponse(pResponse);
void sendResponse(int fd, void* request, pResponse response,void (*)(int*,int*));

void initResponse(pResponse resp){
    resp->envelope = RESPONSE;
    resp->version=V1;
    sprintf(resp->libPath,LIB_DEFAULT_PATH,V1,V1);
    sprintf(resp->currentSymbolName,SYM_DEFAULT_NAME);
    resp->responseTo = WhatIsTheCurrentVersion;
    resp->status = SUCCESS;
}