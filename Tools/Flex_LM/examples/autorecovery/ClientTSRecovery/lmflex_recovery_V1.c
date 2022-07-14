/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description:	This is a sample application program, to illustrate
 *			the use of the Flexible License Manager.
 *
 */

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lm_attr.h"
#include "lm_redir_std.h"

/* Activation headers required by Automatic Repair */
//#include <sys/types.h>
//#include <sys/stat.h>
#include "activation/include/FlxActCommon.h"
#include "activation/include/FlxActError.h"
#include "activation/include/FlxActApp.h"
#include "activation/include/FlxInit.h"

#ifdef WINDOWS
#include "activation/include/windows/wininstaller.h"
#include <io.h>
#if ( _MSC_VER>=1300 )
/* Disable deprecated function warnings */
#pragma warning( disable:4996 )
#endif
#endif /* WINDOWS */

#ifdef PC
#define LICPATH "@localhost"
#else
#define LICPATH "@localhost:license.dat:."
#endif /* PC */

#define FEATURE "f1"
VENDORCODE code;
LM_HANDLE *lm_job;


static void init(struct flexinit_property_handle **);
static void cleanup(struct flexinit_property_handle *);

/* Functions and global variable required by Automatic Repair */
static FlxActBool sRepairTSAndResetJob(struct flexinit_property_handle *);
static FlxActBool sAutomaticRepair();
static FlxActBool sHandleRepair( FlxActHandle  handle);
static void setCommDetails(FlxActHandle	handle);

/* Operations server(FNO) address */
const char *g_pszCommServer = NULL;    	

int
main(int argc, char * argv[])
{
	char feature[MAX_FEATURE_LEN * 2] = {'\0'};
	struct flexinit_property_handle *initHandle = NULL;
	int nlic = 1;
	int i = 1;
	int err = 0;
	FlxActBool	rcv = FLX_ACT_FALSE;
	FlxActBool	bIsServer;
	const char *comm_server = NULL;
	
	init(&initHandle);
	
	/* Parse command line arguments to fetch Operations server(FNO) address  
	 * Where 
	 *    Arg1 : License count
	 *	  Arg2 : Operations Server Address
	 * 
	 * Example:
	 *	  lmflex.exe <License Count> <Operations Server Address>  
	 */
	while( i < argc)
	{
		char *end;
		strtol(argv[i], &end, 10);
	
		if (*end != '\0')
		{
			comm_server = argv[i];
			
			if( strcmp( "-help", comm_server ) == 0 || strcmp( "-h", comm_server ) == 0 )
			{
				printf("\n    Usage: lmflex.exe <License Count> <Operations Server Address> \n\n");
				printf("    Example 1: lmflex.exe 1 http://myfnoserver.com/flexnet/services/ActivationService \n");
				printf("    Example 2: lmflex.exe http://myfnoserver.com/flexnet/services/ActivationService \n");
				exit(0);
			}
			else 
			    g_pszCommServer = comm_server;
		}
		else
			nlic = atoi(argv[i]);
		
		i++;
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
   
	err = lc_checkout(lm_job, feature, "1.0", nlic, LM_CO_NOWAIT, &code, LM_DUP_NONE);
	
	if(err == LM_REPAIR_NEEDED)
	{
		printf("\n Trusted Storage found compromised! \n");	
		
		if ( g_pszCommServer != NULL )
		{
			bIsServer = (flxActCommonGetProtectionMode() == FLX_ACT_SVR)?
											FLX_ACT_TRUE : FLX_ACT_FALSE;	
											
			/* Do automatic recovery only if checkout failed because of broken client Trusted storage */
			if( bIsServer == FLX_ACT_FALSE )
			{
				printf(" Started Automatic recovery of Client Trusted Storage... \n\n");
				rcv = sRepairTSAndResetJob(initHandle);
				printf("\n Completed Automatic recovery of Trusted Storage \n\n");
				if ( rcv )
					err=lc_checkout(lm_job, feature, "1.0", nlic, LM_CO_NOWAIT, &code, LM_DUP_NONE);
			}
			else
			{
				printf(" License checkout attempted from server Trusted Storage \n");
				printf("    Automatic recovery implemented only for client Trusted storage. \n");			
			} 
		}
		else
		{
			printf(" WARNING: Back Office Address(FNO) can not be empty to initiate automatic recovery! \n");
			printf(" Execute \"lmflex.exe -help\" for usage of automatic recovery. \n");	
			printf(" Ignore above message, if don't want to invoke automatic recovery for broken TS. \n\n");
		}				
	}	

	/* Display error, if lc_checkout() failed because of some other reason Or sRepairTSAndResetJob() failed to repair Trusted storage */
	if(err)
	{		
		lc_perror(lm_job, "checkout failed");
		cleanup(initHandle);
		exit (lc_get_errno(lm_job));
	}
	printf("%s checked out...", feature);
	printf("press return to exit...");

	/*
	*	Wait till user hits return
	*/
	getchar();
	lc_checkin(lm_job, feature, 0);
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

/****************************************************************************/
/**	@brief	sRepairTSAndResetJob repair the Trusted Storage and reset the job.
 *	@param	initHandle	used to access property handle 
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool sRepairTSAndResetJob(struct flexinit_property_handle *initHandle )
{
	/* Repair the Trusted Storage */
	FlxActBool rcv=sAutomaticRepair();
	
	if ( rcv != FLX_ACT_FALSE )
	{
		/* Create a new job for license checkout */	
		lc_free_job(lm_job);
		if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
		{
			lc_perror(lm_job, "lc_new_job failed");
			cleanup(initHandle);
			exit(lc_get_errno(lm_job));
		}
		
		(void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
	}
	return rcv;
			
}
/****************************************************************************/
/**	@brief	sAutomaticRepair repair the Trusted Storage.
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool sAutomaticRepair()
{
	FlxActHandle	handle = 0;
	FlxActMode		mode = FLX_ACT_APP;
	FlxActError		error;
	FlxActBool		bRV = FLX_ACT_FALSE;
	
	int				initErr;
	memset(&error, 0, sizeof(FlxActError));
	
	/* Initialize */
	if((initErr=flxActCommonLibraryInit(NULL)) != flxInitSuccess)
	{
		printf("ERROR: Activation library initialization failed: status %d\n", initErr);
		return FLX_ACT_FALSE;
	}
	else
	{
		/* Create handle and process. */
		if(flxActCommonHandleOpen(&handle, mode, &error))
		{
			bRV = sHandleRepair(handle);    	   
			flxActCommonHandleClose(handle);   
		}
		else
		{
			printf("ERROR: flxActCommonHandleOpen - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		}
		
		flxActCommonLibraryCleanup();
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	sHandleRepair, repairs fulfillments those activated from FNO and in broken state.
 *	@param	handle		FlxActHandle used to access trusted storage
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool sHandleRepair(FlxActHandle handle)
{
	FlxActBool			bRV = FLX_ACT_FALSE;
	FlxActBool			bFound = FLX_ACT_FALSE;
	FlxActAppRepair		repair = 0;
	FlxFulfillmentType      type = FULFILLMENT_TYPE_UNKNOWN;
	FlxActError			error;
	FlxActLicSpc		licSpec = 0;
	FlxActProdLicSpc	prodSpec = 0;
	unsigned int		count = 0;
	unsigned int		i = 0;
	const char *		pszFulfillId = NULL;
	const char *		pszOpsError = NULL;
	const char *		pszOutput = NULL;
	
	if(handle)
	{
		/* Find out which one we want to repair */
		bRV = flxActCommonLicSpcCreate(handle, &licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			printf("ERROR: flxActCommonLicSpcCreate\n");
			return bRV;
		}
		/* Populate from TS */
		bRV = flxActCommonLicSpcPopulateAllFromTS(licSpec);
		if(FLX_ACT_FALSE == bRV)
		{
			flxActCommonLicSpcDelete(licSpec);
			return bRV;
		}
		
		/* Add the license server address to the repair object. */
		setCommDetails(handle);
		
		/* Examine fulfillment records and find the one that requires repair. */
		count = flxActCommonLicSpcGetNumberProducts(licSpec);
		if(count)
		{
			for(i = 0; i < count; i++)
			{
				/* Step through the fulfillment records */
				prodSpec = flxActCommonLicSpcGet(licSpec, i);
				if(0 == prodSpec)
				{
					printf("ERROR: flxActCommonLicSpcGet - Invalid index\n");
					flxActCommonLicSpcDelete(licSpec);
					return FLX_ACT_FALSE;
				}

				/*	Check to see whether this fulfillment record requires repair */
				if(FLAGS_FULLY_TRUSTED == flxActCommonProdLicSpcTrustFlagsGet(prodSpec))
				{
					/* Fully trusted, no need to do repair */
					continue;
				}

				/* Check to see whether this fulfillment record is activated from publisher’s activation server.*/
				if ( FULFILLMENT_TYPE_PUBLISHER_ACTIVATION != flxActCommonProdLicSpcFulfillmentTypeGet(prodSpec) )
				{
					continue;
				}

				/* Read the fulfillment id that requires repair */
				pszFulfillId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
				
				printf(" Initiating a repair request to Operations server(FNO)...\n");
				
				/* Create a reapir object */
				if(FLX_ACT_FALSE == flxActAppRepairCreate(handle, &repair))
				{
					printf("ERROR: flxActAppRepairCreate\n");
					return FLX_ACT_FALSE;	
				}
				
				if(pszFulfillId)
				{
					/* Set the repair request parameters */
					flxActAppRepairProdLicSpcSet(repair, prodSpec);
					bFound = FLX_ACT_TRUE;
				}
				
				/* Send the repair request to a license server and process the response */			
			    if(flxActAppRepairSend(repair, &error))
			    {
			    	printf("  Automatic repair succeed:\n");
					printf("    FulfillmentId: %s \n\n",pszFulfillId);
					bRV = FLX_ACT_TRUE;
			    }
			    else
			    {
			    	printf("  Automatic repair request failed:\n");
					printf("    FulfillmentId: %s \n",pszFulfillId);
					
					flxActCommonHandleGetError(handle, &error);		
					if(LM_TS_OPERATIONS == error.majorErrorNo)
					{
						pszOpsError = flxActCommonHandleGetLastOpsErrorString(handle);
						if(pszOpsError)
						{
							printf("    Reason: %d %s\n\n", flxActCommonHandleGetLastOpsError(handle), pszOpsError);
						}
					}
					else
					{
						if( error.majorErrorNo == 50041 )
							printf("    Reason: Failed to connect to the Operations server(FNO).\n\n");
						else
						printf("    ERROR: flxActAppActivationSend - (%d,%d,%d)\n\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
						
					}
				
			    }

				/* Delete a reapir object */
                flxActAppRepairDelete(repair);				
			}
			
		}

		if(FLX_ACT_FALSE == bFound)
		{
			printf(" ERROR: Unable to find any broken fulfillment id those activated from Operations server(FNO). \n");
			flxActCommonLicSpcDelete(licSpec);
			return FLX_ACT_FALSE;
		}
		 
	   	/* Cleanup */
		if(licSpec)
		{
			flxActCommonLicSpcDelete(licSpec);
		}
	}

	return bRV;
}

/****************************************************************************/
/**	@brief	To set comm type and comm server
 *	@param	handle		FlxActHandle used to access trusted storage
 *	@return	void
 ****************************************************************************/
static void setCommDetails(FlxActHandle	handle)
{
	flxActCommonHandleSetRemoteServer(handle, g_pszCommServer);
		
	if (g_pszCommServer != NULL )
		flxActCommonHandleSetCommType(handle, flxCommsMvsnSoap);
}
