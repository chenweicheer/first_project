/*
 * public.h
 *
 *  Created on: 2017-8-30
 *      Author: Administrator
 */

#ifndef PUBLIC_H_
#define PUBLIC_H_

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define BUFFLEN  1024


#define ERROR	1			//"ERROR"
#define OK   	0			//"OK" 没有错误
#define HEADERERROR 2		//"HEADERERROR"
#define LENERROR 	3		//"LENERROR"
#define CRCERROR	4		//"CRCERROR"
#define NOCMD		5		//没有这个命令
#define MALLOC_ERROR 6      //申请内存失败


typedef struct
{
    u8 Hour;                //小时
    u8 Min;                 //分钟
    u8 Sec;                 //秒

}TIME;
typedef struct
{
    u8 Year;                //年
    u8 Month;               //月
    u8 Date;                //日

}DATE;

typedef struct
{
	DATE date;
	TIME time;
}STU_DATE_TIME;


typedef  void (*PFUNC)(u8*data,int socket);



typedef struct
{
	char buf[BUFFLEN];
	int index;
}STU_BUF;


typedef struct
{
    u32 record_number;      //记录编号
    u8  carpool_status;     //拼车状态
    u8  passenger_identity; //乘客标识
    u16 mil;                //行驶里程
    u16 amount;             //金额
    u16 price;              //单价
    u16 drive_times;        //行驶时间
    u16 low_speed_times;    //低速时间

    u8  record_status;      //单程状态
    u8  serv_status;        //服务状态

    DATE         operat_day; //营运日期
    TIME        start_time; //营运开始时间
    TIME        finish_time;//营运结束时间

}SERV_DATA;



//计价参数
typedef struct
{

    TIME    day_time;       //日间开始时间
    TIME    night_time;     //夜间开始时间

    u16 start_mil;          //起步里程数
    u16 float_mil;          //上浮单价里程数

    u16 day_start_price;    //日间起步价
    u16 night_start_price;  //夜间起步价

    u16 day_base_price;     //基本日间单价
    u16 night_base_price;   //基本夜间单价

    u16 wait_price;         //等待时间价格

    u16 higher_price_ratio; //上浮单价比例

    u16 low_speed_price;    //低速价格
    u16 low_speed_standard; //低速标准

    u16 discountof2;        //两位乘客折扣
    u16 discountof3;        //三位乘客折扣
    u16 discountof4;        //四位乘客折扣
    u16 wait_time;          //等待时间计算单元
    u16 low_speed_time;		//低速计算单元

}STU_PRICE_PARA;


typedef struct
{
	char zj_id[26];			//主机ID
	char jl_id[26];			//计量ID
}STU_DEVICE_ID;

typedef struct
{
    u32 record_number;      //记录编号
    u8  carpool_status;     //拼车状态
    u8  passenger_identity; //乘客标识
    u16 mil;                //行驶里程
    u16 amount;             //金额
    u16 price;              //单价
    u16 drive_times;        //行驶时间
    u16 low_speed_times;    //低速时间
    u8  record_status;      //单程状态
    u8  serv_status;        //服务状态
}STU_SERV_DATA;

typedef struct
{
	u8 toplight_status;			//顶灯状态
	u8 jiliang_status;			//计量状态
	u8 printer_status;			//打印状态
	TIME zhuji_time;			//主机时间
	TIME jiliang_time;			//计量时间
	STU_DEVICE_ID device_id;	//设备ID
	u16 K;						//K值
	STU_PRICE_PARA price_param;	//计价参数
	STU_SERV_DATA serv_data;	//运营数据

}STU_START_CHECK_DATA;

#endif /* PUBLIC_H_ */
