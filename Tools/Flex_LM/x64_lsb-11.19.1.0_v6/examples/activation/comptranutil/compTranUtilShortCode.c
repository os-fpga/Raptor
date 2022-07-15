/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This source file is part of a sample utility that demonstrates Composite Transactions and
	other functionality.  

	If declares the functions used for short code transactions.

	See function flxCtuHelpShortCode below for usage.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FlxActCommon.h"
#include "FlxActError.h"

#include "FlxActShortCode.h"

#include "compTranUtilContext.h"
#include "compTranUtilShortCode.h"
#include "compTranUtilFulfillments.h"

static FlxActBool sPrintLoadErrorMessage(FlxCtuContext* pContext, const char* pszAsrFilePath);
static FlxActBool sPrintErrorMessage(FlxCtuContext* pContext);
static FlxActBool sSetVendorData(FlxCtuContext* pContext, const FlxCtuCommand* pActionCommand);
static FlxActBool sDoActivate(FlxCtuContext* pContext, const FlxCtuCommand* pActionCommand);
static FlxActBool sDoReturn(FlxCtuContext* pContext, const FlxCtuCommand* pActionCommand);
static FlxActBool sDoRepair(FlxCtuContext* pContext, const FlxCtuCommand* pActionCommand);
static FlxActBool sIsPendingRequest(FlxCtuContext* pContext);
static FlxActBool sPrintPendingRequest(FlxCtuContext* pContext, const char* pszTag);
static FlxActBool sDoCancelRequest(FlxCtuContext* pContext);
static const char* sGetRequestTypeText(FlxShortCodeType requestType);
static FlxActBool sHasPathChars(const char* pszCode);
static void		  sOnProcessSuccess(FlxCtuContext* pContext, FlxShortCodeType responseType);
static void		  sOnProcessError(FlxCtuContext* pContext);

#if defined OS_WINDOWS || defined PC
#define STDIN_TERMINATE_MSG	"<enter><ctrl-Z><enter>"
#else
#define STDIN_TERMINATE_MSG	"<enter><ctrl-D><enter> (for UNIX)"
#endif

/* Normally, if a response is entered incorrectly, details of incorrect characters will be 
   displayed.  However, if the response is very long it is likely to be totally wrong input, 
   perhaps an XML file, and the detail output would be long and confusing, so a simple 
   "invalid response" message is given.
   Valid responses of any length are always processed correctly.
*/
#define MAX_LENGTH_FOR_ERROR_DETAIL	200

/***************************************************************************************************
*	flxCtuHelpShortCode
*
* Usage
*	
***************************************************************************************************/
void flxCtuHelpShortCode(const FlxCtuContext* pContext)
{
	flxCtuPrintMessage(
		"\n"
		"shortCode command: short code request, creation and management\n"
		"========= \n"
		"\n"
		"      -shortCode|-sc <shortCodeAsrFilePath> [fromBuffer=yes|no] [<action>]\n"
		"\n"
		"    fromBuffer=yes simulates use of built-in ASR (default is no)\n"
		"\n"
		"  Actions:\n"
		"    -activate|-a [<vendor dictionary>]\n"
		"    -return|-r <fufillmentId> [<vendor dictionary>]\n"
		"    -repair|-e <fufillmentId> [<vendor dictionary>]\n"
		"    -cancelShortCodeRequest|-cs\n"
		"\n"
		"    If no action given, pending request will be printed\n"
		"    For vendor data the ASR must have corresponding overrides.\n"
		"    Use command -shortCodeResponse to process the response\n"
	);
	flxCtuPrintOutput("\n");
	flxCtuPrintOutput("Examples: create short code request\n");
	flxCtuPrintOutput("  %s -shortCode publisher/xml/ShortCode2016Low.asr -activate\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -sc publisher/xml/ShortCode2016Low.asr -a\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -shortCode publisher/xml/ShortCode2016Low.asr -return\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -shortCode publisher/xml/ShortCode2016Low.asr -repair\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -shortCode MyScAsrWiithOverrides.asr -activate vendor{MyKey1=123, MyKey2=abc}\n", pContext->pszProgramName);

	flxCtuPrintOutput("\n");
	flxCtuPrintOutput("Example: show pending short code request\n");
	flxCtuPrintOutput("  %s -shortCode publisher/xml/ShortCode2016Low.asr\n", pContext->pszProgramName);

	flxCtuPrintOutput("\n");
	flxCtuPrintOutput("Examples: cancel pending short code request\n");
	flxCtuPrintOutput("  %s -shortCode publisher/xml/ShortCode2016Low.asr -cancelShortCodeRequest\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -sc publisher/xml/ShortCode2016Low.asr -cs\n", pContext->pszProgramName);
}

/***************************************************************************************************
*	flxCtuHelpShortCodeResponse
*
* Usage
*	
***************************************************************************************************/
void flxCtuHelpShortCodeResponse(const FlxCtuContext* pContext)
{
	flxCtuPrintMessage(
		"\n"
		"shortCodeResponse command: short code response processing\n"
		"================= \n"
		"\n"
		"      -shortCodeResponse|-scr [<shortCodeResponseCode>|<filePath>]\n"
		"\n"
		"    The response code can be:\n"
		"       * Included directly on the command line\n"
		"       * Read from a file\n"
		"       * Read from stdin\n"
	);
	flxCtuPrintOutput("\n");
	flxCtuPrintOutput("Examples:\n");
	flxCtuPrintOutput("  %s -shortCodeResponse 850074-081337-916165-725065-898991-131153-297933-847350\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -shortCodeResponse ShortCodeResponseFile.txt\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -shortCodeResponse\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -scr\n", pContext->pszProgramName);
}

/***************************************************************************************************
*	flxCtuDoCommandShortCode
*
***************************************************************************************************/
FlxActBool flxCtuDoCommandShortCode(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	const char*				pszAsrPath = flxCtuCommandGetValue(pCommand); 
	FlxActBool				bSuccess;
	FlxActBool				bIsPendingRequest;
	const FlxCtuCommand*	pActionCommand;

	pContext->exitCode = flxCtuExitInputError;	/* Default */
	
	/* The command value is the asr file path */
	if (pszAsrPath == NULL)
	{
		flxCtuCommandPrintError(pCommand, "short code ASR path missing\n");
		return FLX_ACT_FALSE;
	}
	/* The "fromBuffer" attribute allows the two ways of obtaining the short code handle to be demo'd;
	   the handle is owned by pContext and freed with it
	*/
	if (flxCtuCommandAttributeIsYes(pCommand, CMD_ATTR_FROMBUFFER, FLX_ACT_FALSE))
	{
		/* 1. Read the ASR into a memory buffer and use that to get the handle */
		char* pszAsrXml;
		if ( !flxCtuContextFileLoad(pContext, pszAsrPath, &pszAsrXml, NULL))
			return FLX_ACT_FALSE;	/* Error has been reported */

		bSuccess = flxActShortCodeCreateFromBuffer(pContext->hApi, pszAsrXml, &pContext->hShortCode);
		flxCtuFree(pszAsrXml);
	}
	else
	{	/* 2. Use the ASR directly */
		bSuccess = flxActShortCodeCreate(pContext->hApi, pszAsrPath, &pContext->hShortCode);
	}
	if ( !bSuccess)
		return sPrintLoadErrorMessage(pContext, pszAsrPath);

	/* Now have a valid shortcode handle in pContext->hShortCode; this will be destroyed with pContext */

	/* Display any pending request - we do this regardless of the action specified */
	bIsPendingRequest = sIsPendingRequest(pContext);
	if (bIsPendingRequest)
		sPrintPendingRequest(pContext, "Pending");

	/* Handle the action - Cancel is specific to short code so check that first */
	if (flxCtuContextHasCommand(pContext, CMD_CANCEL_SHORTCODE))
	{
		/* Cancel the request if there is one, otherwise report an error. */
		if (bIsPendingRequest)
		{
			bSuccess = sDoCancelRequest(pContext);
		}
		else
		{
			flxCtuCommandPrintError(pCommand, "no pending request to cancel\n");
			bSuccess = FLX_ACT_FALSE;
		}
	}
	else if (flxCtuGetFirstRequestAction(pContext, &pActionCommand))
	{
		const char* pCommandName = flxCtuCommandGetName(pActionCommand);

		if ( !sSetVendorData(pContext, pActionCommand))
		{
			bSuccess = FLX_ACT_FALSE;
		}

		/* Actions create new requests and because only one request can be pending for 
		   a given ASR it is an error if there already is one. 
		*/
		if (bIsPendingRequest)
		{
			flxCtuCommandPrintError(pCommand, 
				"there is a pending short code request for this ASR.\n"
				"To create a new request, first cancel the pending request\n"
				"using the -cs action.\n");
		}
		else 
		{	/* Process by action name */
			if (0 == strcmp(pCommandName, CMD_ACTIVATE))
				bSuccess = sDoActivate(pContext, pActionCommand);
			else if (0 == strcmp(pCommandName, CMD_RETURN))
				bSuccess = sDoReturn(pContext, pActionCommand);
			else if (0 == strcmp(pCommandName, CMD_REPAIR))
				bSuccess = sDoRepair(pContext, pActionCommand);
			else
			{
				flxCtuCommandPrintError(pActionCommand, "not valid for short codes\n");
				bSuccess = FLX_ACT_FALSE;
			}
			if (bSuccess)
				sPrintPendingRequest(pContext, "New");
		}
	}
	else 
	{
		/* No action specifed which means the user is querying the pending request.
		   If there is one, it has been output above already so nothing more to do.
		   Otherwise give an error.
		*/
		if (!bIsPendingRequest)
			flxCtuPrintMessage("No pending request for %s\n", pszAsrPath);
	}
	return bSuccess;
}

/***************************************************************************************************
*	flxCtuDoCommandShortCodeResponse
*
***************************************************************************************************/
FlxActBool flxCtuDoCommandShortCodeResponse(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	char*				pszResponse = NULL;
	FlxShortCodeType	responseType;
	FlxActBool			bSuccess;

	pContext->exitCode = flxCtuExitInputError;	/* Default */

	if (flxCtuCommandHasValue(pCommand))
	{
		const char*	pszCmdValue = flxCtuCommandGetValue(pCommand);

		/* See if it is a valid file path */
		FILE* hFile = fopen(pszCmdValue, "r");
		if (hFile != NULL)
		{
			bSuccess = flxCtuContextStreamLoad(pContext, hFile, pszCmdValue, &pszResponse, NULL);
			fclose(hFile);
			if ( !bSuccess)
				return FLX_ACT_FALSE;	/* Error has been reported */
		}
		else if ( sHasPathChars(pszCmdValue))
		{
			flxCtuCommandPrintError(pCommand, "\nCan't open %s\n", pszCmdValue);
			return FLX_ACT_FALSE;
		}
		else
		{	/* Command value it is the code itself - dup it so can free in all cases */
			pszResponse = flxCtuStrDup(pszCmdValue);
		}
	}
	else
	{
		/* No command value, read from stdin */
		flxCtuPrintMessage("stdin: enter response code, then %s...\n", STDIN_TERMINATE_MSG);
		if ( !flxCtuContextStreamLoad(pContext, stdin, "stdin", &pszResponse, NULL))
			return FLX_ACT_FALSE;	/* Error has been reported */
	}

	(void)flxCtuStrEraseChars(pszResponse, " \t\n\r");	/* Erase whitespace */

	if (strlen(pszResponse) <= MAX_LENGTH_FOR_ERROR_DETAIL)
		flxCtuPrintMessage("\nProcessing response: %s\n", pszResponse);
	else
		flxCtuPrintMessage("\nProcessing response: %.*s...\n", 
							MAX_LENGTH_FOR_ERROR_DETAIL, pszResponse);

	bSuccess = flxActShortCodeProcessResponse(pContext->hApi, pszResponse, &responseType);
	if (bSuccess)
		sOnProcessSuccess(pContext, responseType);
	else
		sOnProcessError(pContext);

	flxCtuFree(pszResponse);
	return bSuccess;
}

FlxActBool sPrintLoadErrorMessage(FlxCtuContext* pContext, const char* pszAsrFilePath)
{
	flxCtuPrintError("Can't load short code ASR file %s\n", pszAsrFilePath);
	return sPrintErrorMessage(pContext);
}

FlxActBool sPrintErrorMessage(FlxCtuContext* pContext)
{
	flxCtuContextPrintLastApiFlexError(pContext);
	return FLX_ACT_FALSE;
}

FlxActBool  sSetVendorData(FlxCtuContext* pContext, const FlxCtuCommand* pActionCommand)
{
	size_t					dictionaryIndex;
	const FlxCtuDictionary*	pDictionary;

	for (dictionaryIndex = 0; dictionaryIndex < pActionCommand->dictionaryCount; dictionaryIndex++)
	{
		pDictionary = pActionCommand->ppDictionaries[dictionaryIndex];
		if (0 == flxCtuStrCmpLwr(pDictionary->pszName, CMD_DICT_VENDOR))
		{
			size_t			entryIndex;
			FlxCtuKeyValue*	pEntry;

			flxCtuPrintMessage("  Vendor data:\n");

			for (entryIndex = 0; entryIndex < pDictionary->entryCount; entryIndex++)
			{
				pEntry = pDictionary->ppEntries[entryIndex];

				if ( !flxActShortCodeSetVendorData(pContext->hShortCode, pEntry->pszKey, pEntry->pszValue) )
					FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

				flxCtuPrintMessage("      %20s=%s\n", pEntry->pszKey, pEntry->pszValue);
			}
		}
		else 
		{
			flxCtuCommandPrintError(pActionCommand, "invalid dictionary \"%s\". "
													"Only vendor dictionary allowed for short codes.\n");
			return FLX_ACT_FALSE;
		}
	}
	return FLX_ACT_TRUE;
}

FlxActBool sDoActivate(FlxCtuContext* pContext, const FlxCtuCommand* pActionCommand)
{
	const char* pszRequest;

	if ( !flxActShortCodeGetActivationRequest(pContext->hShortCode, &pszRequest))
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}
	return FLX_ACT_TRUE;
}

FlxActBool sDoReturn(FlxCtuContext* pContext, const FlxCtuCommand* pActionCommand)
{
	const char* pszRequest;
	const char* pszFid = flxCtuCommandGetValue(pActionCommand);
	uint32_t	deductionCount;

	/* Don't allow return if any "valid" deductions exist (excludes repairs and expired). */
	if ( !flxCtuFulfillmentGetValidDeductionCount(pContext, pszFid, &deductionCount))
	{
		flxCtuCommandPrintError(pActionCommand, "fulfillment %s not found.\n", pszFid);
		return FLX_ACT_FALSE;
	}
	if (deductionCount > 0)
	{
		flxCtuPrintError("Can't return fulfillment %s - it has %u valid deduction(s).\n",
						 pszFid,
						 deductionCount);
		return FLX_ACT_FALSE;
	}

	/* Create the return request */
	if ( !flxActShortCodeGetReturnRequest(pContext->hShortCode, pszFid, &pszRequest))
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}
	return FLX_ACT_TRUE;
}

FlxActBool sDoRepair(FlxCtuContext* pContext, const FlxCtuCommand* pActionCommand)
{
	const char* pszRequest;
	const char* pszFid = flxCtuCommandGetValue(pActionCommand);

	if ( !flxActShortCodeGetRepairRequest(pContext->hShortCode, pszFid, &pszRequest))
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}
	return FLX_ACT_TRUE;
}

FlxActBool sIsPendingRequest(FlxCtuContext* pContext)
{
	const char*			pszRequest;
	FlxShortCodeType	requestType;	

	if ( flxActShortCodeGetPendingRequest(pContext->hShortCode, &pszRequest, &requestType))
		return FLX_ACT_TRUE;	/* Pending request		*/
	return FLX_ACT_FALSE;		/* No pending request	*/
}

FlxActBool sPrintPendingRequest(FlxCtuContext* pContext, const char* pszTag)
{
	const char*			pszRequest;
	FlxShortCodeType	requestType;	

	if ( !flxActShortCodeGetPendingRequest(pContext->hShortCode, &pszRequest, &requestType))
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}
	else
	{
		if ( flxCtuContextHasCommand(pContext, CMD_BRIEF))
			flxCtuPrintOutput(pszRequest);			/* If brief, print just the request */
		else
		{
			flxCtuPrintMessage( "\n%7.7s request code: %s\n", pszTag, pszRequest);
			flxCtuPrintMessage( "                type: %s\n", sGetRequestTypeText(requestType));

			if (requestType != flxShortCodeTypeActivation)
			{
				/* Get details of the target fulfillment (prodSpc) */
				FlxActLicSpc	 licSpc;
				FlxActProdLicSpc prodSpc;
				/* Need a licspc container for the prodSpc */
				if ( !flxActCommonLicSpcCreate(pContext->hApi, &licSpc))
					FLX_CTU_EXIT_UNEXPECTED_API_FLEX_ERROR(pContext);

				/* Add a prodSpc for the target of the pending request */
				if ( !flxActShortCodeGetPendingRequestProdSpc(pContext->hShortCode, licSpc))
				{
					FlxActError error;
					flxActCommonHandleGetError(pContext->hApi, &error);
					flxActCommonLicSpcDelete(licSpc);
					if (error.sysErrorNo != LM_TSSE_SHORTCODE_FID_NOT_FOUND)
						FLX_CTU_EXIT_UNEXPECTED_API_FLEX_ERROR(pContext);

					flxCtuPrintMessage("Target fulfillment no longer exists\n");
					return FLX_ACT_TRUE;
				}
				/* Get it from the container */
				prodSpc = flxActCommonLicSpcGet(licSpc, 0);

				/* Print the detail summary */
				flxCtuPrintMessage( "      Entitlement Id: %s\n", flxActCommonProdLicSpcEntitlementIdGet(prodSpc));
				flxCtuPrintMessage( "      Fulfillment Id: %s\n", flxActCommonProdLicSpcFulfillmentIdGet(prodSpc));
				flxCtuPrintMessage( "          Product Id: %s\n", flxActCommonProdLicSpcProductIdGet(prodSpc));
				flxCtuPrintMessage( "          Expiration: %s\n", flxActCommonProdLicSpcExpDateGet(prodSpc));

				flxActCommonLicSpcDelete(licSpc);
			}
		}
	}
	return FLX_ACT_TRUE;
}

FlxActBool sDoCancelRequest(FlxCtuContext* pContext)
{
	const char*			pszRequest;
	FlxShortCodeType	requestType;	

	if ( !flxCtuContextConfirm(pContext, "\nCancel the pending request code? "))
	{
		pContext->exitCode = flxCtuExitCancelled;
		return FLX_ACT_FALSE;
	}

	/* Confirmed - note request type for later */

	if ( !flxActShortCodeGetPendingRequest(pContext->hShortCode, &pszRequest, &requestType))
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}

	if ( !flxActShortCodeCancelRequest(pContext->hShortCode))
	{
		flxCtuPrintError("unable to cancel request code\n");
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}

	flxCtuPrintMessage("\nRequest code cancelled.\n");

	if (requestType == flxShortCodeTypeReturn)
	{
		flxCtuPrintMessage(	"\nThe cancelled request was a RETURN and the fulfillment has been\n"
							"re-enabled, allowing it to be used again on this machine.\n"
							"If the return has been completed on the server, you should delete\n"
							"the fulfillment using the -deleteFR command to prevent license loss.\n");
	}
	return FLX_ACT_TRUE;
}

static const char* sGetRequestTypeText(FlxShortCodeType requestType)
{
	switch (requestType)
	{
	case flxShortCodeTypeActivation:	return "ACTIVATION";
	case flxShortCodeTypeReturn:		return "RETURN";
	case flxShortCodeTypeRepair:		return "REPAIR";
	default:							return "UNKNOWN";
	}
}

static FlxActBool sHasPathChars(const char* pszCode)
{
	static const char* pszPathChars = ":\\/.";

	return (strcspn(pszCode, pszPathChars) != strlen(pszCode))? FLX_ACT_TRUE : FLX_ACT_FALSE;
}

static void	sOnProcessSuccess(FlxCtuContext* pContext, FlxShortCodeType responseType)
{
	FlxActError	warning;
    FlxActProdLicSpc prodSpc = NULL;;

	/* Get any response processing warning code (before making any other API calls) */
	flxActCommonHandleGetError(pContext->hApi, &warning);

	switch (responseType)
	{
	case  flxShortCodeTypeActivation:
		flxCtuPrintMessage("Short code ACTIVATION response processed, fulfillment created\n");
		break;
	case  flxShortCodeTypeReturn:
		flxCtuPrintMessage("Short code RETURN response processed, fulfillment returned\n");
		break;
	case  flxShortCodeTypeRepair:
		flxCtuPrintMessage("Short code REPAIR response processed, fulfillment repaired\n");
		break;
	case  flxShortCodeTypeError:
		{
			uint32_t denyReasonGivenByServer = flxActShortCodeGetDenyReason(pContext->hApi);

			flxCtuPrintMessage("Short code ERROR response processed\n"
				"Reason given by the server for denying the request: %d\n", denyReasonGivenByServer);
		}
		break;
	default:	/* Shouldn't happen */
		flxCtuPrintError("Short code response processed - type %d unkown\n", responseType);
		return;
	}

	if (responseType != flxShortCodeTypeError)
	{
		/* Get a prod spc for the target fuilfillment, if any */
		(void)flxActCommonRespProdLicSpcGet(pContext->hApi, &prodSpc);
		if (prodSpc != NULL)
		{
			/* Print the detail summary */
			flxCtuPrintMessage( "      Entitlement Id: %s\n", flxActCommonProdLicSpcEntitlementIdGet(prodSpc));
			flxCtuPrintMessage( "      Fulfillment Id: %s\n", flxActCommonProdLicSpcFulfillmentIdGet(prodSpc));
			flxCtuPrintMessage( "          Product Id: %s\n", flxActCommonProdLicSpcProductIdGet(prodSpc));
			flxCtuPrintMessage( "          Expiration: %s\n", flxActCommonProdLicSpcExpDateGet(prodSpc));

			/* Print any vendor dictionary entries. */
			flxCtuFulfillmentViewVendorDictionary(pContext, prodSpc);
		}
	}

	/* Handle any warnings */
	switch (flxActShortCodeGetWarning(pContext->hApi))
	{
	case flxShortCodeWarningFulfillmentOverwitten:
		flxCtuPrintMessage("WARNING: an existing fulfillment with the same id was overwritten by this one\n");
		break;
	default:
		break;
	}	
}
static void	sOnProcessError(FlxCtuContext* pContext)
{
	FlxActError error;
	const char* pText = NULL;
	flxActCommonHandleGetError(pContext->hApi, &error);

	switch (error.sysErrorNo)
	{
	case LM_TSSE_SHORTCODE_INVALID_CHAR:	/* Drop through */
	case LM_TSSE_SHORTCODE_INVALID_CHECK:
	{
		/* Response code entered incorrectly.  
		 * If the error detail has been set it contains a copy of the code with potentially 
		 * incorrect characters set to '?'.
		 * However, only display this if the code is a sensible length, otherwise chances are the user 
		 * specified an incorrect file and the detail messages would be long and confusing.
		 */
		const char* pszErrorDetail = flxActShortCodeGetErrorDetail(pContext->hApi);
		if (pszErrorDetail != NULL && strlen(pszErrorDetail) <= MAX_LENGTH_FOR_ERROR_DETAIL)
		{
			flxCtuPrintMessage("                     %s\n", pszErrorDetail);
			flxCtuPrintError(  "Response entered incorrectly - check the character(s) replaced by '?' above.\n");
		}
		else
			flxCtuPrintError(  "Invalid short code response.\n");

		break;
	}
	default:
		flxCtuPrintMessage("\nShort code response processing failed...\n"); 
		flxCtuContextPrintLastApiFlexError(pContext);
	}
}
