/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
 * Vendor Function example code-snippet: Use of lc_ api: lc_next_conf 
 * ===================================================================
 *
 * The purpose of this function is to demonstrate the operation of a Vendor Function in calling the
 * lc_ * api.
 *
 * Specifically the function below iterates through a license file using the lc_next_conf() command.
 * The client specifies a particular feature in the message string passed to the Vendor Function
 * The resultant count of licenses is returned as a string.
 *
 * To use this function it must be inserted into ls_vendor.c immediately after the "* vendor message *"
 * comment. The variable 'ls_vendor_msg' should be assigned the value s_vf_func instead of 0:-
 *
 * char *(*ls_vendor_msg)() = s_vf_func;
 *
 * On platforms that support it, a worker thread can be enabled to process Vendor Functions asynchronously
 * and hence not disrupt normal licensing-related operations whilst they are running.  To enable the worker
 * thread, change the initialization of ls_vendor_msg_async from 0 to 1.
 *
 * int ls_vendor_msg_async = 1;
 *
 * There is one final variable associated with the asynchronous processing of Vendor Functions: ls_vendor_msg_list_size
 * This variable limits the number of queued asynchronous Vendor Function request to the chosen value.
 * In most use-cases it is likely that the default value in the lsvendor.c should not need changing.
 *
 */

#include <stdio.h>
#ifdef PC
#include <process.h>
#else
#include <unistd.h>
#endif

static LM_HANDLE* s_create_job();

static FILE* dFile;

static char* s_vf_func(char* msg)
{
	LM_HANDLE *lm_my_job;				/* Job handle for lc_ api calls */
	char featName[MAX_FEATURE_LEN+1];	/* Holds a copy of the supplied feature name */

	/* Hold resultant user count */
	int totalCount = 0;					/* Hold resultant user count */
	char* res = NULL;					/* Pointer to reply string */

	/* Parameters to lc_next_conf() */
	CONFIG *conf = NULL;
	CONFIG *pos = 0;

	dFile = fopen("/tmp/vf_nextconf.log", "a");

	fprintf(dFile, "%d: s_vf_func(%s) called\n", getpid(), (msg ? msg : "<NULL>"));
	fflush(dFile);

	/* Take a copy of the specified feature name */
	strncpy(featName,msg,MAX_FEATURE_LEN);
	featName[MAX_FEATURE_LEN]='\0';

    /* A useful tip to avoid the need for heap management is to use the input buffer
     * to store the reply.
     * Note: The buffer size for message (incoming and outgoing) is limited to LM_VSEND_MAX_LEN bytes (defined in lmclient.h)
     */
    res = msg;
	memset(res, '\0', LM_VSEND_MAX_LEN);

	lm_my_job = s_create_job();

	/* Iterate over the lc_next_conf() calls looking for the specified feature and counting users, as per the manual */
	while(conf = lc_next_conf(lm_my_job, featName, &pos))
	{
		if (!strcmp(featName, conf->feature))
		{
			totalCount += conf->users;
		}
	}

	/* Return the result in the form of the specified feature and user count, separated by a ':' character */
#ifndef PC
	snprintf(res, LM_VSEND_MAX_LEN, "%s:%d", featName, totalCount);
#else
	sprintf(res, "%s:%d", featName, totalCount);
#endif

	/* Delete the job */
	lc_free_job(lm_my_job);

	fprintf(dFile, "%d: s_vf_func(%s) returning %s\n", getpid(), (msg ? msg : "<NULL>"), (res ? res : "<NULL>") );

	return res;
}

/* 
 * Because we're in the Vendor Daemon we must create our job handle directly from the keys in lm_code.h
 * Note: Whilst lc_init is safe to use in a Vendor Function, as per guidance in the FNP C/C++ Function Reference document,
 * lc_init should not be used in FlexEnabled applications shipped to end users.  
 */

#include "lm_code.h"

static LM_HANDLE* s_create_job()
{
	int i;

	/* From lm_code.h */
	LM_CODE(code, ENCRYPTION_SEED1, ENCRYPTION_SEED2, VENDOR_KEY1, VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4, VENDOR_KEY5);

	LM_HANDLE* job = NULL;

	fprintf(dFile, "%d: s_create_job() VENDOR_NAME=%s\n", getpid(), ((VENDOR_NAME) ? (VENDOR_NAME) : "<NULL>") );

	fprintf(dFile, "%d: s_create_job() VENDOR_KEY1=%x\n", getpid(), VENDOR_KEY1 );
	fprintf(dFile, "%d: s_create_job() VENDOR_KEY2=%x\n", getpid(), VENDOR_KEY2 );
	fprintf(dFile, "%d: s_create_job() VENDOR_KEY3=%x\n", getpid(), VENDOR_KEY3 );
	fprintf(dFile, "%d: s_create_job() VENDOR_KEY4=%x\n", getpid(), VENDOR_KEY4 );


	
	if ( lc_init((LM_HANDLE *)0, VENDOR_NAME, &code, &job) != LM_NOERROR )
	{
		job = NULL;
	}

	if( !job )
		fprintf(dFile, "%d: s_create_job() failed to create job\n", getpid() );

	return job;
}

