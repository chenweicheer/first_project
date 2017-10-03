#include "socket_tool.h"
#include "cmd_queue.h"
#include "jiliang.h"

extern STU_CMDCLIENT g_cmd_client;
extern pthread_mutex_t g_lock;





/*void server_recv_hander() {
	char buf[N];
	int sockdf=g_cmd_client.sockdf;
	pthread_t id;
	int len;
	STU_CMD cmd;

	while (1) {
		memset(buf,0,N);
		len=recv(sockdf, buf, N, 0);
		printf("server recv len %d\n",len);
		if (len < 0) {
			perror("fail to recv\n");
			break;
		}
		//客户端正常关闭
		if(len==0)
		{
			break;
		}

		
		//printf("server recv app data\n");
		//命令合法性检查
		if(OK == cmd_check(buf))
		{
			cmd.header_flag=0xA55A;
			cmd.len=buf[2] << 8 | buf[3];
			//命令流向
			cmd.source_dest=buf[4];
					
			//命令或响应标识
			cmd.cmd_flag=buf[5];

			//保留位 2字节
			cmd.sec=(u8)buf[6] << 8 | (u8)buf[7];
			//printf("sec %d\n",(u16)cmd.sec);
			//命令值 2字节
			cmd.cmd_value=buf[8] << 8 | buf[9];

			//crc读取
			cmd.crc=buf[cmd.len-2] << 8 | buf[cmd.len-1];
									
			if(cmd.len-HEADER_LEN>0)
			{
				memcpy(cmd.data,&buf[10],cmd.len-HEADER_LEN);
			}	
			printf("\n\nserver recv cmd:	%d\n",cmd.cmd_value);
			enqueue(&g_recv_cmd_queue,cmd);

		}
	}
	printf("close:%d\n",sockdf);
	close(sockdf);
}*/




void server_recv_hander() {
	u8 buf[N];
	int sockdf=g_cmd_client.sockdf;
	pthread_t id;
	int len,l,i;
	STU_CMD cmd;

	while(1)
	{	
		memset(buf,0,1024);
		len=read(g_cmd_client.sockdf ,&buf[0],1);
		if(len ==0)
		{
			continue;
		}
		//printf("%02x",buf[0]);
		if(0x5A==(unsigned char)buf[0])	
		{	
			len=read(g_cmd_client.sockdf ,&buf[1],1);
			if(0xA5==(unsigned char)buf[1])
			{
				cmd.header_flag=0xA55A;

				len=readn(g_cmd_client.sockdf ,&buf[2],2);
				if(2!=len)
				{	
					printf("read failure len:%d\n",len);
					continue;
				}
				l=buf[2] << 8 | buf[3];



				if(l<HEADER_LEN||l>BUFFLEN)
				{
					printf("cmd len error :%d\n",l);
					continue;
				}


				cmd.len=l;


				len=readn(g_cmd_client.sockdf , &buf[4],l-4);
				if((l-4)!=len)
				{	
					printf("read failure len:%d\n",len);
					continue;
				}


				cmd.source_dest=buf[4];
					

				cmd.cmd_flag=buf[5];


				cmd.sec=buf[6] <<8 |buf[7];


				cmd.cmd_value=buf[8] << 8 | buf[9];

				if(cmd.cmd_value < 1||cmd.cmd_value > AMOUNT_CMD)
				{
					printf("cmd_value error:%d",cmd.cmd_value);
					continue;
				}


				cmd.crc=(u8)buf[cmd.len-2] << 8 | (u8)buf[cmd.len-1];


				if(cmd.crc!=(crc16(buf,cmd.len-2)))
				{
					printf("len %d\n",cmd.len);
					printf("crc %x		calculatecrc %x\n",cmd.crc,(crc16(buf,cmd.len-2)));
					printf("crc16 check error\n");
					continue;
				}
									
				if(cmd.len-HEADER_LEN>0)
				{
					memcpy(cmd.data,&buf[10],cmd.len-HEADER_LEN);
				}
					
				enqueue(&g_recv_cmd_queue,cmd);
				}
			else
			{
				printf("header error: %02x%02x\n",(unsigned char)buf[0],(unsigned char)buf[1]);
			}
			}
		}
	printf("close:%d\n",sockdf);
	close(sockdf);
}



int create_socketServer() {


	int sockfd, connectfd;

	struct sockaddr_in serveraddr, clientaddr;
	socklen_t len = sizeof(clientaddr);
	int server_len=0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("fail to socket");
		return -1;
	}

	bzero(&serveraddr,sizeof(struct sockaddr_in));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=inet_addr(IP_ADDRESS);
	serveraddr.sin_port=htons(ADDRESS_PORT);

	if (bind(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0) {
		perror("fail to bind");
		return -1;
	}

	if (listen(sockfd, 5) < 0) {
		perror("fail to listen");
		return -1;
	}

	printf("listen successed\n");

	while (1) {
		if ((connectfd = accept(sockfd, (struct sockaddr*) &clientaddr, &len))
				< 0) {
			perror("fail to accept");
			break;
		}
		printf("connect success %d\n",connectfd);
		g_cmd_client.sockdf = connectfd;
		//send_cmd(3, NULL, 0, SERVER_TO_APP, CMD_FLAG);
		pthread_t id;
		int i, ret;
		ret = pthread_create(&id, NULL, (void *) server_recv_hander, NULL);
		if (ret != 0) {
			printf("Create pthread error!n");
			break;
		}
	}

	close(sockfd);
	return 0;
}

//发送数据
int socket_send_data(char *data,int len,int fd) {

	if (send(fd, data, len, 0) < 0) {
		perror("fail to send");
		return -1;
	}
	return 0;
}

