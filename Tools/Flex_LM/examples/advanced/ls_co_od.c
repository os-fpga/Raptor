/**************************************************************************************************
* Copyright (c) 2014-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *	Vendor daemon overdraft callback example
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lm_code.h"
#include "lmclient.h"
#include "lm_attr.h"
#include "ls_attr.h"
#include "lsserver.h"

#include "ls_co_od.h"

/**********************************************************************
 * == Sample license file ==
 * 
 * SERVER this_host ANY
 * VENDOR demo
 * USE_SERVER
 * #a counted license
 * INCREMENT f1_od demo 1.0 permanent 4 OVERDRAFT=1000 SIGN=0
 * 
 *********************************************************************/

static FILE *pODLogFp = NULL;

/* 
 * Vendor daemon init callback:
 * e.g. void (*ls_user_init1)() = ls_user_init_callback_example;
 */
void ls_user_init_callback_example()
{
	pODLogFp = fopen("demo_od.log", "w");	

	return;
}

/*
 * Overdraft callback (checkout): 
 * void (*ls_outod_callback)() = ls_outod_callback_example; 
 */
void ls_outod_callback_example()
{
	CLIENT_DATA *pClientData;
	char *pClientHost = NULL;
	char *pClientUser = NULL;
	char *szFeature = NULL, *szVersion = NULL, *szCode = NULL, *szVendorName = NULL;
	int nRequestedLic = 0, nDupSel = 0, nLinger = 0;
	int nODRequests = 0, nTotalODLicInUse = 0, nODLicCount = 0;

	if (!pODLogFp)
		return;

	ls_get_attr(LS_ATTR_FEATURE, &szFeature);
	ls_get_attr(LS_ATTR_VERSION, &szVersion);
	ls_get_attr(LS_ATTR_CLIENT, (char**)&pClientData);

	ls_get_attr(LS_ATTR_CODE, &szCode);
	ls_get_attr(LS_ATTR_VENDOR_NAME, &szVendorName);
	ls_get_attr(LS_ATTR_DUP_SEL, (char**)&nDupSel);
	ls_get_attr(LS_ATTR_LINGER, (char**)&nLinger);

	if (!pClientData)
	{
		fprintf(pODLogFp, "\nOD-OUT: Client info could not be retrieved (null)!");
		fflush(pODLogFp);
		return;
	}

	if ( (!szFeature) || (!szVersion) ) 
	{
		fprintf(pODLogFp, "\nOD-OUT: Feature info could not be retrieved (null)!");
		fflush(pODLogFp);
		return;
	}

	/* Retrieve requested license count */
	ls_get_attr(LS_ATTR_NLIC, (char**)&nRequestedLic);

	/* Retrieve overdraft license count consumed by current checkout */
	ls_get_attr(LS_ATTR_NLIC_OD, (char**)&nODLicCount);

	/* Retrieve number of overdraft users (current checkout inclusive) */
	ls_get_attr(LS_ATTR_OD_REQUESTS, (char**)&nODRequests);

	/* Retrieve overdraft license consumption (current checkout inclusive) */
	ls_get_attr(LS_ATTR_TOTAL_NLIC_OD_INUSE, (char**)&nTotalODLicInUse);

	pClientHost = pClientData->node;
	pClientUser = pClientData->name;

	fprintf(pODLogFp, "\nOD-OUT: Checkout: Client: Handle=%d, Host=%s, User=%s", pClientData->handle, pClientHost, pClientUser);
	fprintf(pODLogFp, "\nOD-OUT: Checkout: Client request: Feature=%s, Version=%s, Count=%d", szFeature, szVersion, nRequestedLic);
	fprintf(pODLogFp, "\nOD-OUT: Checkout: \t Overdraft license consumed by current checkout=%d", nODLicCount);
	fprintf(pODLogFp, "\nOD-OUT: Checkout: \t Overdraft license consumption (current checkout inclusive)=%d", nTotalODLicInUse);
	fprintf(pODLogFp, "\nOD-OUT: Checkout: \t Overdraft requests (current checkout inclusive)=%d", nODRequests);
	fprintf(pODLogFp, "\nOD-OUT: ----------------------\n");
	fflush(pODLogFp);

	return;
}

/*
 * Overdraft callback (checkin): 
 * void (*ls_inod_callback)() = ls_inod_callback_example; 
 */
void ls_inod_callback_example()
{
	char *szFeature  = NULL;
	CLIENT_DATA *pClientData;
	int nODRequests = 0, nTotalODLicInUse = 0, nODLicCount = 0;
	char *pClientHost = NULL;
	char *pClientUser = NULL;
	unsigned int bNeeded = 1;

	if (!pODLogFp)
		return;

	ls_get_attr(LS_ATTR_FEATURE, &szFeature);
	ls_get_attr(LS_ATTR_CLIENT, (char**)&pClientData);

	if (!pClientData)
	{
		fprintf(pODLogFp, "\nOD-IN: Client info could not be retrieved (null)!");
		fflush(pODLogFp);
		return;
	}

	if (!szFeature) 
	{
		fprintf(pODLogFp, "\nOD-IN: Feature info could not be retrieved (null)!");
		fflush(pODLogFp);
		return;
	}

	pClientHost = pClientData->node;
	pClientUser = pClientData->name;

 	/* Retrieve overdraft license count released by current checkin */
	ls_get_attr(LS_ATTR_NLIC_OD, (char**)&nODLicCount);
	
	/*
	 * It is to be noted that the use of attributes LS_ATTR_OD_REQUESTS and 
	 * LS_ATTR_TOTAL_NLIC_OD_INUSE in ls_inod_callback (overdraft checkin callback)
	 * may have a performance impact, so as a best practice use them only if needed.
	 * 
	 * However, in this example bNeeded variable is set to 1 by default and 
	 * these attributes are always used.
	 */

	if (bNeeded)
	{	
		/* Retrieve number of overdraft users (current checkin inclusive) */
		ls_get_attr(LS_ATTR_OD_REQUESTS, (char**)&nODRequests);
	
		/* Retrieve overdraft license consumption (current checkin inclusive) */
		ls_get_attr(LS_ATTR_TOTAL_NLIC_OD_INUSE, (char**)&nTotalODLicInUse);
	}

	fprintf(pODLogFp, "\nOD-IN: Checkin: Client: Handle=%d, Host=%s, User=%s", pClientData->handle, pClientHost, pClientUser);
	fprintf(pODLogFp, "\nOD-IN: Checkin: Client request: Feature=%s", szFeature);
	fprintf(pODLogFp, "\nOD-IN: Checkin: \t Overdraft license released by current checkin=%d", nODLicCount);

	if (bNeeded)
	{
		fprintf(pODLogFp, "\nOD-IN: Checkin: \t Overdraft license consumption (current checkin inclusive)=%d", nTotalODLicInUse);
		fprintf(pODLogFp, "\nOD-IN: Checkin: \t Overdraft requests (current checkin inclusive)=%d", nODRequests);
	}
	fprintf(pODLogFp, "\nOD-IN: ----------------------\n");
	fflush(pODLogFp);

	return;
}

/*
 * Vendor daemon shutdown callback:
 * e.g. void (*ls_vd_shutdown)() = ls_vd_shutdown_callback_example;	
 */
void ls_vd_shutdown_callback_example()
{
	if (!pODLogFp)
		fclose(pODLogFp);

	return;
}
