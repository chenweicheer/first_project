

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
STU_CMDCLIENT   g_cmd_client;


/*
 * 命令对应处理函数
 * */
STU_CMD_AND_FUNC g_cmd_pfunc[AMOUNT_CMD] =
{
    {
        CMD_START_CHECK, start_check
    },
    {
        CMD_SEND_TOPLIGHT, send2_toplight
    },
    {
        CMD_RETURN_TOPLIGHT, toplight_return
    },
    {
        CMD_SYN_TIME, syn_time
    },
    {
        CMD_QRY_PRINTER, query_printer
    },
    {
        CMD_QRY_MEASURE, query_measure
    },
    {
        CMD_QRY_TOPLIGHT, query_toplight
    },
    {
        CMD_CHECK_NUM, check_num
    },
    {
        CMD_OBTN_PRICE_PARA, get_price_para
    },
    {
        CMD_SET_PRICE_PARA, set_price_para
    },
    {
        CMD_RESET_MEASURE, reset_measure
    },
    {
        CMD_START_RECORD, start_record
    },
    {
        CMD_OPRATE_UP, report_data
    },
    {
        CMD_STOP_RECORD, stop_record
    },
    {
        CMD_GET_TOTAL_DATA, get_total_data
    },
    {
        CMD_ONEKEY_POLICE, one_key_alarm
    },
    {
        CMD_HARDWARE_ERR, hardware_error
    },
    {
        CMD_RETN_POLICE_ERR, harw_alarm
    },
    {
        CMD_PRINTER_DATA, print_data
    },
    {
        CMD_PRINTER_UP, report_print
    },
    {
        CMD_GET_K_VALUE, get_k
    },
    {
        CMD_SET_K_VALUE, set_k
    },
    {
        CMD_CHENGE_PASSENGER_STATUS, change_passenger_status
    },
    {
        CMD_SYNCHRONOUS, syndata
    },
    {
        CMD_RENAME_BLUE, rename_bluetooth
    }
};







//命令有效性检查
int cmd_check(u8 * cmd)
{

    int             len = cmd[2] << 8 | cmd[3];
    u16             crc = cmd[len - 2] << 8 | cmd[len - 1];
    u16             cmd_value = cmd[8] << 8 | cmd[9];

    //消息头验证
    if (0x5A != (u8)cmd[0] || 0xA5 != (u8)cmd[1])
    {
        printf("header error\n");
        return HEADERERROR;
    }

    //消息长度验证
    else if (len < HEADER_LEN || len > BUFFLEN)
    {
        printf("len error\n");
        return LENERROR;
    }

    //crc16验证
    else if (crc != (crc16(cmd, len - 2)))
    {
        printf("len %d\n", len);
        printf("crc %x		calculatecrc %x\n", crc, (crc16(cmd, len - 2)));
        printf("crc16 check error\n");
        return CRCERROR;
    }
    else if (cmd_value < 1 || cmd_value > AMOUNT_CMD)
    {
        printf(" no cmd\n");
        return NOCMD;
    }

    //printf("data check is cmd\n");
    return OK;
}





void fault_handle()
{
    printf("fault handle\n");
}



void cmd_dispatch(void * param)
{

    STU_CMD *       cmd = (STU_CMD *)param;

    int             i   = 0;

    for (; i < AMOUNT_CMD; i++)
    {
        if (g_cmd_pfunc[i].cmd == cmd->cmd_value)
        {
            g_cmd_pfunc[i].pfunc((unsigned char *) cmd,
                                 g_cmd_client.sockdf);
        }
    }
}


//接收到响应命令处理 查找队列
void rec_cmd_handle(void * pointer1,void * pointer2)
{
	STU_NODE *node= (STU_NODE *)pointer1;
	STU_CMD *scmd = node->cmd; 
	STU_CMD *rcmd = (STU_CMD*)pointer2;

	//printf("rec_resp_cmd_handle scmd %d rcmd %d\n",scmd->sec,rcmd->sec);
	
	if(rcmd->sec==scmd->sec)
	{
		printf("rec_resp_cmd_handle cmd->sec %d cmd %d\n",scmd->sec,scmd->cmd_value);
		dlist_remove(&g_wait_res_cmd_queue, node);
		dlist_destroy_node(node);
	}
	
}


/*命令分发处理
 *
 * 根据命令的
 * */
void cmd_proc()
{

    STU_CMD *       cmd = NULL;

    //循环读取命令
    while (1)
    {
        cmd                   = dequeue(&g_recv_cmd_queue);

        if (NULL!=cmd)
        {
            //命令是响应
            if (RESPONE_FLAG == cmd->cmd_flag)
            {

				dlist_treaverse(&g_wait_res_cmd_queue,rec_cmd_handle,cmd);
          

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
		printf("node address %p\n",node);
		node->timeout_count++;
		
		printf(" now_sec %d   cmd_sec  %d\n",now_sec,cmd->sec);
		printf("\ntimeout count:%d\n\n",node->timeout_count);
		dlist_add_node_tail(&g_send_cmd_queue,node);
		
	}

}



void check_timeout_proc()
{
	while(1)
	{
		dlist_treaverse(&g_wait_res_cmd_queue,check_timeout,NULL);
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

		//dlist_remove(&g_send_cmd_queue,node);
			
		if(NULL==node)
		{
			//printf("NULL+++++++++++++++++\n");
			sleep(1);
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

		//dlist_remove(&g_send_cmd_queue,node);
		//如果是命令则进入等待回复队列 否则清除内存
		if(CMD_FLAG==cmd->cmd_flag)
		{
			//printf("dlist_enqueue\n");	
			//是命令    从发送队列删除            进入等待回复队列
			//dlist_remove(&g_send_cmd_queue,node);
			dlist_add_node_tail(&g_wait_res_cmd_queue, node);
		}
		else
		{
			//发送完成从队列移除 并清除内存
			//dlist_remove(&g_send_cmd_queue,node);
			dlist_destroy_node(node);
		}
	}
}





void send_cmd(u16 cmd, char * data, int len, unsigned char flag,char cmd_flag)
{
	//printf("send_cmd\n");
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



void respone_cmd(STU_CMD*cmd,char * data, int len, unsigned char source_dest)
{
	

	u16 sec;
	unsigned char tmp[BUFFLEN];	
	u16 crc;


	tmp[0] = 0x5a;
	tmp[1] = 0xa5;
	tmp[2] = len+HEADER_LEN >> 8;
	tmp[3] = len+HEADER_LEN;
	tmp[4] = source_dest;
	tmp[5] = RESPONE_FLAG;


	tmp[6] = ( (u16)cmd->sec) >> 8;
	tmp[7] = ( (u16)cmd->sec);
	tmp[8] = cmd->cmd_value >> 8;
	tmp[9] = cmd->cmd_value;

	//printf("server respone tmp[6] %02x tmp[7] %02x\n",(u8)tmp[6],(u8)tmp[7]);
	//printf("server respone cmd->sec %d\n",cmd->sec);
	
	if (len > 0) {
		memcpy(&tmp[HEADER_LEN - 2], data, len);
	}
	crc = crc16(tmp, len+HEADER_LEN - 2);
	tmp[HEADER_LEN + len - 2] = crc >> 8;
	tmp[HEADER_LEN+len - 1] = crc;

	if (SERVER_TO_APP == source_dest) {
		printf("respone cmd  %d  to APP sec : %d\n", cmd->cmd_value,cmd->sec);
		socket_send_data(tmp, len+HEADER_LEN, g_cmd_client.sockdf);
	} else if (APP_TO_SERVER == source_dest) {
		printf("respone cmd  %d to SERVER \n", cmd->cmd_value);
		socket_send_data(tmp, len+HEADER_LEN, g_cmd_client.sockdf);
	} else if (SERVER_TO_JILIANG == source_dest) {
		printf("respone cmd  %d to jiliang \n", cmd->cmd_value);
		//send_data_to_jiliang(tmp, cmd->len);
	}
}



