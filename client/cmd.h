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
#include "public.h"
#include "crc16.h"


#define AMOUNT_CMD 0x19
#define BUFFLEN  1024
#define HEADER_LEN 12
#define DATALEN  BUFFLEN-HEADER_LEN


//命令发送的起始标志
#define APP_TO_SERVER 			0
#define SERVER_TO_APP 			1
#define JILIANG_TO_SERVER		2
#define SERVER_TO_JILIANG 		3
#define TOPLIGHT_TO_SERVER		4
#define PRINTER_TO_SERVER		5
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

//命令结构
typedef struct CMD
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
	int sockdf;				//socket
	int cmd_status;			//命令状态
}STU_CMDCLIENT;


typedef struct sbuf
{
    u8 buf [BUFFLEN];               
    int index ;
}STU_BUF;




void client_recv_hander();
void cmd_proc();

int cmd_check(u8*);

//故障处理
void fault_handle();
void check_timeout(void * pointer1,void *pointer2);
void cmd_dispatch(void * param);
void rec_resp_cmd_handle(void * pointer1,void * pointer2);
void send_cmd_proc();
void send_cmd(u16 cmd, char * data, int len, unsigned char flag,char cmd_flag);

void respone_cmd(STU_CMD*cmd,char * data, int len, unsigned char source_dest);


#endif /* CMD_H_ */
