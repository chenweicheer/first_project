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






void init_daemon() //�����ػ�����
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
��ʼ��������������
*/

void  init()
{

	g_cmd_client.jiiang_fd=-1;
	g_cmd_client.toplight_fd=-1;
	g_cmd_client.printer_fd=-1;
	//������Ϣ���г�ʼ��
	init_queue(&g_recv_cmd_queue,&g_recv_cmd_queue_lock,&g_recv_cmd_queue_not_full_cond,&g_recv_cmd_queue_not_empty_cond);

	//������Ϣ���г�ʼ��
	dlist_init(&g_send_cmd_queue,&g_send_cmd_queue_lock,&g_send_cmd_queue_not_full_cond,&g_send_cmd_queue_not_empty_cond);

	//�ѷ��͵ȴ���Ӧ���г�ʼ��	
	dlist_init(&g_wait_res_cmd_queue,&g_wait_res_cmd_queue_lock,&g_wait_res_cmd_queue_not_full_cond,&g_wait_res_cmd_queue_not_empty_cond);

	//����ģ���ʼ��
	//init_jiliang();

	//����ģ���ʼ��
	//init_toplight();

	/*//��ӡ��ģ���ʼ��
	init_printer();	*/
}
int main(void) {
	//init_daemon();  //��ʼ��ΪDaemon

	
	init();

	//����������߳�
	pthread_t id1,id2,id3,id4;
	int i, ret;
	ret = pthread_create(&id1, NULL, (void *) cmd_proc,
			NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}

	//�����̷߳�������
	ret = pthread_create(&id2, NULL, (void *)send_cmd_proc,
			NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}

	//����һ���߳��ս��ռ�����������
	ret = pthread_create(&id3, NULL, (void *) jiliang_recv,
			NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}


	//����һ���̼߳�鳬ʱ
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
