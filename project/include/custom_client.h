#ifndef _CUSTOM_CLIENT_H_
#define _CUSTOM_CLIENT_H_

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
extern char **ppTable;
extern int row;
extern int column;
int custom_client();
int look_room();
#endif
