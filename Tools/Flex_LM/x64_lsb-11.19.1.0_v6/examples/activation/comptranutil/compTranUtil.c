/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This sample utility demonstrates FNP Composite Transactions.

	It is intended as a resource for developers, validators and documenters of 
	FNP and FNP enabled products.  It is not intended for release to end-users, 
	rather as a model for the development of activation utilites for FNP enabled
	products.

	IN PARTICULAR the functionality it provides for cancelling stored requests 
	could be used by fraudulent users to obtain additional licences using a manual
	(offline) return transaction - by cancelling the request instead of processing
	the response.  This ability may need to be provided so that bona fide users 
	who start a return transaction but cannot complete can continue to check out 
	from the fulfillment.  The business rules must define the criteria for 
	allowing return requests to be cancelled.

	It is a command line utility written in 'C' to provide maximum coverage of 
	supported FNP platforms.  It is built and released as part of the kit in both
	source and binary form; it is re-built by the kit make file (makefile.act).
	It has extensive command line options, providing the ability to create 
	requests with all the elements define by the XML schema.

	The makefile builds the executable comptranutil and then uses preptool to
	produce two separate executables, one for use with application trusted 
	storage and one for server trusted storage (appcomptranutil and 
	servercomptranutil respectively).

	It uses the API defined by header FlxActTransaction.h to create composite
	requests and process their responses.  It can also manage outstanding 
	requests in trusted storage.
	
	As a convenience it can also 
	* View and delete fulfillment records.
	* View Enterprise License Server fulfillment records.

	Source files

		compTranUtil.c					Main entry (this file)
		compTranUtilArgParser.c			Parse command line arguments to internal objects.
		compTranUtilCommand.c			Command support (get value, attributes, dictionary etc.)
		compTranUtilCommandDef.c		Definition of command line (includes -help)
		compTranUtilContext.c			Context (command list, options, status, handles etc.)
		compTranUtilCreateRequest.c		Composite request creation (-new -activate -return -repair)
		compTranUtilCustom.c			A facility for custom extensions - see comments in the file.
		compTranUtilFulfillments.c		Fulfillment Record view and delete (-view, -deleteFR).
		compTranUtilHelper.c			Helper functions (memory, string, print) 
		compTranUtilManageRequests.c	Manage stored requests (-listRequests, -cancelRequests)
		compTranUtilMisc.c				Miscellaneous command (-umn, -virtual, -version)
		compTranUtilResponse.c			Process responses (-process)
		compTranUtilSend.c				Send response to server and process response (-transaction)

	Function naming conventions

		flxActTranXXX		Composite transaction API functions defined in FlxActTransaction.h 
		flxActCommonXXX		Other Activation API functions defined in FlxActCommon.h 
		flxCtuXXX			Functions within this utility.

	Usage

		Use the -help command to print a usage summary; it defines the other help options, e.g.
			appcomptranutil -help
			servercomptranutil -help

	Error handling.

		Unexpected errors cause an immediate exit; this is an attempt to minimise error code:
			
			FLX_CTU_ASSERT						A developer error
			FLX_CTU_EXIT_UNEXPECTED_ERROR		Either an error specific to this machine, e.g.
												unable to read or write the trusted storage file,
												or a developer error, e.g. invalid handle.
		
		Errors that can reasonably be expected are reported explicitly. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FlxActCommon.h"
#include "FlxActError.h"

#include "FlxActTransaction.h"

#include "compTranUtilContext.h"
#include "compTranUtilCommandDef.h"
#include "compTranUtilArgParser.h"
#include "compTranUtilServer.h"
#include "compTranUtilCreateRequest.h"
#include "compTranUtilFulfillments.h"
#include "compTranUtilManageRequests.h"
#include "compTranUtilResponse.h"
#include "compTranUtilSend.h"
#include "compTranUtilHelper.h"
#include "compTranUtilCustom.h"
#include "compTranUtilShortCode.h"
#include "compTranUtilLocal.h"
#include "compTranUtilMisc.h"

/* Private functions. */
static void sMainCommon(int argc, char * argv[]);
static void sExitOs(int exitCode);
static void sExit(FlxCtuContext* pContext, FlxCtuExitCode exitCode);
static const char* sGetExitCodeString(FlxCtuExitCode exitCode);
static void sPrintCommonLibraryInitError(int errorCode);

#ifdef PC
static char *sAllocConvertWCToUTF8( const wchar_t * pwszInput );
/***************************************************************************************************
*	Windows entry and exit
*	======================
*
* Arguments in UTF-16 need to be converted to UTF-8 before calling sMainCommon
*
***************************************************************************************************/
static char** sAllocArgv = NULL;
int wmain(int argc, wchar_t * wargv[])
{
    int index;

    /* Allocate an array of char pointers; the extra element will be NULL to terminate the array. */
    sAllocArgv = (char**)flxCtuMallocAndZeroise( (argc + 1) * sizeof(char*));

    /* For each argument, convert to UTF-8 in a newly allocated string and add to sAllocArgv. */
    for ( index = 0; index < argc; ++index)
        sAllocArgv[index] = sAllocConvertWCToUTF8(wargv[index]);

    /* Pass aruments to sMainCommon - it calls sExitOs on completion (does not return). */
    sMainCommon(argc, sAllocArgv);
    return 0;
}

void sExitOs(int exitCode)
{
    int index = 0;
    /* Free allocated arguments and the argument array itself. */
    while ( sAllocArgv[index] != NULL )
        flxCtuFree(sAllocArgv[index++]);
    flxCtuFree(sAllocArgv);

    exit(exitCode);
}
#else
/***************************************************************************************************
*	Non-Windows entry and exit
*	==========================
*
* No argument conversion needed - just call sMainCommon
*
***************************************************************************************************/
int main(int argc, char * argv[])
{
    /* Pass arguments to sMainCommon - it calls sExitOs on completion (does not return). */
    sMainCommon( argc, argv );
    return 0;
}
void sExitOs(int exitCode)
{
    exit(exitCode);
}
#endif

/***************************************************************************************************
*	Common main entry point
*	=======================
*
*	argc	Number of command line arguments
*	argv	Array of command line arguments UTF-8 encoded
*
* For usage see flxCtuHelpXXX functions in compTranUtilCommandDef.c
*
***************************************************************************************************/
void sMainCommon(int argc, char * argv[])
{
	FlxActTranResult		tranResult;
	FlxActError				flxError;
	FlxActMode				mode = FLX_ACT_PREP;
	int						intResult = 0;

	const FlxCtuCommandDef* pCommandDefs = NULL; 
	size_t					commandDefCount = 0;
	FlxCtuContext*			pContext = NULL;
	FlxActBool				bSuccess = FLX_ACT_FALSE;
	const FlxCtuCommand*	pCommand = NULL;
	
	/* Get the command definitions - these define the arguments and options that can be used. */
	commandDefCount = flxCtuCommandDefsCreate(&pCommandDefs);

	/* Create a context to hold the commands parsed from the command line, status etc. */
	flxCtuContextCreate(pCommandDefs, commandDefCount, &pContext);

	/* Set the program name (argv[0]) in the context for use in messages. */
	FLX_CTU_ASSERT(argc > 0);
	pContext->pszProgramName = flxCtuStrDupNameFromPath(argv[0]);
	pContext->bprintResponseXml = FLX_ACT_FALSE;

	/* Parse the commands from the remaining command line arguments using the command definitions.
	   Creates a set of command objects within the context object. */
	if ( !flxCtuArgsParseToContext(pContext, (const char**)&argv[1], argc - 1))
		sExit(pContext, flxCtuExitInputError);

	/* Validate the commands - even though they conform to the definitions there are extra 
	   validation rules, for example -transaction requires either -new or -stored as well. */
	if ( !flxCtuContextValidateCommands(pContext) )
		sExit(pContext, flxCtuExitInputError);

	/* Process the "Brief" command (suppresses messages). */
	if (flxCtuContextHasCommand(pContext, CMD_BRIEF))
		flxCtuSetBrief(FLX_ACT_TRUE);

	/* Initilaize timer and enable time printing if CMD_TIME was specified. */
	flxCtuTimeStart(flxCtuContextHasCommand(pContext, CMD_TIME));

	/* Handle custom extensions */
	if ( flxCtuContextGetCommandByName(pContext, CMD_CUSTOM, &pCommand) )	
	{
		flxCtuDoCommandCustom(pContext, pCommand);
		sExit(pContext, pContext->exitCode);
	}

	/* Initialise the Activation API. This will fail if this executable has not been
	   processed with preptool since it was last built or if any FNP components are missing. */
	intResult = flxActCommonLibraryInit((const char*)0);
	if (intResult != 0)
	{
		sPrintCommonLibraryInitError(intResult);
		sExit(pContext, flxCtuExitInitError);
	}
	pContext->bCommonLibraryInitialised = FLX_ACT_TRUE;
	flxCtuTimePrintInterval("flxActCommonLibraryInit");

	/* Note server or application mode. */
	pContext->bIsServer	= (flxActCommonGetProtectionMode() == FLX_ACT_SVR)? 
						  FLX_ACT_TRUE : FLX_ACT_FALSE;

	/* Process any diagnostic commands. */
	if (flxCtuContextGetCommandByName(pContext, CMD_HEALTH_CHECK, &pCommand))
	{
		FlxActBool bSuccess = flxCtuDoCommandHealthCheck(pContext, pCommand);
		sExit(pContext, pContext->exitCode);
	}
	
	/* If this is server TS, call the server function for any additional validation. */
	if (pContext->bIsServer)
	{
		if ( !flxCtuServerCommandValidation(pContext) )
			sExit(pContext, pContext->exitCode);
	}

	/* Process the "Help" command - it is implicit if no commands at all are given. */
	pCommand = NULL;	/* In case no commands. */
	if ((pContext->commandCount == 0) || flxCtuContextGetCommandByName(pContext, CMD_HELP, &pCommand))
	{
		flxCtuCommandHelp(pContext, pCommand);
		sExit(pContext, flxCtuExitSuccess);
	}
	
	/* Get a handle for the Activation API. */
	if ( !flxActCommonHandleOpen(&pContext->hApi, mode, &flxError) )
	{
		flxCtuContextPrintApiFlexError(pContext, &flxError);
		flxCtuOnUnexpectedError(pContext, flxCtuExitApiError);
	}

	flxCtuTimePrintInterval("flxActCommonHandleOpen");

	/* Process commands that need an API handle but not a Transaction object. */
	/* Command  -view */
	if ( flxCtuContextGetCommandByName(pContext, CMD_VIEW, &pCommand) )
		bSuccess = flxCtuDoCommandView(pContext, pCommand);
	/* Command  -viewlong */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_VIEW_LONG, &pCommand) )
		bSuccess = flxCtuDoCommandViewLong(pContext, pCommand);
	/* Command  -serverview */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_SERVER_VIEW, &pCommand) )
		bSuccess = flxCtuDoCommandViewServer(pContext, pCommand);
	/* Command  -serverviewlong */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_SERVER_VIEW_LONG, &pCommand) )
		bSuccess = flxCtuDoCommandViewServerLong(pContext, pCommand);
	/* Command  -deleteFR */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_DELETE_FR, &pCommand) )
		bSuccess = flxCtuDoCommandDeleteFR(pContext, pCommand);
	/* Command  -shortCode */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_SHORTCODE, &pCommand) )
		bSuccess = flxCtuDoCommandShortCode(pContext, pCommand);
	/* Command  -shortCodeResponse */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_SHORTCODE_RESP, &pCommand) )
		bSuccess = flxCtuDoCommandShortCodeResponse(pContext, pCommand);
	/* Command  -localCheck */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_LOCAL_CHECK, &pCommand) )
		bSuccess = flxCtuDoCommandLocalCheck(pContext, pCommand);
	/* Command  -local */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_LOCAL, &pCommand) )
		bSuccess = flxCtuDoCommandLocal(pContext, pCommand);
	/* Command  -localReset */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_LOCAL_RESET, &pCommand) )
		bSuccess = flxCtuDoCommandLocalReset(pContext, pCommand);
	/* Command  -localRepair */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_LOCAL_REPAIR, &pCommand) )
		bSuccess = flxCtuDoCommandLocalRepair(pContext, pCommand);
	/* Command  -unique */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_UNIQUE, &pCommand) )
		bSuccess = flxCtuDoCommandUnique(pContext, pCommand);
	/* Command  -virtual */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_VIRTUAL, &pCommand) )
		bSuccess = flxCtuDoCommandVirtual(pContext, pCommand);
	/* Command  -version */
	else if ( flxCtuContextGetCommandByName(pContext, CMD_VERSION, &pCommand) )
		bSuccess = flxCtuDoCommandVersion(pContext, pCommand);
	else
	{
		/* Create a transaction object - all the following commands require one. */
		bSuccess = flxActTransactionCreate(pContext->hApi, &pContext->hTransaction, &tranResult);
		if ( !bSuccess)
		{
			/* If reason for failure is TS load fail... */
			if (tranResult == FLX_ACT_TRAN_ERR_TS_LOAD)
			{
				/* It's probably because it is untrusted so do a local repair and try again. */
				(void)flxActCommonRepairLocalTrustedStorage(pContext->hApi);
				bSuccess = flxActTransactionCreate(pContext->hApi, &pContext->hTransaction, &tranResult);
			}
		}
		flxCtuTimePrintInterval("flxActTransactionCreate");

		if ( !bSuccess)
		{
			flxCtuPrintError("flxActTransactionCreate failed %d\n", tranResult);
			sExit(pContext, flxCtuExitInitError);
		}

		/* Command  -transaction */
		if ( flxCtuContextGetCommandByName(pContext, CMD_TRANSACTION, &pCommand) )
			bSuccess = flxCtuDoCommandTransaction(pContext, pCommand);

		/* Command  -new */
		else if ( flxCtuContextGetCommandByName(pContext, CMD_NEW, &pCommand) )
			bSuccess = flxCtuDoCommandNew(pContext, pCommand);

		/* Command  -stored */
		else if ( flxCtuContextGetCommandByName(pContext, CMD_STORED, &pCommand) )
			bSuccess = flxCtuDoCommandStored(pContext, pCommand);

		/* Command  -process */
		else if ( flxCtuContextGetCommandByName(pContext, CMD_PROCESS, &pCommand) )
			bSuccess = flxCtuDoCommandProcess(pContext, pCommand);

		/* Command  -list */
		else if ( flxCtuContextGetCommandByName(pContext, CMD_LIST_REQUEST, &pCommand) )
			bSuccess = flxCtuDoCommandListRequests(pContext, pCommand);
		
		/* Command  -cancel */
		else if ( flxCtuContextGetCommandByName(pContext, CMD_CANCEL_REQUEST, &pCommand) )
			bSuccess = flxCtuDoCommandCancelRequest(pContext, pCommand);
		
		/* Command  -cancelAll */
		else if ( flxCtuContextGetCommandByName(pContext, CMD_CANCEL_ALL, &pCommand) )
			bSuccess = flxCtuDoCommandCancelAll(pContext, pCommand);

		else
		{
			flxCtuPrintError("Nothing to do.\n");
			sExit(pContext, flxCtuExitInputError);
		}
	}

	/* Exit. If there has been an error, the exit code will have been set in the context. */
	if (bSuccess)
		sExit(pContext, flxCtuExitSuccess);
	else
		sExit(pContext, pContext->exitCode);
}

/***************************************************************************************************
*	sExit 
*
* Clean up objects held in the context and the context itself, print the exit code and exit.
***************************************************************************************************/
static void sExit(FlxCtuContext* pContext, FlxCtuExitCode exitCode)
{
	if (pContext != NULL)
	{
		flxCtuCustomOnExit(pContext);

		flxCtuContextDestroy(pContext);
	}

	if (exitCode != flxCtuExitSuccess)
		flxCtuPrintMessage("Exit(%d) %s.\n", exitCode, sGetExitCodeString(exitCode));

	flxCtuTimePrintElapsed("Total");
	sExitOs((int)exitCode);
}

/******************************************************************************
*	sGetExitCodeString
*
* Return a string describing an exit code.
******************************************************************************/
static const char* sGetExitCodeString(FlxCtuExitCode exitCode)
{
	switch (exitCode)
	{
		case flxCtuExitSuccess:					return "success";
		case flxCtuExitInputError:				return "error in command line";
		case flxCtuExitInitError:				return "FlexNet initialisation error";
		case flxCtuExitCommsError:				return "communications error";
		case flxCtuExitResponseProcessingError:	return "error when processing response";
		case flxCtuExitApiError:				return "unexpected API failure";
		case flxCtuExitFileError:				return "file access error";
		case flxCtuExitMemoryError:				return "no memory";
		case flxCtuExitAssert:					return "assert";
		case flxCtuExitCustom:					return "fatal error from custom extension module";
		case flxCtuExitCancelled:				return "the user did not confirm the action requested so it was not performed.";
		default:								return "";
	}				
}

/******************************************************************************
*	flxInitLoad error codes
******************************************************************************/

typedef enum FlxInitStatus
{
	flxInitSuccess 					= 0, 	/* Load was successful */
	flxInitUnableToLocate			= 1,	/* Unable to find security runtime */
	flxInitUnableToLoad				= 2,	/* Unable to load security runtime */
	flxInitUnsupportedPlatform		= 3,	/* OS version too old */
	flxInitInitializationError		= 4,	/* Initialization of security runtime failed */
	flxInitAllocationError			= 5,	/* Unable to allocate resources */
	flxInitNotProtected				= 6,	/* preptool has not been run since build */
	flxInitCannotReload				= 7,   	/* Call to flxInitLoad after an flxInitUnload */

	/* Mac OS X specific error values */
	flxInitEncodingError			= 8,	/* path string not in UTF-8? */
	flxInitUrlError					= 9,	/* error creating url */
	flxInitUrlNoPathError			= 10, 	/* error creating path from url */
	flxInitFrameworkNotLoadedError	= 11,	/* framework specified by bundle ID not loaded */
	flxInitBundleIDError			= 12,	/* Not a valid bundle ID */
	flxInitPathOverflow				= 13,	/* computed path is too long */

	/* Service error values */
	flxInitServiceNotInstalled		= 20,	/* service not installed */
	flxInitServiceNotEnoughRights	= 21	/* not enough rights to talk to service.
											Set service to start automatically to resolve this */
} FlxInitStatus;

/***************************************************************************************************
*	sPrintCommonLibraryInitError
*
* Print an error message, including text for the common causes of initialisation errors.
***************************************************************************************************/
static void sPrintCommonLibraryInitError(int errorCode)
{
	const char* pszErrorText = "";	/* Default. */

	switch (errorCode)
	{
	case flxInitUnableToLocate:				
						pszErrorText = "Unable to find FNP security runtime shared object";
						break;
	case flxInitNotProtected:			
						pszErrorText = "FNP preptool has not been run on this executable";
						break;
	case flxInitServiceNotInstalled:	
						pszErrorText = "The FlexNet Licensing Service is not installed on this machine.";
						break;
	case flxInitServiceNotEnoughRights:	
						pszErrorText = "Can't start FlexNet Licensing Service (try setting automatic start)";
						break;
	}
	flxCtuPrintError("flxActCommonInit result %u   %s.\n", errorCode, pszErrorText);
}

/***************************************************************************************************
*	flxCtuOnNoMemory
*
* This will be called by the memory allocation functions on error.
* No messages to avoid using further resources.
***************************************************************************************************/
void flxCtuOnNoMemory()
{
	sExitOs((int)flxCtuExitMemoryError);
}

/***************************************************************************************************
*	flxCtuOnUnexpectedError
*
* Target of unexpected fatal API errors - prints a message and exits.
***************************************************************************************************/
void flxCtuOnUnexpectedError(FlxCtuContext* pContext, FlxCtuExitCode exitCode)
{
	sExit(pContext, exitCode);
}

/***************************************************************************************************
*	flxCtuOnAssert
*
* Target of failed asserts - prints a message and exits.
***************************************************************************************************/
void flxCtuOnAssert(int line, const char* pszFilename)
{
	flxCtuPrintError("Assert failed at line %u of file %s\n", line, pszFilename);
	sExitOs((int)flxCtuExitAssert);
}

#ifdef PC
/***************************************************************************************************
*	sConvertToUTF8
*
* Windows-only UTF-8 to UTF-8 convsersion.
***************************************************************************************************/
#include "lmclient.h"
static
int
sConvertToUTF8(
        const wchar_t * pwszInput,
        char *          pszOutput,
        int             iSize)
{
        if(!pwszInput)
                return 0;
        /*
         *      Do conversion IF iSize != 0, else return number of wchar_t's needed.
         */
        return WideCharToMultiByte(CP_UTF8,                         /* code page */
													0,              /* required to be zero for CP_UTF8 */
													pwszInput,      /* name to convert */
													-1,             /* string is NULL terminated */
													pszOutput,      /* ignored if iSize = 0 */
													iSize,          /* if iSize is 0, calc */
													NULL,           /* use system default for unknown chars */
													NULL);          /* don't care if defaut char used */

}

char *
sAllocConvertWCToUTF8( const wchar_t * pwszInput )
{
	char *pszOutput = NULL;
	int piSize = 0;
	int status = 0;

	piSize = sConvertToUTF8(pwszInput,NULL,0);
	if(piSize)
	{
		pszOutput = (char *) flxCtuMallocAndZeroise(sizeof(char) * ((piSize) + 1));
		if(pszOutput)
		{
			status = sConvertToUTF8(pwszInput, pszOutput, piSize + 1);
			if(status)
				return pszOutput;
		}
	}
	
    return NULL;
}
#endif 
