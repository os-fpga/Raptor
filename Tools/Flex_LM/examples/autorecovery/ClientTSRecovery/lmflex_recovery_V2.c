/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description:	This is a sample application program, to illustrate
 *			the use of the Flexible License Manager with the facility of Automatic Recovery of broken client TS using V2 activation.
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
#include "activation/include/FlxActCommon.h"
#include "activation/include/FlxActTransaction.h"
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
static FlxActBool sRepairFRsFromBackOff(FlxActHandle  handle, FlxActTransaction transaction);
static FlxActBool sAddRepairAction(FlxActHandle hHandle, FlxActTranRequest hTranRequest);
static FlxActBool setCommDetails(FlxActTransaction transaction);
static uint32_t sCommStatusCallback(const void* pUserData, uint32_t statusOld, uint32_t statusNew);

typedef struct CommsCallbackData_struct
{
	FlxActBool			bDidConnect;
	FlxActBool			bDidSendRequest;
	FlxActBool			bDidReceiveResponse;
} CommsCallbackData;

/* Back Office(FNO) address */
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
	
	/* Parse command line arguments to fetch Back Office(FNO) address  
	 * Where 
	 *    Arg1 : License count
	 *	  Arg2 : Back Office Address
	 * 
	 * Example:
	 *	  lmflex.exe <License Count> <Back Office Address>  
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
				printf("\n    Usage: lmflex.exe <License Count> <Back Office Address> \n\n");
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

	if(err == LM_REPAIR_NEEDED )
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
			//printf(" WARNING: Back Office Address(FNO) can not be empty to initiate automatic recovery! \n");
			printf(" Execute \"lmflex.exe -help\" for usage of automatic recovery. \n");	
			printf(" Ignore above message, if don't want to invoke automatic recovery for broken TS. \n\n");
		}		
	}	

	/* Display error, if lc_checkout() failed because of some other reason 
	   Or sRepairTSAndResetJob() failed to repair Trusted storage */
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
/**	@brief	sAutomaticRepair repair the broken FRs activated from Back Office(FNO)
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool sAutomaticRepair()
{
	FlxActHandle	    handle = 0;
	FlxActMode		    mode = FLX_ACT_APP;
	FlxActBool		    bRV = FLX_ACT_FALSE;
	FlxActBool          tranCreate = FLX_ACT_FALSE;
	FlxActTransaction   transaction;
	FlxActTranResult    tranResult;
	FlxActError		    error;	
	int	initErr;
	
	memset(&error, 0, sizeof(FlxActError));
	
	/* Check if this executable processed with preptool */
	if((initErr=flxActCommonLibraryInit(NULL)) != flxInitSuccess)
	{
		printf("    ERROR: Activation library initialization failed: status %d\n", initErr);
		return FLX_ACT_FALSE;
	}
	else
	{
		/* Create handle and process. */
		if(flxActCommonHandleOpen(&handle, mode, &error))
		{
			/* Create a transaction object */	
			tranCreate = flxActTransactionCreate(handle, &transaction, &tranResult);
			if ( !tranCreate)
			{
				/* If reason for failure is TS load fail... */
				if (tranResult == FLX_ACT_TRAN_ERR_TS_LOAD)
				{
					/* It's probably because it is untrusted so do a local repair and try again. */
					(void)flxActCommonRepairLocalTrustedStorage(handle);
					tranCreate = flxActTransactionCreate(handle, &transaction, &tranResult);
				}						
			}
			if ( !tranCreate )
			{
				printf("    ERROR: Failed to create transaction object \n");
				flxActCommonHandleClose(handle);
				return FLX_ACT_FALSE;
			}
			
			if (!setCommDetails(transaction))
				return FLX_ACT_FALSE;
						
			bRV = sRepairFRsFromBackOff(handle, transaction);  
			
			/* Clean up */
			flxActTransactionDestroy(transaction);
			flxActCommonHandleClose(handle);	
		}
		else
			printf("    ERROR: flxActCommonHandleOpen - (%d,%d,%d)\n", error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		
		flxActCommonLibraryCleanup();
	}
	return bRV;
}

/****************************************************************************/
/**	@brief	sRepairFRsFromBackOff, perform a transaction to repair broken FRs from Back Office
 *	@param	handle	FlxActHandle used to access trusted storage
 *  @param  transaction FlxActTransaction used to creat and process an Action
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool sRepairFRsFromBackOff(FlxActHandle  handle, FlxActTransaction transaction)
{
	FlxActTranRequest	tranRequest;
	const char*	        pszUserReference = "";
	FlxActError         actError;	
	CommsCallbackData	callbackData;
		
	/* Create the request object. */
	if ( !flxActTransactionCreateRequest(transaction, pszUserReference, &tranRequest) )
	{
		return FLX_ACT_FALSE;
	}
		
	/* add each action commands to the request object */
	if ( !sAddRepairAction(handle, tranRequest) )
	{		
		return FLX_ACT_FALSE;
	}
	
	/* Initialise the data that will be set by the comms status callback function. */
	callbackData.bDidConnect		 = FLX_ACT_FALSE;
	callbackData.bDidSendRequest	 = FLX_ACT_FALSE;
	callbackData.bDidReceiveResponse = FLX_ACT_FALSE;
	 
	/* Send the request to the server and process the response it sends back. */
	if ( flxActTransactionSend(transaction, sCommStatusCallback, &callbackData) )
	{
		printf("\n    Response processed successfully. Actions were:\n");
		
		FlxActTranResponse	tranResponse;
		if ( flxActTransactionGetResponse(transaction, &tranResponse) )
		{
			uint32_t actionCount = 0;
			uint32_t actionIndex;
			uint32_t denyCount=1;
			
			if (flxActTranResponseGetActionCount(tranResponse, &actionCount) == FLX_ACT_TRUE && actionCount > 0)
			{
				for (actionIndex = 0; actionIndex < actionCount; ++actionIndex)
				{
					FlxActTranRspAction         rspAction = 0;
					const char* 				pszFulfillmentId;
					FlxActTranRspActionType     rspActionType;

					if ( flxActTranResponseGetAction(tranResponse, actionIndex, &rspAction) )
					{
						if ( !flxActTranRspActionGetAttribute(rspAction, FLX_ACT_TRAN_RSP_ACT_FULFILLMENT_ID, &pszFulfillmentId) )
						{
							printf("       ERROR: UNEXPECTED API ERROR flxActTranRspActionGetAttribute()\n");
							return FLX_ACT_FALSE;
						}
						
						if ( flxActTranRspActionGetType(rspAction, &rspActionType) )
						{
							if ( rspActionType == FLX_ACT_TRAN_RSP_ACTION_REPAIR )
								printf("       Repaired fulfillment \"%s\"\n", pszFulfillmentId);
								
							if ( rspActionType == FLX_ACT_TRAN_RSP_ACTION_REPAIR_DENY )
							{
								const char* pszReason;
								FlxActTranRspActionResult	actionResult;
								
								if ( !flxActTranRspActionGetAttribute(rspAction, FLX_ACT_TRAN_RSP_ACT_REASON, &pszReason) )
								{
									printf("       ERROR: UNEXPECTED API ERROR flxActTranRspActionGetAttribute()\n");
									return FLX_ACT_FALSE;
								}
							
								printf("\n       Repair Denied %d\n"
												 "          FulfillmentId:  %s\n"
												 "          Reason:  %s\n",
												 denyCount++,
												 pszFulfillmentId,
												 pszReason);
							}
						}
					}
				}
			}
		}
		
		if( !flxActTransactionDeleteStoredRequest(transaction) )
			printf("Unable to delete request\n");
		
		return FLX_ACT_TRUE;		
	}
	
	else
	{
		printf("\n    ERROR: Automatic repair failed \n");  //To test
		printf("        Reason:  ");	
		FlxActTranResult resCode = flxActTransactionGetResult();

		if (callbackData.bDidReceiveResponse)
		{	
			printf("Response received but error when processing it. \n");
		}
		
		if (!callbackData.bDidConnect)
		{
			printf("No response from Back Office(FNO) %s\n", g_pszCommServer);
			if( !flxActTransactionDeleteStoredRequest(transaction) )
				printf("Unable to delete request\n");
		}
		else
		{			
			if (resCode == FLX_ACT_TRAN_ERR_COMMS_INIT)
			{
				printf("Unable to initialise comms (fnpCommsSoap shared object missing?)\n");
			}
		}
		return FLX_ACT_FALSE;
	}

}

/****************************************************************************/
/**	@brief	setCommDetails, set comm type and comm server
 *	@param	transaction	 FlxActTransaction used to set comm type and comm server
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool setCommDetails(FlxActTransaction transaction)
{
	const char*	 pszCommsType="SOAP";
	
	/* Set the server URI. */
	if ( flxActTransactionSetCommsAttribute(transaction, FLX_ACT_TRAN_SVR_URI, g_pszCommServer) )  
	{
		printf("    Back Office Address:  %s\n", g_pszCommServer);
	
		/* Set the comms type. */
		if ( flxActTransactionSetCommsAttribute(transaction, FLX_ACT_TRAN_SVR_COMMS_TYPE, pszCommsType) )
			return FLX_ACT_TRUE;
	}
	
	return FLX_ACT_FALSE;
}

/****************************************************************************/
/**	@brief	sAddRepairAction, add broken FRs to transaction request object
 *	@param	handle	FlxActHandle used to access trusted storage
 *  @param	tranRequest FlxActTranRequest used to process the repair requests
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
FlxActBool sAddRepairAction(FlxActHandle handle, FlxActTranRequest tranRequest)
{
	FlxActLicSpc	  	licSpec = 0;
	FlxActProdLicSpc  	prodSpec = 0;
	FlxActTranReqAction	hAction;
	const char			*pszFulfillId;
	int 				repairActionCount = 0;
	int 				frCount = 0;
	int 				index;
	
	/* Create the container. */
	if ( !flxActCommonLicSpcCreate(handle, &licSpec) )
		return FLX_ACT_FALSE;
	
	/* Fill the container with all FRs. */
	if ( !flxActCommonLicSpcPopulateAllFromTS(licSpec) )
	{
		flxActCommonLicSpcDelete(licSpec);
		return FLX_ACT_FALSE;
	}
	
	/* Examine fulfillment records and find the one that requires repair. */
	frCount = flxActCommonLicSpcGetNumberProducts(licSpec);
	
	if(frCount)
	{
		for(index = 0; index < frCount; index++)
		{
			/* Step through the fulfillment records */
			prodSpec = flxActCommonLicSpcGet(licSpec, index);
			if(0 == prodSpec)
			{
				printf("    ERROR: flxActCommonLicSpcGet - Invalid index\n");
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
				continue;
	
			/* Read the fulfillment id that requires repair */
			pszFulfillId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
			
			/* Create a new repair action for the request object. */		
			if ( !flxActTranRequestAddAction(tranRequest, FLX_ACT_TRAN_REQ_ACTION_REPAIR, &hAction) )
			{	
				flxActCommonLicSpcDelete(licSpec);		
				return FLX_ACT_FALSE;
			}
			
			/* Set the FulfillmentId attribute from the command value */
			if ( !flxActTranReqActionSetAttribute(hAction,
												FLX_ACT_TRAN_REQ_ACT_FULFILLMENT_ID,
												pszFulfillId) )
			{	
				flxActCommonLicSpcDelete(licSpec);
			    return FLX_ACT_FALSE;
			}
			
			repairActionCount++;
		}
	}
	
	if (repairActionCount == 0)
	{
	 	printf("    WARNING: No fulfillments activated from FNO need repairing...\n");
	}
	
	flxActCommonLicSpcDelete(licSpec);		
	return FLX_ACT_TRUE;
}	

/***************************************************************************************************
*	sCommStatusCallback
*
* This is called from within flxActTransactionSend when each stage of the transaction is completed
* and the status changes.
***************************************************************************************************/
static uint32_t sCommStatusCallback(const void* pUserData, uint32_t statusOld, uint32_t statusNew)
{
	const char* const COMMS_STATUS_STRINGS[] = {
		"Error",
		"Success",
		"Not set",
		"Canceled by caller",
		"Creating request",
		"Request created",
		"Context created",
		"Connected to remote server",
		"Request Sent",
		"Polling for response",
		"Waiting for response",
		"Done"
	};
	CommsCallbackData* pCallbackData = (CommsCallbackData*)pUserData;

	printf("    Status: %d, %s\n",
					statusNew,
					(statusNew < sizeof(COMMS_STATUS_STRINGS) / sizeof(COMMS_STATUS_STRINGS[0]))?
							COMMS_STATUS_STRINGS[statusNew] : "Unknown");

    if (statusNew == flxCommsStatusConnected)
	{
		/* Successfully connected to the server - tell the caller through the user data. */
		pCallbackData->bDidConnect = FLX_ACT_TRUE;
	}
	else if (statusNew == flxCommsStatusRequestSent)
	{
		/* Successfully sent request to the server - tell the caller through the user data. */
		pCallbackData->bDidSendRequest = FLX_ACT_TRUE;
	}
	else if (statusNew == flxCommsStatusDone)
	{
		/* Received a response from the server - tell the caller through the user data. */
		pCallbackData->bDidReceiveResponse = FLX_ACT_TRUE;
	}

	return flxCallbackReturnContinue;
}