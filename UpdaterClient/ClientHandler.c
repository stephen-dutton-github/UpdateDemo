//
// Created by sdutton on 29.11.22.
//

#include <stdio.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <time.h>
#include <dlfcn.h>
#include <stddef.h>
#include <fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <errno.h>
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

extern int errno ;

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
    char tempSoFilePath[22];
    unsigned char tempSoFileData[resp->assemblyDataSize];
    struct tm* tm_info;
    struct timeval tv;

    //---- Create Timestamp
    gettimeofday(&tv, NULL);
    tm_info = localtime(&tv.tv_sec);
    strftime(tBuffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);
    /*
        //--- Create temp file for Data Stream
        strcpy(tempSoFilePath, "/tmp/lib_temp_XXXXXXX");

        int fd = mkstemp(tempSoFilePath);
        FILE* tempSo = fdopen(fd, "r+");

        //Crack open tempFile and use it as a lib.so type file
        write(fd,resp->assemblyData, sizeof(unsigned char) * resp->assemblyDataSize+1);
        close(fd);
    */

    VersionMessageHandler versionMessageHandler;
    if(resp->libPath[0] == '\0'){
        strcpy(resp->libPath, req->libPath);
    }

    if(resp->symbolName[0] == '\0'){
        strcpy(resp->symbolName, req->symbolName);
    }

    void* hLib = dlopen(resp->libPath, RTLD_LAZY);
    if(hLib == NULL){
        printf("loading error %s", dlerror());
        return 0;
    }

    void* data = "Any data";
    versionMessageHandler = (VersionMessageHandler)dlsym(hLib, resp->symbolName);

    //----- risky on main thread... let's not crash the client (now threaded)
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

}

void* onUpdateResponse(pRequest req, pResponse resp){
    printf("Update requested: %s", resp->msgBuffer);
    onVersionResponse(req,resp);
}

void* onShutdownServerResponse(pRequest req, pResponse resp){

}

