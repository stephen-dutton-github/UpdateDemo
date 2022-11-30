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

void* callClientHandler(pRequest requ, pResponse resp)
{
    void* hLib = dlopen(requ->libPath, RTLD_LAZY);
    if(hLib == NULL){
        printf("The message version %s cannot be loaded", resp->version);
    }

}