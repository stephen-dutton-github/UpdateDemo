//
// Created by sdutton on 29.11.22.
//

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include "Handler.h"
#include "Request.h"
#include "Response.h"

// dlopen, dlsym, dlclose

VersionMessageHandler versionMessageHandler;

void* callClientHandler(pRequest requ, pResponse resp)
{
    void* hLib = dlopen(requ->libPath, RTLD_LAZY);
    if(hLib == NULL){
        printf("The message version %u cannot be loaded", resp->version);
        return 0;
    }
    void* data = "Any data";
    versionMessageHandler = (VersionMessageHandler)dlsym(hLib,requ->functionName);

    //risky on main thread... let's crash the client
    char * restrict theMessage = versionMessageHandler(data); ///TODO: central callback to the underlying code asset
    printf("Message version received from server: %s \n", theMessage);

}