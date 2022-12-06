//
// Created by sdutton on 29.11.22.
//

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "ClientHandler.h"
#include "Request.h"
#include "Response.h"

// dlopen, dlsym, dlclose

//handler event delegate
typedef void* (*CallRouter)(pRequest,pResponse,pStateBlock block) ;

//Prototypes
void* onVersionRequest(pRequest req, pResponse resp, pStateBlock block);
void* onShutdownRequest(pRequest req, pResponse resp, pStateBlock block);
void* onAuxiliaryRequest(pRequest req, pResponse resp, pStateBlock block);
void* onUpdateRequest(pRequest req, pResponse resp, pStateBlock block);
void* onMessageRequest(pRequest req, pResponse resp, pStateBlock block);



//Implementations
void* callServerHandler(pRequest req, pResponse resp, pStateBlock block)
{
    CallRouter router;
    switch (resp->responseTo) {
        case WhatIsTheCurrentVersion:
            router = onVersionRequest;
            break;

        case Shutdown:
            router = onShutdownRequest;
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
    strcpy(resp->currentFunctionName, block->symbolName);
    strcpy(resp->libPath,block->path);
    resp->version = block->version;
    resp->responseTo = block->action;
}

void* onShutdownRequest(pRequest req, pResponse resp, pStateBlock block){
    block->trunkHandler(block);
};
void* onAuxiliaryRequest(pRequest req, pResponse resp, pStateBlock block){

    block->trunkHandler(block);
};
void* onUpdateRequest(pRequest req, pResponse resp, pStateBlock block){
    block->symbolName = req->symbolName;
    block->path = req->libPath;
    block->version = req->version;
    block->action = req->cmd;

};
void* onMessageRequest(pRequest req, pResponse resp, pStateBlock block){
    block->trunkHandler(block);
};