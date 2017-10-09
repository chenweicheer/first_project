/*
 * jiliang.h
 *
 *  Created on: 2017-9-11
 *      Author: Administrator
 */

#ifndef JILIANG_H_
#define JILIANG_H_
#include "public.h"
#include<stdio.h>
#include <stdlib.h>
#include "cmd.h"
#include <string.h>

#define JILIANG_NORMAL			0
#define JILIANG_NO_CONN			1
#define JILIANG_FAULT			2
#define JILIANG_SERIAL 			"/dev/ttyS2"
#define JILIANG_BAUDRATE 		115200
#define JILIANG_DATA_BITS 		8
#define JILIANG_CHECK 			's'
#define JILIANG_STOP_BITS 		1



void init_jiliang();
void jiliang_recv();
int send_data_to_jiliang(char *data,int len);



unsigned  char jiliang_get_status();
int jiliang_get_time(unsigned char *);
int jiliang_get_num(unsigned char*);
u16 jiliang_get_k();
int jiliang_get_price_param(STU_PRICE_PARA*);
#endif /* JILIANG_H_ */
