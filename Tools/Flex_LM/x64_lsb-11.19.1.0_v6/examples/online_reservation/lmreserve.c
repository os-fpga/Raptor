/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/***
	Description: This is a sample application to demonstrate online reservation feature
*
*		lmreserve is a command line tool that accepts the following parameters
*			reserve or delete operation
*
*			reserve  - To create the online reservation 
*			delete   - To delete the online reservation with reservation ID.
*			duration - duration for how long the reservation should exist
*		
* 	Example:
*			lmreserve.exe -reserve -duration 100 RESERVE 2 f1 PROJECT sampleproj_1, RESERVE 1 f1 USER ABC
*			lmreserve.exe -delete <Reservation-ID>
* 	The tool prints error or success message to the console (STDOUT)
*/

#include "lmclient.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <time.h>
#include "lm_attr.h"
#include "lm_redir_std.h"

/*
*	Application operations
*/
typedef enum
{
	RESERVATION_CREATE,
	RESERVATION_DELETE,
	RESERVATION_UNSUPPORTED /* default */
} OPERATION;

VENDORCODE code;
LM_HANDLE *lm_job;
static void init(struct flexinit_property_handle **);
static void cleanup(struct flexinit_property_handle *);
static char *replaceWord(const char *s, const char *oldW, const char *newW);
static void sUsage();

int main(int argc, char * argv[])
{
	// Parse input to determine which operation is being requested
	OPERATION	operation = RESERVATION_UNSUPPORTED;	/* default */
	int i = 0;
	char feature[MAX_FEATURE_LEN * 2] = { '\0' };
	char rsvLine[MAX_CONFIG_LINE * 2] = { '\0' };
	int duration = 0;

	struct flexinit_property_handle* initHandle = NULL;
	char rsvResult[512];
	char* reservationLine = NULL;
	int ret;

	if(argc < 2)
	{
		sUsage();
		return 0;
	}	
	else if( argc > 3 && !strcmp(argv[1], "-reserve") && !strcmp(argv[2], "-duration") )
	{
		operation = RESERVATION_CREATE;
		duration = atoi(argv[3]);
	}
	else if(argc > 2 && !strcmp(argv[1], "-delete"))
		operation = RESERVATION_DELETE;
	else if( argc >= 2 && (!strcmp(argv[1], "-help") || !strcmp(argv[1], "--help")) )
	{
		sUsage();
		return 0;
	}
	else
		operation = RESERVATION_UNSUPPORTED;

	init(&initHandle);

	if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
	{
		lc_perror(lm_job, "lc_new_job failed");
		cleanup(initHandle);
		exit(lc_get_errno(lm_job));
	}
	
	switch (operation)
	{
		case RESERVATION_CREATE:
			for(i=4; i<argc; i++)
			{
				strcat(rsvLine, argv[i]);
				strcat(rsvLine, " ");
			}
			rsvLine[strlen(rsvLine)] = '\0';
			printf("\n ReservationLine = [%s]\n", rsvLine);
			reservationLine = replaceWord(rsvLine, ",", " \f");

			ret = lc_reservation_create(lm_job, reservationLine, duration, rsvResult);
			if (ret)
			{
				printf("\n\n RESERVATION failed, return code [%d]\n", ret);
				lc_perror(lm_job, "lc_reservation_create() failed");
				cleanup(initHandle);
				exit(lc_get_errno(lm_job));
			}
			else
			{
				printf("\n\n RESERVATION Succeed:\n");
				printf("     ReserveId = %s \n", rsvResult);
			}
			free(reservationLine);
			break;
			
		case RESERVATION_DELETE:
			sprintf(rsvResult, "%s", argv[2]);
			rsvResult[strlen(rsvResult)] = '\0';

			ret = lc_reservation_delete(lm_job, rsvResult);
			if (ret)
			{
				printf("\n\n RESERVATION DELETE failed, return code [%d]\n", ret);
				lc_perror(lm_job, "lc_reservation_delete() failed");
				cleanup(initHandle);
				exit(lc_get_errno(lm_job));
			}
			else 
			{
				printf("\n RESERVATION DELETE Succeed:\n");
				printf("     ReserveId = %s \n", rsvResult);
			}
			break;
			
		case RESERVATION_UNSUPPORTED:
				// Input error
			printf("\n Incorrect input parameters. Entered [%d] parameters:\n ", argc);
			for (i = 0; i < argc; i++) 
				printf("%s ", argv[i]);
			sUsage();
			lc_perror(lm_job, "Incorrect input parameters");
			exit(lc_get_errno(lm_job));
			break;
	}
	lc_free_job(lm_job);
	cleanup(initHandle);
	return 0;
}

static void init(struct flexinit_property_handle **handle)
{
#ifndef NO_ACTIVATION_SUPPORT
	struct flexinit_property_handle *ourHandle = NULL;
	int stat;

	if ((stat = lc_flexinit_property_handle_create(&ourHandle)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_create() failed: %d\n", stat);
		exit(1);
	}
	if ((stat = lc_flexinit_property_handle_set(ourHandle,
			(FLEXINIT_PROPERTY_TYPE)FLEXINIT_PROPERTY_USE_TRUSTED_STORAGE,
			(FLEXINIT_VALUE_TYPE)1)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_set failed: %d\n", stat);
	    exit(1);
	}
	if ((stat = lc_flexinit(ourHandle)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit failed: %d\n", stat);
	    exit(1);
	}
	*handle = ourHandle;
#endif /* NO_ACTIVATION_SUPPORT */
}

static void cleanup(struct flexinit_property_handle *initHandle)
{
#ifndef NO_ACTIVATION_SUPPORT
	int stat;

	if ((stat = lc_flexinit_cleanup(initHandle)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_cleanup failed: %d\n", stat);
	}
	if ((stat = lc_flexinit_property_handle_free(initHandle)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_free failed: %d\n", stat);
	}
#endif /* NO_ACTIVATION_SUPPORT */
}

static char *replaceWord(const char *s, const char *oldW, const char *newW) 
{ 
    char *result; 
	int i, cnt = 0; 
	int newWlen = strlen(newW); 
	int oldWlen = strlen(oldW); 

	// Counting the number of times old word 
	// occur in the string 
	for (i = 0; s[i] != '\0'; i++) 
	{ 
		if (strstr(&s[i], oldW) == &s[i]) 
		{ 
			cnt++; 
	 
			// Jumping to index after the old word. 
			i += oldWlen - 1; 
		} 
	} 

	// Making new string of enough length 
	result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1); 
 
    i = 0; 
	while (*s) 
	{ 
		// compare the substring with the result 
		if (strstr(s, oldW) == s) 
		{ 
			strcpy(&result[i], newW); 
			i += newWlen; 
			s += oldWlen; 
		} 
		else
			result[i++] = *s++; 
	} 
		
	result[i] = '\0'; 
	return result; 
}

void sUsage()
{
	printf("\tUsage:\n");
	printf("\t\tlmreserve -reserve -duration <time in seconds> RESERVE\n");
	printf("\t\t          RESERVE\n");
	printf("\t\t          <feature quantity>\n");
	printf("\t\t          <feature name>\n");
	printf("\t\t          <USER|HOST|DISPLAY|INTERNET|PROJECT>\n");
	printf("\t\t          <name>\n");
	printf("\t\t          \n");
	printf("\t\tlmreserve -delete <Reservation-ID>\n\n");
	printf("\tExample Reservation:\n");
	printf("\t\tlmreserve -reserve -duration 100 RESERVE 2 f1 PROJECT sampleproj_1, RESERVE 1 f1 USER ABC\n");
	printf("\tExample delete reservation:\n");
	printf("\t\tlmreserve -delete ID-OTAy-NDk2-OTMx-NjM0 \n");
}
