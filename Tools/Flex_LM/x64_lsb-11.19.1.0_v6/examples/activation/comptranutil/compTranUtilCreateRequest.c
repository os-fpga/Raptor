/**************************************************************************************************
* Copyright (c) 1997-2016, 2018-2020 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This source file is part of a sample utility that demonstrates Composite Transactions.

	It declares the function that implements the -new command and supporting functions used
	by that and other commands for request creation and output. 
*/

#include "compTranUtilCreateRequest.h"
#include "compTranUtilManageRequests.h"
#include "compTranUtilCommandDef.h"
#include "compTranUtilFulfillments.h"
#include "compTranUtilServer.h"

static FlxActBool sAddActionDictionaries(FlxCtuContext*			pContext, 
										 const FlxCtuCommand*	pCommand, 
										 FlxActTranReqAction	hTranReqAction);

static FlxActBool sAddActivate(FlxCtuContext*		pContext, 
							   const FlxCtuCommand*	pCommand, 
							   FlxActTranRequest	hTranRequest,
							   FlxActBool pReinstallFlag);

static void sAddDictionary(FlxCtuContext*			pContext, 
						   const FlxCtuDictionary*	pDictionary,
						   FlxActTranDictionary		hDictionary);

static FlxActBool sAddGlobalDictionaries(FlxCtuContext*			pContext, 
										 const FlxCtuCommand*	pCommand, 
										 FlxActTranRequest		hTranRequest);

static FlxActBool sAddRepair(FlxCtuContext*			pContext, 
							 const FlxCtuCommand*	pCommand, 
							 FlxActTranRequest		hTranRequest);

static FlxActBool sAddRepairAll(FlxCtuContext*			pContext, 
								const FlxCtuCommand*	pCommand, 
								FlxActTranRequest		hTranRequest);

static FlxActBool sAddReturn(FlxCtuContext*			pContext, 
							 const FlxCtuCommand*	pCommand, 
							 FlxActTranRequest		hTranRequest);

static FlxActBool sSetFulfillmentInfo(FlxCtuContext*		pContext, 
									  const FlxCtuCommand*	pCommand,
									  FlxActTranRequest		hTranRequest,
									  FlxActTranReqAction	hAction,
									  const char*			pszFulfillmentId);

static FlxActBool sSetOptionalAttribute(FlxCtuContext*					pContext, 
										const FlxCtuCommand*			pCommand, 
										const char*						pszAttributeName,
										FlxActTranReqAction				hAction,
										FlxActTranReqActAttributeType	actionAttribute);

static FlxActBool sSetOptionalServerAttribute(FlxCtuContext*				pContext, 
											  const FlxCtuCommand*			pCommand, 
											  const char*					pszAttributeName,
											  FlxActTranReqAction			hAction,
											  FlxActTranReqActAttributeType	actionAttribute);

static FlxActBool sSetOptionalServerCounts(FlxCtuContext*		pContext, 
										   const FlxCtuCommand*	pCommand, 
										   FlxActTranReqAction	hAction);

static void sReportRequestActionError(FlxCtuContext* pContext, FlxActTranRequest hTranRequest);

static FlxActBool sConfirmIfPartialReturn(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

/***************************************************************************************************
*	flxCtuDoCommandNew
*
* Create a new request.
* The command value is the file to write the request XML (default stdout).
* Other commands specify the request actions, see: -help new
***************************************************************************************************/
FlxActBool flxCtuDoCommandNew(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxActTranRequest	hTranRequest;

	/* 
		Create a request object using the transaction object and command options held in the 
		context.  The request object is a subordinate of the transaction object so gets deleted
		with the transaction object / context.
	*/
	if ( !flxCtuRequestCreate(pContext, pCommand, &hTranRequest, FLX_ACT_FALSE) )
	{
		return FLX_ACT_FALSE;
	}

	/* Write the request XML to file / stdout. */
	if ( !flxCtuRequestOutputXml(pContext, pCommand) )
		return FLX_ACT_FALSE;

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		flxCtuRequestCreate
*
* The pContext contains
*		The composite transaction object handle.
*		A set of commands specifying the request actions required and their attributes.
*
* The pMainCommand command contains the attributes that apply to the whole request (reference, 
* dictionaries).
*
* This function creates a composite request object and one or more subordinate request action
* objects.  It returns a handle to the request object as a convenience, but as that's a subordinate
* of the transaction object it can always be obtained with flxActTransactionGetRequest(). For the
* same reason the request and action objects don't have to be explicitly cleaned up - they are
* destroyed with the transaction object.
*
***************************************************************************************************/
FlxActBool flxCtuRequestCreate(FlxCtuContext*		pContext, 
							   const FlxCtuCommand*	pMainCommand, 
							   FlxActTranRequest*	phTranRequest,
							   FlxActBool pReinstallFlag)
{
	FlxActBool				bIsSuccess;
	const char*				pszUserReference;
	FlxActTranRequest		hTranRequest;
	size_t					commandCount;
	size_t					commandIndex;
	const FlxCtuCommand*	pCommand;	

	flxCtuPrintVerbose("New request:\n");

	/* Default the exit code to flxCtuExitInputError; other exit codes will be set explicitly, 
	   for example by flxCtuContextPrintLastApiError. */
	pContext->exitCode = flxCtuExitInputError;

	/* Get any request reference from the main command. */
	if ( flxCtuCommandHasAttribute(pMainCommand, CMD_ATTR_REFERENCE) )
		pszUserReference = flxCtuCommandGetAttribute(pMainCommand, CMD_ATTR_REFERENCE);
	else
		pszUserReference = "";

	/* Create the request object. */
	if (!flxActTransactionCreateRequest(pContext->hTransaction, pszUserReference, &hTranRequest))
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}

	/* Adjust the existing fulfillment information if required. By default summary (short) info for
	   all fulfillments are included; this can be adjusted to "none" or "long".
	   This choice can be further refined when the actions are added later. */
	if ( flxCtuCommandHasAttribute(pMainCommand, CMD_ATTR_FR) )
	{
		if (flxCtuCommandAttributeIs(pMainCommand, CMD_ATTR_FR, "long"))
		{
			if ( !flxActTranRequestSetExistingFulfillmentDetails(hTranRequest, FLX_ACT_TRUE))
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
		}
		else if (flxCtuCommandAttributeIs(pMainCommand, CMD_ATTR_FR, "none"))
		{
			if ( !flxActTranRequestClearExistingFulfillments(hTranRequest))
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
		}
	}

	/* Iterate over the request action commands and add each one to the request object. 
	   Errors in all actions are reported; failure if there is an error in any. */
	commandCount = flxCtuContextGetCommandCount(pContext);
	for (commandIndex = 0, bIsSuccess = FLX_ACT_TRUE; commandIndex < commandCount; commandIndex++)
	{
		pCommand = flxCtuContextGetCommand(pContext, commandIndex);

		/* Actions have the trait flxCtuCommandTraitIsRequestAction). */
		if ( flxCtuCommandHasTrait(pCommand, flxCtuCommandTraitIsRequestAction) )
		{
			/* Process action by type. */
			const char*	pszCommandName = flxCtuCommandGetName(pCommand);

			if ( strcmp(pszCommandName, CMD_ACTIVATE) == 0 ) 
				bIsSuccess &= sAddActivate(pContext, pCommand, hTranRequest, pReinstallFlag);

			else if ( strcmp(pszCommandName, CMD_RETURN) == 0 ) 
				bIsSuccess &= sAddReturn(pContext, pCommand, hTranRequest);

			else if ( strcmp(pszCommandName, CMD_REPAIR) == 0 ) 
				bIsSuccess &= sAddRepair(pContext, pCommand, hTranRequest);

			else if ( strcmp(pszCommandName, CMD_REPAIR_ALL) == 0 ) 
				bIsSuccess &= sAddRepairAll(pContext, pCommand, hTranRequest);
		}
	}
	if ( !bIsSuccess)
		return FLX_ACT_FALSE;

	/* Add any FLEXnet or Vendor dictionary entries. */
	if ( !sAddGlobalDictionaries(pContext, pMainCommand, hTranRequest))
		return FLX_ACT_FALSE;

	/* If this is for server TS, call the server function for any modifications or additions. */
	if (pContext->bIsServer)
		flxCtuServerOnNewRequest(pContext, hTranRequest);

	/* Check that there is no stored request with identical content; if there is then assume 
	   that the user is just regenerating that request and use the original request, otherwise
	   save this one.  
	   If the user actually needs to generate a new request with identical actions (for example
	   first request activates 5 licenses, second activates another 5) they can prevent a match
	   by using a different requester reference. */
	if ( !flxCtuRequestMatchOrSave(pContext, &hTranRequest))
		return FLX_ACT_FALSE;

	if (pContext->bIsVerbose)
	{
		const char* pszSequenceNo;
		if ( !flxActTranRequestGetAttribute(hTranRequest, FLX_ACT_TRAN_REQ_SEQUENCE_NO, &pszSequenceNo) )
			FLX_CTU_ASSERT(FLX_ACT_FALSE);
		flxCtuPrintVerbose("  Request %s saved.\n", pszSequenceNo);
	}

	/* Return the request object. */
	*phTranRequest = hTranRequest;

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuRequestMatchOrSave
*
* Check whether the request matches an existing stored request, if so use that one.
* Otherwise save the current request.
* *phTranRequest is the current request, updated if a match is found.
***************************************************************************************************/
FlxActBool flxCtuRequestMatchOrSave(FlxCtuContext* pContext, FlxActTranRequest* phTranRequest)
{
	FlxActTranRequest hTranRequestMatch;
	
	/* Reset the interval timer */
	(void)flxCtuTimeGetInterval();

	if ( flxActTranRequestMatchStored(*phTranRequest, &hTranRequestMatch))
	{
		/* There is a matching request - get its sequence number. */
		const char* pszSequenceNo;
		if ( !flxActTranRequestGetAttribute(hTranRequestMatch, 
											FLX_ACT_TRAN_REQ_SEQUENCE_NO,
											&pszSequenceNo))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		/* Make the matched request the current request. 
		   Note that this also updates the caller's request handle,
		*/
		if ( !flxCtuGetRequestBySequenceNo(pContext, pszSequenceNo, phTranRequest) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		flxCtuPrintMessage("Re-using identical stored request, sequence number %s\n", 
						   pszSequenceNo);

		pContext->bUsingStoredRequest = FLX_ACT_TRUE;
	}
	else
	{	/* No match - save the new request in Trusted Storage. */
		if ( !flxActTranRequestSave(*phTranRequest) )
		{
			/* This failure may be user error (for example attempting to return a disabled FR)
			   or a more serious error from the file system. */
			FlxActTranResult result = flxActTransactionGetResult();
			switch (result)
			{
			case FLX_ACT_TRAN_ERR_FR_DISABLED:		/* Drop through. */
			case FLX_ACT_TRAN_ERR_FR_TRUSTED:		/* Drop through. */
			case FLX_ACT_TRAN_ERR_FR_UNTRUSTED: 
				sReportRequestActionError(pContext, *phTranRequest);
				break;
			default:
				flxCtuContextPrintLastApiError(pContext);
				flxCtuPrintError("Unable to save request to trusted storage.\n");
				break;
			}
			return FLX_ACT_FALSE;
		}
	}

	/* Print the time taken */
	flxCtuTimePrintInterval("create and save request");

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuRequestOutputXml
*
* Get the request XML from the current request object and write it to file / stdout.
***************************************************************************************************/
FlxActBool flxCtuRequestOutputXml(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxActBool			bSuccess;
	FlxActTranRequest   hTranRequest;
	const char*			pszRequestXml;
	const char*			pszRequestFilename;

	/* Get the current request object. */
	if ( !flxActTransactionGetRequest(pContext->hTransaction, &hTranRequest) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	/* Get the request XML. */
	if ( !flxActTranRequestGetXML(hTranRequest, &pszRequestXml) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	/* Write the XML to the output file if one was specified as the value for the command. */
	if ( flxCtuCommandHasValue(pCommand) )
	{
		pszRequestFilename = flxCtuCommandGetValue(pCommand);

		bSuccess = flxCtuContextFileDump(pContext, 
										 pszRequestFilename, 
										 pszRequestXml, 
										 strlen(pszRequestXml));
		if (bSuccess)
			flxCtuPrintMessage("Request written to %s\n", pszRequestFilename);
		else
			flxCtuPrintError("Unable to write request to %s\n", pszRequestFilename);
	}
	else
	{	/* Print the request XML to stdout. */
		flxCtuPrintOutput(pszRequestXml);
		bSuccess = FLX_ACT_TRUE;
	}
	return bSuccess;
}

/***************************************************************************************************
*	sAddActivate
*
* Add an activate action to the request.  The command value is the RightsId, other attributes
* and dictionaries are taken from the command object.
***************************************************************************************************/
static FlxActBool sAddActivate(FlxCtuContext*		pContext, 
							   const FlxCtuCommand*	pCommand, 
							   FlxActTranRequest	hTranRequest,
							   FlxActBool pReinstallFlag)
{
	FlxActTranReqAction	hAction;
	const char*			pszRightsId = NULL;

	flxCtuPrintVerbose("  Request action: Activate\n");
	/* 
		Create a new request action for the request object. 
	*/
	if ( !flxActTranRequestAddAction(hTranRequest, FLX_ACT_TRAN_REQ_ACTION_ACTIVATE, &hAction) )
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}

	/* 
		The command value is the "RightsId value" attribute - it can be omitted if Product Id is present
	*/
	if ( !flxCtuCommandHasValue(pCommand) )
	{
		if ( !flxCtuCommandHasAttribute(pCommand, CMD_ATTR_PID))
		{
			flxCtuCommandPrintError(pCommand, "does not have a RightsId nor a ProductId value.\n");
			return FLX_ACT_FALSE;
		}
	}
	else
	{
		/* 
			Set the RightsId value attribute from the command value. 
		*/
		pszRightsId = flxCtuCommandGetValue(pCommand);
		if ( !flxActTranReqActionSetAttribute(hAction, 
											  FLX_ACT_TRAN_REQ_ACT_RIGHTS_ID_VALUE, 
											  pszRightsId) )
		{
			flxCtuContextPrintLastApiError(pContext);
			return FLX_ACT_FALSE;
		}
		flxCtuPrintVerbose("    %20s=%s\n", "RightsId", pszRightsId);
	}

	/* Handle the Product Id optional attribute. */
	if ( !sSetOptionalAttribute(pContext,
								pCommand, 
								CMD_ATTR_PID, 
								hAction, 
								FLX_ACT_TRAN_REQ_ACT_PRODUCT_ID))
		return FLX_ACT_FALSE;

	/* 
		If the RightsId is present as EntitlementId in an existing fulfillment record, 
		warn the user. 
	*/
	if ( !flxCtuWarnIfEntitlementAlreadyUsed(pContext, pszRightsId))
	{
		/* User chose not to continue so quit but don't report an error. */
		pContext->exitCode = flxCtuExitSuccess; 
		return FLX_ACT_FALSE;	
	}

	/* 
		Set FulfillmentId if present.  This is an existing fulfillment that the requester
		wishes to identify to the server (e.g. for upgrade). 
	*/
	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_FID))
	{
		const char* pszFulfillmentId = flxCtuCommandGetAttribute(pCommand, CMD_ATTR_FID);
		if (!sSetFulfillmentInfo(pContext, pCommand, hTranRequest, hAction, pszFulfillmentId) )
			return FLX_ACT_FALSE;
	}

	/* Handle the RightsId Type optional attribute. */
	if ( !sSetOptionalAttribute(pContext,
								pCommand, 
								CMD_ATTR_RID_TYPE, 
								hAction, 
								FLX_ACT_TRAN_REQ_ACT_RIGHTS_ID_TYPE))
		return FLX_ACT_FALSE;

	/* Handle the Count optional attribute. */
	if ( !sSetOptionalAttribute(pContext,
								pCommand,
								CMD_ATTR_COUNT, 
								hAction, 
								FLX_ACT_TRAN_REQ_ACT_COUNT))
		return FLX_ACT_FALSE;

	/* Handle the Duration optional attribute (ELS only). */
	if ( flxCtuCommandHasAttribute(pCommand, CMD_ATTR_DURATION_DAYS) )
	{
		/* Set the expiration type to specify duration. 
   		   The type must be set first to enable the correct validation for the value. */
		if ( !flxActTranReqActionSetAttribute(hAction, 
											  FLX_ACT_TRAN_REQ_ACT_EXPIRATION_TYPE, 
											  FLX_ACT_TRAN_ELS_EXPIRATION_TYPE_DAYS) )
		{
			flxCtuContextPrintLastApiError(pContext);
			return FLX_ACT_FALSE;
		}
		/* Set the expiration value to the number of days. */
		if ( !sSetOptionalAttribute(pContext,
									pCommand,
									CMD_ATTR_DURATION_DAYS, 
									hAction, 
									FLX_ACT_TRAN_REQ_ACT_EXPIRATION_VALUE))
			return FLX_ACT_FALSE;

		/* Note that an ELS-only attribute has been set */
		pContext->bIsPresentElsAttribute = FLX_ACT_TRUE;
	}
	else
	{
		/* Handle the Expiration Type optional attribute. 
		   Note that command validation prevents both duration and expiration being specified. */
		if ( !sSetOptionalAttribute(pContext,
									pCommand,
									CMD_ATTR_EXP_TYPE, 
									hAction, 
									FLX_ACT_TRAN_REQ_ACT_EXPIRATION_TYPE))
			return FLX_ACT_FALSE;

		/* Handle the Expiration Value optional attribute. */
		if ( !sSetOptionalAttribute(pContext,
									pCommand,
									CMD_ATTR_EXP_VALUE, 
									hAction, 
									FLX_ACT_TRAN_REQ_ACT_EXPIRATION_VALUE))
			return FLX_ACT_FALSE;
	}

	/* Set the reason if 'reinstall' flag is set */
	if (pReinstallFlag)
	{
		const char* reasonValue = "1";
		if ( !flxActTranReqActionSetAttribute(hAction,  FLX_ACT_TRAN_REQ_ACT_REASON, reasonValue) )
		{
			flxCtuContextPrintLastApiError(pContext);
			return FLX_ACT_FALSE;
		} 
	}
	
	/* Handle the Reason optional attribute. */
	if ( !sSetOptionalAttribute(pContext,
								pCommand,
								CMD_ATTR_REASON, 
								hAction, 
								FLX_ACT_TRAN_REQ_ACT_REASON))
		return FLX_ACT_FALSE;

	/* Handle the server count attributes. */
	if ( !sSetOptionalServerCounts(pContext, pCommand, hAction))
		return FLX_ACT_FALSE;

	/* Add any FLEXnet or Vendor dictionary entries. */
	if ( !sAddActionDictionaries(pContext, pCommand, hAction))
		return FLX_ACT_FALSE;

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		sAddActionDictionaries
*
*	Add any FLEXnet and Vendor dictionary entries to the action.
***************************************************************************************************/
static FlxActBool sAddActionDictionaries(FlxCtuContext*			pContext, 
									     const FlxCtuCommand*	pCommand, 
									     FlxActTranReqAction	hTranReqAction)
{
	size_t					dictionaryIndex;
	const FlxCtuDictionary*	pDictionary;
	FlxActTranDictionary	hDictionary;

	for (dictionaryIndex = 0; dictionaryIndex < pCommand->dictionaryCount; dictionaryIndex++)
	{
		pDictionary = pCommand->ppDictionaries[dictionaryIndex];
		if (0 == flxCtuStrCmpLwr(pDictionary->pszName, CMD_DICT_FLEX))
		{
			if ( !flxActTranReqActionGetFLEXnetDictionary(hTranReqAction, &hDictionary) )
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

			flxCtuPrintVerbose("    FLEXnet dictionary:\n");
			sAddDictionary(pContext, pCommand->ppDictionaries[dictionaryIndex], hDictionary);
		}
		else if (0 == flxCtuStrCmpLwr(pDictionary->pszName, CMD_DICT_VENDOR))
		{
			if ( !flxActTranReqActionGetVendorDictionary(hTranReqAction, &hDictionary) )
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

			flxCtuPrintVerbose("    Vendor dictionary:\n");
			sAddDictionary(pContext, pCommand->ppDictionaries[dictionaryIndex], hDictionary);
		}
		else
		{
			flxCtuPrintError("Command %s has invalid dictionary %s\n", 
							 pCommand->pszNameForMessages, 
							 pDictionary->pszName);
			return FLX_ACT_FALSE;
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		sAddDictionary
*
*	Copy the dictionary entries from the command to the API dictionary object.
***************************************************************************************************/
static void sAddDictionary(FlxCtuContext*			pContext, 
						   const FlxCtuDictionary*	pDictionary,
						   FlxActTranDictionary		hDictionary)
{
	size_t			entryIndex;
	FlxCtuKeyValue*	pEntry;

	flxCtuPrintVerbose("  Dictionary %s\n", pDictionary->pszName);

	for (entryIndex = 0; entryIndex < pDictionary->entryCount; entryIndex++)
	{
		pEntry = pDictionary->ppEntries[entryIndex];

		if ( !flxActTranDictionarySetByKey(hDictionary, pEntry->pszKey, pEntry->pszValue) )
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		flxCtuPrintVerbose("      %20s=%s\n", pEntry->pszKey, pEntry->pszValue);
	}
}

/***************************************************************************************************
*		sAddGlobalDictionaries
*
* Add any global FLEXnet and Vendor dictionary entries to the request.  Global dictionaries
* contain entries that apply to the request as a whole, as opposed to an individual action.
***************************************************************************************************/
static FlxActBool sAddGlobalDictionaries(FlxCtuContext*			pContext, 
										 const FlxCtuCommand*	pCommand, 
										 FlxActTranRequest		hTranRequest)
{
	size_t					dictionaryIndex;
	const FlxCtuDictionary*	pDictionary;
	FlxActTranDictionary	hDictionary;

	for (dictionaryIndex = 0; dictionaryIndex < pCommand->dictionaryCount; dictionaryIndex++)
	{
		pDictionary = pCommand->ppDictionaries[dictionaryIndex];
		if (flxCtuStrCmpLwr(pDictionary->pszName, CMD_DICT_GLOBAL_FLEX) == 0)
		{
			if ( !flxActTranRequestGetFLEXnetDictionary(hTranRequest, &hDictionary) )
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

			flxCtuPrintVerbose("    gFLEXnet dictionary:\n");
			sAddDictionary(pContext, pCommand->ppDictionaries[dictionaryIndex], hDictionary);
		}
		else if (flxCtuStrCmpLwr(pDictionary->pszName, CMD_DICT_GLOBAL_VENDOR) == 0)
		{
			if ( !flxActTranRequestGetVendorDictionary(hTranRequest, &hDictionary) )
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

			flxCtuPrintVerbose("    gVendor dictionary:\n");
			sAddDictionary(pContext, pCommand->ppDictionaries[dictionaryIndex], hDictionary);
		}
		else
		{
			flxCtuPrintError("Command %s has invalid dictionary %s\n", 
				pCommand->pszNameForMessages, 
				pDictionary->pszName);
			return FLX_ACT_FALSE;
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sAddRepair
*
* Add a repair action to the request.  The command value is the FulfillmentId, any dictionaries are 
* taken from the command object.
***************************************************************************************************/
static FlxActBool sAddRepair(FlxCtuContext*			pContext, 
							 const FlxCtuCommand*	pCommand, 
							 FlxActTranRequest		hTranRequest)
{
	FlxActTranReqAction	hAction;

	flxCtuPrintVerbose("  Request action: Repair\n");

	/* Create a new repair action for the request object. */
	if ( !flxActTranRequestAddAction(hTranRequest, FLX_ACT_TRAN_REQ_ACTION_REPAIR, &hAction) )
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}
	/* The command value is the "FulfillmentId" attribute - make sure it's present.  */
	if ( !flxCtuCommandHasValue(pCommand) )
	{
		flxCtuCommandPrintError(pCommand, "does not have a FulfillmentId value.\n");
		return FLX_ACT_FALSE;
	}
	/* Set the FulfillmentId attribute from the command value. Will fail if the FR does not exist. */
	if ( !sSetFulfillmentInfo(pContext, pCommand, hTranRequest, hAction, flxCtuCommandGetValue(pCommand)) )
		return FLX_ACT_FALSE;

	/* Add any FLEXnet or Vendor dictionary entries. */
	if ( !sAddActionDictionaries(pContext, pCommand, hAction))
		return FLX_ACT_FALSE;

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sAddRepairAll
*
* Add a repair action for each fulfillment in trusted storage that is not fully trusted.
***************************************************************************************************/
static FlxActBool sAddRepairAll(FlxCtuContext*			pContext, 
								const FlxCtuCommand*	pCommand, 
								FlxActTranRequest		hTranRequest)
{
	uint32_t repairActionCount;

	flxCtuPrintVerbose("  Request action: Repair All\n");

	if ( !flxCtuFulfillmentsAddRepairActions(pContext, &repairActionCount))
		return FLX_ACT_FALSE;

	if (repairActionCount == 0)
	{
		flxCtuPrintError("no fulfillments need repairing.\n");
		return FLX_ACT_FALSE;
	}

	flxCtuPrintVerbose("                  %u repair actions added to request.\n",
					   repairActionCount);

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sAddReturn
*
* Add a return action to the request.  The command value is the FulfillmentId, other attributes
* and dictionaries are taken from the command object.
***************************************************************************************************/
static FlxActBool sAddReturn(FlxCtuContext*			pContext, 
							 const FlxCtuCommand*	pCommand, 
							 FlxActTranRequest		hTranRequest)
{
	FlxActTranReqAction	hAction;
	const char*			pszFulfillmentId;
	uint32_t			deductionCount;

	flxCtuPrintVerbose("  Request action: Return\n");

	/* Create a new return action for the request object. */
	if ( !flxActTranRequestAddAction(hTranRequest, FLX_ACT_TRAN_REQ_ACTION_RETURN, &hAction) )
	{
		flxCtuContextPrintLastApiError(pContext);
		return FLX_ACT_FALSE;
	}
	/* The command value is the "FulfillmentId" attribute - make sure it's present.  */
	if ( !flxCtuCommandHasValue(pCommand) )
	{
		flxCtuCommandPrintError(pCommand, "does not have a FulfillmentId value.\n");
		return FLX_ACT_FALSE;
	}
	pszFulfillmentId = flxCtuCommandGetValue(pCommand);

	/* Set the FulfillmentId attribute. Will fail if the FR does not exist. */
	if ( !sSetFulfillmentInfo(pContext, pCommand, hTranRequest, hAction, pszFulfillmentId) )
		return FLX_ACT_FALSE;

	/* Don't allow return if any "valid" deductions exist (excludes repairs and expired). */
	if ( !flxCtuFulfillmentGetValidDeductionCount(pContext, pszFulfillmentId, &deductionCount))
		/* Unexpected - we know the fulfillment exists */
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	if (deductionCount > 0)
	{
		flxCtuPrintError("Can't return fulfillment %s - it has %u valid deduction(s).\n",
						 pszFulfillmentId,
						 deductionCount);
		return FLX_ACT_FALSE;
	}

	/* Handle the optional attributes. */
	if ( !sSetOptionalAttribute(pContext,
								pCommand,
								CMD_ATTR_COUNT, 
								hAction, 
								FLX_ACT_TRAN_REQ_ACT_COUNT))
		return FLX_ACT_FALSE;

	if ( !sSetOptionalAttribute(pContext,
								pCommand,
								CMD_ATTR_REASON, 
								hAction, 
								FLX_ACT_TRAN_REQ_ACT_REASON))
		return FLX_ACT_FALSE;

	/* Handle the server count attributes. */
	if ( !sSetOptionalServerCounts(pContext, pCommand, hAction))
		return FLX_ACT_FALSE;

	/* Warn if it is a partial return and get confirmation. */
	if ( !sConfirmIfPartialReturn(pContext, pCommand))
		return FLX_ACT_FALSE;

	/* Add any FLEXnet or Vendor dictionary entries. */
	if ( !sAddActionDictionaries(pContext, pCommand, hAction))
		return FLX_ACT_FALSE;

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		sReportRequestActionError
*
* Identify the action that has caused the error print a message.
***************************************************************************************************/
static void sReportRequestActionError(FlxCtuContext* pContext, FlxActTranRequest hTranRequest)
{
	FlxActTranResult	actionResult = flxActTransactionGetResult();
	uint32_t			actionCount;
	uint32_t			actionIndex;

	flxCtuPrintError("Unable to create request because of an error in a request action.\n");

	if ( !flxActTranRequestGetActionCount(hTranRequest, &actionCount))
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	/* Iterate over the request actions. */
	for (actionIndex = 0; actionIndex < actionCount; ++actionIndex)
	{
		FlxActTranReqAction		hAction;
		FlxActTranReqActionType actionType;
		const char*				pszFulfillmentId;
		FlxActBool				bIsTrusted;
		FlxActBool				bIsEnabled;

		if ( !flxActTranRequestGetAction(hTranRequest, actionIndex, &hAction))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		if ( !flxActTranReqActionGetType(hAction, &actionType))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

		if ( flxActTranReqActionGetAttribute(hAction, 
											 FLX_ACT_TRAN_REQ_ACT_FULFILLMENT_ID, 
											 &pszFulfillmentId)
			&& flxCtuFulfillmentGetState(pContext, pszFulfillmentId, &bIsTrusted, &bIsEnabled))
		{
			/* Fulfillment exists - report the specific error. */
			if ((actionType == FLX_ACT_TRAN_REQ_ACTION_RETURN) && !bIsEnabled)
			{
				/*  return + FR disabled. */
				flxCtuPrintError(
					"Fulfillment \"%s\" is disabled (already being returned).\n",
					pszFulfillmentId);
			}
			else if ((actionType == FLX_ACT_TRAN_REQ_ACTION_RETURN) && !bIsTrusted)
			{
				/*  return + FR not trusted. */
				flxCtuPrintError(
					"Fulfillment \"%s\" it is not trusted (needs repair).\n",
					pszFulfillmentId);
			}
			else if ((actionType == FLX_ACT_TRAN_REQ_ACTION_REPAIR) && bIsTrusted)
			{
				/*  repair + FR trusted. */
				flxCtuPrintError(
					"Fulfillment \"%s\" is fully trusted (does not need repair).\n",
					pszFulfillmentId);
			}
			else if ((actionType == FLX_ACT_TRAN_REQ_ACTION_ACTIVATE) && !bIsEnabled)
			{
				/*  activate + FR disabled. */
				flxCtuPrintError(
					"Fulfillment \"%s\" is disabled (being returned).\n",
					pszFulfillmentId);
			}
			else if ((actionType == FLX_ACT_TRAN_REQ_ACTION_ACTIVATE) && !bIsTrusted)
			{
				/*  activate + FR not trusted. */
				flxCtuPrintError(
					"Fulfillment \"%s\" is not trusted (needs repair).\n",
					pszFulfillmentId);
			}
		} 
		else
		{
			/* Fulfillment does not exists. */
			flxCtuPrintError(
				"Fulfillment \"%s\" does not exists.\n",
				pszFulfillmentId);
		}
	}
}

/***************************************************************************************************
*		sSetFulfillmentInfo
*
* Set the FulfillmentId action attribute.  This will fail if an FR with the id does not exist or is
* in the wrong state for the action being performed (e.g. trying to return an untrusted FR).
***************************************************************************************************/
static FlxActBool sSetFulfillmentInfo(FlxCtuContext*		pContext, 
									  const FlxCtuCommand*	pCommand, 
									  FlxActTranRequest		hTranRequest,
									  FlxActTranReqAction	hAction, 
									  const char*			pszFulfillmentId)
{
	FlxActBool bFullDetails;

	if ( !flxActTranReqActionSetAttribute(hAction, 
										  FLX_ACT_TRAN_REQ_ACT_FULFILLMENT_ID, 
										  pszFulfillmentId) )
	{
		FlxActTranResult lastResult = flxActTransactionGetResult();
		const char *infoStr = NULL;

		switch (lastResult)
		{
		case FLX_ACT_TRAN_ERR_FR_NOT_FOUND:	infoStr = "does not exist";
											break;

		case FLX_ACT_TRAN_ERR_FR_DISABLED:	infoStr = "is disabled (already being returned)";
											break;

		case FLX_ACT_TRAN_ERR_FR_UNTRUSTED:	infoStr = "is not fully trusted (needs repair)";
											break;

		case FLX_ACT_TRAN_ERR_FR_TRUSTED:	infoStr = "does not need repair";
											break;

		default:	FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);	/* Does not return. */
		}

		if (flxCtuContextHasCommand(pContext, CMD_VERBOSE))
			flxCtuContextPrintLastApiError(pContext);

		flxCtuCommandPrintError(pCommand, "Fulfillment Id \"%s\" %s.\n", pszFulfillmentId, infoStr);
		return FLX_ACT_FALSE;
	}


	flxCtuPrintVerbose("    %20s=%s\n", "FulfillmentId", pszFulfillmentId);

	/* Adjust the existing fulfillment information from the default for the main command;
	   the CMD_ATTR_FR attribute can be used to specify "short" or "long". */
	if ( flxCtuCommandHasAttribute(pCommand, CMD_ATTR_FR) )
	{
		if (flxCtuCommandAttributeIs(pCommand, CMD_ATTR_FR, "long"))
			bFullDetails = FLX_ACT_TRUE;	 
		else
			bFullDetails = FLX_ACT_FALSE;	/* Not "long" so must be short (summary info). */

		if ( !flxActTranRequestAddExistingFulfillment(hTranRequest, pszFulfillmentId, bFullDetails))
			FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
	}

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		sSetOptionalAttribute
***************************************************************************************************/
static FlxActBool sSetOptionalAttribute(FlxCtuContext*					pContext, 
										const FlxCtuCommand*			pCommand, 
										const char*						pszAttributeName,
										FlxActTranReqAction				hAction,
										FlxActTranReqActAttributeType	actionAttribute)
{
	/* If the command has the optional attribute... */
	if (flxCtuCommandHasAttribute(pCommand, pszAttributeName))
	{
		/* Get the attribute value from the command. */
		const char* value = flxCtuCommandGetAttribute(pCommand, pszAttributeName);

		/* Set it in the request action object. */
		if ( !flxActTranReqActionSetAttribute(hAction, actionAttribute, value) )
		{
			flxCtuCommandPrintError(pCommand, 
									"Unable to set attribute \"%s\" to \"%s\"\n",
									pszAttributeName, 
									value);
			flxCtuContextPrintLastApiError(pContext);
			pContext->exitCode = flxCtuExitInputError;	/* The most likely cause */
			return FLX_ACT_FALSE;
		}
		/* If verbose, validate by get and check */
		if (pContext->bIsVerbose)
		{
			const char* gotValue;
			if ( !flxActTranReqActionGetAttribute(hAction, actionAttribute, &gotValue) )
			{
				flxCtuCommandPrintError(pCommand, 
										"Unable to get attribute \"%s\"\n",
										pszAttributeName);
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
			}
			if ( strcmp(value, gotValue) != 0 )
			{
				flxCtuCommandPrintError(pCommand, 
										"Attribute \"%s\" set as \"%s\" but got as \"%s\"\n",
										pszAttributeName, value, gotValue);
				FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
			}
		}
		flxCtuPrintVerbose("    %20s=%s\n", pszAttributeName, value);
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		sSetOptionalServerAttribute
***************************************************************************************************/
static FlxActBool sSetOptionalServerAttribute(FlxCtuContext*				pContext, 
											  const FlxCtuCommand*			pCommand, 
											  const char*					pszAttributeName,
											  FlxActTranReqAction			hAction,
											  FlxActTranReqActAttributeType	actionAttribute)
{
	if ( !flxCtuCommandHasAttribute(pCommand, pszAttributeName))
		return FLX_ACT_TRUE;	/* Attribute not present is OK. */

	if ( !pContext->bIsServer)
	{
		flxCtuCommandPrintError(pCommand, 
								"\"%s\" is only allowed in server mode.\n", 
								pszAttributeName);
		return FLX_ACT_FALSE;
	}
	return sSetOptionalAttribute(pContext, pCommand, pszAttributeName, hAction, actionAttribute);
}

/***************************************************************************************************
*		sSetOptionalServerCounts
*
* The Enterprise License Server defines counts of different types.
* They can be used instead of the more general CMD_ATTR_COUNT attribute.
***************************************************************************************************/
static FlxActBool sSetOptionalServerCounts(FlxCtuContext*		pContext, 
										  const FlxCtuCommand*	pCommand, 
										  FlxActTranReqAction	hAction)
{
	if (	 sSetOptionalServerAttribute(pContext, 
										 pCommand, 
										 CMD_ATTR_ACTIVATABLE, 
										 hAction, 
										 FLX_ACT_TRAN_REQ_ACT_COUNT_ACTIVATABLE)
		&&	 sSetOptionalServerAttribute(pContext, 
										 pCommand, 
										 CMD_ATTR_ACTIVATABLE_OD, 
										 hAction, 
										 FLX_ACT_TRAN_REQ_ACT_COUNT_ACTIVATABLE_OD)
		&&	 sSetOptionalServerAttribute(pContext, 
										 pCommand, 
										 CMD_ATTR_CONCURRENT, 
										 hAction, 
										 FLX_ACT_TRAN_REQ_ACT_COUNT_CONCURRENT)
		&&	 sSetOptionalServerAttribute(pContext, 
										 pCommand, 
										 CMD_ATTR_CONCURRENT_OD, 
										 hAction, 
										 FLX_ACT_TRAN_REQ_ACT_COUNT_CONCURRENT_OD)
		&&	 sSetOptionalServerAttribute(pContext, 
										 pCommand, 
										 CMD_ATTR_HYBRID, 
										 hAction, 
										 FLX_ACT_TRAN_REQ_ACT_COUNT_HYBRID)
		&&	 sSetOptionalServerAttribute(pContext, 
										 pCommand, 
										 CMD_ATTR_HYBRID_OD, 
										 hAction, 
										 FLX_ACT_TRAN_REQ_ACT_COUNT_HYBRID_OD)
		&&	 sSetOptionalServerAttribute(pContext, 
										 pCommand, 
										 CMD_ATTR_REPAIR_COUNT, 
										 hAction, 
										 FLX_ACT_TRAN_REQ_ACT_COUNT_REPAIR) )
	{
		return FLX_ACT_TRUE;
	}
	return FLX_ACT_FALSE;
}

/***************************************************************************************************
*		sConfirmIfPartialReturn
*
* If a count is specified for return this is  a "partial" return, i.e. part but not all of the 
* fulfillment is being returned.  Servers should implement this using a modify response but 
* some just treat it as a full return, deleting the fulfillment.
* So we warn the user this might happen and ask for confirmation.
***************************************************************************************************/
static FlxActBool sConfirmIfPartialReturn(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	/* If any count has been specified... */
	if (	flxCtuCommandHasAttribute(pCommand, CMD_ATTR_COUNT)
		 || flxCtuCommandHasAttribute(pCommand, CMD_ATTR_ACTIVATABLE)
		 || flxCtuCommandHasAttribute(pCommand, CMD_ATTR_ACTIVATABLE_OD)
		 || flxCtuCommandHasAttribute(pCommand, CMD_ATTR_CONCURRENT)
		 || flxCtuCommandHasAttribute(pCommand, CMD_ATTR_CONCURRENT_OD)
		 || flxCtuCommandHasAttribute(pCommand, CMD_ATTR_HYBRID)
		 || flxCtuCommandHasAttribute(pCommand, CMD_ATTR_HYBRID_OD)
		 || flxCtuCommandHasAttribute(pCommand, CMD_ATTR_REPAIR_COUNT))
	{
		if ( !flxCtuContextConfirm(pContext, 
			"\nThis may be a partial return (one or more counts specified).\n"
			"Servers that do not support partial returns may treat this as a\n"
			"full return and delete the fulfillment.\n"
			"Confirm partial return? "))
		{
			return FLX_ACT_FALSE;
		}
	}
	return FLX_ACT_TRUE;
}

