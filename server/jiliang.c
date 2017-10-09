/*
 * jiliang.c
 *
 *  Created on: 2017-9-11
 *      Author: Administrator
 */

#include "jiliang.h"
#include "cmd.h"
unsigned  char jiliang_get_status()
{
	return JILIANG_NORMAL;
}

int jiliang_get_time(unsigned char *a)
{
	return 0;
}


int jiliang_get_num(unsigned char* num)
{
	return 0;
}
u16 jiliang_get_k()
{
	return 0;
}
int jiliang_get_price_param(STU_PRICE_PARA* price_param)
{
	return 0;
}

void init_jiliang()
{
	g_cmd_client.jiiang_fd = open_dev(JILIANG_SERIAL);
	if(g_cmd_client.jiiang_fd <0)
	{
		printf("open serial failure\n");
		return ;
	}
	set_speed(g_cmd_client.jiiang_fd ,JILIANG_BAUDRATE);
	set_parity(g_cmd_client.jiiang_fd ,JILIANG_DATA_BITS,JILIANG_STOP_BITS,JILIANG_CHECK);
	return;
}

/*
	串口接收数据
*/
void jiliang_recv()
{
	u8 buf[BUFFLEN]={0}; 
	int len;
	STU_CMD cmd;
	if(g_cmd_client.jiiang_fd  < 0)
	{
		return ;
	}
	u16 l=0;
	
		
	while(1)
	{	
		memset(buf,0,1024);
		len=read(g_cmd_client.jiiang_fd ,&buf[0],1);
		printf("%02x ",buf[0]);
		if(len>0)
		{
			//读取第一个字节
			if(0x5A==(unsigned char)buf[0])	
			{	//读取第二个字节
				len=read(g_cmd_client.jiiang_fd ,&buf[1],1);
				if(len<0)
				{	
					printf("read failure\n");
					continue;
				}
				if(0xA5==(unsigned char)buf[1])
				{
					cmd.header_flag=0xA55A;

					//读取长度
					len=readn(g_cmd_client.jiiang_fd ,&buf[2],2);
					if(2!=len)
					{	
						printf("read failure len:%d\n",len);
						continue;
					}
					l=buf[2] << 8 | buf[3];


					//长度检查
					if(l<HEADER_LEN||l>BUFFLEN)
					{
						printf("cmd len error :%d\n",l);
						continue;
					}

					//命令长度赋值
					cmd.len=l;

					//读取后面的数据
					len=readn(g_cmd_client.jiiang_fd , &buf[4],l-4);
					if((l-4)!=len)
					{	
						printf("read failure len:%d\n",len);
						continue;
					}

					//命令流向
					cmd.source_dest=buf[4];
					
					//命令或响应标识
					cmd.cmd_flag=buf[5];

					//保留位 2字节
					cmd.sec=

					//命令值 2字节
					cmd.cmd_value=buf[8] << 8 | buf[9];

					//命令值检查
					if(cmd.cmd_value < 1||cmd.cmd_value > AMOUNT_CMD)
					{
						printf("cmd_value error:%d",cmd.cmd_value);
						continue;
					}

					//crc读取
					cmd.crc=buf[cmd.len-2] << 8 | buf[cmd.len-1];


					//crc验证
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
					
					//将命令放在命令队列
					enqueue(cmd);
				}
				else
				{
					printf("header error: %02x%02x\n",(unsigned char)buf[0],(unsigned char)buf[1]);
				}
			}
		}
	}
}




int send_data_to_jiliang(char *data,int len)
{
	int i=0;
	printf("send data to jiliang:");
	for(;i<len;i++)
	{
		printf("%02x ",(unsigned char)data[i]);
	}
	printf("\n");
	if(g_cmd_client.jiiang_fd <0)
	{
		printf("searial error\n");
		return -1;
	}
	if(write(g_cmd_client.jiiang_fd ,data,len)<0)
	{
		perror("fail to send");
		return -1;
	}
	return 0;
}


