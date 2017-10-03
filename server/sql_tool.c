
#include "sql_tool.h"


int exc_sql(char * db_name, char * sql,int (*callback)(void*,int,char**,char**))
{
	sqlite3 *		db;
	char *			zErrMsg = 0;
	int 			rc,result;
	char *			sql;

	/* Open database */
	rc					= sqlite3_open(db_name, &db);

	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		result=-1;
	}
	else 
	{
		fprintf(stdout, "Opened database successfully\n");
		
		/* Execute SQL statement */
			rc					= sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			
		
			if (rc != SQLITE_OK)
			{
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			}
			else 
			{
				fprintf(stdout, "sql excute successfully\n");
				result =0;
			}
	}
	sqlite3_close(db);
	return result;
}


