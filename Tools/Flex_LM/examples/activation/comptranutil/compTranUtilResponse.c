/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This source file is part of a sample utility that demonstrates Composite Transactions.

	If defines the functions for response processing.
*/
#include "compTranUtilContext.h"
#include "compTranUtilResponse.h"
#include "compTranUtilFulfillments.h"
#include "lm_redir_std.h"
static const char* sGetActionNameFromType(FlxActTranRspActionType rspActionType);
static const char* sGetTextForActionResult(FlxActTranRspActionResult actionResult);
static void		   sProcessDenyAction(FlxCtuContext* pContext, FlxActTranRspAction hRspAction);

static void sPrintDictionary(FlxCtuContext*			pContext, 
							 FlxActTranDictionary	hDictionary, 
							 const char*			pszDictionaryName);

static void sProcessResponseActions(FlxCtuContext*		pContext, 
								  FlxActTranResponse	hTranResponse, 
								  uint32_t*				pDenyCount);

static void sPrintResponseAction(FlxCtuContext* pContext, FlxActTranRspAction hRspAction);
static void sPrintResponseActionConfig(FlxCtuContext* pContext, FlxActTranRspAction hRspAction);
static void sPrintResponseActionDeny(FlxCtuContext* pContext, FlxActTranRspAction hRspAction);
static void sPrintResponseActionDictionaries(FlxCtuContext* pContext, FlxActTranRspAction hRspAction);
static void sPrintResponseActionWithFid(FlxCtuContext* pContext, FlxActTranRspAction hRspAction);
static void sPrintResponseDictionaries(FlxCtuContext* pContext, FlxActTranRspAction hTranResponse);
static void sPrintLastResponseXml(FlxCtuContext* pContext);

/***************************************************************************************************
*	flxCtuDoCommandProcess
*
* Process a composite response.  The command value is the file containing the XML response.
***************************************************************************************************/
FlxActBool flxCtuDoCommandProcess(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	const char*			pszResponseFilename;
	char*				pszResponseXml;

	/* Get the command object, which should exist. */
	FLX_CTU_ASSERT( flxCtuContextGetCommandByName(pContext, CMD_PROCESS, &pCommand) );

	/* There should be a value, the response file name - TODO handle stdin.	*/
	if ( !flxCtuCommandHasValue(pCommand) )
	{
		pszResponseFilename = "<STDIN>";

		/* Read in the response. */
		if ( !flxCtuContextStreamLoad(pContext, lm_flex_stdin(), pszResponseFilename, &pszResponseXml, NULL) )
			return FLX_ACT_FALSE;
		
	}
	else
	{
		pszResponseFilename = flxCtuCommandGetValue(pCommand);

		/* Read in the response. */
		if ( !flxCtuContextFileLoad(pContext, pszResponseFilename, &pszResponseXml, NULL) )
			return FLX_ACT_FALSE;
	}

	/* Process the response. */
	flxCtuTimeResetInterval();
	if (flxActTransactionProcessResponse(pContext->hTransaction, pszResponseXml))
	{
		flxCtuTimePrintInterval("flxActTransactionProcessResponse - success");
		flxCtuResponseOnProcessSuccess(pContext);

		/* Finished with request and response so delete them from storage. */
		if ( !flxActTransactionDeleteStoredRequest(pContext->hTransaction) )
		{
			flxCtuPrintError("Unable to delete request.\n");
			flxCtuContextPrintLastApiError(pContext);
			flxCtuFree(pszResponseXml);
			return FLX_ACT_FALSE;
		}
	}
	else
	{
		flxCtuTimePrintInterval("flxActTransactionProcessResponse - error");
		flxCtuResponseOnProcessError(pContext);
		flxCtuFree(pszResponseXml);
		return FLX_ACT_FALSE;
	}

	flxCtuFree(pszResponseXml);
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		flxCtuResponseOnProcessError
*
* Called on unsuccessful response processing (online or offline).
***************************************************************************************************/
void flxCtuResponseOnProcessError(FlxCtuContext* pContext)
{
	FlxActTranResult	 result  = flxActTransactionGetResult();
	const char*			 infoStr = NULL;

	/* Print the standard API error message. */
	flxCtuContextPrintLastApiError(pContext);

	/* There is a response so print it if required */
	sPrintLastResponseXml(pContext);

	/* Print an information string for some common results. */
	switch (result)
	{
	case FLX_ACT_TRAN_ERR_RSP_XML_HEADER:		/* Drop though. */
	case FLX_ACT_TRAN_ERR_RSP_XML_MISSING:		/* Drop though. */
	case FLX_ACT_TRAN_ERR_RSP_NOT_COMPOSITE:	/* Drop though. */
	case FLX_ACT_TRAN_ERR_RSP_XML_VERSION:		/* Drop though. */
	case FLX_ACT_TRAN_ERR_RSP_SIGNATURE_1:	
	case FLX_ACT_TRAN_ERR_RSP_SIG_VERSION:  
		infoStr = "The response XML supplied is not a valid composite response.";
		break;
	case FLX_ACT_TRAN_ERR_RSP_NO_REQUEST:		
		infoStr = "The most likely reason is that this response has already been processed.";
		break;
	case FLX_ACT_TRAN_ERR_RSP_REQUEST_NOT_PENDING:
		infoStr = "The response is for a request that is no longer usable, \n"
				  "probably because the response has already been processed.\n"
				  "You can check the status of stored requests with the -listRequest command.";
		break;
	case FLX_ACT_TRAN_ERR_RSP_REQUEST_UNTRUSTED:
		infoStr = "The response is for a request that is no longer trusted.\n"
				  "You should re-send the request to the server using the -stored command.";
		break;
	case FLX_ACT_TRAN_ERR_RSP_NOT_REQUESTER:
		infoStr = "The response is not valid for this machine.";
		break;
	case FLX_ACT_TRAN_ERR_RSP_ACTION_FAILED:
		infoStr = "Response valid but some actions could not be performed.";
		break;
	default:
		break;
	}
	flxCtuPrintMessage("\nResponse processing failed %u:\n", result);
	if (infoStr != NULL)
		flxCtuPrintMessage("%s\n", infoStr);

	if (result == FLX_ACT_TRAN_ERR_RSP_ACTION_FAILED)
	{	
		/* The response was valid but some of the actions could not be performed.
		   Get the response object and print the result of each individual action. */
		FlxActTranResponse	hTranResponse;
		if ( !flxActTransactionGetResponse(pContext->hTransaction, &hTranResponse))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		sProcessResponseActions(pContext, hTranResponse, NULL);

		/* Print any response dictionaries */
		sPrintResponseDictionaries(pContext, hTranResponse);
	}
	pContext->exitCode = flxCtuExitResponseProcessingError;
}

/***************************************************************************************************
*		flxCtuResponseOnProcessSuccess
*
* Called on successful response processing (online or offline).
*
* Extracts and prints information from the response object created and attached to the transaction
* object during response processing.
*
***************************************************************************************************/
void flxCtuResponseOnProcessSuccess(FlxCtuContext* pContext)
{
	FlxActTranResponse	hTranResponse;
	uint32_t			denyCount = 0;

	sPrintLastResponseXml(pContext);

	flxCtuPrintMessage("Response processed successfully. Actions were:\n");
	/* Get the response object (there should be one). */
	if ( !flxActTransactionGetResponse(pContext->hTransaction, &hTranResponse)  )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	/* Enumerate the response actions and report on each one. */
	sProcessResponseActions(pContext, hTranResponse, &denyCount);

	/* Print any global response dictionary entries. */
	flxCtuPrintMessage("\n");
	sPrintResponseDictionaries(pContext, hTranResponse);

	/* Warn if any actions were denied. */
	if (denyCount > 0)
	{
		flxCtuPrintMessage("\nWARNING: %u request actions were denied by the server.\n", 
						   denyCount);
	}
}

/***************************************************************************************************
*		flxCtuResponsePrint
*
***************************************************************************************************/
void flxCtuResponsePrint(FlxCtuContext* pContext, FlxActTranResponse hTranResponse)
{
	uint32_t denyCount;

	/* Enumerate the response actions and report on each one. */
	sProcessResponseActions(pContext, hTranResponse, &denyCount);

	/* Print any global response dictionary entries. */
	sPrintResponseDictionaries(pContext, hTranResponse);
}

/***************************************************************************************************
*		sGetActionNameFromType
*
***************************************************************************************************/
static const char* sGetActionNameFromType(FlxActTranRspActionType rspActionType)
{
	switch (rspActionType)
	{
	case FLX_ACT_TRAN_RSP_ACTION_CONFIG:		return "Configuration";
	case FLX_ACT_TRAN_RSP_ACTION_CREATE:		return "Create";
	case FLX_ACT_TRAN_RSP_ACTION_DELETE:		return "Delete";
	case FLX_ACT_TRAN_RSP_ACTION_MODIFY:		return "Modify";
	case FLX_ACT_TRAN_RSP_ACTION_REPAIR:		return "Repair";
	case FLX_ACT_TRAN_RSP_ACTION_ACTIVATE_DENY:	return "Activate Deny";
	case FLX_ACT_TRAN_RSP_ACTION_RETURN_DENY:	return "Return Deny";
	case FLX_ACT_TRAN_RSP_ACTION_REPAIR_DENY:	return "Repair Deny";
	default:									return "<Unknown>";
	}
}

/***************************************************************************************************
*		sGetTextForActionResult
*
***************************************************************************************************/
static const char* sGetTextForActionResult(FlxActTranRspActionResult actionResult)
{
	switch (actionResult)
	{
	case FLX_ACT_TRAN_RSP_ACT_SUCCESS:		return "Success"; 
	case FLX_ACT_TRAN_RSP_ACT_WARN_DENY:	return "Success but note this is a deny action";
	case FLX_ACT_TRAN_RSP_ACT_ERR_NA:		return "Not attempted because of another error";
	case FLX_ACT_TRAN_RSP_ACT_ERR_SKIP:		return "Valid but not performed because of another error";
	case FLX_ACT_TRAN_RSP_ACT_WARN_FR_OVERWRITE:
											return "WARNING: Fulfillment record overwritten";
	case FLX_ACT_TRAN_RSP_ACT_ERR_NO_FR:	return "ERROR: Fulfillment record does not exist";
	case FLX_ACT_TRAN_RSP_ACT_ERR_CONFIG:	return "FAILED: Unable to import trusted config";
	case FLX_ACT_TRAN_RSP_ACT_ERR_3STS:	    return "FAILED: Fulfillment invalid for 3-Server TS";
	case FLX_ACT_TRAN_RSP_ACT_WARN_NO_FR:	return "WARNING: Fulfillment record does not exist";
	default:								return "Unknown action result";
	}
}

/***************************************************************************************************
*		sPrintDictionary
*
* Print the name and entries of a dictionary.
***************************************************************************************************/
static void sPrintDictionary(FlxCtuContext* pContext, 
					  FlxActTranDictionary hDictionary, 
					  const char* pszDictionaryName)
{
	uint32_t entryCount;
	uint32_t entryIndex;

	if ( !flxActTranDictionaryGetCount(hDictionary, &entryCount))
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (entryCount > 0)
	{
		flxCtuPrintMessage("    %s dictionary:\n", pszDictionaryName);

		for (entryIndex = 0; entryIndex < entryCount; entryIndex++)
		{
			const char* pszKey;
			const char* pszValue;

			if ( !flxActTranDictionaryGetByIndex(hDictionary, entryIndex, &pszKey, &pszValue))
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

			flxCtuPrintMessage("      %20s=%s\n", pszKey, pszValue);

		}
	}
}

/***************************************************************************************************
*		sPrintResponseAction
*
* Print a response action by type.
***************************************************************************************************/
static void sPrintResponseAction(FlxCtuContext* pContext, FlxActTranRspAction hRspAction)
{
	FlxActTranRspActionType		rspActionType;
	FlxActTranRspActionResult	actionResult;

	if ( !flxActTranRspActionGetType(hRspAction, &rspActionType) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	switch (rspActionType)
	{
	case FLX_ACT_TRAN_RSP_ACTION_CONFIG:		sPrintResponseActionConfig(pContext, hRspAction);
												break;
	case FLX_ACT_TRAN_RSP_ACTION_CREATE:	
	case FLX_ACT_TRAN_RSP_ACTION_DELETE:	
	case FLX_ACT_TRAN_RSP_ACTION_MODIFY:	
	case FLX_ACT_TRAN_RSP_ACTION_REPAIR:		sPrintResponseActionWithFid(pContext, hRspAction);
												break;
	case FLX_ACT_TRAN_RSP_ACTION_ACTIVATE_DENY:	
	case FLX_ACT_TRAN_RSP_ACTION_RETURN_DENY:
	case FLX_ACT_TRAN_RSP_ACTION_REPAIR_DENY:	sPrintResponseActionDeny(pContext, hRspAction);
												break;
	default:									flxCtuPrintError("Unknown action in response.\n");
												break;
	}

	/* If action failed, print result. */
	if ( !flxActTranRspActionGetResult(hRspAction, &actionResult))
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (actionResult != FLX_ACT_TRAN_RSP_ACT_SUCCESS)
		flxCtuPrintMessage("                   %s.\n", sGetTextForActionResult(actionResult));
}

/***************************************************************************************************
*		sPrintResponseActionConfig
*
* Print details of config response action.
***************************************************************************************************/
static void sPrintResponseActionConfig(FlxCtuContext* pContext, FlxActTranRspAction hRspAction)
{
	const char *pszTrustedId;

	if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_TRUSTED_ID, &pszTrustedId) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	flxCtuPrintMessage("    %-14s TrustedId \"%s\"\n", 
					   sGetActionNameFromType(FLX_ACT_TRAN_RSP_ACTION_CONFIG),
					   pszTrustedId);

	sPrintResponseActionDictionaries(pContext, hRspAction);
}

/***************************************************************************************************
*		sPrintResponseActionDeny
*
* Print details of a deny action.
* All have Reason and Comment attributes, Activate Deny has Rights Id and Type, others 
* Fulfillment Id.
***************************************************************************************************/
static void sPrintResponseActionDeny(FlxCtuContext* pContext, FlxActTranRspAction hRspAction)
{
	const char* pszReason;
	const char* pszComment;
	FlxActTranRspActionType rspActionType;


	if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_REASON, &pszReason) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_COMMENT, &pszComment) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if ( !flxActTranRspActionGetType(hRspAction, &rspActionType) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (rspActionType == FLX_ACT_TRAN_RSP_ACTION_ACTIVATE_DENY)
	{
		const char *pszRightsIdValue;
		const char *pszRightsIdType;

		if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_RIGHTS_ID_VALUE, &pszRightsIdValue) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_RIGHTS_ID_TYPE, &pszRightsIdType) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		flxCtuPrintMessage("   %s\n"
						   "            RightsId:  %s\n"
						   "        RightsIdType:  %s\n"
						   "              Reason:  %s\n"
						   "             Comment:  %s\n",
						   sGetActionNameFromType(rspActionType),
						   pszRightsIdValue,
						   pszRightsIdType,
						   pszReason,
						   pszComment);
	}
	else
	{
		const char *pszFulfillmentId;

		if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_FULFILLMENT_ID, &pszFulfillmentId) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		flxCtuPrintMessage("   %s\n"
						   "       FulfillmentId:  %s\n"
						   "              Reason:  %s\n"
						   "             Comment:  %s\n",
						   sGetActionNameFromType(rspActionType),
						   pszFulfillmentId,
						   pszReason,
						   pszComment);
	}
	sPrintResponseActionDictionaries(pContext, hRspAction);
}

/***************************************************************************************************
*		sPrintResponseActionDictionaries
*
* Print the FLEXnet and Vendor dictionaries of the response action.
***************************************************************************************************/
static void sPrintResponseActionDictionaries(FlxCtuContext* pContext, FlxActTranRspAction hRspAction)
{
	FlxActTranDictionary hDictionary;

	if ( !flxActTranRspActionGetFLEXnetDictionary(hRspAction, &hDictionary))
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
	sPrintDictionary(pContext, hDictionary, "FLEXnet response action");

	if ( !flxActTranRspActionGetVendorDictionary(hRspAction, &hDictionary))
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
	sPrintDictionary(pContext, hDictionary, "Vendor response action");
}

/***************************************************************************************************
*		sProcessResponseActions
*
* Print all the response actions and returns the number of deny actions.
***************************************************************************************************/
static void sProcessResponseActions(FlxCtuContext*		pContext, 
						   FlxActTranResponse	hTranResponse, 
						   uint32_t*			pDenyCount)
{
	uint32_t			actionCount;
	uint32_t			actionIndex;
	uint32_t			denyCount = 0;

	/* Enumerate the response actions and report on each one. */
	if ( !flxActTranResponseGetActionCount(hTranResponse, &actionCount) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	for (actionIndex = 0; actionIndex < actionCount; actionIndex++)
	{
		FlxActTranRspAction			hRspAction;
		FlxActTranRspActionResult	actionResult;

		if ( !flxActTranResponseGetAction(hTranResponse, actionIndex, &hRspAction) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		sPrintResponseAction(pContext, hRspAction);

		if ( !flxActTranRspActionGetResult(hRspAction, &actionResult) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		if (actionResult == FLX_ACT_TRAN_RSP_ACT_WARN_DENY)
		{
			denyCount++;
			sProcessDenyAction(pContext, hRspAction);
		}
	}
	if (pDenyCount != NULL)
		*pDenyCount = denyCount;
}

/***************************************************************************************************
*		sProcessDenyAction
*
* If the deny is for a return and the server reason indicates that the FR has already been returned,
* delete the FR. 
* This is a workaround needed until the server is changed to send a delete rather than a deny.
***************************************************************************************************/
static void	sProcessDenyAction(FlxCtuContext* pContext, FlxActTranRspAction hRspAction)
{
	FlxActTranRspActionType	actionType;
	const char*				pszDenyReason;

	if ( !flxActTranRspActionGetType(hRspAction, &actionType))
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (actionType == FLX_ACT_TRAN_RSP_ACTION_RETURN_DENY)
	{
		if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_REASON, &pszDenyReason))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		/* The FNO deny reason for "already returned" is "7284" ... */
		if (NULL != strstr(pszDenyReason, "7284;"))
		{
			const char* pszFulfillmentId;

			if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_FULFILLMENT_ID, &pszFulfillmentId))
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

			if (flxCtuFulfillmentDelete(pContext, pszFulfillmentId))
			{
				flxCtuPrintMessage("              Action:  fulfillment deleted as it has already been returned\n"); 
			}
			else
			{
				flxCtuContextPrintLastApiFlexError(pContext);
				flxCtuPrintMessage("Unable to delete fullfillment %s\n", pszFulfillmentId);
			}
		}
	}
}

/***************************************************************************************************
*		sPrintResponseActionWithFid
*
* Print details of action that has a fulfillment id attribute.
***************************************************************************************************/
static void sPrintResponseActionWithFid(FlxCtuContext* pContext, FlxActTranRspAction hRspAction)
{
	const char*				pszFulfillmentId;
	FlxActTranRspActionType rspActionType;
	const char*				pszActionName;

	if ( !flxActTranRspActionGetType(hRspAction, &rspActionType) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	pszActionName = sGetActionNameFromType(rspActionType);

	if ( !flxActTranRspActionGetAttribute(hRspAction, FLX_ACT_TRAN_RSP_ACT_FULFILLMENT_ID, &pszFulfillmentId) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	flxCtuPrintMessage("    %-14s fulfillment \"%s\"\n", pszActionName, pszFulfillmentId);

	sPrintResponseActionDictionaries(pContext, hRspAction);
}

/***************************************************************************************************
*		sPrintResponseDictionaries
*
* Print the FLEXnet and Vendor dictionaries of the response.
***************************************************************************************************/
static void sPrintResponseDictionaries(FlxCtuContext* pContext, FlxActTranResponse hTranResponse)
{
	FlxActTranDictionary hDictionary;

	if ( !flxActTranResponseGetFLEXnetDictionary(hTranResponse, &hDictionary))
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
	sPrintDictionary(pContext, hDictionary, "FLEXnet response");

	if ( !flxActTranResponseGetVendorDictionary(hTranResponse, &hDictionary))
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
	sPrintDictionary(pContext, hDictionary, "Vendor response");
}

/***************************************************************************************************
* sPrintLastResponseXml
*
* Print the last response XML that was processed using this transaction handle.
***************************************************************************************************/
static void sPrintLastResponseXml(FlxCtuContext* pContext)
{	
	if (pContext && pContext->bprintResponseXml)
	{
		const char*	pszResponseXml = NULL;

		if (flxActTransactionGetResponseXML(pContext->hTransaction, &pszResponseXml))
			flxCtuPrintMessage("\nResponse XML: \n%s\n\n", pszResponseXml ? pszResponseXml : "");
	}

	return;
}
