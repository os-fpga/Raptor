/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

#include <windows.h>
#include <string.h>

#include "lmpolicy.h"
#include "lmprikey.h"
#include "lmseeds.h"
#include "lmclient.h"
#include "lm_attr.h"

#include "lmgr11.h"

/***********************************************************************
*
*  DLL Entry point.
*
************************************************************************/

BOOL WINAPI DllMain( HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved )
{
	// add any additional DLL initialization code here
	return TRUE;
}

/***********************************************************************
*
*  Function     new_job
*
*  Description  This will attempt to create a new job.
*
*  Parameters   (LM_HANDLE**) jobPtr - Pointer to job for the current 
*                                      process
*               (char*) szLicFileList - location of lic file
*
************************************************************************/

int WINAPI new_job(LM_HANDLE **jobPtr, char *szLicFileList)
{
	VENDORCODE code;
	if (jobPtr == NULL)
	{
		return (-1);
	}
	if (lc_new_job(0, NULL, &code, jobPtr))
	{
		return (-1);
	}
	/* Call lc_set_attr() with the expected location
	of the application’s license file directory. */
	(void)lc_set_attr(*jobPtr, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)szLicFileList);
	return 0;
}

/***********************************************************************
*
*  Function     checkout
*
*  Description  This will attempt to checkout a feature passed to it
*               from the interfacing code.
*
*  Parameters   (LM_HANDLE**) jobPtr - Pointer to job for the current 
*                                      process
*               (char*) szFeatureName - Feature name provided by caller
*               (char*) szFeatureVer - Feature version id provided by
*                                      caller
*               int iNumLicenses - number of licenses
*               (int) ckoutOptionFlag - checkout option flag
*
************************************************************************/

int WINAPI checkout(
	LM_HANDLE *job,
	char *szFeatureName,
	char *szFeatureVer,
	int iNumLicenses,
	int ckoutOptionFlag)
{
	VENDORCODE code;
	if (lc_checkout(job, szFeatureName, szFeatureVer, iNumLicenses, ckoutOptionFlag, &code, LM_DUP_NONE))
	{
		lc_perror(job, "checkout failed");
		return (lc_get_errno(job));
	}
	return 0;
}

/***********************************************************************
*
*  Function     checkin
*
*  Description  Returns a checked out license.
*
*  Parameters   (LM_HANDLE*) job - job for the current process
*               (char*) feature - Feature name provided by caller
*               int keep - Drop or keep connection if the license was
*                          obtained from a license server
*
************************************************************************/

void WINAPI checkin(
	LM_HANDLE * job,
	const char * feature,
	int keep)
{
	lc_checkin(job, feature, keep);
}

/***********************************************************************
*
*  Function     free_job
*
*  Description  This will free a job.
*
*  Parameters   (LM_HANDLE*) job - job for the current process
*
************************************************************************/

void WINAPI free_job(LM_HANDLE *job)
{
	lc_free_job(job);
}

/***********************************************************************
*
*  Function     heartbeat
*
*  Description  Sends a manual heartbeat to the license server.
*
*  Parameters   (LM_HANDLE*) job - job for the current process
*               int ret_num_reconnects - number of reconnects in 
*                                        the last minutes
*               int minutes - Reporting period for start and stop in 
*                             minutes
*
************************************************************************/

int WINAPI heartbeat(
	LM_HANDLE * job,
	int * ret_num_reconnects,
	int minutes)
{
	return lc_heartbeat(job, ret_num_reconnects, minutes);
}

/***********************************************************************
*
*  Function     errstring
*
*  Description  Returns ASCII string for the last FLEXlm error set.
*
*  Parameters   (LM_HANDLE*) job - job for the current process
*
************************************************************************/

char* WINAPI errstring(LM_HANDLE * job)
{
	return lc_errstring(job);
}

/***********************************************************************
*
*  Function     l_perror()
*
*  Description  Presents an error dialog box on windows only, along with
*               a user provided string.
*
*  Parameters   (LM_HANDLE*) job - job for the current process
*               (char*) szErrStr - a string describing error context
*
************************************************************************/

void WINAPI l_perror(
	LM_HANDLE * job,
	char * szErrStr)
{
	lc_perror(job, szErrStr);
}
