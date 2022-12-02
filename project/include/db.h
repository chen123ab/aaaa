#ifndef _DB_H_
#define _DB_H_

#include <stdio.h>
#include <string.h>

extern char **ppTable;
extern int row;
extern int column;

int dblnit();
int room_register(const char *pName,const char *pPwd);
int room_login(const char *name,const char *pwd);
int add_room(char *type, char *id, char *price, char *arr);
int add_reserve(char *myid,char *id, char *price, char *name,char *iphone);
int update_room(char *type, char *id, char *price, char *arr);
int update1_room(char *id, char *arr);
int delete_room(char *id);
int delete_reserve(char *myid);
int screening_room(char *price);
int look_room();
int look_reserve();
int screening_reserve(char *myid);

#endif
