#include "custom_client.h"
#include <stdio.h>
#include <string.h>

int custom_client()
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
	char send_buf[1024] = {0};
	char recv_buf[1024] = {0};
	char msg_buf[1024] = {0};
	char buf[1024] = {0};
	char name[50] = {0};
	char pwd [50] = {0};
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
				char name_buf[50] = {0};
				memset(name_buf, 0, sizeof(name_buf));
				strcpy(name_buf,name);
				while(1)
				{
					printf("quit--------退出程序\n");
					printf("look--------查看房间信息\n");	
					printf("price-------按价钱搜索房间\n");
					printf("reserve-----查看预订信息\n");
					printf("ok----------预定房间\n");
					printf("exit--------退房\n");
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
					else if(0 == strcmp(buf,"look"))
					{
						memset(send_buf, 0, sizeof(send_buf));
						sprintf(send_buf,"%s",buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						while(1)
						{
							memset(recv_buf, 0, sizeof(recv_buf));
							recv(sockFd,recv_buf,sizeof(recv_buf),0);
							if(0 == strcmp(recv_buf,"ok"))
							{
								printf("查看完成\n");
								break;
							}
							printf("%s\n",recv_buf);
						}
					}
					else if(0 == strcmp(buf,"price"))
					{
						char price[50] = {0};
						printf("请输入要查找房间的价格：");
						scanf("%s",price);
						memset(send_buf, 0, sizeof(send_buf));
						sprintf(send_buf,"%s-%s",buf,price);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						while(1)
						{
							memset(recv_buf, 0, sizeof(recv_buf));
							recv(sockFd,recv_buf,sizeof(recv_buf),0);
							if(0 == strcmp(recv_buf,"ok"))
							{
								printf("查看完成\n");
								break;
							}
							printf("%s\n",recv_buf);
						}
					}
					else if(0 == strcmp(buf,"reserve"))
					{
						memset(send_buf, 0, sizeof(send_buf));
						sprintf(send_buf,"%s-%s",buf,name_buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						while(1)
						{
							memset(recv_buf, 0, sizeof(recv_buf));
							recv(sockFd,recv_buf,sizeof(recv_buf),0);
							if(0 == strcmp(recv_buf,"ok"))
							{
								printf("查看完成\n");
								break;
							}
							printf("%s\n",recv_buf);
						}
					}
					else if(0 == strcmp(buf,"ok"))
					{
						char per_buf[50] = {0};
						char mon_buf[50] = {0};
						memset(mon_buf, 0, sizeof(mon_buf));
						memset(msg_buf, 0, sizeof(msg_buf));
						memset(send_buf, 0, sizeof(send_buf));
						memset(per_buf, 0, sizeof(per_buf));
						printf("请需要预订的房间号\n");
						printf("请输入：");
						scanf("%s",msg_buf);
						printf("请输入您的信息：姓名-电话 如：李明-12234557\n");
						printf("请输入：");
						scanf("%s",per_buf);
						printf("请支付100订金\n");
						printf("请支付:");
						scanf("%s",mon_buf);
						sprintf(send_buf,"%s-%s-%s-%s-%s",buf,name_buf,msg_buf,mon_buf,per_buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						memset(recv_buf, 0, sizeof(recv_buf));
						recv(sockFd, recv_buf, sizeof(recv_buf), 0);
						if(0 == (ret = strcmp(recv_buf, "ok"))&&0 == (ret = strcmp(mon_buf, "100")))
						{
							printf("预订成功\n");
						}
						else
						{
							printf("预订失败\n");	
						}
					}
					else if(0 == strcmp(buf,"exit"))
					{
						memset(msg_buf, 0, sizeof(msg_buf));
						memset(send_buf, 0, sizeof(send_buf));
						printf("请输入退房的房间号：");
						scanf("%s",msg_buf);
						sprintf(send_buf,"%s-%s",buf,msg_buf);
						send(sockFd, send_buf, sizeof(send_buf), 0);
						memset(recv_buf, 0, sizeof(recv_buf));
						recv(sockFd, recv_buf, sizeof(recv_buf), 0);
						if(0 == (ret = strcmp(recv_buf, "exit ok")))
						{
							printf("退房成功\n");
						}
						else
						{
							printf("退房失败\n");	
						}
					}
					else
					{
						printf("指令错误，请重新输入！\n");
					}
				}
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
