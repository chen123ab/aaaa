#include "network.h"
#include "db.h"
#include "sqlite3.h"
#include <stdio.h>

//解析命令内容
void getuserlnFo(char *recvBuf, char *infoBuf[])
{
	infoBuf[0] = recvBuf;
	int i = 0,j = 0;
	while('\0' != recvBuf[i])
	{
		if('-' == recvBuf[i])
		{
			j++;
			recvBuf[i] = '\0';
			infoBuf[j] = &recvBuf[i+1];
		}
		i++;
	}
}

//建立服务器
int serverlnit()
{
	//创建套结字
	int sockFd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockFd < 0)
	{
		perror("socket error");
		return -1;
	}
	printf("socket ok!\n");

	int on = 1;
	setsockopt(sockFd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	//绑定ip和端口号
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	int ret = bind(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		perror("bind error");
		close(sockFd);
		return -1;
	}
	printf("bind ok!\n");
	ret = listen(sockFd, 20);
	if(ret < 0)
	{
		perror("listen error");
		close(sockFd);
		return -1;
	}
	return sockFd;
}

//等待客户端连接并通信
int waitClient(int sockFd)
{
	char recv_buf[1024] = {0};//接受消息
	char send_buf[1024] = {0};//发送消息

	//struct sockaddr_in clientAddr;
	//socklen_t len = sizeof(struct sockaddr_in);
	//创建一个读的集合
	fd_set rfds;

	//清空集合
	FD_ZERO(&rfds);

	//将sockfd加入集合
	FD_SET(sockFd, &rfds);
	int maxFd = sockFd;
	while(1)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		memset(send_buf, 0, sizeof(send_buf));
		fd_set tmpFds = rfds;
		//printf("before select....\n");
		int ret = select(maxFd+1,&tmpFds, NULL, NULL, NULL);
		if(ret < 0)
		{
			perror("select error");
			close(sockFd);
			return -1;
		}
		int i;
		for(i = 0; i < maxFd+1; i++)
		{
			if(FD_ISSET(i,&tmpFds))
			{
				if(i == sockFd)
				{
					//printf("client ....\n");

					//等待并建立连接
					int connFd = accept(i ,NULL, NULL);
					if(connFd < 0)
					{
						perror("accept error");
						close(sockFd);
						return -1;
					}
					printf("accept ok!\n");
					FD_SET(connFd, &rfds);
					if(connFd > maxFd)
					{
						maxFd = connFd;
					}
				}
				else
				{
					//通信
					memset(recv_buf, 0, sizeof(recv_buf));
					memset(send_buf, 0, sizeof(send_buf));
					int ret = recv(i, recv_buf, sizeof(recv_buf), 0);
					char *infoArr[10] = {0};
					printf("%s\n",recv_buf);
					getuserlnFo(recv_buf,infoArr);
					if(ret < 0)
					{
						perror("recv error");
						continue;
					}
					else if(0 == ret)
					{
						printf("客户端退出!\n");
						close(i);
						FD_CLR(i,&rfds);
					}
					else{
						char arr[] = "未预定";
						char brr[] = "已预定";
						if(0 == strcmp(infoArr[0],"regiester"))
						{
							ret = room_register(infoArr[1],infoArr[2]);
							if(0 == ret)
							{
								strcpy(send_buf,"regiester ok");
								printf("%s\n",send_buf);
								send(i, send_buf, sizeof(send_buf), 0);
							}
							else
							{
								strcpy(send_buf,"regiester error");
								send(i, send_buf, sizeof(send_buf), 0);
							}
						}
						else if(0 == strcmp(infoArr[0],"quit"))
						{
							printf("客户端退出!\n");
							strcpy(send_buf,"quit ok");
							printf("%s\n",send_buf);
							send(i, send_buf, sizeof(send_buf), 0);
						}
						else if(0 == strcmp(infoArr[0],"login"))
						{
							ret = room_login(infoArr[1],infoArr[2]);
							printf("---------------\n");
							if(0 == ret)
							{
								strcpy(send_buf,"login ok");
								printf("%s\n",send_buf);
								send(i, send_buf, sizeof(send_buf), 0);
							}
							else
							{
								strcpy(send_buf,"login error");
								send(i, send_buf, sizeof(send_buf), 0);
							}
						}
						else if(0 == strcmp(infoArr[0],"add"))
						{
							ret = add_room(infoArr[1], infoArr[2],infoArr[3],infoArr[4]);
							memset(send_buf, 0, sizeof(send_buf));
							if(0 == ret)
							{
								strcpy(send_buf,"add ok");
								printf("%s\n",send_buf);
								send(i, send_buf, sizeof(send_buf), 0);
							}
							else
							{
								strcpy(send_buf,"add error");
								send(i, send_buf, sizeof(send_buf), 0);
							}
						}
						else if(0 == strcmp(infoArr[0],"ok"))
						{
							ret = add_reserve(infoArr[1], infoArr[2],infoArr[3],infoArr[4],infoArr[5]);
							int rat = update1_room(infoArr[2],brr);
							memset(send_buf, 0, sizeof(send_buf));
							if(0 == ret && 3 != rat)
							{
								strcpy(send_buf,"ok");
								printf("%s\n",send_buf);
								send(i, send_buf, sizeof(send_buf), 0);
							}
							else
							{
								strcpy(send_buf,"error");
								send(i, send_buf, sizeof(send_buf), 0);
							}
						}
						else if(0 == strcmp(infoArr[0],"update"))
						{
							ret = update_room(infoArr[1], infoArr[2],infoArr[3],infoArr[4]);
							memset(send_buf, 0, sizeof(send_buf));
							if(0 == ret)
							{
								strcpy(send_buf,"update ok");
								send(i, send_buf, sizeof(send_buf), 0);
							}
							else
							{
								strcpy(send_buf,"update error");
								send(i, send_buf, sizeof(send_buf), 0);
							}	
						}
						else if(0 == strcmp(infoArr[0],"delete"))
						{
							ret = delete_room(infoArr[1]);
							if(0 == ret)
							{
								strcpy(send_buf,"delete ok");
								send(i, send_buf, sizeof(send_buf), 0);
							}
							else
							{
								strcpy(send_buf,"delete error");
								send(i, send_buf, sizeof(send_buf), 0);
							}
						}
						else if(0 == strcmp(infoArr[0],"exit"))
						{
							ret = delete_reserve(infoArr[1]);
							int rat = update1_room(infoArr[1],arr);
							if(0 == ret && 0 == rat)
							{
								strcpy(send_buf,"exit ok");
								send(i, send_buf, sizeof(send_buf), 0);
							}
							else
							{
								strcpy(send_buf,"exit error");
								send(i, send_buf, sizeof(send_buf), 0);
							}
						}
						else if(0 == strcmp(infoArr[0],"reserve"))
						{
							screening_reserve(infoArr[1]);
							if(NULL != ppTable)
							{
								int g,j;
								for(g = 0;g < row+1;g++)
								{
									memset(send_buf,0,sizeof(send_buf));
									for(j = 0;j<column;j++)
									{
										if(NULL != ppTable[g*column+j])
										{
											sprintf(send_buf,"%s%s\t",send_buf,ppTable[g*column+j]);
											//printf("%s\n",send_buf);
										}
									}
									send(i,send_buf,sizeof(send_buf),0);	
									//printf("%d\n",ret);
									printf("%s\n",send_buf);
								}
								memset(send_buf,0,sizeof(send_buf));
								strcpy(send_buf,"ok");
								send(i,send_buf,sizeof(send_buf),0);
								sqlite3_free_table(ppTable);
							}	
							else
							{
								memset(send_buf,0,sizeof(send_buf));
								strcpy(send_buf,"error");
								send(i,send_buf,sizeof(send_buf),0);	
							}
						}
						else if(0 == strcmp(infoArr[0],"price"))
						{
							screening_room(infoArr[1]);
							if(NULL != ppTable)
							{
								int g,j;
								for(g = 0;g < row+1;g++)
								{
									memset(send_buf,0,sizeof(send_buf));
									for(j = 0;j<column;j++)
									{
										if(NULL != ppTable[g*column+j])
										{
											sprintf(send_buf,"%s%s\t",send_buf,ppTable[g*column+j]);
											//printf("%s\n",send_buf);
										}
									}
									send(i,send_buf,sizeof(send_buf),0);	
									//printf("%d\n",ret);
									printf("%s\n",send_buf);
								}
								memset(send_buf,0,sizeof(send_buf));
								strcpy(send_buf,"ok");
								send(i,send_buf,sizeof(send_buf),0);
								sqlite3_free_table(ppTable);
							}	
							else
							{
								memset(send_buf,0,sizeof(send_buf));
								strcpy(send_buf,"error");
								send(i,send_buf,sizeof(send_buf),0);	
							}
						}
						else if(0 == strcmp(infoArr[0],"look"))
						{
							look_room();
							if(NULL != ppTable)
							{
								int g,j;
								for(g = 0;g < row+1;g++)
								{
									memset(send_buf,0,sizeof(send_buf));
									for(j = 0;j<column;j++)
									{
										if(NULL != ppTable[g*column+j])
										{
											sprintf(send_buf,"%s%s\t",send_buf,ppTable[g*column+j]);
											//printf("%s\n",send_buf);
										}
									}
									send(i,send_buf,sizeof(send_buf),0);	
									//printf("%d\n",ret);
									printf("%s\n",send_buf);
								}
								memset(send_buf,0,sizeof(send_buf));
								strcpy(send_buf,"ok");
								send(i,send_buf,sizeof(send_buf),0);
								sqlite3_free_table(ppTable);
							}	
							else
							{
								memset(send_buf,0,sizeof(send_buf));
								strcpy(send_buf,"error");
								send(i,send_buf,sizeof(send_buf),0);	
							}
						}
						else if(0 == strcmp(infoArr[0],"l_reserve"))
						{
							look_reserve();
							if(NULL != ppTable)
							{
								int g,j;
								for(g = 0;g < row+1;g++)
								{
									memset(send_buf,0,sizeof(send_buf));
									for(j = 0;j<column;j++)
									{
										if(NULL != ppTable[g*column+j])
										{
											sprintf(send_buf,"%s%s\t",send_buf,ppTable[g*column+j]);
											//printf("%s\n",send_buf);
										}
									}
									send(i,send_buf,sizeof(send_buf),0);	
									//printf("%d\n",ret);
									printf("%s\n",send_buf);
								}
								memset(send_buf,0,sizeof(send_buf));
								strcpy(send_buf,"ok");
								send(i,send_buf,sizeof(send_buf),0);
								sqlite3_free_table(ppTable);
							}	
							else
							{
								memset(send_buf,0,sizeof(send_buf));
								strcpy(send_buf,"error");
								send(i,send_buf,sizeof(send_buf),0);	
							}
						
						}
					}
				}
			}
		}
	}
	printf("连接成功!\n");
	return 0;
}
