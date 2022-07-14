/**************************************************************************************************
* Copyright (c) 2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
	This sample licensing client (lmflex_coavail) demonstrates license checkout with 
	checkout-avail (LM_CO_AVAIL_NOWAIT) option flag.

	Source file(s)
	==============
		lmflex_coavail.c

	API Update
	===========
		Use, 
			- new checkout option flag LM_CO_AVAIL_NOWAIT 
			- new config member nCOAvailLastLicCount 
		
		Refer header "lmclient.h" in toolkit folder machind.

	Sample license file
	===================
        SERVER this_host ANY
        VENDOR demo
        USE_SERVER
        #a counted license
        INCREMENT f1 demo 1.0 permanent 4 SIGN=0
        INCREMENT f3 demo 2.0 permanent 6 SIGN=0

	Build and run
	=============
		Building lmflex_coavail application is similar to out-of-box licensing client lmflex. 
		Include lmflex_coavail target in toolkit makefile for certificate licensing or 
		makefile.act for trusted storage licensing and trigger the build.

	Usage
	=====
		Refer sPrintUsage()

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
#endif 

#if OS_WINDOWS
/* Disable deprecated function warnings */
#pragma warning( disable:4996 )
#endif

/*-----------------------------------------------------------------------------------------*/
#define FNP_DEFAULT_FEATURE "f1"

static void sPrintUsage();
static void sPrintConfigInfo(const CONFIG *pConf);
static void sPrintErrorInfo(const LM_HANDLE_PTR job);
static const char* sGetConfigType(int nConfigType);
static void sInit(struct flexinit_property_handle **handle);
static void sCleanup(struct flexinit_property_handle *initHandle);

/*-----------------------------------------------------------------------------------------
 * Print the usage 
 *-----------------------------------------------------------------------------------------*/
static void sPrintUsage()
{
	printf(	"\n"
			"-------------------------------------------------------------------------------"
            "\nThis sample shows how to use checkout option flag (LM_CO_AVAIL_NOWAIT) in checkout API.\n"
            "\n"
            "  Usage: lmflex_coavail <license_count> \n"
            "\n"
            "  If <license_count> is specified, the requested license count \n"
            "  in checkout API is updated with this count otherwise \n"
            "  license count 1 is used as a default."
			"\n-------------------------------------------------------------------------------\n\n");
	return;
}

/*-----------------------------------------------------------------------------------------
 * Print CONFIG info (feature, version, code and nCOAvailLastLicCount)
 *-----------------------------------------------------------------------------------------*/
static void sPrintConfigInfo(const CONFIG *pConf)
{
	if (!pConf)
	{
		printf("\nERROR: Failed to get CONFIG details - CONFIG is NULL!");
		return;
	}

	printf("\n\n\t\t====== CONFIG Info ============");
	printf("\n\t\tFeature = %s", pConf->feature);
	printf("\n\t\tVersion = %s", pConf->version);
	printf("\n\t\tCode = %s", pConf->code);
	printf("\n\t\tCOAVAIL license consumption count = %d", pConf->nCOAvailLastLicCount);
	printf("\n\t\t====== END CONFIG Info ============\n");
	return;
}

/*-----------------------------------------------------------------------------------------
 * Print error info associated with the job handle
 *-----------------------------------------------------------------------------------------*/
static void sPrintErrorInfo(const LM_HANDLE_PTR job)
{
	if (!job)
		return;
	
	printf("\n\n== ERROR Info ========================================\n");
	printf("\nError code = %d", lc_get_errno(job));
	printf("\n\n%s", lc_errstring(job) ? lc_errstring(job) : "NULL");
	printf("\n=============================================================\n");
	return;
}

/*-----------------------------------------------------------------------------------------*/
static const char* sGetConfigType(int nConfigType)
{
	switch(nConfigType)
	{	
		case FNP_CONFIG_TYPE_UNKNOWN:		return "FNP_CONFIG_TYPE_UNKNOWN";
		case FNP_CONFIG_TYPE_CLIENT_TS:		return "FNP_CONFIG_TYPE_CLIENT_TS";
		case FNP_CONFIG_TYPE_UNSERVED_CERT: return "FNP_CONFIG_TYPE_UNSERVED_CERT";
		case FNP_CONFIG_TYPE_SERVED_TS:     return "FNP_CONFIG_TYPE_SERVED_TS";
		case FNP_CONFIG_TYPE_SERVED_CERT:   return "FNP_CONFIG_TYPE_SERVED_CERT";
		default: 							return "Unknown Config Type";
	}
}

/*-----------------------------------------------------------------------------------------
 * Reconnection callback for LM_A_USER_RECONNECT_V1 attribute
 *-----------------------------------------------------------------------------------------*/
int sUserReconnectV1(FNP_RECONNECT_DATA *rcData)
{
	if (!rcData)
		return LM_NULLPOINTER;
	
	printf("\n== Reconnection: feature=%s, current_retry=%d, retry_count=%d, retry_interval=%d, lic=%d, (COAVAIL checkouts only) licConsumedsoFar=%d\n",
			rcData->szFeature, rcData->nCurrentRetry, rcData->nRetryCount, rcData->nRetryInterval, rcData->nLicTotal, rcData->nCOAvailLicCurrent);
	
	return LM_NOERROR;
}

/*-----------------------------------------------------------------------------------------
 * Reconnection-done callback for LM_A_USER_RECONNECT_V1_DONE attribute
 *-----------------------------------------------------------------------------------------*/
int sUserReconnectV1Done(FNP_RECONNECT_DATA *rcData)
{
	if (!rcData)
		return LM_NULLPOINTER;
	
	printf("\n== Reconnection (DONE): feature=%s, current_retry=%d, retry_count=%d, retry_interval=%d, lic=%d, (COAVAIL checkouts only) licConsumedsoFar=%d\n",
			rcData->szFeature, rcData->nCurrentRetry, rcData->nRetryCount, rcData->nRetryInterval, rcData->nLicTotal, rcData->nCOAvailLicCurrent);
	
	return LM_NOERROR;
}

/*-----------------------------------------------------------------------------------------*/
static void sInit(struct flexinit_property_handle **handle)
{
#ifndef NO_ACTIVATION_SUPPORT
	struct flexinit_property_handle *ourHandle;
	int stat = 0;

	if ((stat = lc_flexinit_property_handle_create(&ourHandle)))
	{
		fprintf(lm_flex_stderr(), "ERROR: lc_flexinit_property_handle_create failed: %d\n", stat);
		exit(1);
	}
	if ((stat = lc_flexinit_property_handle_set(ourHandle,
			(FLEXINIT_PROPERTY_TYPE)FLEXINIT_PROPERTY_USE_TRUSTED_STORAGE,
			(FLEXINIT_VALUE_TYPE)1)))
	{
		fprintf(lm_flex_stderr(), "ERROR: lc_flexinit_property_handle_set failed: %d\n", stat);
	    exit(1);
	}
	if ((stat = lc_flexinit(ourHandle)))
	{
		fprintf(lm_flex_stderr(), "ERROR: lc_flexinit failed: %d\n", stat);
	    exit(1);
	}
	*handle = ourHandle;
#endif /* NO_ACTIVATION_SUPPORT */
}

/*-----------------------------------------------------------------------------------------*/
static void sCleanup(struct flexinit_property_handle *initHandle)
{
#ifndef NO_ACTIVATION_SUPPORT
	int stat = 0;

	if ((stat = lc_flexinit_cleanup(initHandle)))
	{
		fprintf(lm_flex_stderr(), "ERROR: lc_flexinit_cleanup failed: %d\n", stat);
	}
	if ((stat = lc_flexinit_property_handle_free(initHandle)))
	{
		fprintf(lm_flex_stderr(), "ERROR: lc_flexinit_property_handle_free failed: %d\n", stat);
	}
#endif /* NO_ACTIVATION_SUPPORT */
}

/*-----------------------------------------------------------------------------------------*/
int
main(int argc, char * argv[])
{
	VENDORCODE code;
	LM_HANDLE *lm_job = NULL;
	struct flexinit_property_handle *initHandle = NULL;

	char szFeature[MAX_FEATURE_LEN*2] = {'\0'};
	char szVersion[MAX_VER_LEN+1] = {'\0'};

	int nTotalLicCountObtained = 0, nLicCountReq = 0, nLicCountNeeded = 0;
	int nIteration = 0;
	int bCheckoutSuccess = 0;

	FNP_RECONNECT_DATA userReconnectData;
	CONFIG *pConf = NULL;

	int nErrorCode = -1; /* Default error code */

	if (argc > 2)
	{
		sPrintUsage();
		return 1;
	}

	/* Activation library component initialization */
	sInit(&initHandle);

	/* Create a new job handle */
	if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
	{
		printf("\nERROR: lc_new_job failed");
		sCleanup(initHandle);
		return lc_get_errno(lm_job);
	}

	printf("--------------------------------------------------\n");
	printf("Enter \"f1\" to demo floating functionality\n");
	printf("Enter \"f2\" to demo node-locked functionality\n");
	printf("Enter feature to checkout [default: \"%s\"]: ", FNP_DEFAULT_FEATURE);

	fgets(szFeature, MAX_FEATURE_LEN + 2, lm_flex_stdin()); 
	szFeature[strlen(szFeature) - 1] = '\0';

	if(!*szFeature)
		strcpy(szFeature, FNP_DEFAULT_FEATURE);	/* Default feature */

	strcpy(szVersion, "1.0");		/* Default version */
	nLicCountReq = 1;				/* Default license count */

	memset(&userReconnectData, 0, sizeof(FNP_RECONNECT_DATA));

	if (argc > 1)
	{
		nLicCountReq = atoi(argv[1]);
	}

	printf("--------------------------------------------------");

	(void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
	(void)lc_set_attr(lm_job, LM_A_USER_RECONNECT_V1_DATA, (LM_A_VAL_TYPE)&userReconnectData);
	(void)lc_set_attr(lm_job, LM_A_USER_RECONNECT_V1, (LM_A_VAL_TYPE)sUserReconnectV1);
	(void)lc_set_attr(lm_job, LM_A_USER_RECONNECT_V1_DONE, (LM_A_VAL_TYPE)sUserReconnectV1Done);

	nLicCountNeeded = nLicCountReq;
	nTotalLicCountObtained = 0;
	nIteration = 1;

	do
	{
		printf("\n===== COAVAIL Checkout Request-%d =====\n", nIteration++);
		printf("\n\tFeature = %s", szFeature);
		printf("\n\tVersion = %s", szVersion);
		printf("\n\tLicense count requested = %d", nLicCountReq);
		printf("\n\tLicense count needed = %d", nLicCountNeeded);

		/* Checkout license for a given feature */
		if ((nErrorCode = lc_checkout(lm_job, 
				szFeature, 
				szVersion, 
				nLicCountNeeded,    /* number of licenses to check out */
				LM_CO_AVAIL_NOWAIT, /* COAVAIL checkout option flag */
				&code, 
				LM_DUP_NONE)) != LM_NOERROR)
		{
			printf("\n\n\tERROR: lc_checkout failed. \n");
			sPrintErrorInfo(lm_job);
			break;
		}

		bCheckoutSuccess = 1;
		printf("\n\n\tFeature %s checkout success...", szFeature);

		/* Get the CONFIG for the checked-out feature */
		pConf = lc_auth_data(lm_job, szFeature);

		if (!pConf)
		{
			nErrorCode = lc_get_errno(lm_job);
			printf("\\n\n\tERROR: lc_auth_data failed. ");
			sPrintErrorInfo(lm_job);
			break;
		}

		printf("\n\t\tIncremental license count: (requested = %d) ==> (obtained = %d)", nLicCountNeeded, pConf->nCOAvailLastLicCount);

#if defined(FNP_LMFLEX_COAVAIL_VERBOSE)
		printf("\n\n\t\t----------------------------------------------");
		printf("\n\t\tnConfigType = %d (%s)", pConf->nConfigType, sGetConfigType(pConf->nConfigType));
		printf("\n\t\tnCOAvailLastLicCount = %d", pConf->nCOAvailLastLicCount);
		printf("\n\t\t----------------------------------------------");
#endif

		if ( (pConf->nConfigType == FNP_CONFIG_TYPE_SERVED_CERT) ||
				(pConf->nConfigType == FNP_CONFIG_TYPE_SERVED_TS) )
		{
			if (pConf->nCOAvailLastLicCount > 0)
			{
				/* Get license consumption count from nCOAvailLastLicCount */
				nTotalLicCountObtained += pConf->nCOAvailLastLicCount;
			}
			else
			{
				printf("\n\t\tWarning! License consumption count cannot be 0");
				break;
			}
		}
		else
		{
			printf("\n\n\tNot a Served Config. Ignore using \"License consumption count\" member");
			break;
		}

		printf("\n\n\t*** Feature %s (%s), total license count obtained = %d ***\n", szFeature, szVersion, nTotalLicCountObtained);
		sPrintConfigInfo(pConf);

		nLicCountNeeded = nLicCountReq-nTotalLicCountObtained;

		if (nLicCountNeeded <= 0)
			printf("\n\tNo more license needed...");

	} while (nLicCountNeeded > 0);

	if (bCheckoutSuccess)
	{
		printf("\n--------------------------------------------------\n");
		printf("\n\tPress ENTER to checkin license(s)...\n");
		getchar();

		/* Check-in license for a given feature */
		(void) lc_checkin(lm_job, szFeature, 0);
		printf("\n\n\tFeature %s checkin success...", szFeature);
	}

	printf("\n--------------------------------------------------\n");
	printf("\n\tPress ENTER to exit the application...\n");
	getchar();

	/* Free the job handle that has been allocated by lc_new_job */
	lc_free_job(lm_job);

	/* Unload the activation library and perform any cleanup functions required */
	sCleanup(initHandle);

	return nErrorCode;
}
