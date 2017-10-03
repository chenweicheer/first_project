/*
 ============================================================================
 Name        : test.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include "socket_tool.h"
#include <signal.h>
#include "cmd_queue.h"
#include "jiliang.h"
#include "double_link.h"
#include "cmd.h"


#define  NOFILE 0


STU_QUEUE_CMD g_recv_cmd_queue;
pthread_mutex_t g_recv_cmd_queue_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_recv_cmd_queue_not_full_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t g_recv_cmd_queue_not_empty_cond=PTHREAD_COND_INITIALIZER;


STU_DLIST g_send_cmd_queue;
pthread_mutex_t g_send_cmd_queue_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_send_cmd_queue_not_full_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t g_send_cmd_queue_not_empty_cond=PTHREAD_COND_INITIALIZER;


STU_DLIST g_wait_res_cmd_queue;
pthread_mutex_t g_wait_res_cmd_queue_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_wait_res_cmd_queue_not_full_cond=PTHREAD_COND_INITIALIZER;
pthread_cond_t g_wait_res_cmd_queue_not_empty_cond=PTHREAD_COND_INITIALIZER;






void init_daemon() //创建守护进程
{
	int pid;
	int k;
	if((pid = fork()) < 0)	//create new progress
	{
		perror("fork failed\n");
		exit(1);
	}

	if(pid == 0)
	{
		exit(0);
	}
	setsid();
	if(pid  = fork())
		exit(0);
	else if(pid < 0)
		exit(1);
	for(k = 0 ; k < NOFILE; k++)
	{
		close(k);
	}
	chdir("/");
	umask(0);
	return;

}




/*
初始化操作放在这里
*/

void  init()
{

	g_cmd_client.jiiang_fd=-1;
	g_cmd_client.toplight_fd=-1;
	g_cmd_client.printer_fd=-1;
	//接收消息队列初始化
	init_queue(&g_recv_cmd_queue,&g_recv_cmd_queue_lock,&g_recv_cmd_queue_not_full_cond,&g_recv_cmd_queue_not_empty_cond);

	//发送消息队列初始化
	dlist_init(&g_send_cmd_queue,&g_send_cmd_queue_lock,&g_send_cmd_queue_not_full_cond,&g_send_cmd_queue_not_empty_cond);

	//已发送等待响应队列初始化	
	dlist_init(&g_wait_res_cmd_queue,&g_wait_res_cmd_queue_lock,&g_wait_res_cmd_queue_not_full_cond,&g_wait_res_cmd_queue_not_empty_cond);

	//计量模块初始化
	//init_jiliang();

	//顶灯模块初始化
	//init_toplight();

	/*//打印机模块初始化
	init_printer();	*/
}
int main(void) {
	//init_daemon();  //初始化为Daemon

	
	init();

	//启动命令处理线程
	pthread_t id1,id2,id3,id4;
	int i, ret;
	ret = pthread_create(&id1, NULL, (void *) cmd_proc,
			NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}

	//建立线程发送命令
	ret = pthread_create(&id2, NULL, (void *)send_cmd_proc,
			NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}

	//建立一个线程收接收计量串口数据
	ret = pthread_create(&id3, NULL, (void *) jiliang_recv,
			NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}


	//建立一个线程检查超时
	ret = pthread_create(&id4, NULL, (void *) check_timeout_proc,
			NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}


	create_socketServer();
	while(1)
	{
		//dlist_treaverse(&g_wait_res_cmd_queue,check_timeout,NULL);
	}
	
	return 0;

}
