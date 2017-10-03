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
#include "cmd_queue.h"


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



//���ӵ�����������socket
int connection_server();


//���������յ����ݴ���
void server_recv_hander();

//������һ��������
int create_socketServer();

//����������
int socket_send_data(char *data,int len,int fd);

extern STU_QUEUE_CMD g_recv_cmd_queue;




#endif
