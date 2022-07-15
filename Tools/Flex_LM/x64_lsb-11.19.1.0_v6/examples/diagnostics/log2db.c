/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *  Description:    This is an example program, to demonstrate the capture of
 *                  diagnostic logging information from the Vendor Daemon diagnostic port.
 *                  It is not intended to be a complete production quality tool, it exists
 *                  purely to illustrate the basic interaction with the diagnostics port.
 */
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/* Definition of the diagnostics port address */
#define DIAGNOSTICS_PORT_ADDRESS "./.logger"

/* Macro for socket family */
#ifdef AF_LOCAL
#define DIAG_SOCKET_DOMAIN  AF_LOCAL
#else
#define DIAG_SOCKET_DOMAIN  AF_UNIX
#endif


/* Filters */
typedef struct
{
	const char* column;
	const char* regex;
} Filter;

/* Regex filters for the diagnostic record columns */
static Filter filters[] = 
{
	{ "ARRIVAL", NULL },
	{ "MESSAGE", NULL },
	{ "LATENCY", NULL },
	{ "USER", NULL },
	{ "SOURCE", NULL },
	{ "PROCESS", NULL },
	{ "SESSION", NULL },
	{ "COMPONENT", NULL },
	{ "INFO", NULL },
	{ NULL, NULL }
};

const char* s_dbName = "diag.db";
int s_dbInit = 0;									/* Flag to control DB initialization */

/* Update a column filter */
int s_updateFilter(const char* colName, const char* val)
{
	int res = 0;									/* Optimist */
	Filter* f;

	for( f = filters; f->column != NULL && strcmp(f->column, colName); f++ )
		;

	if( f->column )
		f->regex = val;
	else
		res = -1;

	return res;
}

/* Check if the filter associated with the matches the pattern */
static int s_isMatch(const char *colName, const char *pattern)
{
	int res = 0;

	do
	{
		regex_t re;

		if( regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0)
			break;

		res = (regexec(&re, colName, (size_t) 0, NULL, 0) != 0) ? 0 : 1;
		
		regfree(&re);

	} while(0);

	return res;
}

/* Return: 0 if filter not matched, else filter matched */
int s_filterOk(const char* name, const char* val)
{
	int res = 0;									/* Pessimist */
	Filter* f;

	for( f = filters; f->column != NULL && strcmp(f->column, name); f++ )
		;

	if( f->column && ( f->regex == NULL || s_isMatch(val, f->regex) ) )
		res = 1;

	return res;
}

/* parse the command-line */
static int s_parse_commandline(int argc, char** argv)
{
	int res = 0;		/* optimist */
	int i;

	for(i=1; i<argc; i++)
	{
		char* ptr;

		if( !strcmp(argv[i], "-d") )
		{
			i++;
			if( i<argc )
				s_dbName = argv[i];
			else
			{
				res = -1;
				fprintf(stderr,"Missing db name\n");
				break;
			}

			fprintf(stderr,"Set dbname to %s\n",s_dbName);
		}
		else if( !strcmp(argv[i], "-i") )
		{
			s_dbInit = 1;
			fprintf(stderr,"Set init to %d\n",s_dbInit);
		}
		else if( ptr = strchr(argv[i],'=') )
		{
			char* filterName = argv[i];
			char* filterVal = ptr+1;
			*ptr = '\0';
			if( s_updateFilter(filterName, filterVal) < 0 )
			{
				res = -1;
				fprintf(stderr,"Bad filter: %s\n",filterName);
				break;
			}
			fprintf(stderr,"Set filer %s to %s\n",filterName,filterVal);

		}
		else
		{
			res = -1;
			fprintf(stderr,"Bad parameter: %s\n",argv[i]);
			break;
		}
	}

	if( res != 0 )
	{
		fprintf(stderr, "Syntax is\nlog2db [-d <dbfile>] [-i] [<column_name>=<regex>]...\n");
	}

	return res;
}

static int s_sqlite_callback(void* x, int colCount, char **colVal, char **colName)
{
	int i;
	for(i = 0; i<colCount; i++)
	{
		printf("%s = %s\n", colName[i], colVal[i] ? colVal[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/* Open the specified database and initialize if requested */
static int s_sqlite_open(const char* dbName, int doInit, sqlite3** dbHandle)
{
	int res;

	do
	{
		/* Make the DB connection */
		if( (res = sqlite3_open(dbName, dbHandle)) != SQLITE_OK )
		{
			fprintf(stderr, "Can't open database %d: %s\n", dbName, sqlite3_errmsg(*dbHandle));
			break;
		}
		else
		{
			fprintf(stderr, "Opened database %s successfully\n", dbName);
		}

		/* Initialize the DB if requested */
		if( doInit )
		{	
			char* errMsg;

			/* DB Intialize SQL statement */
			static const char* sqlInit =	"CREATE TABLE RECORD("  \
												"ARRIVAL	CHAR(28)	PRIMARY KEY	NOT NULL," \
												"MESSAGE	CHAR(32)				NOT NULL," \
												"LATENCY	REAL					NOT NULL," \
												"USER		CHAR(16)						," \
												"SOURCE		CHAR(24)						," \
												"PROCESS	INT								," \
												"SESSION	CHAR(8)							,"\
												"COMPONENT	TEXT							," \
												"INFO		TEXT							);";


			fprintf(stderr,"Initializing database %s\n", dbName);

			/* Execute SQL statement */
			res = sqlite3_exec(*dbHandle, sqlInit, s_sqlite_callback, 0, &errMsg);
			if( res != SQLITE_OK )
			{
				fprintf(stderr, "SQL error: %s\n", errMsg);
				sqlite3_free(errMsg);
				break;
			}
			else
			{
				fprintf(stderr, "Initialization complete\n");
				break;
			}
		}
	} while(0);

	return res;
}

int main(int argc, char** argv)
{
	/* Diagnostics port */
	struct sockaddr_un dp_sa;
	int dp_sock = -1;								/* Diagnostics port socket */
	FILE* dp_stream = NULL;

	/* Database settings */
	sqlite3* db = NULL;

	do
	{
		if( s_parse_commandline(argc, argv) , 0 )
			break;

		if( s_sqlite_open(s_dbName, s_dbInit, &db) != SQLITE_OK )
			break;

		/* The database is ready, so now we can connect to the VD to collect diagnostics */

		/* Create a local socket */
		dp_sock = socket(DIAG_SOCKET_DOMAIN, SOCK_STREAM, 0);
		if( dp_sock == -1 )
		{
			perror("diagnostics socket create");
			break;
		}

		/* Setup the diagnostics port address */
		memset(&dp_sa, 0, sizeof(dp_sa));
		dp_sa.sun_family = DIAG_SOCKET_DOMAIN;
		strncpy(dp_sa.sun_path, DIAGNOSTICS_PORT_ADDRESS, sizeof(dp_sa.sun_path)-1);

		/* Connect to the diagnostics port */
		if( connect(dp_sock, (struct sockaddr*)&dp_sa, sizeof(dp_sa) ) == -1 )
		{
			perror("diagnostics socket connect");
			break;
		}

		if( !(dp_stream = fdopen(dp_sock,"r")) )
		{
			perror("diagnostics socket stream");
			break;
		}

		/* Loop reading text from the port and writing to the databaset */
		while(1)
		{
			int res;
			char* errMsg;
			char text[1024];
            memset(text, 0, sizeof(text));

			if( fgets(text,sizeof(text),dp_stream) == 0 )	
            {
				if( !feof(dp_stream) )
                	perror("diagnostics socket read");
                break;
            }
            else if( strlen(text) == 0 )
            {
                /* The diagnostics port was closed by the Vendor Daemon */
                break;
            }
			else
			{

				if( strlen(text) > 1 )
				{
					static const char* sqlInsertFormat =
   						"INSERT INTO RECORD (ARRIVAL,MESSAGE,LATENCY,USER,SOURCE,PROCESS,SESSION,COMPONENT,INFO) "
         				"VALUES ('%s', '%s', %.3f, '%s', '%s', %d, '%s', '%s', '%s' ); ";

					char sqlStatement[2048];
					char *arrival,*message,*latency,*user,*source,*process,*session,*component,*info;

					/* Tokenize the CSV record we got from the diagnostics port */
					arrival = strtok(text,",");
					if( !s_filterOk("ARRIVAL", arrival) )
						continue;

					/* Discard the CSV header line */
					if( strcmp(arrival,"Arrival Time")== 0 )
						continue;

					message = strtok(NULL,",");
					if( !s_filterOk("MESSAGE", message) )
						continue;

					latency = strtok(NULL,",");
					if( !s_filterOk("LATENCY", latency) )
						continue;

					user = strtok(NULL,",");
					if( !s_filterOk("USER", user) )
						continue;

					source = strtok(NULL,",");
					if( !s_filterOk("SOURCE", source) )
						continue;

					process = strtok(NULL,",");
					if( !s_filterOk("PROCESS", process) )
						continue;

					session = strtok(NULL,",");
					if( !s_filterOk("SESSION", session) )
						continue;

					component = strtok(NULL,",");
					if( !s_filterOk("COMPONENT", component) )
						continue;

					info = strtok(NULL,",");
					if( info[strlen(info)-1] == '\n' )		/* Lose the CSV terminating line-feed character */
						info[strlen(info)-1] = '\0';
					if( !s_filterOk("INFO", info) )
						continue;

					/* Create SQL statement */
					sprintf(sqlStatement,sqlInsertFormat,arrival,message,atof(latency),user,source,atoi(process),session,component,info);

					/* Execute SQL statement */
					if( sqlite3_exec(db, sqlStatement, s_sqlite_callback, 0, &errMsg) != SQLITE_OK )
					{
						fprintf(stderr, "SQL error: %s\n", errMsg);
						sqlite3_free(errMsg);
						break;
					}
				}
			}
		}

		if( dp_stream )
			(void)fclose(dp_stream);
		else if( dp_sock != -1 )
			(void)close(dp_sock);

	} while(0);

	sqlite3_close(db);
	return 0;
}
