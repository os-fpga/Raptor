/**************************************************************************************************
* Copyright (c) 1997-2018, 2020-2022 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description:	This is a sample application program, to illustrate
 *			the use of the Flexible License Manager.
 *
 */

/* Uncomment the following line to run the original batch prototype */
/* #define BATCH_PROTO */

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef PC
#include <time.h>
#else
#include <sys/time.h>
#endif
#include "lm_attr.h"
#include "lm_redir_std.h"
#ifdef PC
#define LICPATH "@localhost"
#else
#define LICPATH "@localhost:license.dat:."
#include <sys/time.h>
#endif /* PC */

#define FEATURE "f1"
#define FEATURE_DELIM	","

VENDORCODE code;
LM_HANDLE *lm_job;
static int nlic = 1;
static int secure_comms = 0;
static struct flexinit_property_handle *initHandle = NULL;
static void init(struct flexinit_property_handle **);
static void cleanup(struct flexinit_property_handle *);

static const char* s_errs[] = {
#include "lmerrors.h"
};

void exit_err(const char* msg, int err)
{
	if( err )
	{
		fprintf(stderr,"%s failed, status: %d\n",msg,err);
		exit(1);
	}
}

void doBatch(char** f_list, int f_count)
{
	int i;
	LM_TXN txn = LM_TXN_NULL;

#ifndef PC
	struct timeval tStart,tEnd;
	float latency;
#endif

	/* 
	 * Create the batch checkout
 	*/
	exit_err("lc_txn_create", lc_txn_create(lm_job, &txn));

	for(i=0; i<f_count; i++)
	{
		LM_TXN_ENTRY tx_entry = LM_TXN_ENTRY_NULL;
	    exit_err("lc_txn_create_checkout", lc_txn_create_checkout(lm_job, &txn, &tx_entry, f_list[i], "1.0", nlic, LM_CO_NOWAIT, LM_DUP_NONE));
	}

	/* 
	 * Send the batch checkout
	*/
#ifndef PC
	gettimeofday(&tStart, NULL);
#endif
	if( lc_txn_send(lm_job,&txn) != LM_NOERROR )
	{
		lc_perror(lm_job, "batch checkout failed lc_txn_send");
           cleanup(initHandle);
           exit (lc_get_errno(lm_job));
    }
#ifndef PC
	gettimeofday(&tEnd, NULL);
	latency = (tEnd.tv_sec - tStart.tv_sec)*1000.0 + (tEnd.tv_usec - tStart.tv_usec)/1000.0;
	printf("Batch checkout complete (latency=%.1fms)...\n", latency);
#else
	printf("Batch checkout complete...\n");
#endif

	/*
	 * Display the results
	*/
	for(i=0; i<f_count; i++)
	{
		int entryRes;
		LM_TXN_ENTRY tx_entry = LM_TXN_ENTRY_NULL;
		
		(void)lc_txn_entry(lm_job, &txn, &tx_entry, i);
		lc_txn_entry_status(lm_job,&tx_entry,&entryRes);

		if( entryRes == LM_NOERROR )
		{
			printf("%s batch checkout ok...\n", f_list[i]);
		}
		else
		{
			printf("%s batch checkout failed (%d,%s)...\n", f_list[i], entryRes, s_errs[-1*entryRes]);
		}
	}

	printf("press return to exit...");

	/*
	*	Wait till user hits return
	*/
	getchar();

	/*
	 * Checkin the features
	*/
	for(i=0; i<f_count; i++)
	{
		int entryRes;
		LM_TXN_ENTRY tx_entry;
		
		(void)lc_txn_entry(lm_job, &txn, &tx_entry, i);
		lc_txn_entry_status(lm_job, &tx_entry, &entryRes);

		if( entryRes == LM_NOERROR )
		{
			lc_checkin(lm_job, f_list[i], 0);
		}
	}

	lc_txn_destroy(lm_job, &txn);
}

int
main(int argc, char * argv[])
{
	int i;
	char feature[(MAX_FEATURE_LEN+1) * 48 + 2] = {'\0'};
#ifndef PC
	struct timeval tStart,tEnd;
	float latency;
#endif

	init(&initHandle);

	/* Process command line */
	for( i=1; i<argc; i++ )
	{
		if( isdigit(*argv[i]) )
		{
			nlic = atoi(argv[i]);
		}
		else if( !strcmp(argv[i],"--secure_comms") )
		{
			secure_comms = 1;
		}
	}

	if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
	{
		lc_perror(lm_job, "lc_new_job failed");
		cleanup(initHandle);
		exit(lc_get_errno(lm_job));
	}

	printf("Enter \"f1\" to demo floating functionality\n");
	printf("Enter \"f2\" to demo node-locked functionality\n");
	printf("Enter feature to checkout [default: \"%s\"]: ", FEATURE);


	(void)fgets(feature, sizeof(feature), lm_flex_stdin());	/*	add 2 for \n and \0	*/
	feature[strlen(feature) - 1] = '\0';
	if(!*feature)
		strcpy(feature, FEATURE);

	(void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
	if( secure_comms )
	{
		if ( lc_set_attr(lm_job, LM_A_SECURE_COMMS, (LM_A_VAL_TYPE)1) != LM_NOERROR )
		{
			lc_perror(lm_job, "Failed to enable secure comms");
			cleanup(initHandle);
			exit (lc_get_errno(lm_job));
		}
	}

	if( !strstr(feature, FEATURE_DELIM) )
	{
		/* 
		 * Single feature checkout
		*/
#ifndef PC
		gettimeofday(&tStart, NULL);
#endif
		if(lc_checkout(lm_job, feature, "1.0", nlic, LM_CO_NOWAIT, &code, LM_DUP_NONE))
		{
			lc_perror(lm_job, "checkout failed");
			cleanup(initHandle);
			exit (lc_get_errno(lm_job));
		}
#ifndef PC
		gettimeofday(&tEnd, NULL);
		latency = (tEnd.tv_sec - tStart.tv_sec)*1000.0 + (tEnd.tv_usec - tStart.tv_usec)/1000.0;
#endif
		printf("%s checked out...", feature);
		printf("\n");
#ifndef PC
		printf("latency=%.1fms\n",latency);
#endif
		printf("press return to exit...");

		/*
		*	Wait till user hits return
		*/
		getchar();
		lc_checkin(lm_job, feature, 0);
	}
	else
	{
		/* 
		 * Batch feature checkout
		*/
		int f_count=1;		/* We know there's at least one feature, plus 1 for each ',' */
		int i;

		char** f_list;
		char* f;

		/* 
		 * Setup the list iof features 
		*/
		for( f=feature; *f; f++ )
		{
			if( *f == *FEATURE_DELIM )
				++f_count;
		}
		f_list = calloc(f_count, sizeof(*f_list));
		f=strtok(feature, FEATURE_DELIM);

		for(i=0; i<f_count; i++)
		{
			f_list[i] = f;
			f=strtok(NULL, FEATURE_DELIM);
		}

		doBatch(f_list, f_count);

		free(f_list);
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
