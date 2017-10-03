#ifndef SOCKET_TOOL_H_
#define SOCKET_TOOL_H_


#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include "cmd.h"
#include "public.h"


#define N	1024
#define ADDRESS_PORT 8000
#define IP_ADDRESS "127.0.0.1"
typedef struct socket_and_data
{
	int socket;
	char *buff;
}STU_SOCKET_AND_DATA;


typedef struct socket_and_pfunc
{
	int socket;
	void *pfunc;
}STU_SOCKET_AND_FUNC;







//连接到服务器返回socket
int ConnectionServer();


//发发送数据
int socket_send_data(char *data,int len,int fd);

int readn(int fd, void *ptr, int n);



#endif
