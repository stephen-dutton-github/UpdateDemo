//
// Created by sdutton on 29.11.22.
//


#include "Request.h"
#include "Response.h"


#ifndef UPDATEDEMO_MESSAGEHANDLER_H
#define UPDATEDEMO_MESSAGEHANDLER_H

//Messaging delegate callback

void* callClientHandler(pRequest, pResponse, void(*)(void*));
void* callServerHandler(pRequest, pResponse, void(*)(void*));

#endif //UPDATEDEMO_MESSAGEHANDLER_H
