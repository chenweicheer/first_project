/*
 * cmd.h
 *
 *  Created on: 2017-8-28
 *      Author: Administrator
 */

#ifndef CMD_H_
#define CMD_H_
#include<stdio.h>
#include<stdlib.h>
#include"func.h"
#include "public.h"
#include "crc16.h"



#define AMOUNT_CMD 0x19
#define HEADER_LEN 12
#define DATALEN  BUFFLEN-HEADER_LEN


//命令发送的起始标志
#define APP_TO_SERVER 			0
#define SERVER_TO_APP 			1
#define JILIANG_TO_SERVER		2
#define TOPLIGHT_TO_SERVER		3
#define PRINTER_TO_SERVER		4
#define SERVER_TO_JILIANG 		5
#define SERVER_TO_TOPLIGHT 		6
#define SERVER_TO_PRINTER 		7

//命令和响应
#define CMD_FLAG 	1
#define RESPONE_FLAG 0

//超时和重发次数
#define TIMEOUT 					2
#define TIMEOUT_MAX_COUNT			3


/*命令状态*/
#define CMD_STATUS_RET_SUCCES 	1				//命令收到回复
#define CMD_STATUS_WAIT			0				//等待命令处理



/*命令枚举*/
enum {
	CMD_START_CHECK=1,
	 CMD_SEND_TOPLIGHT=2,
	 CMD_RETURN_TOPLIGHT=3,
	 CMD_SYN_TIME=4,
	 CMD_QRY_PRINTER=5,
	 CMD_QRY_MEASURE=6,
	 CMD_QRY_TOPLIGHT=7,
	 CMD_CHECK_NUM=8,
	 CMD_OBTN_PRICE_PARA=9,
	 CMD_SET_PRICE_PARA=10,
	 CMD_RESET_MEASURE=11,
	 CMD_START_RECORD=12,
	 CMD_OPRATE_UP=13,
	 CMD_STOP_RECORD=14,
	 CMD_GET_TOTAL_DATA=15,
	 CMD_ONEKEY_POLICE=16,
	 CMD_HARDWARE_ERR=17,
	 CMD_RETN_POLICE_ERR=18,
	 CMD_PRINTER_DATA=19,
	 CMD_PRINTER_UP=20,
	 CMD_GET_K_VALUE=21,
	 CMD_SET_K_VALUE=22,
	 CMD_CHENGE_PASSENGER_STATUS=23,
	 CMD_SYNCHRONOUS=24,
	 CMD_RENAME_BLUE=25
};



/*这里定义命令 与定义顺序有关 定义注意顺序*/
/*#define CMD_START_CHECK			0X0001
#define CMD_SEND_TOPLIGHT   		0X0002
#define CMD_RETURN_TOPLIGHT    		0X0003
#define CMD_SYN_TIME  				0X0004
#define CMD_QRY_PRINTER 			0X0005
#define CMD_QRY_MEASURE    			0X0006
#define CMD_QRY_TOPLIGHT    		0X0007
#define CMD_CHECK_NUM   			0X0008
#define CMD_OBTN_PRICE_PARA 		0X0009
#define CMD_SET_PRICE_PARA   		0X000A
#define CMD_RESET_MEASURE		    0X000B
#define CMD_START_RECORD    		0X000C
#define CMD_OPRATE_UP       		0X000D
#define CMD_STOP_RECORD    			0X000E
#define CMD_GET_TOTAL_DATA 			0X000F
#define CMD_ONEKEY_POLICE   		0X0010
#define CMD_HARDWARE_ERR    		0X0011
#define CMD_RETN_POLICE_ERR 		0X0012
#define CMD_PRINTER_DATA    		0X0013
#define CMD_PRINTER_UP      		0X0014
#define CMD_GET_K_VALUE    			0X0015
#define CMD_SET_K_VALUE     		0X0016
#define CMD_CHENGE_PASSENGER_STATUS 0X0017
#define CMD_SYNCHRONOUS     		0X0018
#define CMD_RENAME_BLUE           	0X0019*/






//命令结构
typedef struct
{
	u16 header_flag;			//头标识
	u16 len;					//总的数据长度
	u8 source_dest;				//命令流向标志 命令从哪里来到哪里去
	u8 cmd_flag;				//处理结果 最后一位标识处理结果倒数第二位标识是命令还是回复
	u16 sec;				//保留升级用
	u16 cmd_value;				//命令


	u8 data[DATALEN];	//数据
	u16 crc;			//crc验证
}STU_CMD;


typedef struct CMDCLIENT
{
	//STU_CMD current_cmd;		//保存当前处理命令
	//int time_out_count;		//记录超时次数
	int sockdf;				//socket
	int jiiang_fd;			//计量串口描述符
	int toplight_fd;		//顶灯串口描述符
	int printer_fd;			//串口描述符
	//int cmd_status;			//命令状态
}STU_CMDCLIENT;


typedef struct CMD_AND_FUNC
{
	int cmd;
	PFUNC pfunc;
}STU_CMD_AND_FUNC;


int cmd_check(u8*cmd);
void cmd_proc();

//故障处理
void fault_handle();

void send_cmd(u16 cmd, char * data, int len, unsigned char flag,char cmd_flag);

//发送命令过程
void send_cmd_proc();

void respone_cmd(STU_CMD*cmd,char * data, int len, unsigned char source_dest);
void  check_timeout(void * pointer1,void *pointer2);


void check_timeout_proc();


extern STU_CMDCLIENT   g_cmd_client;




#endif /* CMD_H_ */
