#include "sqlite3.h"
#include "db.h"
#include <stdio.h>
#include <string.h>

sqlite3 *pDb;
char **ppTable;
int row;
int column;

//创建数据库并注册表
int dblnit()
{
	int ret = sqlite3_open("hotel.db",&pDb);
	if(ret != SQLITE_OK)
	{
		printf("sqlite3_open error!\n");
		return -1;
	}
	printf("sqlite3_open ok!\n");

	char *errmsg = NULL;
	char *sql = "create table shoplnfo(name text primary key not null,pwd text not null);";
	ret = sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("create shoplnfo table error!!:%s\n",errmsg);

		sqlite3_free(errmsg);
	}
	printf("create shoplnfo table ok!\n");

	errmsg = NULL;
	sql="create table room(type text not null,id text primary key not null,price not null,msg text not null);";
	ret=sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("creat room table error:%s\n",errmsg);
		sqlite3_free(errmsg);
	}
	printf("creat room table ok!\n");

	errmsg = NULL;
	sql="create table reserve(myid text not null,id text not null primary key,price text not null,name not null,iphone text not null);";
	ret=sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("creat order table error:%s\n",errmsg);
		sqlite3_free(errmsg);
	}
	printf("creat room table ok!\n");
	return 0;
}

//注册
int room_register(const char *pName,const char *pPwd)
{
	char selectSql[1024]={0};
	sprintf(selectSql,"select pwd from shoplnfo where name = '%s';",pName);
	char **ppTable = NULL;
	int row = 0;
	int column;
	char *errmsg  = NULL;

	int ret=sqlite3_get_table(pDb,selectSql,&ppTable,&row,&column,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("register get table error!:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	if(1 == row)
	{
		return 0;
	}
	char sql[1024]={0};
	errmsg = NULL;
	sprintf(sql,"insert into shoplnfo values('%s','%s');",pName,pPwd);
	ret = sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("register shoplnfo error:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	printf("register shoplnfo ok!\n");
	return 0;
}

//登陆
int room_login(const char *name,const char *pwd)
{
	char selectSql[1024] = {0};
	sprintf(selectSql,"select pwd from shoplnfo where name ='%s';",name);
	char **ppTable = NULL;
	int row = 0;
	int column;
	char *errmsg = NULL;

	int ret = sqlite3_get_table(pDb,selectSql,&ppTable,&row,&column,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("login get table error!:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	if(0 == row)
	{
		return -1;
	}
	if(0 == strcmp(ppTable[1],pwd))
	{
		return 0;
	}
	else
	{
		return -1;
	}
	printf("login ************************\n");
}

//增加房间信息
int add_room(char *type, char *id, char *price,char *arr)
{
	char sql[1024]={0};
	sprintf(sql,"insert into room values('%s','%s','%s','%s');",type,id,price,arr);
	char *errmsg = NULL;
	int ret = sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("add room error:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	printf("add room ok!\n");
	return 0;
}

//增加订单信息
int add_reserve(char *myid,char *id, char *price, char *name,char *iphone)
{
	char selectSql[1024]={0};
	sprintf(selectSql,"select * from reserve where id = '%s';",id);
	char **ppTable = NULL;
	int row = 0;
	int column;
	char *errmsg  = NULL;

	int ret=sqlite3_get_table(pDb,selectSql,&ppTable,&row,&column,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("reserve get table error!:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	if(1 == row)
	{
		return 3;
	}
	char sql[1024]={0};
	sprintf(sql,"insert into reserve values('%s','%s','%s','%s','%s');",myid,id,price,name,iphone);
	errmsg = NULL;
	ret = sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("add reserve error:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	printf("add reserve ok!\n");
	return 0;
}
//修改房间信息
int update_room(char *type, char *id, char *price,char *arr)
{
	char sql[1024]={0};
	sprintf(sql,"update room set type = '%s',price = '%s',msg = '%s'where id = '%s';",type,price,arr,id);
	char *errmsg = NULL;
	int ret = sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("update room error:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	printf("updata room ok!\n");
	return 0;
}
//修改房间信息
int update1_room(char *id, char *arr)
{
	char sql[1024]={0};
	sprintf(sql,"update room set msg = '%s'where id = '%s';",arr,id);
	char *errmsg = NULL;
	int ret = sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("update room error:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	printf("updata room ok!\n");
	return 0;
}
//删除房间信息
int delete_room(char *id)
{
	char sql[1024]={0};
	sprintf(sql,"delete from room where id = '%s';",id);
	char *errmsg = NULL;
	int ret = sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("delete room error:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	printf("delete room ok!\n");
	return 0;	
}

//删除订单信息
int delete_reserve(char *id)
{
	char sql[1024]={0};
	sprintf(sql,"delete from reserve where id = '%s';",id);
	char *errmsg = NULL;
	int ret = sqlite3_exec(pDb,sql,NULL,NULL,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("delete room error:%s\n",errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	printf("delete reserve ok!\n");
	return 0;	
}

//按条件查找房间
int screening_room(char *price)
{
	char sql[1024] = {0};
	sprintf(sql,"select *from room where price = '%s';",price);
	char *errmsg = NULL;
	int ret = sqlite3_get_table(pDb,sql,&ppTable,&row,&column,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("select all room error!\n");
		sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}

//查找全部房间信息
int look_room()
{
	char *sql = "select *from room;";
	char *errmsg = NULL;
	int ret = sqlite3_get_table(pDb,sql,&ppTable,&row,&column,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("select all room error!\n");
		sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}

//查看全部订单信息
int look_reserve()
{
	char *sql = "select *from reserve;";
	char *errmsg = NULL;
	int ret = sqlite3_get_table(pDb,sql,&ppTable,&row,&column,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("select all reserve error!\n");
		sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}

//查看订单信息
int screening_reserve(char *myid)
{
	char sql[1024] = {0};
	sprintf(sql,"select *from reserve where myid = '%s';",myid);
	char *errmsg = NULL;
	int ret = sqlite3_get_table(pDb,sql,&ppTable,&row,&column,&errmsg);
	if(ret != SQLITE_OK)
	{
		printf("select all reserve error!\n");
		sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}

