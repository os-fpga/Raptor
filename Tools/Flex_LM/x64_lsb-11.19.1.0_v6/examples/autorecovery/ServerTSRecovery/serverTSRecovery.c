/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description: This is a sample application program, to illustrate
 *			     how to achieve automatic recovery of a broken Server TS using V2 transaction.
 *
 *  Points to note:
 *               - Automatic repair method will repair only the broken FRs those activated from FNO/ Back Office.
 *               - It will not repair other type of FRs (i,e. FRs activated from local ASR or from other enterprise server, etc. ) 
 *               - The repaired FRs will reflect in license server after next TS polling interval. 
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef PC
	// For Window platform
	#include <windows.h>
	#include <conio.h>	
#else
	// For Non Windows platform
	#include <pthread.h>	
#endif /* PC */	

#include "serverTSRecovery.h"
#include "lmclient.h"
#include "lm_attr.h"
#include "lm_redir_std.h"

/* Activation headers required by Automatic Repair */
#include "activation/include/FlxActCommon.h"
#include "activation/include/FlxActTransaction.h"
#include "activation/include/FlxActError.h"
#include "activation/include/FlxActApp.h"
#include "activation/include/FlxInit.h"

#ifdef PC
#define TSRecoveryPID GetCurrentProcessId()
#else
#define	TSRecoveryPID getpid()
#endif	

typedef struct CommsCallbackData_struct
{
	FlxActBool			bDidConnect;
	FlxActBool			bDidSendRequest;
	FlxActBool			bDidReceiveResponse;
} CommsCallbackData;

/* Functions and global variable required for Server TS Repair */
static FlxActBool sRepairServTS(char * g_pszCommServer);
static FlxActBool sRepairFRsFromBackOff(FlxActHandle  handle, FlxActTransaction transaction, char * g_pszCommServer);
static FlxActBool sAddRepairAction(FlxActHandle hHandle, FlxActTranRequest hTranRequest);
static FlxActBool setCommDetails(FlxActTransaction transaction, char * g_pszCommServer);
static uint32_t sCommStatusCallback(const void* pUserData, uint32_t statusOld, uint32_t statusNew);

static repairCnt = 0;

/****************************************************************************/
/**	@brief	servTSRecovery, will create a new thread for TS Recovery in Windows platform
 *	@param	g_pszCommServer, Back Office Address(FNO)
 *	@return	void
 ****************************************************************************/
void servTSRecovery(char * g_pszCommServer)
{
	FlxActBool	bIsServer;
	int initErr;

	if ( g_pszCommServer != NULL )
	{
		/* Check if this executable processed with preptool */
		if ((initErr=flxActCommonLibraryInit(NULL)) != flxInitSuccess)
		{
			printf("\t TSRecovery (PID-%d): Activation library initialization failed: status %d\n", TSRecoveryPID, initErr);
			return;
		}
			
		bIsServer = (flxActCommonGetProtectionMode() == FLX_ACT_SVR)?
										FLX_ACT_TRUE : FLX_ACT_FALSE;	
										
		/* Do automatic recovery only if server Trusted storage is in broken state*/
		if ( bIsServer == FLX_ACT_TRUE )
		{			
			if (sRepairServTS(g_pszCommServer) == FLX_ACT_TRUE)
			{
				printf("\n\t TSRecovery (PID-%d): %d, Fulfilment records get repaired! \n",TSRecoveryPID, repairCnt);
				
				if(repairCnt)
					printf("\t TSRecovery (PID-%d): Changes will be reflected in Vendor daemon in next TS update poll interval. \n\n", TSRecoveryPID);
			}
			else
				printf("\n\t TSRecovery (PID-%d): ERROR: Automatic recovery of Trusted Storage completed with error\n",TSRecoveryPID);
		}
		else
			printf("\n\t TSRecovery (PID-%d): ERROR: It is not a Server Trusted Storage, hence exit from repair! \n\n",TSRecoveryPID);
	}
	else
	{
		printf("\n\t TSRecovery (PID-%d): WARNING: Back Office Address(FNO) can not be empty to start automatic recovery! \n",TSRecoveryPID);
	}

	printf("\t TSRecovery (PID-%d): Exit from child process. \n\n",TSRecoveryPID);	
}

/****************************************************************************/
/**	@brief	sRepairServTS repair the Trusted Storage and reset the job.
 *	@param	g_pszCommServer, Back Office Address(FNO)
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool sRepairServTS(char * g_pszCommServer)
{
	FlxActHandle	    handle = 0;
	FlxActMode		    mode = FLX_ACT_SVR;
	FlxActBool		    bRV = FLX_ACT_FALSE;
	FlxActBool          tranCreate = FLX_ACT_FALSE;
	FlxActTransaction   transaction;
	FlxActTranResult    tranResult;
	FlxActError		    error;	
	
	memset(&error, 0, sizeof(FlxActError));
	
		/* Create handle and process. */
		if (flxActCommonHandleOpen(&handle, mode, &error))
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
				printf("\t TSRecovery (PID-%d): Failed to create transaction object \n", TSRecoveryPID);
				flxActCommonHandleClose(handle);
				return FLX_ACT_FALSE;
			}
			
			if (!setCommDetails(transaction, g_pszCommServer))
				return FLX_ACT_FALSE;
						
			bRV = sRepairFRsFromBackOff(handle, transaction, g_pszCommServer);  
			
			/* Clean up */
			flxActTransactionDestroy(transaction);
			flxActCommonHandleClose(handle);	
		}
		else
			printf("\t TSRecovery (PID-%d): ERROR: flxActCommonHandleOpen - (%d,%d,%d)\n", TSRecoveryPID, error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
		
		flxActCommonLibraryCleanup();

	return bRV;
}

/****************************************************************************/
/**	@brief	sRepairFRsFromBackOff, perform a transaction to repair broken FRs from Back Office
 *	@param	handle	FlxActHandle used to access trusted storage
 *  @param  transaction FlxActTransaction used to creat and process an Action
 *	@param	g_pszCommServer, Back Office Address(FNO)
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool sRepairFRsFromBackOff(FlxActHandle  handle, FlxActTransaction transaction, char * g_pszCommServer)
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
		printf("\n\t TSRecovery (PID-%d): Response processed successfully. Actions were:\n", TSRecoveryPID);
		
		FlxActTranResponse	tranResponse;
		if ( flxActTransactionGetResponse(transaction, &tranResponse) )
		{
			uint32_t actionCount = 0;
			uint32_t actionIndex;
			uint32_t deniedCount=0;
			
			if (flxActTranResponseGetActionCount(tranResponse, &actionCount) == FLX_ACT_TRUE && actionCount > 0)
			{
				for (actionIndex = 0; actionIndex < actionCount; ++actionIndex)
				{
					FlxActTranRspAction         rspAction = 0;
					const char* 				pszFulfillmentId;
					FlxActTranRspActionType     rspActionType;

					if ( flxActTranResponseGetAction(tranResponse, actionIndex, &rspAction) )
					{										
						if ( flxActTranRspActionGetType(rspAction, &rspActionType) )
						{
							if ( rspActionType == FLX_ACT_TRAN_RSP_ACTION_ACTIVATE_DENY )
							{
								const char* pszReason;
								FlxActTranRspActionResult	actionResult;
								
								if ( !flxActTranRspActionGetAttribute(rspAction, FLX_ACT_TRAN_RSP_ACT_REASON, &pszReason) )
								{
									printf("\t TSRecovery (PID-%d): ERROR: UNEXPECTED API ERROR flxActTranRspActionGetAttribute()\n", TSRecoveryPID);
									printf("\t TSRecovery (PID-%d): ERROR: Not able to fetch Reason for Repair Denied\n",TSRecoveryPID );
									continue;
								}
								printf("\n\t TSRecovery (PID-%d): Repair Denied %d\n"
												 "\t\t Reason:  %s\n",
												 TSRecoveryPID,
												 deniedCount++,
												 pszReason);
								continue;
								
							}
							
							if ( !flxActTranRspActionGetAttribute(rspAction, FLX_ACT_TRAN_RSP_ACT_FULFILLMENT_ID, &pszFulfillmentId) )
							{
								printf("\t TSRecovery (PID-%d): ERROR: UNEXPECTED API ERROR flxActTranRspActionGetAttribute()\n", TSRecoveryPID);
								printf("\t TSRecovery (PID-%d): ERROR: Not able to fetch FulfilmentID for Repair\n",TSRecoveryPID );
								continue;
							}
							
							if ( rspActionType == FLX_ACT_TRAN_RSP_ACTION_REPAIR )
							{
								printf("\n\t TSRecovery (PID-%d): Repair Success %d\n"
											"\t\t FulfillmentId:  %s\n", TSRecoveryPID, ++repairCnt, pszFulfillmentId);
							}
								
							if ( rspActionType == FLX_ACT_TRAN_RSP_ACTION_REPAIR_DENY )
							{								
								const char* pszReason;
								FlxActTranRspActionResult	actionResult;
								
								if ( !flxActTranRspActionGetAttribute(rspAction, FLX_ACT_TRAN_RSP_ACT_REASON, &pszReason) )
								{
									printf("\t TSRecovery (PID-%d): ERROR: UNEXPECTED API ERROR flxActTranRspActionGetAttribute()\n",TSRecoveryPID);
									printf("\t TSRecovery (PID-%d): ERROR: Not able to fetch Reason for Repair Denied\n",TSRecoveryPID );
									continue;
								}
								printf("\n\t TSRecovery (PID-%d): Repair Denied %d\n"
												 "\t\t FulfillmentId:  %s\n"
												 "\t\t Reason:  %s\n",
												 TSRecoveryPID,
												 ++deniedCount,
												 pszFulfillmentId,
												 pszReason);
							}
						}
					}
				}
			}
		}
		
		if( !flxActTransactionDeleteStoredRequest(transaction) )
			printf("\t TSRecovery (PID-%d): Unable to delete request\n", TSRecoveryPID);
		
		return FLX_ACT_TRUE;		
	}
	
	else
	{
		printf("\n\t TSRecovery (PID-%d): ERROR: Automatic repair failed \n", TSRecoveryPID);  //To test
		printf("\t TSRecovery (PID-%d): Reason:  ", TSRecoveryPID);	
		FlxActTranResult resCode = flxActTransactionGetResult();

		if (callbackData.bDidReceiveResponse)
		{	
			printf("\t TSRecovery (PID-%d): Response received but error when processing it. \n", TSRecoveryPID);
		}
		
		if (!callbackData.bDidConnect)
		{
			printf("\t TSRecovery (PID-%d): No response from Back Office(FNO) %s\n", TSRecoveryPID, g_pszCommServer);
			if( !flxActTransactionDeleteStoredRequest(transaction) )
				printf("\t TSRecovery (PID-%d): Unable to delete request\n", TSRecoveryPID );
		}
		else
		{			
			if (resCode == FLX_ACT_TRAN_ERR_COMMS_INIT)
			{
				printf("\t TSRecovery (PID-%d): Unable to initialise comms (fnpCommsSoap shared object missing?)\n", TSRecoveryPID);
			}
		}
		return FLX_ACT_FALSE;
	}

}

/****************************************************************************/
/**	@brief	setCommDetails, set comm type and comm server
 *	@param	transaction	 FlxActTransaction used to set comm type and comm server
 *	@param	g_pszCommServer, Back Office Address(FNO)
 *	@return	FLX_ACT_TRUE on success, else FLX_ACT_FALSE
 ****************************************************************************/
static FlxActBool setCommDetails(FlxActTransaction transaction, char * g_pszCommServer)
{
	const char*	 pszCommsType="SOAP";
	
	/* Set the server URI. */
	if ( flxActTransactionSetCommsAttribute(transaction, FLX_ACT_TRAN_SVR_URI, g_pszCommServer) )  
	{
		printf("\n\t TSRecovery (PID-%d): Back Office Address:  %s\n", TSRecoveryPID, g_pszCommServer);
	
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
				printf("\t TSRecovery (PID-%d): ERROR: flxActCommonLicSpcGet - Invalid index\n", TSRecoveryPID);
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
	 	printf("\t TSRecovery (PID-%d): WARNING: No fulfillments activated from FNO need repairing...\n",TSRecoveryPID);
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

	printf("\t TSRecovery (PID-%d): Status: %d, %s\n", 
	                TSRecoveryPID,
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
