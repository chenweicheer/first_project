/*
 * toplight.c
 *
 *  Created on: 2017-8-28
 *      Author: Administrator
 */

#include "toplight.h"
#include <pthread.h>

pthread_mutex_t g_toplight_lock=PTHREAD_MUTEX_INITIALIZER;


//后屏公司名字长度22
unsigned char back_screen_company_name[]={0xAA,0x55,0xFF,0xFF,0x10,0x00,0x01,0x00,0xC1,0x02,0x18,0x08,0x05,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xF4,0x05};
//后屏sos 长度22
unsigned char back_screen_sos[]={0xAA,0x55,0xFF,0xFF,0x10,0x00,0x02,0x00,0xC1,0x02,0x18,0x08,0x05,0x00,0x00,0x01,0x00,0x00,0xFF,0xFF,0xF6,0x05};
//前屏空车 长度22
unsigned char front_screen_empty[]={0xAA,0x55,0xFF,0xFF,0x10,0x00,0x03,0x00,0xC1,0x02,0x18,0x08,0x05,0x00,0x01,0x00,0x00,0x00,0xFF,0xFF,0xF7,0x05};
//前屏可以拼一人 22
unsigned char front_screen_carpool_1[]={0xAA,0x55,0xFF,0xFF,0x10,0x00,0x04,0x00,0xC1,0x02,0x18,0x08,0x05,0x00,0x01,0x01,0x00,0x00,0xFF,0xFF,0xF9,0x05};
//前屏满车 长度22
unsigned char front_screen_full[]={0xAA,0x55,0xFF,0xFF,0x10,0x00,0x05,0x00,0xC1,0x02,0x18,0x08,0x05,0x00,0x01,0x02,0x00,0x00,0xFF,0xFF,0xFB,0x05};
//前屏sos 长度22
unsigned char front_screen_sos[]={0xAA,0x55,0xFF,0xFF,0x10,0x00,0x06,0x00,0xC1,0x02,0x18,0x08,0x05,0x00,0x01,0x03,0x00,0x00,0xFF,0xFF,0xFD,0x05};


//前后屏状态
unsigned char g_front_screen_status=0;
unsigned char g_back_screen_status=0;

int toplight_set_empty()
{
	if(TOPLIGHT_BACK_SCREEN_COMPANY_NAME!=g_back_screen_status)
	{
		if(0!=send_cmd_to_toplight(back_screen_company_name, 22))
		{
			printf("toplight fault\n");
			return -1;
		}
	}
	int ret =send_cmd_to_toplight(front_screen_empty, 22);
	if(0!=ret)
	{
		printf("toplight fault\n");
		return -1;
	}
	g_back_screen_status=TOPLIGHT_BACK_SCREEN_COMPANY_NAME;
	g_front_screen_status=TOPLIGHT_FRONT_SCREEN_EMPTY;
	return 0;
}


int toplight_set_full()
{
	if(TOPLIGHT_BACK_SCREEN_COMPANY_NAME!=g_back_screen_status)
	{
		if(0!=send_cmd_to_toplight(back_screen_company_name, 22))
		{
			printf("toplight fault\n");
			return -1;
		}
	}
	int ret=send_cmd_to_toplight(front_screen_full, 22);
	if(0!=ret)
	{
		printf("toplight fault\n");
		return -1;
	}
	g_back_screen_status=TOPLIGHT_BACK_SCREEN_COMPANY_NAME;
	g_front_screen_status=TOPLIGHT_FRONT_SCREEN_FULL;
	return 0;
}

int toplight_set_carpool_1()
{
	if(TOPLIGHT_BACK_SCREEN_COMPANY_NAME!=g_back_screen_status)
	{
		if(0!=send_cmd_to_toplight(back_screen_company_name, 22))
		{
			printf("toplight fault\n");
			return -1;
		}
	}
	int ret=send_cmd_to_toplight(front_screen_carpool_1, 22);
	if(0!=ret)
	{
		printf("toplight fault\n");
		return -1;
	}
	g_back_screen_status=TOPLIGHT_BACK_SCREEN_COMPANY_NAME;
	g_front_screen_status=TOPLIGHT_FRONT_SCREEN_FULL;
	return 0;
}

int toplight_set_sos()
{
	int ret= send_cmd_to_toplight(back_screen_sos, 22);
	if(0!=ret)
	{
		printf("toplight fault\n");
		return -1;
	}
	ret = send_cmd_to_toplight(front_screen_sos, 22);
	if(0!=ret)
	{
		printf("toplight fault\n");
		return -1;
	}
	g_back_screen_status=TOPLIGHT_BACK_SCREEN_SOS;
	g_front_screen_status=TOPLIGHT_FRONT_SCREEN_SOS;
	return 0;
}


int init_toplight()
{
	g_cmd_client.toplight_fd = open_dev(TOPLIGHT_SERIAL);
	if(g_cmd_client.toplight_fd <0)
	{
		printf("open serial failure\n");
		return ;
	}
	set_speed(g_cmd_client.toplight_fd ,TOPLIGHT_BAUDRATE);
	set_parity(g_cmd_client.toplight_fd ,TOPLIGHT_DATA_BITS,TOPLIGHT_STOP_BITS,TOPLIGHT_CHECK);

	toplight_set_sos();
	return 0;
}



unsigned  char toplight_get_status()
{
	return g_front_screen_status;
}



u16 cal_checksum(u8 *data,int len)
{
	int i=0,sum=0;;
	for(;i<len;i++)
	{
		sum+=data[i];
	}

	return (u16)sum;
}



int  top_light_start()
{
	if(g_cmd_client.toplight_fd <0)
	{
		printf("searial error\n");
		return -1;
	}	
	
}


int send_cmd_to_toplight(char *data,int len)
{
	
	unsigned char buf[13];
	int l=0;
	pthread_mutex_lock(&g_toplight_lock);
	int ret = send_data_to_toplight(data,len);
	if(ret!=0)
	{
		printf("toplight write timeout\n");
		return -1;
	}
	l = readn(g_cmd_client.toplight_fd,buf,13);
	
	pthread_mutex_unlock(&g_toplight_lock);
	int i=0;
	for(;i<l;i++)
	{
		printf("%02x",buf[i]);
	}
	printf("\n");
	if(13!=l)
	{
		printf("toplight recv error\n");
		return -1;
	}
	
	
	return 0;
	
}


int send_data_to_toplight(char *data,int len)
{
	int i=0;
	printf("send data to toplight:");
	for(;i<len;i++)
	{
		printf("%02x ",(unsigned char)data[i]);
	}
	printf("\n");
	if(g_cmd_client.toplight_fd <0)
	{
		printf("searial error\n");
		return -1;
	}	
	if(write(g_cmd_client.toplight_fd ,data,len)<0)
	{
		perror("fail to send");
		return -1;
	}
	return 0;
}





