
#include<stdio.h>
#include "func.h"
#include "socket_tool.h"
#include "toplight.h"
#include "jiliang.h"
#include "printer.h"
#include "public.h"





void start_check(u8 * data, int sockdf)
{
	printf("Call StartCheck\n");

	STU_CMD *		cmd = (STU_CMD *)data;


	//send_cmd(1, NULL, 0, SERVER_TO_JILIANG,CMD_FLAG);

	//获取顶灯状态

	/*tmp->toplight_status=toplight_get_status();
	//获取计量状态
	tmp->jiliang_status=jiliang_get_status();
	//获取打印机状态
	tmp->printer_status=printer_get_status();
	//获取计量时间
	tmp->jiliang_time=jiliang_get_time(NULL);

	tmp->K=jiliang_get_k();*/
	respone_cmd(cmd, "Start data", strlen("Start data"), SERVER_TO_APP);
}


void send2_toplight(u8 * data, int sockdf)
{
	//printf("Call Send2Toplight %02x\n",(unsigned char)data[HEADER_LEN-2]);
	printf("Call Send2Toplight %c\n",(unsigned char)data[HEADER_LEN-2]);

	//回复收到命令
	STU_CMD *		cmd = (STU_CMD *)data;
	int ret=-1;
	respone_cmd(cmd, NULL, 0, SERVER_TO_APP);
	//调用车顶灯命令返回数据
	/*switch(cmd->data[0])
	{	
		case TOPLIGHT_EMPTY:{printf("empty\n");ret =toplight_set_empty();break;}
		case TOPLIGHT_FULL:{printf("full\n");ret =toplight_set_full();break;}
		case TOPLIGHT_CARPOOL_1:{printf("carploll_1\n");ret =toplight_set_carpool_1();break;}
		case TOPLIGHT_SOS:{printf("sos\n");ret =toplight_set_sos();break;}
	}*/
	
	//unsigned char front_screen_sos[]={0xAA,0x55,0xFF,0xFF,0x10,0x00,0x06,0x00,0xC1,0x02,0x18,0x08,0x05,0x00,0x01,0x03,0x00,0x00,0xFF,0xFF,0xFD,0x05};
	//send_cmd_to_toplight(front_screen_sos, 22);

	send_cmd_to_toplight(cmd->data, cmd->len-HEADER_LEN);
	
	unsigned char buf[1];
	buf[0]=(unsigned char)ret;
	//发送命令给客户端
	send_cmd(3, buf,1 , SERVER_TO_APP,CMD_FLAG);
	//send_cmd(1, buf,1 , SERVER_TO_APP,CMD_FLAG);
	return;
}


void toplight_return(u8 * data, int sockdf)
{
	/*printf("Call ToplightReturn\n");
	STU_CMD*cmd =(STU_CMD*)data;
	//获取了顶灯数据  数据处理
	printf("TopLightData : %s",cmd->data);
	ResponeCmd(cmd,sockdf);*/
}


void syn_time(u8 * message, int sockdf)
{
	printf("Call SynTime\n");

	//响应命令
	STU_CMD *		cmd = (STU_CMD *)message;

	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}

}


void query_printer(u8 * data, int sockdf)
{
	printf("Call QueryPrinter\n");
	STU_CMD *		cmd = (STU_CMD *)

	data;
	send_cmd(cmd->cmd_value, NULL, 0, SERVER_TO_APP,RESPONE_FLAG);
}


void query_measure(u8 * data, int sockdf)
{
	printf("Call QueryMeasure\n");

	//响应命令
	STU_CMD *		cmd = (STU_CMD *)data;

	if (APP_TO_SERVER == cmd->source_dest)
	{
		//计量连接错误
		if (g_cmd_client.jiiang_fd < 0)
		{
			cmd->data[0]		= 1;
			cmd->len			= HEADER_LEN + 1;
			send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,CMD_FLAG);
		}
		else 
		{
			send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,RESPONE_FLAG);
		}

	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}

}


void query_toplight(u8 * data, int sockdf)
{
	printf("Call QueryToplight\n");
	unsigned char buf;
	/*if(g_cmd_client.toplight_fd<0)
	{
		buf=1;
	}
	else
	{
		buf=toplight_get_status();
	}*/
	if(g_cmd_client.toplight_fd<0)
	{
		buf=1;
	}
	else
	{
		buf=0;
	}
	send_cmd(CMD_QRY_TOPLIGHT, &buf, 1, SERVER_TO_APP, RESPONE_FLAG);
	return ;
}


void check_num(u8 * data, int sockdf)
{
	printf("Call CheckNum\n");
	pthread_t		id;
	STU_SOCKET_AND_DATA sock_and_data;

	sock_and_data.socket = sockdf;
	char			buf[1024] = "CheckNum";

	sock_and_data.buff	= buf;

}


void get_price_para(u8 * data, int sockdf)
{
	printf("Call GetPricePara\n");

	STU_CMD *		cmd = (STU_CMD *)data;

	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}


}


void set_price_para(u8 * data, int sockdf)
{
	printf("Call SetPricePara\n");

	STU_CMD *		cmd = (STU_CMD *)data;

	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}

}


void reset_measure(u8 * data, int sockdf)
{
	printf("Call ResetMeasure\n");

	STU_CMD *		cmd = (STU_CMD *)data;

	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}

}


void start_record(u8 * data, int sockdf)
{
	printf("Call StartRecord\n");

	STU_CMD *		cmd = (STU_CMD *)data;

	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}


}


void report_data(u8 * data, int sockdf)
{
	printf("Call Reportdata\n");
	pthread_t		id;
	STU_SOCKET_AND_DATA sock_and_data;

	sock_and_data.socket = sockdf;
	char			buf[1024] = "Reportdata,int sockdf";

	sock_and_data.buff	= buf;


}


void stop_record(u8 * data, int sockdf)
{
	printf("Call StopRecord\n");

	STU_CMD *		cmd = (STU_CMD *)data;
	
	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}


}


void get_total_data(u8 * data, int sockdf)
{
	printf("Call GetTotaldata,int sockdf\n");

	STU_CMD *		cmd = (STU_CMD *)data;

	
	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}



}


void one_key_alarm(u8 * data, int sockdf)
{
	printf("Call Alarm\n");
	pthread_t		id;
	STU_SOCKET_AND_DATA sock_and_data;

	sock_and_data.socket = sockdf;
	char			buf[1024] = "Alarm";

	sock_and_data.buff	= buf;



}


void hardware_error(u8 * data, int sockdf)
{
	pthread_t		id;
	STU_SOCKET_AND_DATA sock_and_data;

	sock_and_data.socket = sockdf;
	char			buf[1024] = "HardwareError";

	sock_and_data.buff	= buf;



}


void harw_alarm(u8 * data, int sockdf)
{
	printf("Call HarwAlarm\n");
	pthread_t		id;
	STU_SOCKET_AND_DATA sock_and_data;

	sock_and_data.socket = sockdf;
	char			buf[1024] = "HarwAlarm";

	sock_and_data.buff	= buf;



}


void print_data(u8 * data, int sockdf)
{
	printf("Call Printdata,int sockdf\n");
	pthread_t		id;
	STU_SOCKET_AND_DATA sock_and_data;

	sock_and_data.socket = sockdf;
	char			buf[1024] = "Printdata,int sockdf";

	sock_and_data.buff	= buf;



}


void report_print(u8 * data, int sockdf)
{
	printf("Call ReportPrint\n");
	pthread_t		id;
	STU_SOCKET_AND_DATA sock_and_data;

	sock_and_data.socket = sockdf;
	char			buf[1024] = "ReportPrint";

	sock_and_data.buff	= buf;



}


void get_k(u8 * data, int sockdf)
{
	printf("Call GetK\n");

	STU_CMD *		cmd = (STU_CMD *)data;

	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}


}


void set_k(u8 * data, int sockdf)
{
	printf("Call SetK\n");

	STU_CMD *		cmd = (STU_CMD *)data;


	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}

}


void change_passenger_status(u8 * data, int sockdf)
{
	printf("Call ChangePassengerStatus\n");

	STU_CMD *		cmd = (STU_CMD *)data;
	
	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}

}


void syndata(u8 * data, int sockdf)
{
	printf("Call Syndata,int sockdf\n");

	STU_CMD *		cmd = (STU_CMD *)data;

	if (APP_TO_SERVER == cmd->source_dest)
	{
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_JILIANG,CMD_FLAG);
	}

	if (JILIANG_TO_SERVER == cmd->source_dest)
	{
		printf("len:%d\n", cmd->len);
		int 			i	= 0;

		for (i = 0; i < cmd->len - HEADER_LEN; i++)
		{
			printf("%02x ", (unsigned char) cmd->data[i]);
		}

		printf("\n");
		send_cmd(cmd->cmd_value, cmd->data, cmd->len - 12, SERVER_TO_APP,RESPONE_FLAG);
	}

}


void rename_bluetooth(u8 * data, int sockdf)
{
	printf("Call  RenameBluetooth\n");

	//修改过蓝牙名
	//回复命令
}


