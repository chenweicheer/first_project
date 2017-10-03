/*
 * func.h
 *
 *  Created on: 2017-8-30
 *      Author: Administrator
 */

#ifndef FUNC_H_
#define FUNC_H_
#include "public.h"
void start_check(u8 *data,int sockdf); 
void send2_toplight(u8 *data, int sockdf);
void toplight_return(u8 *data, int sockdf); 
void syn_time(u8 *data, int sockdf); 
void query_printer(u8 *data, int sockdf); 
void query_measure(u8 *data, int sockdf); 
void query_toplight(u8 *data, int sockdf); 
void check_num(u8 *data, int sockdf); 
void get_price_para(u8 *data, int sockdf); 
void set_price_para(u8 *data, int sockdf); 
void reset_measure(u8 *data, int sockdf); 
void start_record(u8 *data, int sockdf); 
void report_data(u8 *data, int sockdf); 
void stop_record(u8 *data, int sockdf); 
void get_total_data(u8 *data, int sockdf); 
void one_key_alarm(u8 *data, int sockdf); 
void hardware_error(u8 *data, int sockdf); 
void harw_alarm(u8 *data, int sockdf); 
void print_data(u8 *data, int sockdf); 
void report_print(u8 *data, int sockdf); 
void get_k(u8 *data, int sockdf); 
void set_k(u8 *data, int sockdf); 
void change_passenger_status(u8 *data, int sockdf); 
void syndata(u8 *data, int sockdf); 
void rename_bluetooth(u8*data, int sockdf); 







#endif /* FUNC_H_ */
