/*
 * printer.h
 *
 *  Created on: 2017-9-11
 *      Author: Administrator
 */

#ifndef PRINTER_H_
#define PRINTER_H_

#define PRINTER_NORMAL			0
#define PRINTER_NO_CONN			1
#define PRINTER_FAULT			2

#define PRINTER_SERIAL 		"/dev/ttyS0"
#define PRINTER_BAUDRATE 		15200
#define PRINTER_DATA_BITS 		8
#define PRINTER_CHECK 			'N'
#define PRINTER_STOP_BITS 		1


extern int printer_fd;


unsigned  char printer_get_status();

void init_printer();

#endif /* PRINTER_H_ */
