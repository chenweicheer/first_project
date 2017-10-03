#include "socket_tool.h"
extern STU_CMDCLIENT g_cmd_client;
extern pthread_mutex_t g_lock;


int ConnectionServer()
{
	int sockfd;
	struct sockaddr_in serveraddr;

	//创建socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("fail to sockfd");
		return -1;
	}

	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr= inet_addr(IP_ADDRESS);
	serveraddr.sin_port=htons(ADDRESS_PORT);

	//连接服务器
	if(connect(sockfd,(struct sockaddr*) &serveraddr,sizeof(serveraddr))<0)
	{
		perror("fail to connect");
		return -1;
	}
	return sockfd;
}


//发送数据
int socket_send_data(char *data,int len,int fd) {

	if (send(g_cmd_client.sockdf, data, len, 0) < 0) {
		perror("fail to send");
		return -1;
	}
	return 0;
}


int readn(int fd, void *ptr, int n)
{
	int nleft;
	int nread;
	nleft = n;
	while (nleft > 0)
	{
		if ((nread = read(fd, ptr, nleft)) < 0)
		{
			if (nleft == n)
				return (-1);
			else
				break;
		}
		else if (nread == 0)
		{
			break;
		}
		nleft -= nread;
		ptr += nread;
	}
	return(n - nleft);      /* return >= 0 */
}


