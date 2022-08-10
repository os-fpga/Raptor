/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description:	This is a sample application program, to illustrate the
 *			        use of Secure Data Types in License File-Based licensing.
 *
 *  Usage  : lmflexsdt <Number of licenses> <no-checkout>
 *  Example: lmflexsdt 1 no-checkout
 */

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lm_attr.h"
#ifdef PC
#define LICPATH "@localhost"
#else
#define LICPATH "@localhost:license.dat:."
#endif /* PC */

#include "lmflexsdtdefs.h"
#include "lm_redir_std.h"

#define FEATURE "f1"

VENDORCODE code;
LM_HANDLE *lm_job;
static void init(struct flexinit_property_handle **);
static void cleanup(struct flexinit_property_handle *);

int
main(int argc, char * argv[])
{
	char feature[MAX_FEATURE_LEN * 2] = {'\0'};
	char * output = NULL;
	struct flexinit_property_handle *initHandle;
	int nlic = 1;
	unsigned int nSkipCheckout = 0;

	init(&initHandle);

	if (argc > 1)
	{
		/* Number of licenses. */
		nlic = atoi(argv[1]); 

		/* To demonstrate the behavior of SDTs when valid license checkout is circumvented. */
		if (argv[2]) 
		{
			nSkipCheckout = 1;
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


	fgets(feature, MAX_FEATURE_LEN + 2, lm_flex_stdin());	/*	add 2 for \n and \0	*/
	feature[strlen(feature) - 1] = '\0';
	if(!*feature)
		strcpy(feature, FEATURE);

	(void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);

	if (!nSkipCheckout)
	{
		if (lc_checkout(lm_job, feature, "1.0", nlic, LM_CO_NOWAIT, &code, LM_DUP_NONE))
		{
			lc_perror(lm_job, "lc_checkout(): checkout failed.");
			cleanup(initHandle);
			exit (lc_get_errno(lm_job));
		}

		printf("\nCheckout succeeded for feature (%s).", feature);
	}
	else
	{
		printf("\nCheckout skipped for feature (%s).", feature);
	}

	if (strcmp(feature, FEATURE) == 0)
	{
		unsigned int nValue1 = 10, nValue2 = 20;
		unsigned int nExpectedResult = 30; /* nValue1 + nValue2 */
		unsigned int nSDTResult = 0;	   
		
		/* Declare secure variables for feature f1. */
		SDT_FEATURE_DECLARE_VAR( FeatureF1, nSDTValue1 );
		SDT_FEATURE_DECLARE_VAR( FeatureF1, nSDTValue2 );

		/* Set value of secure variables. */
		SDT_FEATURE_SET( nSDTValue1, nValue1  );
		SDT_FEATURE_SET( nSDTValue2, nValue2  );

		/* Addition operation on secure variables, 
		   nValue1 = nValue1 + nValue2 */
		SDT_FEATURE_ADD( nSDTValue1, nSDTValue2 );

		/* Retrieve the result of above arithmetic operation from secure variable. */
		nSDTResult = SDT_FEATURE_GET( nSDTValue1 );

		printf("\nAdding SDT value %d to SDT value %d gives %d.\n", nValue1, nValue2, nSDTResult);
	}
	else
	{
		printf("\nSecure Data Types not defined for feature (%s).\n", feature);
		goto CHECK_IN;
	}

	printf("\nPress ENTER key to exit...");

	/*
	*	Wait till user hits return
	*/
	getchar();
CHECK_IN:
	if (!nSkipCheckout)
		lc_checkin(lm_job, feature, 0);

	lc_free_job(lm_job);
	cleanup(initHandle);
	return 0;
}

static void init(struct flexinit_property_handle **handle)
{
#ifndef NO_ACTIVATION_SUPPORT
	struct flexinit_property_handle *ourHandle;
	int stat;

	if (stat = lc_flexinit_property_handle_create(&ourHandle))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_create() failed: %d\n", stat);
		exit(1);
	}
	if (stat = lc_flexinit_property_handle_set(ourHandle,
			(FLEXINIT_PROPERTY_TYPE)FLEXINIT_PROPERTY_USE_TRUSTED_STORAGE,
			(FLEXINIT_VALUE_TYPE)1))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_set failed: %d\n", stat);
	    exit(1);
	}
	if (stat = lc_flexinit(ourHandle))
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

	if (stat = lc_flexinit_cleanup(initHandle))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_cleanup failed: %d\n", stat);
	}
	if (stat = lc_flexinit_property_handle_free(initHandle))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_free failed: %d\n", stat);
	}
#endif /* NO_ACTIVATION_SUPPORT */
}

