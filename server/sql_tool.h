#ifndef SQL_TOOL_H
#define SQL_TOOL_H

#define "sqlite3.h"

#define DB_NAME 	"database.db"			//数据库名
#define T_SYS_PARAM 	  	"t_sys_param"			//系统参数
#define T_VALU_PARAM 	  	"t_valu_parpam"			//计价参数
#define T_OPERATE_DATA	"t_operate_data"		//营业数据
#define T_PRINT_DATA		"t_print_data"			//打印数据
#define T_ADD_UP_DATA		"t_add_up_data"			//累计数据


#define CREATE_T_SYS_PARAM "CREATE TABLE T_SYS_PARAM  ("  \
		 "F_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"	\
         "F_HOST_NO 		CHAR(26)    NOT NULL," \
         "F_JILIANG_NO  	CHAR(26)    NOT NULL," \
         "F_K            	INT     NOT NULL," \
         "F_CAR_NO          TEXT);"

#define CREATE_T_VALU_PARAM "CREATE TABLE T_VALU_PARAM ("  \
		 "F_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"		\
         "F_DAY_START_TIME 		TIME    NOT NULL," \
         "F_NIGHT_START_TIME  	TIME    NOT NULL," \
         "F_START_MILE          INT2    NOT NULL," \
         "F_RISE_PRICE_MILE     INT2	NOT NULL," \
         "F_DAY_START_PRICE     INT2    NOT NULL," \
         "F_NIGHT_START_PRICE   INT2    NOT NULL," \
         "F_DAY_BASE_PRICE	    INT2    NOT NULL," \
         "F_NIGHT_BASE_PRICE    INT2    NOT NULL," \
         "F_WAIT_TIME_PRICE     INT2    NOT NULL," \
         "F_RISE_PRICE_RATE	    INT2    NOT NULL," \
         "F_LOW_SPEED_PRICE	    INT2    NOT NULL," \
         "F_LOW_SPEED_VALUE     INT2    NOT NULL," \
         "F_TWO_DISCOUNT	    INT2    NOT NULL," \
         "F_THREE_DISCOUNT      INT2    NOT NULL," \
         "F_FOUR_DISCOUNT       INT2    NOT NULL," \
         "F_WAIT_TIME_UNIT      INT2    NOT NULL," \         
         "F_LOW_SPEED_UNIT      INT2	NOT NULL);"

#define CREATE_T_OPERATE_DATA "CREATE TABLE T_OPERATE_DATA ("  \
		 "F_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"		\
         "F_RECORD_NO   		INT4    NOT NULL," \
         "F_CARPOOL_STATUS  	INT1    NOT NULL," \
         "F_PASSENGER_ID        INT1    NOT NULL," \
         "F_MILE    		    INT2	NOT NULL," \
         "F_AMOUNT_PRICE        INT4    NOT NULL," \
         "F_PRICE			    INT2    NOT NULL," \
         "F_TRAVEL_TIME  	    INT4    NOT NULL," \
         "F_LOW_SPEED_TIME      INT4    NOT NULL," \
         "F_ONE_WAY_STATUS      INT1    NOT NULL," \
         "F_SERVE_STATUS	    INT1    NOT NULL," \
         "F_OPERATE_DAY  	    DATE    NOT NULL," \
         "F_OPERATE_START_TIME  TIME    NOT NULL," \
         "F_OPERATE_END_TIME	TIME    NOT NULL);" 

#define CREATE_T_PRINT_DATA "CREATE TABLE T_PRINT_DATA   ("  \
		 "F_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"	\
         "F_COMPANY 		ITN4    NOT NULL," \
         "F_TEL  			INT4    NOT NULL," \
         "F_CAR_LICENSE     CHAR(7)     NOT NULL," \
         "F_LICENSE		    CHAR(6)     NOT NULL," \
         "F_TIME            DATE     NOT NULL," \
         "F_CAR_LICENSE     CHAR(7)     NOT NULL," \
         "F_CAR_LICENSE     CHAR(7)     NOT NULL," \
         "F_CAR_LICENSE     CHAR(7)     NOT NULL," \
         "F_CAR_LICENSE     CHAR(7)     NOT NULL," \
         "F_CAR_NO          TEXT);"

#define CREATE_T_ADD_UP_DATA "CREATE TABLE T_ADD_UP_DATA   ("  \
		 "F_ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"	\
         "F_SUM_MILE 	INT4    NOT NULL," \
         "F_SUM_PRICE  	INT4    NOT NULL," \
         "F_SUM_TRAVEL_TIME  INT4     NOT NULL," \
         "F_SUM_LOW_SPEED_TIME INT4   NOT NULL);"

int save_data(char* db_name);
int exc_sql(char* db_name,char*sql,int (*callback)(void*,int,char**,char**));
#endif

