/*
 * printer.c
 *
 *  Created on: 2017-9-11
 *      Author: Administrator
 */

#include "printer.h"

int printer_fd=-1;

unsigned  char printer_get_status()
{
	return PRINTER_NORMAL;
}



void init_PRINTER()
{
	printer_fd = open_uart(PRINTER_SERIAL);
	if(fd<0)
	{
		return ;
	}
	set_opt(printer_fd,PRINTER_BAUDRATE,PRINTER_DATA_BITS,PRINTER_CHECK,PRINTER_STOP_BITS);
	return;
	
}

