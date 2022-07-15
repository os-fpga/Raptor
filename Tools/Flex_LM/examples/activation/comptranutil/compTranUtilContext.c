/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This source file is part of a sample utility that demonstrates Composite Transactions.

	It defines the functions that relate to the FlxCtuContext (context) object. 
*/
#include "compTranUtilContext.h"
#include "compTranUtilCustom.h"
#include "lm_redir_std.h"

static FlxActBool sFindCommand(const FlxCtuContext*	pContext, 
							   const char*			pszCommandName, 
							   size_t*				pCommandIndex);

static const char* sGetTextForSysError(uint32_t sysError);

/*******************************************************************************
*	flxCtuContextCreate
*
* Create a new command context and return a pointer to it.
* The FlxCtuCommandDef list defines the commands that are allowed. 
*******************************************************************************/
void flxCtuContextCreate(const FlxCtuCommandDef*	pCommandDefs, 
						 size_t						commandDefCount,
						 FlxCtuContext**			ppContext)
{
	*ppContext = (FlxCtuContext*)flxCtuMallocAndZeroise(sizeof(FlxCtuContext));
	(*ppContext)->exitCode				= flxCtuExitUnknown;
	(*ppContext)->pCommandDefs			= pCommandDefs;
	(*ppContext)->commandDefCount		= commandDefCount;
	(*ppContext)->bUsingStoredRequest	= FLX_ACT_FALSE;	
}

void flxCtuContextDestroy(FlxCtuContext* pContext)
{
	if ((pContext != (FlxCtuContext*)0))
	{
		/* Close any open handles and the common library. */
		if (pContext->hTransaction != (FlxActTransaction)0)
			flxActTransactionDestroy(pContext->hTransaction);

		if (pContext->hShortCode != NULL)
			flxActShortCodeDestroy(pContext->hShortCode);

		if (pContext->hApi != (FlxActHandle)0)
			flxActCommonHandleClose(pContext->hApi);

		if (pContext->bCommonLibraryInitialised)
			flxActCommonLibraryCleanup();

		/* Destroy the commands.	*/
		{
			size_t commandIndex;
			for (commandIndex = 0; commandIndex < pContext->commandCount; commandIndex++)
				flxCtuCommandDestroy(pContext->ppCommands[commandIndex]);
		}

		/* Free the list. */
		flxCtuFree(pContext->ppCommands);
		
		/* Free the strings. */
		flxCtuFree(pContext->pszProgramName);
		
		/* Destroy the command definitions. */
		flxCtuCommandDefsDestroy((FlxCtuCommandDef*)pContext->pCommandDefs, 
								 pContext->commandDefCount);

		/* Finally free the context structure itself. */
		flxCtuFree(pContext);
	}
}

const FlxCtuCommand* flxCtuContextGetCommand(const FlxCtuContext* pContext, size_t commandIndex)
{
	FLX_CTU_ASSERT( commandIndex < pContext->commandCount );
	return pContext->ppCommands[commandIndex];
}

/***************************************************************************************************
*		flxCtuContextGetCommandByName
*
*	Get a pointer to the first FlxCtuCommand object with the specified name. 
*	Return true if pCommandName is present in the command list.
*	Specify the command name in lower case.
*	Set the command index in the context so that the next command can be obtained by calling
*	flxCtuCommandGetNextCommand
***************************************************************************************************/
FlxActBool flxCtuContextGetCommandByName(const FlxCtuContext*	pContext, 
										 const char*			pszCommandName, 
										 const FlxCtuCommand**	ppCommand)
{
	size_t commandIndex;

	if ( sFindCommand(pContext, pszCommandName, &commandIndex) )
	{
		*ppCommand = pContext->ppCommands[commandIndex];
		return FLX_ACT_TRUE;
	}
	return FLX_ACT_FALSE;
}

size_t flxCtuContextGetCommandCount(const FlxCtuContext* pContext)
{
	return pContext->commandCount;
}

/***************************************************************************************************
*		flxCtuContextFileDump
*
*	Create/replace a file from a buffer.
***************************************************************************************************/
FlxActBool flxCtuContextFileDump(FlxCtuContext*	pContext, 
								 const char*	pszFilename, 
								 const char*	pFileContent,
								 size_t			contentSize)
{
	FILE*	hFile;

	hFile = fopen(pszFilename, "w");
	if (hFile == NULL)
	{
		flxCtuPrintError("Can't create file: %s\n", pszFilename);
		pContext->exitCode = flxCtuExitInputError;	/* Assume incorrect path given. */
		return FLX_ACT_FALSE;
	}
	if (1 != fwrite(pFileContent, contentSize, 1, hFile))
	{
		fclose(hFile);
		flxCtuPrintError("Can't write file: %s\n", pszFilename);
		pContext->exitCode = flxCtuExitFileError;
		return FLX_ACT_FALSE;
	}
	fclose(hFile);
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		flxCtuContextFileLoad
*
*	Load a file into memory (free with flxCtuFree).
***************************************************************************************************/
FlxActBool flxCtuContextFileLoad(FlxCtuContext*	pContext, 
								 const char*	pszFilename, 
								 char**			ppFileContent,
								 size_t*		pContentSize)
{
	FILE*				hFile;
	FlxActBool			res;

	/* Open the file.	*/
	hFile = fopen(pszFilename, "r");
	if (hFile  == NULL)
	{
		flxCtuPrintError("can't open file %s\n", pszFilename);
		pContext->exitCode = flxCtuExitInputError;	/* Assume incorrect filename given. */
		return FLX_ACT_FALSE;
	}

	res = flxCtuContextStreamLoad(pContext, hFile, pszFilename, ppFileContent, pContentSize);

	fclose(hFile);

	return res;
}

/***************************************************************************************************
*		flxCtuContextStreamLoad
*
*	Load the contents of a stream into memory (free with flxCtuFree).
***************************************************************************************************/
FlxActBool flxCtuContextStreamLoad(FlxCtuContext*	pContext, 
								FILE*          	pFile,
							 	const char*		pszFilename, 
								char**			ppFileContent,
								size_t*			pContentSize)
{
	size_t	fileSize=0;
	char*	contents=NULL;

	/* Check the file stream.	*/
	if (pFile == NULL)
	{
		flxCtuPrintError("can't access file %s\n", pszFilename);
		pContext->exitCode = flxCtuExitInputError;	/* Assume incorrect filename given. */
		return FLX_ACT_FALSE;
	}

	/* Read from the stream.  */
	while( !ferror(pFile) && !feof(pFile) )
	{
		char	buffer[256];			
		size_t	actual;
	
		actual=fread(buffer, 1, sizeof(buffer), pFile);

		if( actual > 0 )
		{
			size_t newSize = fileSize + actual;
			contents = (char*)flxCtuRealloc(contents, newSize); 
			memcpy( &contents[fileSize], buffer, actual );
			fileSize = newSize;
		}
	}

	/* Check reading terminated successfully.  */
	if( !feof(pFile) )
	{
		flxCtuPrintError("Can't read file %s\n", pszFilename);
		pContext->exitCode = flxCtuExitFileError;
		flxCtuFree(contents);
		return FLX_ACT_FALSE;
	}
	
	/* Check for empty file. */
	if (fileSize == 0)
	{
		flxCtuPrintError("File %s is empty.\n", pszFilename);
		pContext->exitCode = flxCtuExitFileError;
		flxCtuFree(contents);
		return FLX_ACT_FALSE;
	}

	/* Allocate extra byte of memory for NUL terminator. */
	contents = (char*)flxCtuRealloc(contents, fileSize+1); 

	/* Append a NUL as a convenience for the caller. */
	contents[fileSize] = '\0';

	/* Return file stream contents plus the size if required. */
	*ppFileContent = contents;	
	if (pContentSize != NULL)
		*pContentSize = fileSize;

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*		flxCtuContextFindCommandDef
*
***************************************************************************************************/
FlxActBool flxCtuContextFindCommandDef(const FlxCtuContext*		pContext, 
									   const char*				pszComamndOrAlias,
									   const FlxCtuCommandDef**	ppCommandDef)
{
	const char*	pszComamndOrAliasLwr = flxCtuStrDupLwr(pszComamndOrAlias);
	size_t index;
	for (index = 0; index < pContext->commandDefCount; index++)
	{
		if (	(strcmp(pszComamndOrAliasLwr, pContext->pCommandDefs[index].pszName) == 0)
			|| 	(strcmp(pszComamndOrAliasLwr, pContext->pCommandDefs[index].pszAlias) == 0) )
		{
			*ppCommandDef = &pContext->pCommandDefs[index];
			break;
		}
	}
	flxCtuFree(pszComamndOrAliasLwr);

	return (index < pContext->commandDefCount)? FLX_ACT_TRUE : FLX_ACT_FALSE;
}

FlxActBool flxCtuCommandHasTrait(const FlxCtuCommand*	pCommand,
								 FlxCtuCommandTrait		trait)
{
	return (pCommand->pDef->traits & trait) != 0;
}

/***************************************************************************************************
*		flxCtuContextHasCommand
*
*	Return true if pCommandName is present in the command list.
*	Specify the command name in lower case.
***************************************************************************************************/
FlxActBool flxCtuContextHasCommand(const FlxCtuContext* pContext, const char* pszCommandName)
{
	return sFindCommand(pContext, pszCommandName, NULL);
}

/***************************************************************************************************
*		flxCtuContextPrintApiFlexError
*
* These are errors from non- flxActTranXXX functions that report errors using a FlxActError object.
***************************************************************************************************/
void flxCtuContextPrintApiFlexError(FlxCtuContext* pContext, FlxActError* pFlxError)
{
	const char* sysErrorText;
	sysErrorText = sGetTextForSysError(pFlxError->sysErrorNo);

	flxCtuPrintError("API function failed (%u,%u,%u)\n%s\n",
					 pFlxError->majorErrorNo,
					 pFlxError->minorErrorNo,
					 pFlxError->sysErrorNo,
					 sysErrorText);

	pContext->exitCode = flxCtuExitApiError;
}

/***************************************************************************************************
 *		flxCtuContextPrintLastApiError
 *
 * Print the details of the error set by the last flxActTranXXX function called by this thread. 
 ***************************************************************************************************/
void flxCtuContextPrintLastApiError(FlxCtuContext* pContext)
{
	FlxActTranResult		resultCode		= flxActTransactionGetResult();
	FlxActTranFunctionId	functionId		= flxActTransactionGetFunctionId();
	const char*				pszLogString	= flxActTransactionGetLogString();

	flxCtuPrintError("API function %u failed, result %u\n"
					 "%s\n",
					 functionId,
					 resultCode,
					 flxCtuGetTextForResult(resultCode));

	flxCtuPrintMessage("Additional - the last element of the tuple is the detail error (e.g. comms)\n");
	flxCtuContextPrintLastApiFlexError(pContext);

	/* If verbose, output the log string. */
	flxCtuPrintVerbose("Log string: %s\n", pszLogString);

	/* If CMD_LOG_ERRORS specified, write the error details to the FNP event log. */
	if (flxCtuContextHasCommand(pContext, CMD_LOG_ERRORS))
	{
		if (flxActTransactionLogError())
			flxCtuPrintMessage("Error logged.\n");
		else
			flxCtuPrintError("flxActTransactionLogError() failed.\n");
	}

	pContext->exitCode = flxCtuExitApiError;
}

/***************************************************************************************************
*		flxCtuContextPrintLastApiFlexError
*
* Print an error set by a flxActXXX function other than a flxActTranXXX().
* The last error is obtained from the top level API handle.
***************************************************************************************************/
void flxCtuContextPrintLastApiFlexError(FlxCtuContext* pContext)
{
	FlxActError flxError;
	flxActCommonHandleGetError(pContext->hApi, &flxError);
	flxCtuContextPrintApiFlexError(pContext, &flxError);
	switch (flxError.majorErrorNo)
	{
	case LM_TS_CONNECTION_FAILED:
		flxCtuPrintError("Failed to connect to the Enterprise License Server or Operations Server\n");
		break;
	}
	pContext->exitCode = flxCtuExitApiError;
}

/***************************************************************************************************
*		flxCtuContextValidateCommands
*
* Additional validation needed after successful command parsing - command combinations etc.
***************************************************************************************************/
FlxActBool flxCtuContextValidateCommands(FlxCtuContext* pContext)
{
	const FlxCtuCommand* pCommand;
	const FlxCtuCommand* pCommand2;

	/* Rule: if CMD_TRANSACTION is present then... */
	if (flxCtuContextGetCommandByName(pContext, CMD_TRANSACTION, &pCommand))
	{
		/* ...the server address must be specified as the command value. */
		if ( !flxCtuCommandHasValue(pCommand) )
		{
			flxCtuCommandPrintError(pCommand, "server address not specified.\n");
			return FLX_ACT_FALSE;
		}
	}

	/* Rule: if CMD_NEW is present then ... */
	if (flxCtuContextGetCommandByName(pContext, CMD_NEW, &pCommand))
	{
		/* Any main command must be CMD_TRANSACTION. */
		if (pContext->bIsMainCommandSet)
		{
			if ( !flxCtuContextHasCommand(pContext, CMD_TRANSACTION))
			{
				flxCtuCommandPrintError(pCommand, "and \"%s\" cannot both be present.\n", 
					pContext->pMainCommand->pszNameForMessages);
				return FLX_ACT_FALSE;
			}
		}
		else
		{
			/* No main command so set this as the main command. */
			pContext->bIsMainCommandSet = FLX_ACT_TRUE;
			pContext->pMainCommand	    = pCommand;
		}
	}

	/* Rule: if CMD_STORED is present then ... */
	if (flxCtuContextGetCommandByName(pContext, CMD_STORED, &pCommand))
	{
		/* Any main command must be CMD_TRANSACTION. */
		if (pContext->bIsMainCommandSet)
		{
			if ( !flxCtuContextHasCommand(pContext, CMD_TRANSACTION))
			{
				flxCtuCommandPrintError(pCommand, "and \"%s\" cannot both be present.\n", 
										pContext->pMainCommand->pszNameForMessages);
				return FLX_ACT_FALSE;
			}
		}
		else
		{
			/* No main command so set this as the main command. */
			pContext->bIsMainCommandSet = FLX_ACT_TRUE;
			pContext->pMainCommand	    = pCommand;
		}
	}

	/* If there are any request actions... */
	if (flxCtuGetFirstRequestAction(pContext, &pCommand))
	{
        /* Rule: if there are any actions then CMD_NEW, CMD_TRANSACTION or CMD_SHORTCODE must be present. */
		if ( !(		flxCtuContextHasCommand(pContext, CMD_NEW)
				 ||	flxCtuContextHasCommand(pContext, CMD_TRANSACTION)
				 ||	flxCtuContextHasCommand(pContext, CMD_SHORTCODE)))
		{
			flxCtuCommandPrintError(pCommand, "is only valid with \"-%s\", \"-%s\" or \"-%s\"\n", 
											  CMD_NEW, CMD_TRANSACTION, CMD_SHORTCODE);
			return FLX_ACT_FALSE;
		}
        /* For each request action... */
        do
        {
            /* Rule: the count attribute may not be zero. */
           	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_COUNT))
            {
	            /* Command parser will have ensured the value is numeric */
	            if (0 == atoi(flxCtuCommandGetAttribute(pCommand, CMD_ATTR_COUNT)))
		        {
			        flxCtuCommandPrintError(pCommand, "The 'count' attribute may not be zero.\n"); 
			        return FLX_ACT_FALSE;
		        }
            }
        } while (flxCtuGetNextRequestAction(pContext, &pCommand));
	}

	/* Rule: CMD_REPAIR and CMD_REPAIR_ALL cannot both be present. */
	if (	flxCtuContextGetCommandByName(pContext, CMD_REPAIR, &pCommand)
		 && flxCtuContextGetCommandByName(pContext, CMD_REPAIR_ALL, &pCommand2))
	{
		flxCtuCommandPrintError(pCommand, "is not valid with %s\n", pCommand2->pszNameForMessages);
		return FLX_ACT_FALSE;
	}

	/* Rule: CMD_CANCEL_SHORTCODE can only be an action for CMD_SHORTCODE. */
	if (	flxCtuContextGetCommandByName(pContext, CMD_CANCEL_SHORTCODE, &pCommand)
		 && !flxCtuContextHasCommand(pContext, CMD_SHORTCODE))
	{
		flxCtuCommandPrintError(pCommand, "is only valid with -shortCode\n");
		return FLX_ACT_FALSE;
	}

	/* Rules for CMD_ACTIVATE */
	if (flxCtuContextGetCommandByName(pContext, CMD_ACTIVATE, &pCommand))
	{
		/* Rule: if attribute CMD_ATTR_FR is present then... */
		if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_FR))
		{
			/* ... attribute CMD_ATTR_FID must also be present. */
			if ( !flxCtuCommandHasAttribute(pCommand, CMD_ATTR_FID))
			{
				flxCtuCommandPrintError(pCommand, 
										"fulfillmentRecord|fr is only valid with fulfillmentId|fid\n");
				return FLX_ACT_FALSE;
			}
		}
		/* Rule: if attribute CMD_ATTR_DURATION_DAYS is present then... */
		if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_DURATION_DAYS))
		{
			/* ... it may not be zero */
			int durationDays = atoi(flxCtuCommandGetAttribute(pCommand, CMD_ATTR_DURATION_DAYS));
			if (durationDays == 0)
			{
				flxCtuCommandPrintError(pCommand, "duration may not be zero\n");
				return FLX_ACT_FALSE;
			}

			/* ...the exiration attributes may not be. */
			if (	flxCtuCommandHasAttribute(pCommand, CMD_ATTR_EXP_TYPE)
				 ||	flxCtuCommandHasAttribute(pCommand, CMD_ATTR_EXP_VALUE))
			{
				flxCtuCommandPrintError(pCommand, "both duration and expiration specified\n");
				return FLX_ACT_FALSE;
			}
		}
		/* Rule: if attribute CMD_ATTR_EXP_TYPE is present then ... */
		if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_EXP_TYPE))
		{
			/* ... attribute CMD_ATTR_EXP_VALUE must also be present. */
			if ( !flxCtuCommandHasAttribute(pCommand, CMD_ATTR_EXP_VALUE))
			{
				flxCtuCommandPrintError(pCommand, 
										"expirationType|expType is only valid with expiration|exp\n");
				return FLX_ACT_FALSE;
			}
		}
	}
	/* Rules for CMD_SHORTCODE */
	if (flxCtuContextGetCommandByName(pContext, CMD_SHORTCODE, &pCommand))
	{
		/* Rule: there must be a value... */
		if ( !flxCtuCommandHasValue(pCommand))
		{
			flxCtuCommandPrintError(pCommand, "short code ASR must be specified\n");
			return FLX_ACT_FALSE;
		}
		/* Rule: there must not be more than one request action (cancel counts as an action here)... */
		if (	pContext->actionCount > 1
			 || (pContext->actionCount == 1 && flxCtuContextHasCommand(pContext, CMD_CANCEL_SHORTCODE)))
		{
			flxCtuCommandPrintError(pCommand, "only one action is allowed for short code requests\n");
			return FLX_ACT_FALSE;
		}
		/* Rule: shortcode activate actions may not have a value */
		if (	flxCtuContextGetCommandByName(pContext, CMD_ACTIVATE, &pCommand2)
			 && flxCtuCommandHasValue(pCommand2))
		{
			flxCtuCommandPrintError(pCommand2, "no value allowed for shortcode activate\n");
			return FLX_ACT_FALSE;
		}
		/* Rule: shortcode return and repair actions must have a value */
		if ((	flxCtuContextGetCommandByName(pContext, CMD_RETURN, &pCommand2)
			 || flxCtuContextGetCommandByName(pContext, CMD_REPAIR, &pCommand2))
			 &&	!flxCtuCommandHasValue(pCommand2))
		{
			flxCtuCommandPrintError(pCommand2, "fulfillment id must be specified\n");
			return FLX_ACT_FALSE;
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuUnexpectedApiError
*
* Unexpected fatal flxActTran* API errors - prints a message and exits.
***************************************************************************************************/
void flxCtuUnexpectedApiError(FlxCtuContext* pContext, const char* pszFilename, int line)
{
	flxCtuPrintError("Unexpected fatal API error in %s at line %u\n", pszFilename, line);
	flxCtuContextPrintLastApiError(pContext);											\
	flxCtuOnUnexpectedError(pContext, flxCtuExitApiError);
}

/***************************************************************************************************
*	flxCtuUnexpectedApiFlexError
*
* Unexpected fatal flxAct* API error - prints a message and exits.
***************************************************************************************************/
void flxCtuUnexpectedApiFlexError(FlxCtuContext* pContext, const char* pszFilename, int line)
{
	flxCtuPrintError("Unexpected fatal API error in %s at line %u\n", pszFilename, line);
	flxCtuContextPrintLastApiFlexError(pContext);											\
	flxCtuOnUnexpectedError(pContext, flxCtuExitApiError);
}

/***************************************************************************************************
*		flxCtuGetTextForResult
*
* Return text string for some of the more likely/interesting result codes.
***************************************************************************************************/
const char* flxCtuGetTextForResult(FlxActTranResult result)
{
	switch (result)
	{
	case FLX_ACT_TRAN_ERR_NO_MEMORY:		return "No memory.";
	case FLX_ACT_TRAN_ERR_HANDLE:			return "Invalid handle.";
	case FLX_ACT_TRAN_ERR_TRANS_HANDLE:		return "Invalid Transaction handle";
	case FLX_ACT_TRAN_BAD_REQ_HANDLE:		return "Invalid Request handle";
	case FLX_ACT_TRAN_ERR_RSP_XML_HEADER:	return "XML header invalid/wrong XML namespace";
	case FLX_ACT_TRAN_ERR_RSP_XML_MISSING:	return "Mandatory XML element missing";
	case FLX_ACT_TRAN_ERR_RSP_NOT_COMPOSITE:return "Not a composite response";
	case FLX_ACT_TRAN_ERR_RSP_XML_VERSION:	return "Response XML version number not supported";
	case FLX_ACT_TRAN_ERR_RSP_SIGNATURE_1:	return "First (or only) signature invalid";
	case FLX_ACT_TRAN_ERR_RSP_NO_REQUEST:	return "No stored request for this response";
	case FLX_ACT_TRAN_ERR_RSP_NOT_REQUESTER:return "Request generated on a different machine";
	case FLX_ACT_TRAN_ERR_RSP_SIGNATURE_2:	return "Second or subsequent signature invalid";
	case FLX_ACT_TRAN_ERR_RSP_ACTION_INFO:	return "Invalid or missing response action attribute";
	case FLX_ACT_TRAN_ERR_RSP_ACTION_FAILED:return "One or more response actions failed";
	case FLX_ACT_TRAN_ERR_RSP_SIG_VERSION:	return "Response XML signature version not supported";
	case FLX_ACT_TRAN_ERR_TS_LOAD:			return "Trusted storage load failed";
	case FLX_ACT_TRAN_ERR_TS_SAVE:			return "Trusted storage save failed";
	case FLX_ACT_TRAN_ERR_FR_DISABLED:		return "Fulfillment record is disabled";
	case FLX_ACT_TRAN_ERR_FR_UNTRUSTED:		return "Fulfillment record is not fully trusted";
	case FLX_ACT_TRAN_ERR_FR_TRUSTED:		return "Fulfillment record does not need repair";
	case FLX_ACT_TRAN_ERR_RSP_NOT_XML:		return "Not an XML document";

	case FLX_ACT_TRAN_ERR_RSP_REQUEST_NOT_PENDING:	return "Stored request state incorrect";

	case FLX_ACT_TRAN_ERR_ELS_CONNECT:		return "Cannot connect to the Enterprise License Server";
	case FLX_ACT_TRAN_ERR_ELS_SEND_RECEIVE:	return "Cannot communicate with Enterprise License Server";
	case FLX_ACT_TRAN_ERR_PARAM_DATE:		return "Invalid date - use dd-mmm-yyyy, d-mmm-yyyy or permanent";

	default:								return "";
	}
}

/***************************************************************************************************
*		flxCtuGetTextForSystemError
*
* Return text string for some of the more likely/interesting system error codes.
***************************************************************************************************/
const char* flxCtuGetTextForSystemError(uint32_t systemError)
{
	return sGetTextForSysError(systemError);
}

/***************************************************************************************************
*	Iterate over request action commands.
*
***************************************************************************************************/
FlxActBool flxCtuGetFirstRequestAction(FlxCtuContext*	        pContext, 
									   const FlxCtuCommand**	ppCommand)
{
    pContext->requestActionIndex = 0;
    return flxCtuGetNextRequestAction(pContext, ppCommand);
}

FlxActBool flxCtuGetNextRequestAction( FlxCtuContext*	        pContext, 
									   const FlxCtuCommand**	ppCommand)
{
	while (pContext->requestActionIndex < pContext->commandCount)
	{
		const FlxCtuCommand* pCommand = pContext->ppCommands[pContext->requestActionIndex++];
		if ((pCommand->pDef->traits & flxCtuCommandTraitIsRequestAction) != 0)
		{
			if (ppCommand != NULL)
				*ppCommand = pCommand;
			return FLX_ACT_TRUE;
		}
	}
	return FLX_ACT_FALSE;
}

/***************************************************************************************************
*		flxCtuContextConfirm
*
***************************************************************************************************/
FlxActBool flxCtuContextConfirm(FlxCtuContext* pContext, const char *pszFormatStr, ...)
{
	va_list argList;
	va_start(argList, pszFormatStr);
	flxCtuPrintOutputV(pszFormatStr, argList);
	va_end(argList);
	while (1)
	{
		char	pszYesNo[16];
		flxCtuPrintOutput("Enter \"yes\" or \"no\": ");
		if (flxCtuContextHasCommand(pContext, CMD_YES))
		{
			flxCtuPrintOutput("yes\n");
			return FLX_ACT_TRUE;
		}
		else if (flxCtuContextHasCommand(pContext, CMD_NO))
		{
			flxCtuPrintOutput("no\n");
			pContext->exitCode = flxCtuExitCancelled;
			return FLX_ACT_FALSE;
		}
		else if (NULL != fgets(pszYesNo, sizeof(pszYesNo), lm_flex_stdin()))
		{
			flxCtuStrLwr(pszYesNo);
			if (0 == strncmp(pszYesNo, "yes", 3) || 0 == strncmp(pszYesNo, "y", 1))
				return FLX_ACT_TRUE;
			if (0 == strncmp(pszYesNo, "no", 2)  || 0 == strncmp(pszYesNo, "n", 1))
			{
				pContext->exitCode = flxCtuExitCancelled;
				return FLX_ACT_FALSE;
			}
			fflush(lm_flex_stdin());	/* Discard rest of buffer, take new input from user */
		}
	}
}

/***************************************************************************************************
*		sFindCommand
*
***************************************************************************************************/
FlxActBool sFindCommand(const FlxCtuContext*	pContext, 
						const char*				pszCommandName, 
						size_t*					pCommandIndex)
{
	size_t commandIndex;

	for (commandIndex = 0; commandIndex < pContext->commandCount; commandIndex++)
	{
		if (strcmp(pszCommandName, pContext->ppCommands[commandIndex]->pDef->pszName) == 0)
		{
			if (pCommandIndex != NULL)
				*pCommandIndex = commandIndex;
			return FLX_ACT_TRUE;
		}
	}
	return FLX_ACT_FALSE;
}

/*
	Text for some common system errors - not an exhaustive list, see FlxActSystemError.h for more info.
*/
static const char* sGetTextForSysError(uint32_t sysError)
{
	switch (sysError)
	{
	case LM_TSSE_SERVICE_NOT_PRESENT:		return "The licensing service is not installed.";
	case LM_TSSE_SERVICE_TOO_OLD:			return "The licensing service is too old.";				
	case LM_TSSE_SERVICE_NOT_ENOUGH_RIGHTS:	return "Insufficient rights to connect to the licensing service.";
	case LM_TSSE_SERVICE_DISABLED:			return "Licensing service disabled.";
	case LM_TSSE_SERVICE_MARKED_FOR_DELETE:	return "Licensing service marked for delete.";
	case LM_TSSE_SERVICE_DATABASE_LOCKED:	/* Drop through */	
	case LM_TSSE_SERVICE_CONFIGURATION:		/* Drop through */	
	case LM_TSSE_SERVICE_NOT_PRESENT_MAC:	return "Licensing service not present.";
	case LM_TSSE_SERVICE_FAILED_MAC:		return "Licensing service failed.";
	case LM_TSSE_SERVICE_DID_NOT_START:		return "Licensing service did not start";
	case LM_TSSE_SERVICE_READ_ONLY_PORTAL:	return "The licensing service read-only portal interface call failed\n"
												   "See event log for portal error code (service status).";
	case LM_TSSE_SERVICE_NONE:				return "This platform does not have a licensing service.";
	case LM_TSSE_SERVICE_INFO_NOT_AVAILABLE:
											return "The information requested is not available from the "
													"licensing service on this platform.";

	case LM_TSSE_ASR_PATH:				return "Cannot open ASR.";
	case LM_TSSE_ASR_READ:				return "The ASR can be opened but not read.";
	case LM_TSSE_ASR_NOT_XML:			return "The ASR does not contain well-formed XML.";
	case LM_TSSE_ASR_NOT_ASR:			return "Not an ASR (XML root element not 'ActivationSpecificationRecord'.";
	case LM_TSSE_ASR_XML_ELEMENT_MISSING: 
										return "The ASR supplied has a missing XML element.";
	case LM_TSSE_ASR_SIGNATURE:			return "The ASR signature does not verify.";
	case LM_TSSE_ASR_LOADED_ALREADY:	return "The ASR has been loaded already; its fulfillment exists in Trusted Storage.";
	case LM_TSSE_ASR_LOADED_PREVIOUSLY:	return "The ASR has been loaded previously; its fulfillment (or Trusted Storage) "
											   "has been deleted at some time.";
	case LM_TSSE_ASR_WRONG_SERVER_MODE:	return "The ASR is for server TS and is being loaded to application TS, or vice versa.";
	case LM_TSSE_ASR_NO_RESET_ATTRIBUTE:
										return "The ASR can't be used to reset a trial because it does not have the reset attribute.";
	case LM_TSSE_ASR_IS_SHORTCODE:		return "The ASR cannot be loaded because it is for use only in shortcode transactions.";
	case LM_TSSE_SHORTCODE_LOCAL_ASR:	return "The ASR cannot be loaded because it is for local activations only.";
	case LM_TSSE_SHORTCODE_SIGNATURE:	return "The shortcode response signature did not verify (response entered incorrectly "
											   "or response is not for the pending request on this machine).";
	case LM_TSSE_SHORTCODE_NO_REQUEST:	return "No pending request for this response.";
	case LM_TSSE_SHORTCODE_TYPE:		return "Shortcode response type unknown (response code entered incorrectly).";
	case LM_TSSE_SHORTCODE_NO_TS_SECTION:
										return "The trusted section needed by the short code ASR does not exist.";
	case LM_TSSE_SHORTCODE_FID_NOT_FOR_ASR:	return "The FID specified for return or repair was not created using the specified ASR.";
	case LM_TSSE_SHORTCODE_FID_NOT_FOUND:	return "No fulfillment record with the FID specified exists.";
	case LM_TSSE_SHORTCODE_FR_DISABLED:	return "Cannot return because there is a pending non-shortcode return request for the FR.";
	case LM_TSSE_SHORTCODE_PENDING:		return "Cannot create request because there is one pending for the ASR.";
	case LM_TSSE_SHORTCODE_INVALID_CHAR:	return "Invalid character(s) in shortcode.";
	case LM_TSSE_SHORTCODE_INVALID_CHECK:	return "Shortcode check digit(s) do not verify.";
	case LM_TSSE_SHORTCODE_BAD_ASR_CDT:		return "Short code ASR 'CheckDigiting' property not 'none', 'all' or 'group'.";
	case LM_TSSE_SHORTCODE_TOO_BIG:			return "Short code exceeds 1024 bits (check ASR overrides).";
	case LM_TSSE_SHORTCODE_OVERRIDE_SIZE:	return "Short code ASR override size is invalid (non-numeric or greater than UINT32_T_MAX).";

	case LM_TSSE_CHECK_EVENT_LOG:			return "can't log events";
	case LM_TSSE_CHECK_PLATFORM	:			return "check not implemented on this platform";
	case LM_TSSE_CHECK_NO_INVALID:			return "check nunmber invalid";
	case LM_TSSE_CHECK_NOT_RUN:				return "check not run";
	case LM_TSSE_CHECK_ANCHOR_WRITE:		return "anchor write error";
	case LM_TSSE_CHECK_ANCHOR_READ:			return "anchor read error";
	case LM_TSSE_CHECK_ANCHOR_DATA:			return "anchor data incorrect";
	case LM_TSSE_CHECK_ANCHOR_REMOVE:		return "anchor remove error";
	case LM_TSSE_CHECK_BINDING_NO_VALUE:	return "can't get a value for any binding entity";
	case LM_TSSE_CHECK_EXCEPTION:			return "exception - see event log";
	case LM_TSSE_CHECK_TS_CREATE:			return "can't create TS file";
	case LM_TSSE_CHECK_GET_COMMON:			return "can't get common section";
	case LM_TSSE_CHECK_CREATE_SECTION:		return "can't create Health Check section";
	case LM_TSSE_CHECK_SET_VALUE:			return "can't get set value in HC section";
	case LM_TSSE_CHECK_GET_VALUE:			return "can't get get value from HC section";
	case LM_TSSE_CHECK_WRONG_VALUE:			return "value read is not value written";
	case LM_TSSE_CHECK_TS_SAVE:				return "can't save	TS";
	case LM_TSSE_CHECK_DELETE_SECTION:		return "can't delete Health Check section";
	case LM_TSSE_CHECK_PREPPED_CONFIG:		return "processing prepped trusted config failed";
	case LM_TSSE_CHECK_OPEN_UNTRUSTED:		return "can't open trusted section even when ignoring trust issues";
	case LM_TSSE_CHECK_OPEN_TRUSTED:		return "section is not trusted";
	case LM_TSSE_CHECK_OPEN_PTC_SECTION:	return "the prepped trusted config section was not created";
	case LM_TSSE_CHECK_DELETE_PTC_SECTION:	return "could not delete the prepped trusted config section";
	case LM_TSSE_CHECK_DELETE_PTC_ANCHORS:	return "could not delete the prepped trusted config section anchors";
	case LM_TSSE_CHECK_SERVICE:				return "service errror";

	default:							return "";
	}
}
