/*
 * cmd.c
 *
 *  Created on: 2017-8-28
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <stddef.h>
#include <signal.h>
#include "cmd.h"
#include "socket_tool.h"
#include "cmd_queue.h"
#include "double_link.h"

extern STU_DLIST g_wait_res_cmd_queue;
extern STU_DLIST g_send_cmd_queue;
extern STU_QUEUE_CMD g_recv_cmd_queue;


STU_CMDCLIENT g_cmd_client;


//命令有效性检查
int cmd_check(u8*cmd)
{


	int len = cmd[2] << 8 | cmd[3];
	u16 crc = cmd[len-2]<<8|cmd[len-1];
	u16 cmd_value = cmd[8] << 8 | cmd[9];
	//消息头验证
	if(0x5A!=cmd[0]|| 0xA5!=cmd[1])
	{
		printf("header error\n");
		return HEADERERROR;
	}
	//消息长度验证
	else if(len<HEADER_LEN||len>BUFFLEN)
	{
		printf("len error\n");
		return LENERROR;
	}
	//crc16验证
	else if(crc!=(crc16(cmd,len-2)))
	{
		printf("len %d\n",len);
		printf("crc %x		calculatecrc %x\n",crc,(crc16(cmd,len-2)));
		printf("crc16 check error\n");
		return CRCERROR;
	}
	else if(cmd_value < 1||cmd_value > AMOUNT_CMD)
	{
		printf(" no cmd\n");
		return NOCMD;
	}
	//printf("data check is cmd\n");
	return OK;
}


void client_recv_hander() {
	u8 buf[N];
	int len=0,l;
	STU_CMD cmd;
	while(1)
	{	
		memset(buf,0,1024);
		len=read(g_cmd_client.sockdf ,&buf[0],1);
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


				cmd.sec=buf[6] <<8 | buf[7];


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
				//printf("\n");	
				enqueue(&g_recv_cmd_queue,cmd);
				}
			else
			{
				printf("header error: %02x%02x\n",(unsigned char)buf[0],(unsigned char)buf[1]);
			}
			}
		}
	printf("close:%d\n",g_cmd_client.sockdf);
	close(g_cmd_client.sockdf);
}




void  check_timeout(void * pointer1,void *pointer2)
{
	
	STU_NODE *node= (STU_NODE *)pointer1;
	if(NULL==node)
	{
		printf("check_timeout null point\n");
		return;
	}
	STU_CMD *cmd = node->cmd;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	u16 now_sec= (u16)tv.tv_sec*1000+tv.tv_usec/1000;

	//printf(" now_sec %d   cmd_sec  %d\n",now_sec,cmd->sec);
	//printf("second:%d\n",(u16)tv.now_sec);  //秒
	//printf("\ntimeout count:%d\n\n",node->timeout_count);
	
	int value = cmd->sec-now_sec;	

	if(value<10 && value>=0)
	//if(now_sec==cmd->sec)
	{
		dlist_remove(&g_wait_res_cmd_queue, node);
		if(TIMEOUT_MAX_COUNT == node->timeout_count)
		{
			//清空内存 进入故障处理
			dlist_destroy_node(node);
			printf("fault\n");
			return;
		}
		//printf("node address %p\n",node);
		node->timeout_count++;
		
		//printf(" now_sec %d   cmd_sec  %d\n",now_sec,cmd->sec);
		printf("timeout count:%d\n\n",node->timeout_count);
		dlist_add_node_tail(&g_send_cmd_queue,node);
		
	}

}


void cmd_dispatch(void * param)
{
	int i;
	STU_CMD * cmd = (STU_CMD *) param;

	switch (cmd->cmd_value)
	{
	case CMD_RETURN_TOPLIGHT:
		//send_cmd(CMD_RETURN_TOPLIGHT, NULL, 0, APP_TO_SERVER,RESPONE_FLAG);
		respone_cmd(cmd, NULL, 0, APP_TO_SERVER);
		printf(" client execute cmd%d\n",cmd->cmd_value);
		break;
	case CMD_SYN_TIME:
		printf("syn time success\n");
		for (i = 0; i < cmd->len - HEADER_LEN; i++) {
			printf("%d ", cmd->data[i]);
		}
		break;
	case CMD_START_CHECK:
		printf("recv start start check respone :%s\n",cmd->data);
		break;
	}
}

void fault_handle()
{
	printf("fault handle\n");
}




void cmd_proc()
{
    STU_CMD *       cmd = NULL;
    //循环读取命令
    while (1)
    {
        cmd                   = dequeue(&g_recv_cmd_queue);
		//printf("cmd_proc\n");
        if (NULL!=cmd)
        {
            //命令是响应
            if (RESPONE_FLAG == cmd->cmd_flag)
            {
            	printf("client recv respone cmd:%d\n",cmd->cmd_value);
				dlist_treaverse(&g_wait_res_cmd_queue,rec_resp_cmd_handle,cmd);
                //启动线程处理
                pthread_t       id;
                int             ret;
                ret                 = pthread_create(&id, NULL, (void *) cmd_dispatch, (void *)cmd);
                if (ret != 0)
                {
                    printf("Create pthread error!n");
                }
            }
            //命令不是响应
            else
            {
            	printf("client recv cmd:%d",cmd->cmd_value);
                //启动线程处理
                pthread_t       id;
                int             ret;
                ret                 = pthread_create(&id, NULL, (void *) cmd_dispatch, (void *)cmd);
                if (ret != 0)
                {
                    printf("Create pthread error!n");
                }
            }
        }
    }
}

//接收到响应命令处理 查找队列
void rec_resp_cmd_handle(void * pointer1,void * pointer2)
{
	STU_NODE *node= (STU_NODE *)pointer1;
	STU_CMD *scmd = node->cmd;
	STU_CMD *rcmd = (STU_CMD*)pointer2;
	
	//printf("rec_resp_cmd_handle scmd %d rcmd %d\n",scmd->sec,rcmd->sec);

	if((u16)rcmd->sec==(u16)scmd->sec && rcmd->cmd_value ==scmd->cmd_value )
	{
	
		printf("rec_resp_cmd_handle scmd %d cmd %d\n",scmd->sec,rcmd->cmd_value);
		dlist_remove(&g_wait_res_cmd_queue, node);
		dlist_destroy_node(node);
	}
}




void send_cmd_proc()
{
	struct timeval tv;
	STU_CMD * cmd;
	STU_NODE *node;
	unsigned char   tmp[BUFFLEN];
	u16            		 crc ;
	u16 				 sec;
	tmp[0]              = 0x5a;
	tmp[1]              = 0xa5;
	while(1)
	{
		node = dlist_dequeue(&g_send_cmd_queue);

		if(NULL==node)
		{
			//printf("NULL+++++++++++++++++\n");
			continue;
		}

		cmd =node->cmd;
		//printf("send cmd %d\n",cmd->cmd_value);
		//清除节点内存 命令内存不清
		//free(node);
		//node=NULL;

		
		
	    tmp[2]              = (cmd->len) >> 8;
	    tmp[3]              = (cmd->len);
	    tmp[4]              = cmd->source_dest;
	    tmp[5]              = cmd->cmd_flag;


		//获取当前时间值
    	gettimeofday(&tv,NULL);
    	//printf("second:%d\n",(u16)tv.tv_sec);  //秒
		sec = (u16)(tv.tv_sec*1000+tv.tv_usec/1000 + 1000*TIMEOUT);
		cmd->sec=sec;
		
	    tmp[6]              = sec>>8;
	    tmp[7]              = sec;
	    tmp[8]              = cmd->cmd_value >> 8;
	    tmp[9]              = cmd->cmd_value;
		if (cmd->len-HEADER_LEN > 0)
	    {
	        memcpy(&tmp[HEADER_LEN - 2], cmd->data, cmd->len-HEADER_LEN);
	    }
		crc = crc16(tmp,cmd->len - 2);
		tmp[cmd->len - 2] = crc >> 8;
    	tmp[cmd->len - 1] = crc;

		//printf("crc %x\n",crc);
		
		if (SERVER_TO_APP == cmd->source_dest )
	    {
	    	printf("send cmd  %d to APP \n", cmd->cmd_value);
	        socket_send_data(tmp, cmd->len, g_cmd_client.sockdf);
	    }
		else if(APP_TO_SERVER==cmd->source_dest)
		{
			printf("send cmd  %d to SERVER \n", cmd->cmd_value);
			socket_send_data(tmp, cmd->len, g_cmd_client.sockdf);
		}
	    else if (SERVER_TO_JILIANG == cmd->source_dest)
	    {
	    	printf("send cmd  %d to jiliang \n", cmd->cmd_value);
	        //send_data_to_jiliang(tmp, cmd->len);
	    }

		//发送完成从发送队列中删除
		//dlist_remove(&g_send_cmd_queue,node);
		
		//如果是命令则进入等待回复队列 否则清除内存
		if(CMD_FLAG==cmd->cmd_flag)
		{
			//printf("dlist_enqueue\n");	
			//是命令    从发送队列删除            进入等待回复队列
			//dlist_remove(&g_send_cmd_queue,node);
			dlist_add_node_tail(&g_wait_res_cmd_queue, node);
			//dlist_destroy_node(node);
		}
		else
		{
			//发送完成从队列移除 并清除内存
			//dlist_remove(&g_send_cmd_queue,node);
			dlist_destroy_node(node);
		}
	}
}



void respone_cmd(STU_CMD*cmd,char * data, int len, unsigned char source_dest)
{
	printf("cmd->sec %d\n",cmd->sec);

	u16 sec;
	unsigned char tmp[BUFFLEN];	
	u16 crc;


	tmp[0] = 0x5a;
	tmp[1] = 0xa5;
	tmp[2] = len+HEADER_LEN >> 8;
	tmp[3] = len+HEADER_LEN;
	tmp[4] = source_dest;
	tmp[5] = RESPONE_FLAG;


	tmp[6] = ( cmd->sec) >> 8;
	tmp[7] = ( cmd->sec);
	tmp[8] = cmd->cmd_value >> 8;
	tmp[9] = cmd->cmd_value;
	if (len > 0) {
		memcpy(&tmp[HEADER_LEN - 2], data, len);
	}
	crc = crc16(tmp, len+HEADER_LEN - 2);
	tmp[HEADER_LEN + len - 2] = crc >> 8;
	tmp[HEADER_LEN+len - 1] = crc;

	if (SERVER_TO_APP == source_dest) {
		printf("respone cmd  %d to APP \n", cmd->cmd_value);
		socket_send_data(tmp, len+HEADER_LEN, g_cmd_client.sockdf);
	} else if (APP_TO_SERVER == source_dest) {
		printf("respone cmd  %d to SERVER \n", cmd->cmd_value);
		socket_send_data(tmp, len+HEADER_LEN, g_cmd_client.sockdf);
	} else if (SERVER_TO_JILIANG == source_dest) {
		printf("respone cmd  %d to jiliang \n", cmd->cmd_value);
		//send_data_to_jiliang(tmp, cmd->len);
	}
}




void send_cmd(u16 cmd, char * data, int len, unsigned char flag,char cmd_flag)
{
	//printf("call send_cmd\n");
	STU_CMD *tmp = malloc(sizeof(STU_CMD));
	if(NULL==tmp)
	{
		printf("malloc error\n");
		return;
	}
	tmp->header_flag=0xA55A;
	tmp->cmd_value=cmd;
	tmp->cmd_flag=cmd_flag;
	tmp->len=len+HEADER_LEN;
	tmp->source_dest=flag;
	if (len > 0)
	{
		memcpy(tmp->data, data, len);
	}
	dlist_enqueue(&g_send_cmd_queue, tmp);
}
