/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
 * Client Application to exercise the lc_vsend interface
 * =====================================================
 *
 * The purpose of this application is to demonstrate the operation of the lc_vsend() function
 *
 * Specifically the program allows a string to be passed to the Vendor Function running a 
 * Vendor Daemon.  The result from that call can be collected either synchronously (the default)
 * or asynchronously.
 *
 * The asynchronous mode fits use-cases where the Vendor Function is relatively slow to complete 
 * and the client application can usefully be performing other activities instead of just waiting.
 * It is analogous to using the LM_CO_QUEUE flag on lc_checkout() calls.
 *
 * The simplest way to compile this program is simply to overwrite the lmflex.c file in the
 * machind kit directory and rebuild the lmflex target or alternatively use the lmflex
 * target and action lines from the kit makefile as a guide for creating a new lmvsend target.
 *
 * Usage:
 * lmvsend [-A] <string>
 *
 * By passing the -A option, the lc_vsend call is made asynchronously.
 * NB: The maximum length of <string> is LM_VSEND_MAX_LEN characters. 
 */

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lm_attr.h"
#include "lm_redir_std.h"

#ifdef PC
#define LICPATH "@localhost"
#else
#define LICPATH "@localhost:license.dat:."
#endif /* PC */

VENDORCODE code;
LM_HANDLE *lm_job;
static void init(struct flexinit_property_handle **);
static void cleanup(struct flexinit_property_handle *);
static int s_analyseError(const char* activity, int status);

int main(int argc, char * argv[])
{
	struct flexinit_property_handle *initHandle = NULL;
	int retVal = EXIT_FAILURE;								/* Pessimistic */

	char* message;
	char* res;
	int status;
	int async = 0;
	int exitEarly = 0;

	/* Parse the command-line */
	if (argc == 3 && strcmp(argv[1],"-A") == 0 )
	{
		async = 1;
		message = argv[2];
	}
	else if (argc == 3 && strcmp(argv[1],"-a") == 0 )
	{
		async = 1;
		exitEarly = 1;
		message = argv[2];
	}
	else if(argc==2)
	{
		message = argv[1];
	}
	else
	{
		printf("Usage: %s [-A | -a] <string>\n", argv[0]);
		return retVal;
	}

	/* Setup the job handle */	
	init(&initHandle);
	if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
	{
		lc_perror(lm_job, "lc_new_job failed");
		cleanup(initHandle);
		retVal = lc_get_errno(lm_job);
		return retVal;
	}

	(void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);

	/* lc_vsend related code */
	do
	{
		if( async  )
		{
			(void)lc_set_attr(lm_job, LM_A_VSEND_NOWAIT, (LM_A_VAL_TYPE)1);
		}

		/* Send the message to the Vendor Daemon. */
		res = lc_vsend(lm_job, message);
		status = lc_get_errno(lm_job);

		if( !async )
		{
			/* We're in synchronous mode, so we're expecting the response to have been delivered */
			if( status == LM_NOERROR )
			{
				printf("Response: %s\n", (res ? res : "<NULL>") );
			}
			else
			{
				retVal = s_analyseError("Synchronous call",status);
				break;
			}
		}
		else
		{
			/* We're in asynchronous mode, so first we check our message was sent OK */
			if( status != LM_NOERROR )
			{
				/* Something went wrong sending the message */
				retVal = s_analyseError("Sending Asynchronous vsend message", status);
				break;
			}
			else if( !exitEarly )
			{
				/* Now we poll for the response */
				do
				{
					fprintf(stderr,".");					/* Output a dot to show each loop iteration */

#ifdef PC
					Sleep(1000);							/* 1000 milliseconds */
#else
					sleep(1);								/* 1 second */
#endif
					/* In real use-cases, do something more useful than sleeping! */

					/* Poll to see if there is a response from the Vendor Daemon */
					res = lc_vsend(lm_job, LM_VSEND_STATUS);
					status = lc_get_errno(lm_job);

				} while( status == LM_VSEND_ASYNC_BUSY );

				fprintf(stderr,"\n");

				/* We got something other than a BUSY reply */
				if( status == LM_NOERROR )
				{
					printf("Response: %s\n", (res ? res : "<NULL>") );
					retVal = EXIT_SUCCESS;
					break;
				}
				else
				{
					retVal = s_analyseError("Receiving Asynchronous vsend reply",status);
					break;
				}
			}
		}
	} while(0);
			
	lc_free_job(lm_job);
	cleanup(initHandle);
	return retVal;
}

static int s_analyseError(const char* activity, int status)
{
	lc_perror(lm_job,activity);
	return status;
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
