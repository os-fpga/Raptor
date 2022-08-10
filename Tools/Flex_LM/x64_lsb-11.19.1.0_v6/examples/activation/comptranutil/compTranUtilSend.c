/**************************************************************************************************
* Copyright (c) 1997-2016, 2018-2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
	This sample utility demonstrates Composite Transactions.

	If defines the function used for online transactions.

		comptranutil -transaction ... 

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FlxActCommon.h"
#include "FlxActError.h"

#include "FlxActTransaction.h"

#include "compTranUtilContext.h"
#include "compTranUtilSend.h"
#include "compTranUtilCreateRequest.h"
#include "compTranUtilManageRequests.h"
#include "compTranUtilResponse.h"

static uint32_t sCommStatusCallback(const void* pUserData, uint32_t statusOld, uint32_t statusNew);

typedef struct CommsCallbackData_struct
{
	FlxActBool			bDidConnect;
	FlxActBool			bDidSendRequest;
	FlxActBool			bDidReceiveResponse;
} CommsCallbackData;

static FlxActBool sSetComms(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
static void		  sSetCommsProxy(FlxCtuContext* pContext, const char* pszProxyDetails);
static void		  sSetCommsSsl(FlxCtuContext* pContext, const char* pszSslDetails);
static FlxActBool sCheckRequestVersion(FlxCtuContext* pContext, FlxActTranRequest hTranRequest);

#define DEFAULT_RETRY_COUNT	3	/* If the response from the server is not received. */

/***************************************************************************************************
*	flxCtuDoCommandTransaction
*
* Perform a transaction with  Publisher Server (send request, receive and process response).
***************************************************************************************************/
FlxActBool flxCtuDoCommandTransaction(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxActTranRequest	 hTranRequest = 0;
	CommsCallbackData	 callbackData;
	FlxActBool			 bDidConnect;
	FlxActBool			 executeReinstallOnce = FLX_ACT_TRUE;
	size_t				 retryCount = DEFAULT_RETRY_COUNT;
	size_t				 tryNumber;
	FlxCtuCommand*       pCommandNew = NULL;
	const FlxCtuCommand* pCommandStored = NULL;

	/* Set the server comms attributes from the command. */
	if ( !sSetComms(pContext, pCommand) )
		return FLX_ACT_FALSE;

	/* Set the retry count from the command attribute, if provided. */
	if ( flxCtuCommandHasAttribute(pCommand, CMD_ATTR_RETRIES))
		retryCount = (size_t)atoi( flxCtuCommandGetAttribute(pCommand, CMD_ATTR_RETRIES) );

	/* Get a request, either new or existing. */
	if (flxCtuContextHasCommand(pContext, CMD_NEW))
	{
		/* A "new" command is present, get it. */

		if ( !flxCtuContextGetCommandByName(pContext, CMD_NEW, (const FlxCtuCommand**)&pCommandNew))
			FLX_CTU_ASSERT(FLX_ACT_FALSE);	/* Know command exists so should not happen. */

		/* Create and save a new request, or match a stored one that has identical attributes. */
		if ( !flxCtuRequestCreate(pContext, pCommandNew, &hTranRequest, FLX_ACT_FALSE) )
			return FLX_ACT_FALSE;
	}
	else if (flxCtuContextHasCommand(pContext, CMD_STORED))
	{	
		/* Use stored request. */

		if ( !flxCtuContextGetCommandByName(pContext, CMD_STORED, &pCommandStored) )
			FLX_CTU_ASSERT(FLX_ACT_FALSE);	/* Know command exists so should not happen. */

		if ( !flxCtuLoadStoredRequest(pContext, pCommandStored) )
			return FLX_ACT_FALSE;

		if ( !flxActTransactionGetRequest(pContext->hTransaction, &hTranRequest))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
	}
	else
	{	/* Neither stored nor new specified, use this as the main command for creating the request. */
		if ( !flxCtuRequestCreate(pContext, pCommand, &hTranRequest, FLX_ACT_FALSE) )
			return FLX_ACT_FALSE;
	}

	/* Check the request version is appropriate for the comms type */
	if (!sCheckRequestVersion(pContext, hTranRequest))
		return FLX_ACT_FALSE;

	/* Initialise the data that will be set by the comms status callback function. */
	callbackData.bDidConnect		 = FLX_ACT_FALSE;
	callbackData.bDidSendRequest	 = FLX_ACT_FALSE;
	callbackData.bDidReceiveResponse = FLX_ACT_FALSE;

	/* Verbose display of request to be sent */
	{
		const char* requestXML;
		if( flxActTranRequestGetXML(hTranRequest, &requestXML ) )
		{
			flxCtuPrintVerbose("\nRequest XML:\n%s\n\n", requestXML);
		}
		else
		{
			flxCtuPrintVerbose("Could not display current request XML\n");
		}
	}

	/* Send the request to the server and process the response it sends back. */
	for (tryNumber = 0, bDidConnect = FLX_ACT_FALSE; tryNumber <= retryCount; tryNumber++)
	{
		if (tryNumber > 0)
		{
			if ( !flxCtuContextConfirm(pContext, "Retry (%d of %d)?\n", tryNumber, retryCount))
				return FLX_ACT_FALSE;
		}

		if (flxActTransactionSend(pContext->hTransaction, sCommStatusCallback, &callbackData))
		{
			flxCtuTimePrintInterval("process response"); /* Interval since status flxCommsStatusDone notified */
			flxCtuResponseOnProcessSuccess(pContext);

			/* Request no longer needed so delete it. */
			if ( !flxActTransactionDeleteStoredRequest(pContext->hTransaction) )
			{
				flxCtuPrintError("Unable to delete request\n");
				flxCtuContextPrintLastApiError(pContext);
			}
			break;	/* No retries needed */
		}
		else
		{
			FlxActTranResult	resCode = flxActTransactionGetResult();
			
			if (callbackData.bDidReceiveResponse)
			{	/* Response received but error when processing it. */
				flxCtuResponseOnProcessError(pContext);
			}
			else 
			{	/* Comms unsuccessful. */
				flxCtuContextPrintLastApiError(pContext);

				/* Note whether ANY attempt connected to the server successfully. */
				bDidConnect |= callbackData.bDidConnect;
			}

			/* Send the 'reinstall' request during the retries, only if it is a network error, request from 'Servercomptranutil' and it  is an activation request */
			
			if (((resCode == FLX_ACT_TRAN_ERR_COMMS_SEND) && (pContext->bIsServer)) && ((tryNumber < retryCount) && (executeReinstallOnce)))
			{
                uint32_t			actionCount;
				uint32_t			actionIndex;
				FlxActBool          reinstallFlag;

				if (!flxActTranRequestGetActionCount(hTranRequest, &actionCount))
					FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

				for (actionIndex = 0; actionIndex < actionCount; ++actionIndex)
				{
					FlxActTranReqAction		hAction;
					FlxActTranReqActionType actionType;

					if (!flxActTranRequestGetAction(hTranRequest, actionIndex, &hAction))
						FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

					if (!flxActTranReqActionGetType(hAction, &actionType))
						FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
					
					/* Check if it is activation request */

					if (actionType == FLX_ACT_TRAN_REQ_ACTION_ACTIVATE)
					{
						reinstallFlag = FLX_ACT_TRUE;
					}
					else
					{
						reinstallFlag = FLX_ACT_FALSE;
						break;
					}
				}

				if (reinstallFlag)
				{
					if ( !pContext->bUsingStoredRequest)
					{
						if (!flxActTransactionDeleteStoredRequest(pContext->hTransaction))
						{
							flxCtuPrintError("Unable to delete request\n");
							flxCtuContextPrintLastApiError(pContext);
						}
					}

					if (pCommandNew)
					{
						if (!flxCtuRequestCreate(pContext, pCommandNew, &hTranRequest, reinstallFlag))
							return FLX_ACT_FALSE;
					}
					else
					{
						if ((!pCommandStored) && (!flxCtuRequestCreate(pContext, pCommand, &hTranRequest, reinstallFlag)))
							return FLX_ACT_FALSE;
					}

					if (!pCommandStored)
					{	
						if (!sCheckRequestVersion(pContext, hTranRequest))
							return FLX_ACT_FALSE;

						callbackData.bDidConnect = FLX_ACT_FALSE;
						callbackData.bDidSendRequest = FLX_ACT_FALSE;
						callbackData.bDidReceiveResponse = FLX_ACT_FALSE;
					}
				}
												
				executeReinstallOnce = FLX_ACT_FALSE;
			}
		}
	}

	if (tryNumber > retryCount)
	{
		/* All retries failed */
		FlxActTranResult result = flxActTransactionGetResult();
		const char* serverUri;
		if ( !flxActTransactionGetCommsAttribute(pContext->hTransaction, 
													FLX_ACT_TRAN_SVR_URI, 
													&serverUri))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		if (bDidConnect)
		{	/* At least one attempt connected to the server so we don't know whether the server processed 
				the request or not so leave it in trusted storage for re-sending. */
			flxCtuPrintError("No response from server %s\n", serverUri);
		}
		else
		{
			/* Unable to connect - the server can't have received the request it so it can be
				canceled, re-enabling any FRs that were being returned. If/when the user retries,
				a new request will be created. 
				We only do this if the request was created for this transaction - if an existing
				stored request was used we leave it for re-use when the server is available. */
			if ( !pContext->bUsingStoredRequest)
			{
				if ( !flxActTransactionDeleteStoredRequest(pContext->hTransaction) )
				{
					flxCtuPrintError("Unable to delete request\n");
					flxCtuContextPrintLastApiError(pContext);
				}
			}
				
			if (result == FLX_ACT_TRAN_ERR_COMMS_INIT)
			{
				flxCtuPrintError("Unable to initialise comms (fnpCommsSoap shared object missing?)\n");
			}
			else
			{
				flxCtuPrintError("Unable to connect to server %s\n", serverUri);
			}
		}
		pContext->exitCode = flxCtuExitCommsError;
		return FLX_ACT_FALSE;
	}
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

	flxCtuPrintMessage("Status: %d, %s\n", 
					   statusNew, 
					   (statusNew < sizeof(COMMS_STATUS_STRINGS) / sizeof(COMMS_STATUS_STRINGS[0]))? 
							COMMS_STATUS_STRINGS[statusNew] : "Unknown");

	if (statusNew == flxCommsStatusRequestCreated)
	{
		flxCtuTimeResetInterval();
	}
	else if (statusNew == flxCommsStatusConnected)
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
		flxCtuTimePrintInterval("server");
	}

	return flxCallbackReturnContinue;
}

/***************************************************************************************************
 *	sSetComms
 *
 * Set the Publisher Server communications attributes from the command.
 ***************************************************************************************************/
static FlxActBool sSetComms(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	const char*	pszCommsType;
	const char* pszServerUri = flxCtuCommandGetValue(pCommand); /* Mandatory. */
	/* Set the server URI. */
	if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction, 
											 FLX_ACT_TRAN_SVR_URI, 
											 pszServerUri) )
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}
	flxCtuPrintVerbose("Server URI:     %s\n", pszServerUri);

	/* Set the comms type. */
	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_COMMS_TYPE))
	{
		pszCommsType = flxCtuCommandAttributeIs(pCommand, CMD_ATTR_COMMS_TYPE, ATTR_VALUE_FLEX)?
						FLX_ACT_TRAN_SVR_COMMS_TYPE_FLEX : FLX_ACT_TRAN_SVR_COMMS_TYPE_SOAP;
	}
	else
	{	/* No comms type - assume FLEX if URI is port@name format */
		pszCommsType = (NULL != strchr(pszServerUri, '@'))?
						FLX_ACT_TRAN_SVR_COMMS_TYPE_FLEX : FLX_ACT_TRAN_SVR_COMMS_TYPE_SOAP;
	}
	if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction, 
											 FLX_ACT_TRAN_SVR_COMMS_TYPE, 
											 pszCommsType) )
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}
	flxCtuPrintVerbose("Comms Type:     %s\n", pszCommsType);

	/* Set any proxy attributes. */
	if ( flxCtuCommandHasAttribute(pCommand, CMD_ATTR_PROXY) )
		sSetCommsProxy(pContext, flxCtuCommandGetAttribute(pCommand, CMD_ATTR_PROXY));
	
	/* Set any SSL attributes. */
	if ( flxCtuCommandHasAttribute(pCommand, CMD_ATTR_SSL) )
		sSetCommsSsl(pContext, flxCtuCommandGetAttribute(pCommand, CMD_ATTR_SSL));
	
	/* Set any timeout. */
	if ( flxCtuCommandHasAttribute(pCommand, CMD_ATTR_TIMEOUT) )
	{
		const char* pszTimeout = flxCtuCommandGetAttribute(pCommand, CMD_ATTR_TIMEOUT);
		/* Set the attribute; this call should not fail given valid parameters. */
		if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction,
												 FLX_ACT_TRAN_SVR_TIMEOUT,
												 pszTimeout) )
 			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		flxCtuPrintVerbose("Timeout:        %s\n", pszTimeout);
	}

	/* Set any poll interval. */
	if ( flxCtuCommandHasAttribute(pCommand, CMD_ATTR_POLL) )
	{
		const char* pszPollInterval = flxCtuCommandGetAttribute(pCommand, CMD_ATTR_POLL);
		/* Set the attribute; this call should not fail given valid parameters. */
		if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction,
												 FLX_ACT_TRAN_SVR_POLL_INTERVAL,
												 pszPollInterval) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		flxCtuPrintVerbose("Poll interval:  %s\n", pszPollInterval);
	}

	flxCtuPrintMessage("Server %s, comms type %s\n", pszServerUri, pszCommsType);

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
 *	sSetCommsProxy
 *
 * Set the server communications proxy attributes.  The details are in the form
 * "host port userId password".
 ***************************************************************************************************/
static void sSetCommsProxy(FlxCtuContext* pContext, const char* pszProxyDetails)
{
	/* Parse the details and duplicate each space-delimited item (empty string if missing). */
	const char* pszHost		= flxCtuStrDupDelim(&pszProxyDetails, ' ');
	const char* pszPort		= flxCtuStrDupDelim(&pszProxyDetails, ' ');
	const char* pszUserId	= flxCtuStrDupDelim(&pszProxyDetails, ' ');
	const char* pszPassword = flxCtuStrDupDelim(&pszProxyDetails, ' ');

	/* Set the attributes; these calls should not fail given valid parameters. */
	if (strlen(pszHost) > 0)
		if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction,
												 FLX_ACT_TRAN_SVR_PROXY_HOST,
												 pszHost))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (strlen(pszPort) > 0)
		if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction,
												 FLX_ACT_TRAN_SVR_PROXY_PORT,
												 pszPort))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (strlen(pszUserId) > 0)
		if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction,
												 FLX_ACT_TRAN_SVR_PROXY_USER,
												 pszUserId))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (strlen(pszPassword) > 0)
		if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction,
												 FLX_ACT_TRAN_SVR_PROXY_PASSWORD,
												 pszPassword))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	flxCtuPrintVerbose("Proxy settings: host=%s port=%s username=%s password=%s\n",
					   pszHost,
					   pszPort,
					   pszUserId,
					   pszPassword);

	/* Free the duplicated strings. */
	flxCtuFree(pszHost);
	flxCtuFree(pszPort);
	flxCtuFree(pszUserId);
	flxCtuFree(pszPassword);
}

/***************************************************************************************************
 *	sSetCommsSsl
 *
 * Set the server communications SSL attributes.  The details are in the form
 * "CaCert CaPath".
 ***************************************************************************************************/
static void sSetCommsSsl(FlxCtuContext* pContext, const char* pszSslDetails)
{
	/* Parse the details and duplicate each space-delimited item (empty string if missing). */
	const char* pszCaCert	= flxCtuStrDupDelim(&pszSslDetails, ' ');
	const char* pszCaPath	= flxCtuStrDupDelim(&pszSslDetails, ' ');

	/* Set the attributes; these calls should not fail given valid parameters. */
	if (strlen(pszCaCert) > 0)
		if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction,
														   FLX_ACT_TRAN_SVR_SSL_CACERT,
														   pszCaCert))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (strlen(pszCaPath) > 0)
		if ( !flxActTransactionSetCommsAttribute(pContext->hTransaction,
												 FLX_ACT_TRAN_SVR_SSL_CAPATH,
												 pszCaPath))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	flxCtuPrintVerbose("SSL settings:   CaCert=%s CaPath=%s\n",
					   pszCaCert,
					   pszCaPath);

	/* Free the duplicated strings. */
	flxCtuFree(pszCaCert);
	flxCtuFree(pszCaPath);
}

/***************************************************************************************************
 *	sCheckRequestVersion
 *
 * Depending on the attributes specified, the request XML will either be version 6 or 7.
 * At the time of release FNO did not support version 7 but the ELS does.
 * So, if comms type is SOAP and request is version 7, check with user that it's OK to send - it 
 * may be that a new version of FNO has come along or they are using a different SOAP server.
 *
 * Also request confirmation if an ELS-only attribute is being sent to a SOAP server in a v6 request.
 ***************************************************************************************************/
static FlxActBool sCheckRequestVersion(FlxCtuContext* pContext, FlxActTranRequest hTranRequest)
{
	/* Determine the comms type */
	const char* pszCommsType;
	if ( !flxActTransactionGetCommsAttribute(pContext->hTransaction, 
											 FLX_ACT_TRAN_SVR_COMMS_TYPE, 
											 &pszCommsType) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (0 == strcmp(pszCommsType, FLX_ACT_TRAN_SVR_COMMS_TYPE_SOAP))
	{
		/* It's a SOAP transaction - check the request version */
		const char* pszRequestXml;
		if ( !flxActTranRequestGetXML(hTranRequest, &pszRequestXml) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		if (strstr(pszRequestXml, "<VersionNumber>7<"))
		{
			/* It's version 7 - request confirmation */
			if ( !flxCtuContextConfirm(pContext,
						 "\nThe attributes specified require a version 7 request.\n"
						 "The Enterprise License Server supports this but the publisher server\n"
						 "specified for this transaction may not. Confirm send?\n"))
			{
				pContext->exitCode = flxCtuExitSuccess; /* User has quit, don't report any error. */
				return FLX_ACT_FALSE;
			}
		}

		/* Version 6 requests can still contain ELS-only attributes, e.g. Duration */
		if (pContext->bIsPresentElsAttribute)
		{
			if ( !flxCtuContextConfirm(pContext,
						 "\nThe request contains attributes that are supported by the\n"
						 "Enterprise License Server but which may not be by the\n"
						 "publisher server specified for this transaction. Confirm send?\n"))
			{
				pContext->exitCode = flxCtuExitSuccess; /* User has quit, don't report any error. */
				return FLX_ACT_FALSE;
			}
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
 *	sWait
 *
 * Platform independent, sleep would be better.
 ***************************************************************************************************/
static void	  sWait(size_t seconds)
{
	float interval = (float)seconds;
	flxCtuTimeResetInterval();
	while (interval > flxCtuTimeGetInterval()) {}
}
