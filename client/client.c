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
#include "cmd.h"
#include "socket_tool.h"
#include "cmd_queue.h"
#include "double_link.h"

#define N	1024

extern STU_CMDCLIENT g_cmd_client;

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




void client_init()
{
	

	//接收消息队列初始化
	init_queue(&g_recv_cmd_queue, &g_recv_cmd_queue_lock,
			&g_recv_cmd_queue_not_full_cond, &g_recv_cmd_queue_not_empty_cond);

	//发送消息队列初始化
	dlist_init(&g_send_cmd_queue, &g_send_cmd_queue_lock,
			&g_send_cmd_queue_not_full_cond, &g_send_cmd_queue_not_empty_cond);

	//已发送等待响应队列初始化
	dlist_init(&g_wait_res_cmd_queue, &g_wait_res_cmd_queue_lock,
			&g_wait_res_cmd_queue_not_full_cond,
			&g_wait_res_cmd_queue_not_empty_cond);
	//连接到服务器
	int sockdf= ConnectionServer();
	g_cmd_client.sockdf=sockdf;
	if(sockdf<=0)
	{
		printf("连接失败");
		return ;
	}

}



void check_timeout_pro()
{
	while(1)
	{
		dlist_treaverse(&g_wait_res_cmd_queue,check_timeout,NULL);
	}

}


void print_menu()
{
	
	printf("\n*******************************************************\n");
	printf("1.开机检查\n");
	printf("2.发送数据到顶灯\n");
	printf("3.校时");
	printf("4.打印状态查询\n");
	printf("5.计量设备状态查询\n");
	printf("6.顶灯状态查询\n");
	printf("7.获取主机计量设备号\n");
	printf("8.获取主机和计量单元计价参数\n");
	printf("9.设置计价参数\n");
	printf("10.清零计量当前的某条运营数据\n");
	printf("11.乘客上车指令\n");
	printf("12，乘客下车指令\n");
	printf("13.获取累计营业数据\n");
	printf("14.确认故障报警\n");
	printf("15.打印\n");
	printf("16.获取设置K\n");
	printf("17.设置K\n");
	printf("18.修改拼车状态\n");
	printf("20.同步计量数据\n");
	printf("\n*******************************************************\n");
}

int main(void) {

	//初始化
	client_init();
	//启动命令处理线程
	pthread_t id1, id2, id3,id4;
	int i, ret,input;
	char tmp[60];
	STU_DATE_TIME  date_time;
	int record_no;
	char * p ;
	ret = pthread_create(&id1, NULL, (void *) cmd_proc, NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}

	//建立线程发送命令
	ret = pthread_create(&id2, NULL, (void *) send_cmd_proc, NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
		return -1;
	}

	ret = pthread_create(&id3, NULL,(void*)client_recv_hander,NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
	}

	
	
	
	print_menu();
	
	
		


	
	/*ret = pthread_create(&id4, NULL,(void*)check_timeout_pro,NULL);
	if (ret != 0) {
		printf("Create pthread error!n");
	}*/
	//send_cmd(1,NULL,0,APP_TO_SERVER,CMD_FLAG);
	//send_cmd(2,NULL,0,APP_TO_SERVER,CMD_FLAG);
	for(i=0;i<3;i++)
	{
		//send_cmd(1,NULL,0,APP_TO_SERVER,CMD_FLAG);
		//sleep(1);
		//send_cmd(2,NULL,0,APP_TO_SERVER,CMD_FLAG);
	}
	/*//send_cmd(1,NULL,0,APP_TO_SERVER);


	STU_DATE_TIME  date_time;
	date_time.date.Year=17;
	date_time.date.Month=9;
	date_time.date.Date=30;
	date_time.time.Hour=1;
	date_time.time.Min=30;
	date_time.time.Sec=0;

	char toplight_cmd[]={3,4,5,6};
	//send_cmd(2,&toplight_cmd[3],1,APP_TO_SERVER);
	int i=0;
	for(;i<4;i++)
	{
		send_cmd(2,&toplight_cmd[1],1,APP_TO_SERVER);
		sleep(5);
	}*/


	/*//开机检查
	//send_cmd(1,NULL,0,APP_TO_SERVER);

	//时间同步
	send_cmd(4,(char *)&date_time,sizeof(date_time),APP_TO_SERVER);

	//计量设备状态查询
	send_cmd(6,NULL,0,APP_TO_SERVER);


	//获取主机计量设备编号
	//send_cmd(8,NULL,0,APP_TO_SERVER);

	//获取计价参数
	send_cmd(9,NULL,0,APP_TO_SERVER);

	//设置计价参数
	send_cmd(10,NULL,0,APP_TO_SERVER);

	//清零计量当前运营的莫条数据
	send_cmd(11,NULL,0,APP_TO_SERVER);*/

	

	while(1)
	{	
		input =-1;
		scanf("%d",&input);
		getchar();
		if(input <0 || input >20)
		{
			system("clear");
			printf("请输入正确选项\n");
			print_menu();
		}
		else
		{
			switch(input)
			{
			case 1: send_cmd(1,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 2: 
				printf("输入发给顶灯的数据：");
				scanf("%s",tmp);
				send_cmd(2,tmp,20,APP_TO_SERVER,CMD_FLAG);
				break;
			case 3: 
				
				date_time.date.Year=17;
				date_time.date.Month=9;
				date_time.date.Date=30;
				date_time.time.Hour=1;
				date_time.time.Min=30;
				date_time.time.Sec=0;
				send_cmd(4,(char *)&date_time,sizeof(STU_DATE_TIME),APP_TO_SERVER,CMD_FLAG);
				break;
			case 4: send_cmd(5,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 5: send_cmd(6,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 6: send_cmd(7,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 7: send_cmd(8,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 8: send_cmd(9,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 9: 
				send_cmd(10,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 10: 
				printf("请输入记录编号:");
				//int record_no;
				scanf("%d",&record_no);
				p = (char*)(&record_no);
				tmp[0]=*(p+3);
				tmp[1]=*(p+2);
				tmp[2]=*(p+1);
				tmp[3]=*p;
				send_cmd(11,tmp,4,APP_TO_SERVER,CMD_FLAG);
				break;
			case 11: send_cmd(1,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 12: send_cmd(1,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 13: send_cmd(1,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 14: send_cmd(1,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 15: send_cmd(1,NULL,0,APP_TO_SERVER,CMD_FLAG);break;

			case 16: 
					send_cmd(19,NULL,0,APP_TO_SERVER,CMD_FLAG);
					break;
			case 17: send_cmd(21,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			case 18: 
					printf("请输入K值:");
					int k;
					scanf("%d",&k);
					tmp[0]=k>>8;
					tmp[1]=k;
					send_cmd(22,tmp,2,APP_TO_SERVER,CMD_FLAG);
					break;
			case 19:
					printf("请输入记录编号:");
					//int record_no;
					scanf("%d",&record_no);
					p= (char*)(&record_no);
					tmp[0]=*(p+3);
					tmp[1]=*(p+2);
					tmp[2]=*(p+1);
					tmp[3]=*p;
					printf("请输入乘客标识:");
					scanf("%c",&tmp[4]);
					printf("请输入拼车状态:");
					scanf("%c",&tmp[5]);
					send_cmd(23,tmp,6,APP_TO_SERVER,CMD_FLAG);
					break;
			case 20: send_cmd(24,NULL,0,APP_TO_SERVER,CMD_FLAG);break;
			
			
			
			
			
			
			}
		}
	}
	close(g_cmd_client.sockdf);
	return 0;

}
