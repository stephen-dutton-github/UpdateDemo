//
// Created by sdutton on 01.12.22.
//
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <string.h>
#include "Request.h"

#ifndef UPDATEDEMO_CONNECTION_H
#define UPDATEDEMO_CONNECTION_H

#define MAX 80
#define PORT 8080
#define SA struct sockaddr


//Specify NULL for local
int initServerConnection(void* data);
int initClientConnection(void* data);
void closeConnection(int fd);

#endif //UPDATEDEMO_CONNECTION_H


