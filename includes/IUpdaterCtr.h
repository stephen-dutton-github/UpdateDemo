//
// Created by sdutton on 29.11.22.
//

#include <stdlib.h>

#ifndef UPDATEDEMO_ABIDEFINITION_H
#define UPDATEDEMO_ABIDEFINITION_H

#define MAX_PATH 4096
#define MAX_DATA 4096
#define MAX_BUFFER 64
#define MAX_MSG_BUFFER 140
#define STACK_COOKIE_LEN 40
#define STACK_COOKIE_FAKE "59c35cf155a17f850a160956e13ecdb4be6a500"    //would be supplied by hashing function
#define SYMBOL_NAME_DEFAULT "../messageV%u/libmessageV%u.so"
typedef enum {V1=1,V2=2,V3=3} Version;
typedef enum {WhatIsTheCurrentVersion, Update, Shutdown, DisplayMessage, Aux } Action;
typedef enum {SUCCESS, FAILURE} Status;
typedef char* restrict (*VersionMessageHandler)(void*);
char* restrict getVersionMessage(void*);

#endif //UPDATEDEMO_ABIDEFINITION_H

