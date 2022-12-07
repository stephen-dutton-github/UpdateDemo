//
// Created by sdutton on 29.11.22.
//

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "Handler.h"
#include "Request.h"
#include "Response.h"
#include "VersionSwitch.h"

// dlopen, dlsym, dlclose

//handler event delegate
typedef void* (*CallRouter)(pRequest,pResponse,pStateBlock block) ;

//Prototypes
void* onVersionRequest(pRequest req, pResponse resp, pStateBlock block);
void* onShutdownRequest(pRequest req, pResponse resp, pStateBlock block);
void* onShutdownServerRequest(pRequest req, pResponse resp, pStateBlock block);
void* onAuxiliaryRequest(pRequest req, pResponse resp, pStateBlock block);
void* onUpdateRequest(pRequest req, pResponse resp, pStateBlock block);
void* onMessageRequest(pRequest req, pResponse resp, pStateBlock block);

//Implementations
void* callHandler(pRequest req, pResponse resp, pStateBlock block)
{
    CallRouter router;
    switch (req->cmd) {
        case WhatIsTheCurrentVersion:
            router = onVersionRequest;
            break;

        case Shutdown:
            router = onAuxiliaryRequest;
            break;

        case ShutDownServer:
            router = onShutdownServerRequest;
            break;

        case Aux:
            router = onAuxiliaryRequest;
            break;

        case Update:
            router = onUpdateRequest;
            break;

        case DisplayMessage:
            router = onMessageRequest;
            break;

    }
    router(req,resp,block);
}

void* onVersionRequest(pRequest req, pResponse resp, pStateBlock block){
    ///TODO: set up response indicating what the values of the server block are

    getBinaryVersion(req->version,block->assemblyData,&block->assemblyDataSize);
    strcpy(resp->symbolName, block->symbolName);
    strcpy(resp->libPath,block->libPath);
    resp->version = block->version;
    resp->responseTo = block->action;
    resp->assemblyDataSize = block->assemblyDataSize;
    strcpy(resp->assemblyData, block->assemblyData);
}

void* onShutdownRequest(pRequest req, pResponse resp, pStateBlock block){

    ///TODO: Think of some imaginary task domain
    printf("Remote client process ending...");
    resp->responseTo = Shutdown;

};

void* onShutdownServerRequest(pRequest req, pResponse resp, pStateBlock block){
    printf("Full shutdown, server process ending...");
    block->trunkHandler(block);
}

void* onAuxiliaryRequest(pRequest req, pResponse resp, pStateBlock block){
    ///TODO: Think of some imaginary task domain
    if(req->cmd == Shutdown){
        printf("Client shutdown, server remaining up...");
    }

    block->trunkHandler(block);
};
void* onUpdateRequest(pRequest req, pResponse resp, pStateBlock block){
    strcpy(block->symbolName,req->symbolName);
    strcpy(block->libPath, req->libPath);
    block->version = req->version;
    block->action = req->cmd;
    getBinaryVersion(req->version,req->assemblyData,&req->assemblyDataSize);
}


void* onMessageRequest(pRequest req, pResponse resp, pStateBlock block){
    printf("Message from Client: %s", req->msgBuffer);
};