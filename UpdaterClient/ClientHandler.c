//
// Created by sdutton on 29.11.22.
//
#include <unistd.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include "Handler.h"
#include "Request.h"
#include "Response.h"

// dlopen, dlsym, dlclose

//handler event delegate
typedef void* (*CallRouter)(pRequest,pResponse) ;

//Prototypes
void* onVersionResponse(pRequest req, pResponse resp);
void* onShutdownResponse(pRequest req, pResponse resp);
void* onAuxiliaryResponse(pRequest req, pResponse resp);
void* onUpdateResponse(pRequest req, pResponse resp);
void* onMessageResponse(pRequest req, pResponse resp);



//Implementations
void* callHandler(pRequest req, pResponse resp, pStateBlock block)
{
    CallRouter router;
    switch (resp->responseTo) {
        case WhatIsTheCurrentVersion:
            router = onVersionResponse;
            break;

        case Shutdown:
            router = onShutdownResponse;
            block->action = Shutdown;
            block->trunkHandler(block);
            break;

        case Aux:
            router = onAuxiliaryResponse;
            break;

        case Update:
            router = onUpdateResponse;
            break;

        case DisplayMessage:
            router = onMessageResponse;
            break;
    }

    router(req,resp);
}

void* onVersionResponse(pRequest req, pResponse resp){

    VersionMessageHandler versionMessageHandler;
    void* hLib = dlopen(resp->libPath, RTLD_LAZY);
    if(hLib == NULL){
        printf("The message version %u cannot be loaded", resp->version);
        return 0;
    }
    void* data = "Any data";
    versionMessageHandler = (VersionMessageHandler)dlsym(hLib, req->symbolName);

    //risky on main thread... let's crash the client
    char * restrict theMessage = versionMessageHandler(data);
    printf("Message version received from server: %s \r", theMessage);
    dlclose(hLib);
}

void* onShutdownResponse(pRequest req, pResponse resp){
    printf("Closing down.\nClient will exit.\n");
}

void* onMessageResponse(pRequest req, pResponse resp){
    printf("Message response from server. the message:\n%s\n was displayed\n", resp->msgBuffer);
}

void* onAuxiliaryResponse(pRequest req, pResponse resp){
    printf("Auxiliary response: %s", resp->commandAux);
}

void* onUpdateResponse(pRequest req, pResponse resp){
    printf("updated library path %s", resp->libPath);
    printf("updated symbol name %s, from previous %s", resp->symbolName, resp->previousSymbolName);
}
