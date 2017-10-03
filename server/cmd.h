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


//����͵���ʼ��־
#define APP_TO_SERVER 			0
#define SERVER_TO_APP 			1
#define JILIANG_TO_SERVER		2
#define TOPLIGHT_TO_SERVER		3
#define PRINTER_TO_SERVER		4
#define SERVER_TO_JILIANG 		5
#define SERVER_TO_TOPLIGHT 		6
#define SERVER_TO_PRINTER 		7

//�������Ӧ
#define CMD_FLAG 	1
#define RESPONE_FLAG 0

//��ʱ���ط�����
#define TIMEOUT 					2
#define TIMEOUT_MAX_COUNT			3


/*����״̬*/
#define CMD_STATUS_RET_SUCCES 	1				//�����յ��ظ�
#define CMD_STATUS_WAIT			0				//�ȴ������



/*����ö��*/
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



/*���ﶨ������ �붨��˳���й� ����ע��˳��*/
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






//����ṹ
typedef struct
{
	u16 header_flag;			//ͷ��ʶ
	u16 len;					//�ܵ����ݳ���
	u8 source_dest;				//���������־ �����������������ȥ
	u8 cmd_flag;				//������ ���һλ��ʶ�����������ڶ�λ��ʶ������ǻظ�
	u16 sec;				//����������
	u16 cmd_value;				//����


	u8 data[DATALEN];	//����
	u16 crc;			//crc��֤
}STU_CMD;


typedef struct CMDCLIENT
{
	//STU_CMD current_cmd;		//���浱ǰ��������
	//int time_out_count;		//��¼��ʱ����
	int sockdf;				//socket
	int jiiang_fd;			//��������������
	int toplight_fd;		//���ƴ���������
	int printer_fd;			//����������
	//int cmd_status;			//����״̬
}STU_CMDCLIENT;


typedef struct CMD_AND_FUNC
{
	int cmd;
	PFUNC pfunc;
}STU_CMD_AND_FUNC;


int cmd_check(u8*cmd);
void cmd_proc();

//���ϴ���
void fault_handle();

void send_cmd(u16 cmd, char * data, int len, unsigned char flag,char cmd_flag);

//�����������
void send_cmd_proc();

void respone_cmd(STU_CMD*cmd,char * data, int len, unsigned char source_dest);
void  check_timeout(void * pointer1,void *pointer2);


void check_timeout_proc();


extern STU_CMDCLIENT   g_cmd_client;




#endif /* CMD_H_ */
