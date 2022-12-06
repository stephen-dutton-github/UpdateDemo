//
// Created by sdutton on 29.11.22.
//
#define MSG_SIZE 1024

#include <stdio.h>
#include "strings.h"
#include "IUpdaterCtr.h"

char msg[MSG_SIZE];

//delegate type
typedef char * restrict (*msgCallback)(void*);

//delegate function prototype
char* restrict __updatedImpl(void*);

//Different implementation
char* restrict getVersionMessage(void* args){
    msgCallback msb =__updatedImpl;
    return msb("with some additional data");
}

//delegate function
char* restrict __updatedImpl(void* data){
    bzero(msg,MSG_SIZE);
    sprintf(msg, "version 2 implementation, with %s", (char *) data);
    return msg;
}