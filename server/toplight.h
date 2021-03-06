

/*
 * toplight.h
 *
 *  Created on: 2017-8-28
 *      Author: Administrator
 */
#ifndef TOPLIGHT_H_
#define TOPLIGHT_H_
#include <stdio.h>
#include "public.h"
#include "cmd.h"

#define TOPLIGHT_NORMAL         0
#define TOPLIGHT_NO_CONN        1
#define TOPLIGHT_FAULT          2
#define TOPLIGHT_EMPTY			3
#define TOPLIGHT_CARPOOL_1		4
#define TOPLIGHT_FULL			5
#define TOPLIGHT_SOS			6


#define TOPLIGHT_BACK_SCREEN_COMPANY_NAME 	1
#define TOPLIGHT_BACK_SCREEN_SOS		  	2
#define TOPLIGHT_FRONT_SCREEN_EMPTY			3
#define TOPLIGHT_FRONT_SCREEN_CARPOOL_1		4
#define TOPLIGHT_FRONT_SCREEN_FULL			5
#define TOPLIGHT_FRONT_SCREEN_SOS			6



//串口定义
#define TOPLIGHT_SERIAL         "/dev/ttyS1"
#define TOPLIGHT_BAUDRATE       9600
#define TOPLIGHT_DATA_BITS      8
#define TOPLIGHT_CHECK          's'
#define TOPLIGHT_STOP_BITS      1

//顶灯返回错误码定义
#define LED_RTCODE_OK           0           /*无错误*/
#define LED_RTCODE_NOTCMD       1           /*不支持命令*/
#define LED_RTCODE_CHECKSUM_REEOR 2         /*校验和错误*/
#define LED_RTCODE_NOT_TAG      3           /*TAG不支持*/
#define LED_RTCODE_ID_ERROR     4           /*节目，分区，播放项ID超出范围*/
#define LED_RTCODE_NOT_SCREEN_PARAM 5       /*屏参不支持*/
#define LED_RTCODE_DATA_TOO_LONG 6          /*数据包过长最大1024*/
#define LED_RTCODE_NO_EXIST     7           /*指定节目，分区，播放项不存在*/
#define LED_RTCODE_NO_IO        8           /*IO配置中 IO不存在*/
#define LED_RTCODE_TAG_PARAM_ERROR 9            /*TAG参数非法*/
#define LED_RTCODE_NOT_DEFINE   0xff        /*没有定义错误*/


//屏幕特效定义
#define LED_LEFT_SHIFT_CONTINUOUS 0         /*连续左移*/
#define LED_SHOW_NOW              1         /*立即显示*/
#define LED_LEFT_SHIFT            2         /*左移*/
#define LED_RIGHT_SHIFT           3         /*右移*/
#define LED_UP_SHIFT              4         /*上移*/
#define LED_DOWN_SHIFT            5         /*下移*/
#define LED_GLINT_DIM             0x1E      /*亮灭闪烁*/
#define LED_GLINT_WHITE           0X1F      /*亮白闪烁*/







/*

协议头*/
typedef struct
{
    u16             syn;                                //通讯同步头0x55aa  发送时先低字节发送AA在发55
    u16             addr;                               //控制卡协议地址只有地址匹配的协议卡才会响应 0xffff为广播地址
    u16             len;                                //协议包长度 由SERIAL开始计算到协议末尾的校验和
    u16             segial;                             //包序号只需要保证不同包序号不同
    u8              type;                               //定制固定为0xc1
    u8              cmd;                                //0x2表示写，0x3表示读 在回包时 0x82回命令2 ，0x83回命令3
} STU_LED_HEADER;


/* tag 功能

0x01  保留
0x02  保留
0x03  保留
0x04  开关机
0x05  实时时钟
0x06  亮度
0x07  协议地址
0x08  删除指令
0x09  存储方式
0x0A  设备信息
0x0B  屏参
0x0C  节目 ID
0x0D  分区 ID
0x0E  播放项 ID
0x0F  节目属性
0x10  分区属性
0x11  播放项属性
0x12  数据包属性
0x13  数据包
0x14  播放项特效
0x15  保留
0x16  时段控制
0x17  IO 绑定
0x18  切换指令
0x19  实时刷新
0x1A  节目资源分配
*/
typedef struct
{
    u8              tag;                                //TLV单元标签
    u8              length[3]; /*TLV value的长度 本身为变长 当length值小于0x80长度为一字节，
                        大于等于0x80小于0xff时为2个字节第一个字节为0x81，
                        大于0xff本身为3个字节,
                        控制发送来实现
                        */
    u8 *            value;                              //TLV具体数据  长度length确定

} STU_LED_TLV;


typedef struct
{
    STU_LED_HEADER  header;
	STU_LED_TLV tlv;	
	u16 checksum;
} STU_LED_PACK;



u16 cal_checksum(u8 *data,int len);



unsigned char toplight_get_status();


int init_toplight();


int send_data_to_toplight(char *data,int len);
int send_cmd_to_toplight(char *data,int len);
int toplight_set_sos();
int toplight_set_carpool_1();
int toplight_set_full();
int toplight_set_empty();







#endif /* TOPLIGHT_H_ */

