/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description:	This is a sample application program, to illustrate
 *			the use of the Flexible License Manager and Composite
 *                      Hostid.
 *
 *	Last changed:  07 Mar 2003
 *	D. Birns
 *
 */

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include "lm_attr.h"
#include "lm_redir_std.h"
#ifdef PC
#define LICPATH "license.dat;."
#else
#define LICPATH "@localhost:license.dat:."
#endif /* PC */

#define FEATURE "f1"
VENDORCODE code;
LM_HANDLE *lm_job;

void
main(int argc, char * argv[])
{
	int ret;
	char feature[MAX_FEATURE_LEN * 2] = {'\0'};

#ifdef PC
	int hostid_list[] = {HOSTID_ETHER, HOSTID_DISK_SERIAL_NUM};
	int hostid_list_count = 2;
#else
	int hostid_list[] = {HOSTID_INTERNET, HOSTID_HOSTNAME};
	int hostid_list_count = 2;
#endif /* PC */

	if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
	{
		lc_perror(lm_job, "lc_new_job failed");
		exit(lc_get_errno(lm_job));
	}

/*
 *	Initialize Composite HostID settings
 */
	ret = lc_init_simple_composite(lm_job, hostid_list, hostid_list_count);
	if (ret != 0)
	{
		printf("Failed to init composite hostid. Errno: %d\n", errno);

		lc_free_job(lm_job);
		exit(0);
	}

	printf("Enter \"f1\" to demo floating functionality\n");
	printf("Enter \"f2\" to node-locked functionality\n");
	printf("Enter feature to checkout [default: \"%s\"]: ", FEATURE);

	fgets(feature, MAX_FEATURE_LEN + 2, lm_flex_stdin());	/*	add 2 for \n and \0	*/
	feature[strlen(feature) - 1] = '\0';
	if(!*feature)
		strcpy(feature, FEATURE);

	lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
	if (lc_checkout(lm_job, feature, "1.0", 1, LM_CO_NOWAIT, &code,
								LM_DUP_NONE))
	{
		lc_perror(lm_job, "checkout failed");
		exit (lc_get_errno(lm_job));
        }
	printf("%s checked out...", feature);
	printf("press return to exit...");
/*
 *	Wait till user hits return
 */
	getchar();
	lc_checkin(lm_job, feature ,0);

	lc_free_job(lm_job);
	exit(0);
}
