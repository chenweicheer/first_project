/*
 * uart_test.c
 *
 *  Created on: 2017-9-11
 *      Author: Administrator
 */
#include "uart_tool.h"

int main()
{
	int fd = open_uart("/dev/ttyS0");
	if(fd<0)
	{
		return -1;
	}
	set_opt(fd,9600,8,'N',1);
	write(fd,"1213hello",strlen("1213hello"));
	char buf[1024];
	int len =read(fd,buf,10);
	if(len >0)
	{
			printf("%s\n",buf);
	}
	printf("%d",len);
	close(fd);
	return 0;
}
