//
// Created by sdutton on 29.11.22.
//

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <dlfcn.h>
#include <stddef.h>
#include "Handler.h"
#include "Request.h"
#include "Response.h"

// dlopen, dlsym, dlclose

//handler event delegate
typedef void* (*CallRouter)(pRequest,pResponse) ;

//Prototypes
void* onVersionResponse(pRequest req, pResponse resp);
void* onShutdownResponse(pRequest req, pResponse resp);
void* onShutdownServerResponse(pRequest req, pResponse resp);
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
            break;

        case ShutDownServer:
            router = onShutdownServerResponse;
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

    char tBuffer[26];
    struct tm* tm_info;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tm_info = localtime(&tv.tv_sec);
    strftime(tBuffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);
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
    printf("\r \b \b");
    printf("Message version received from server: %s %s\n", theMessage, tBuffer);
    dlclose(hLib);
}

void* onShutdownResponse(pRequest req, pResponse resp){
    printf("\r \b \b");
    printf("Closing down.\nClient will exit.\n");
    exit(0);
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
    onVersionResponse(req,resp);
}

void* onShutdownServerResponse(pRequest req, pResponse resp){

}