//
// Created by sdutton on 02.12.22.
//

#include "Response.h"

void initResponse(pResponse);
void signResponse(pResponse);
int checkResponse(pResponse);
void sendResponse(int fd, void* request, pResponse response,void (*)(int*,int*));

