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
#define OK   	0			//"OK" û�д���
#define HEADERERROR 2		//"HEADERERROR"
#define LENERROR 	3		//"LENERROR"
#define CRCERROR	4		//"CRCERROR"
#define NOCMD		5		//û���������
#define MALLOC_ERROR 6      //�����ڴ�ʧ��


typedef struct
{
    u8 Hour;                //Сʱ
    u8 Min;                 //����
    u8 Sec;                 //��

}TIME;
typedef struct
{
    u8 Year;                //��
    u8 Month;               //��
    u8 Date;                //��

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
    u32 record_number;      //��¼���
    u8  carpool_status;     //ƴ��״̬
    u8  passenger_identity; //�˿ͱ�ʶ
    u16 mil;                //��ʻ���
    u16 amount;             //���
    u16 price;              //����
    u16 drive_times;        //��ʻʱ��
    u16 low_speed_times;    //����ʱ��

    u8  record_status;      //����״̬
    u8  serv_status;        //����״̬

    DATE         operat_day; //Ӫ������
    TIME        start_time; //Ӫ�˿�ʼʱ��
    TIME        finish_time;//Ӫ�˽���ʱ��

}SERV_DATA;



//�Ƽ۲���
typedef struct
{

    TIME    day_time;       //�ռ俪ʼʱ��
    TIME    night_time;     //ҹ�俪ʼʱ��

    u16 start_mil;          //�������
    u16 float_mil;          //�ϸ����������

    u16 day_start_price;    //�ռ��𲽼�
    u16 night_start_price;  //ҹ���𲽼�

    u16 day_base_price;     //�����ռ䵥��
    u16 night_base_price;   //����ҹ�䵥��

    u16 wait_price;         //�ȴ�ʱ��۸�

    u16 higher_price_ratio; //�ϸ����۱���

    u16 low_speed_price;    //���ټ۸�
    u16 low_speed_standard; //���ٱ�׼

    u16 discountof2;        //��λ�˿��ۿ�
    u16 discountof3;        //��λ�˿��ۿ�
    u16 discountof4;        //��λ�˿��ۿ�
    u16 wait_time;          //�ȴ�ʱ����㵥Ԫ
    u16 low_speed_time;		//���ټ��㵥Ԫ

}STU_PRICE_PARA;


typedef struct
{
	char zj_id[26];			//����ID
	char jl_id[26];			//����ID
}STU_DEVICE_ID;

typedef struct
{
    u32 record_number;      //��¼���
    u8  carpool_status;     //ƴ��״̬
    u8  passenger_identity; //�˿ͱ�ʶ
    u16 mil;                //��ʻ���
    u16 amount;             //���
    u16 price;              //����
    u16 drive_times;        //��ʻʱ��
    u16 low_speed_times;    //����ʱ��
    u8  record_status;      //����״̬
    u8  serv_status;        //����״̬
}STU_SERV_DATA;

typedef struct
{
	u8 toplight_status;			//����״̬
	u8 jiliang_status;			//����״̬
	u8 printer_status;			//��ӡ״̬
	TIME zhuji_time;			//����ʱ��
	TIME jiliang_time;			//����ʱ��
	STU_DEVICE_ID device_id;	//�豸ID
	u16 K;						//Kֵ
	STU_PRICE_PARA price_param;	//�Ƽ۲���
	STU_SERV_DATA serv_data;	//��Ӫ����

}STU_START_CHECK_DATA;

#endif /* PUBLIC_H_ */
