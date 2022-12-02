#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>


void getuserlnFo(char *recvBuf, char **infBuf);
int serverlnit();
int waitClient();

#endif
