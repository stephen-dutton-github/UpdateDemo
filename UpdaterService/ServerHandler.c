//
// Created by sdutton on 29.11.22.
//

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include "ClientHandler.h"
#include "Request.h"
#include "Response.h"

// dlopen, dlsym, dlclose

//handler event delegate
typedef void* (*CallRouter)(pRequest,pResponse) ;

//Prototypes
void* onVersionRequest(pRequest req, pResponse resp);
void* onShutdownRequest(pRequest req, pResponse resp);
void* onAuxiliaryRequest(pRequest req, pResponse resp);
void* onUpdateRequest(pRequest req, pResponse resp);
void* onMessageRequest(pRequest req, pResponse resp);



//Implementations
void* callServerHandler(pRequest req, pResponse resp, ApplicationTrunkHandler ath)
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
    router(req,resp);
}
void* onVersionRequest(pRequest req, pResponse resp){};
void* onShutdownRequest(pRequest req, pResponse resp){};
void* onAuxiliaryRequest(pRequest req, pResponse resp){};
void* onUpdateRequest(pRequest req, pResponse resp){};
void* onMessageRequest(pRequest req, pResponse resp){};