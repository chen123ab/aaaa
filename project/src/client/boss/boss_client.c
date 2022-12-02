#include "boss_client.h"
#include "db.h"
#include "sqlite3.h"
#include <stdio.h>
sqlite3 *pDb;
char **ppTable;
int row;
int column;
int boss_client()
{
	//创建套结字
	int sockFd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockFd < 0)
	{
		perror("socket error");
		return -1;
	}
	printf("socket ok!\n");
	//连接服务器
	struct sockaddr_in servAddr = {0};
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(8888);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int ret ;
	ret = connect(sockFd, (struct sockaddr *)&servAddr, sizeof(servAddr));
	if(ret < 0)
	{
		perror("connect error");
		close(sockFd);
		return -1;
	}
	printf("connect ok!\n");
	//通信
	char send_buf[1024] = {0};//发送消息
	char recv_buf[1024] = {0};//接受消息
	char msg_buf[1024] = {0};//消息内容
	char buf[1024] = {0};//接受指令
	char name[50] = {0};//用户名
	char pwd [50] = {0};//密码
	while(1)
	{
chose:
		memset(buf, 0, sizeof(buf));
		printf("******************\n");
		printf("*quit--------退出*\n");
		printf("*regiester---注册*\n");
		printf("*login-------登陆*\n");
		printf("******************\n");
		printf("请输入:");
		scanf("%s",buf);
		//sprintf(send_buf,"%s",buf);
		printf("%s\n",buf);
		if(0 == strcmp(buf,"quit"))
		{
			memset(send_buf, 0, 1024);
			sprintf(send_buf,"%s",buf);
			send(sockFd, send_buf, sizeof(send_buf), 0);
			memset(recv_buf, 0, sizeof(recv_buf));
			recv(sockFd, recv_buf, sizeof(recv_buf), 0);
			printf("%s\n",recv_buf);
			if(0 == (ret = strcmp(recv_buf,"quit ok")))
			{
				printf("退出成功\n");
				break;
			}
			else
			{
				printf("退出失败\n");	
			}
		}
		//注册
		else if(0 == strcmp(buf, "regiester"))
		{
			memset(send_buf, 0, 1024);
			memset(name, 0, 50);
			memset(pwd, 0 ,50);
			printf("请输入用户名：");
			scanf("%s",name);
			printf("请输入密码：");
			scanf("%s",pwd);
			sprintf(send_buf,"%s-%s-%s",buf,name,pwd);
			send(sockFd, send_buf, sizeof(send_buf), 0);
			memset(recv_buf, 0, sizeof(recv_buf));
			recv(sockFd, recv_buf, sizeof(recv_buf), 0);
			printf("%s\n",recv_buf);
			if(0 == (ret = strcmp(recv_buf, "regiester ok")))
			{
				printf("注册成功，请登陆！\n");
				memset(recv_buf, 0, sizeof(recv_buf));
				goto chose;
			}
			else
			{
				printf("注册失败，请重新输入!\n");	
			}
		}
		else if(0 == strcmp(buf,"login"))
		{
			memset(send_buf, 0, 1024);
			memset(name, 0, 50);
			memset(pwd, 0, 50);
			printf("请输入用户名：");
			scanf("%s",name);
			printf("请输入密码：");
			scanf("%s",pwd);
			sprintf(send_buf,"%s-%s-%s",buf,name,pwd);
			send(sockFd, send_buf, sizeof(send_buf), 0);
			memset(recv_buf, 0, sizeof(recv_buf));
			recv(sockFd, recv_buf, sizeof(recv_buf), 0);
			printf("%s\n",recv_buf);
			if(0 == (ret = strcmp(recv_buf, "login ok")))
			{
				memset(recv_buf, 0, sizeof(recv_buf));
				printf("登陆成功！\n");
				while(1)
				{
					printf("quit----------退出程序\n");
					printf("add-----------添加信息\n");	
					printf("update--------修改信息\n");
					printf("delete--------删除信息\n");
					printf("look----------查看信息\n");
					printf("l_reserve-----预订信息\n");
					printf("请输入：");
					memset(buf, 0, sizeof(buf));
					scanf("%s", buf);
					if(0 == strcmp(buf,"quit"))
					{
						memset(send_buf, 0, 1024);
						sprintf(send_buf,"%s",buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						memset(recv_buf, 0, sizeof(recv_buf));
						recv(sockFd, recv_buf, sizeof(recv_buf), 0);
						if(0 == (ret = strcmp(recv_buf,"quit ok")))
						{
							printf("退出成功\n");
							break;
						}
						else
						{
							printf("退出失败\n");	
						}
					}
					else if(0 == strcmp(buf,"add"))
					{
						memset(msg_buf, 0, sizeof(msg_buf));
						memset(send_buf, 0, sizeof(send_buf));
						printf("请依次输入需要添加的房间信息\n");
						printf("房间类型-房间号-价格-预定情况 例如:单人间-1101-88-未预定\n");
						printf("请输入：");
						scanf("%s",msg_buf);
						sprintf(send_buf,"%s-%s",buf,msg_buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						memset(recv_buf, '\0', sizeof(recv_buf));
						recv(sockFd, recv_buf, sizeof(recv_buf), 0);
						printf("%s",recv_buf);
						if(0 == (ret = strcmp(recv_buf,"add ok")))
						{
							printf("添加成功\n");
						}
						else
						{
							printf("添加失败\n");	
						}
					}
					else if(0 == strcmp(buf,"update"))
					{
						memset(msg_buf, 0, sizeof(msg_buf));
						memset(send_buf, 0, sizeof(send_buf));
						printf("请依次输入需要修改的房间信息\n");
						printf("房间类型-房间号-价格-预定情况 例如:单人间-1101-88-未预定\n");
						printf("请输入：");
						scanf("%s",msg_buf);
						sprintf(send_buf,"%s-%s",buf,msg_buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						memset(recv_buf, 0, sizeof(recv_buf));
						recv(sockFd, recv_buf, sizeof(recv_buf), 0);
						if(0 == (ret = strcmp(recv_buf, "update ok")))
						{
							printf("修改成功\n");
						}
						else
						{
							printf("修改失败\n");	
						}
					}
					else if(0 == strcmp(buf,"delete"))
					{
						memset(msg_buf, 0, sizeof(msg_buf));
						memset(send_buf, 0, sizeof(send_buf));
						printf("请需要删除的房间号\n");
						printf("请输入：");
						scanf("%s",msg_buf);
						sprintf(send_buf,"%s-%s",buf,msg_buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						memset(recv_buf, 0, sizeof(recv_buf));
						recv(sockFd, recv_buf, sizeof(recv_buf), 0);
						if(0 == (ret = strcmp(recv_buf, "delete ok")))
						{
							printf("删除成功\n");
						}
						else
						{
							printf("删除失败\n");	
						}
					}
					else if(0 == strcmp(buf,"look"))
					{
						
						memset(send_buf, 0, sizeof(send_buf));
						sprintf(send_buf,"%s",buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						while(1)
						{
							memset(recv_buf, 0, sizeof(recv_buf));
							recv(sockFd,recv_buf,sizeof(recv_buf),0);
							//printf("%d\n",ret);
							if(0 == strcmp(recv_buf,"ok"))
							{
								printf("查看完成\n");
								break;
							}
							printf("%s\n",recv_buf);
						}
					}
					else if(0 == strcmp(buf,"l_reserve"))
					{
						
						memset(send_buf, 0, sizeof(send_buf));
						sprintf(send_buf,"%s",buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						while(1)
						{
							memset(recv_buf, 0, sizeof(recv_buf));
							recv(sockFd,recv_buf,sizeof(recv_buf),0);
							//printf("%d\n",ret);
							if(0 == strcmp(recv_buf,"ok"))
							{
								printf("查看完成\n");
								break;
							}
							printf("%s\n",recv_buf);
						}
					}
					else
					{
						printf("指令错误，请重新输入！\n");
					}
				}
			}
			else
			{
				printf("登陆失败\n");
			}
		}
		else
		{
			printf("指令错误，请重新输入！\n");
		}
	}
	close(sockFd);
	return 0;
}
