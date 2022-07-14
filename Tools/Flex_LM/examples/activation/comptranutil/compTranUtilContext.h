/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions.

	It declares the FlxCtuContext (context) object and the functions that relate to it.
	It is the top level object and contains all other objects that persist for the execution
	of the utility. 
*/
#ifndef COMP_TRAN_UTIL_CONTEXT_H
#define COMP_TRAN_UTIL_CONTEXT_H

#include "FlxActTransaction.h"
#include "FlxActShortCode.h"
#include "compTranUtilCommandDef.h"
#include "compTranUtilCommand.h"
#ifdef __cplusplus
extern "C" {
#endif
/* One of these is returned to the OS when the utility exits. */
typedef enum flxCtuExitCode_enum
{
	flxCtuExitSuccess					= 0,
	flxCtuExitInputError				= 1,	/* Invalid command line input.					*/
	flxCtuExitInitError					= 2,	/* FNP initialisation failed.					*/
	flxCtuExitCommsError				= 3,	/* Error communicating with server.				*/
	flxCtuExitResponseProcessingError	= 4,	/* flxActTransactionProcessResponse failed.		*/
	flxCtuExitApiError					= 5,	/* Other FNP API call failed.					*/
	flxCtuExitFileError					= 6,	/* Error accessing file.						*/
	flxCtuExitMemoryError				= 7,	/* No memory.									*/
	flxCtuExitAssert					= 8,	/* An assert failed.							*/
	flxCtuExitCustom					= 9,	/* Fatal error from custom extension module.	*/
	flxCtuExitCancelled					= 10,	/* Cancelled by user ("no" to confirm prompt).	*/

	flxCtuExitUnknown					= 20
} FlxCtuExitCode;


typedef enum flxCtuParameterType_enum
{
	flxCtuParameterTypeCommand,
	flxCtuParameterTypeValue,
	flxCtuParameterTypeKeyValue,
} FlxCtuParameterType;

struct FlxCtuCustomContext_struct;

typedef struct FlxCtuContext_struct 
{
	const char*					pszProgramName;
	FlxCtuExitCode				exitCode;
	FlxActBool					bIsServer;
	FlxActBool					bIsVerbose;
	FlxActBool					bCommonLibraryInitialised;
	FlxActHandle				hApi;
	FlxActTransaction			hTransaction;

	const FlxCtuCommandDef*		pCommandDefs;
	size_t						commandDefCount;
	
	const FlxCtuCommand**		ppCommands;
	size_t						commandCount;
	size_t						actionCount;
    size_t                      requestActionIndex;

	const FlxCtuCommand*		pMainCommand;
	FlxActBool					bIsMainCommandSet;

	size_t						dictionaryCount;
	const FlxCtuDictionary**	ppDictionaries;

	FlxActBool					bprintResponseXml;
	FlxActBool					bIsPresentElsAttribute;
	FlxActBool					bIsElsTransaction;
	FlxActBool					bUsingStoredRequest;

	FlxActShortCode				hShortCode;

	struct FlxCtuCustomContext_struct*	pCCtx;	/* For use by any custom extension */

} FlxCtuContext;


void flxCtuContextCreate(const FlxCtuCommandDef*	pCommandDefs, 
						 size_t						commandDefCount,
						 FlxCtuContext**			ppContext);

void flxCtuContextDestroy(FlxCtuContext* pContext);

FlxActBool flxCtuContextHasCommand(const FlxCtuContext* pContext, const char* pszCommandName);

FlxActBool flxCtuContextGetCommandByName(const FlxCtuContext*	pContext, 
									     const char*			pszCommandName, 
									     const FlxCtuCommand**	ppCommand);

size_t flxCtuContextGetCommandCount(const FlxCtuContext*	pContext);

const FlxCtuCommand* flxCtuContextGetCommand(const FlxCtuContext* pContext, size_t commandIndex);

FlxActBool flxCtuContextFindCommandDef(const FlxCtuContext*		pContext, 
									   const char*				pszComamndOrAlias,
									   const FlxCtuCommandDef**	ppCommandDef);

FlxActBool flxCtuContextValidateCommands(FlxCtuContext* pContext);

/*
	Load a file into memory (free with flxCtuFree).
*/
FlxActBool flxCtuContextFileLoad(FlxCtuContext*	pContext, 
								 const char*	pszFilename, 
								 char**			ppFileContent,
								 size_t*		pContentSize);

/*
	Load file stream content into memory (free with flxCtuFree).
*/
FlxActBool flxCtuContextStreamLoad(FlxCtuContext*	pContext, 
								 FILE*			pFileStream,
								 const char*	pszFilename, 
								 char**			ppFileContent,
								 size_t*		pContentSize);

/*
	Create/replace a file from a buffer.
*/
FlxActBool flxCtuContextFileDump(FlxCtuContext*	pContext, 
								 const char*	pszFilename, 
								 const char*	pFileContent,
								 size_t			contentSize);

void flxCtuContextPrintLastApiError(FlxCtuContext* pContext);

void flxCtuContextPrintApiFlexError(FlxCtuContext* pContext, FlxActError* pFlxError);
void flxCtuContextPrintLastApiFlexError(FlxCtuContext* pContext);

const char* flxCtuGetTextForResult(FlxActTranResult result);
const char* flxCtuGetTextForSystemError(uint32_t systemError);

/* 
    Iterate over request actions. 
*/
FlxActBool flxCtuGetFirstRequestAction(FlxCtuContext*	        pContext, 
									   const FlxCtuCommand**	ppCommand);
FlxActBool flxCtuGetNextRequestAction( FlxCtuContext*	        pContext, 
									   const FlxCtuCommand**	ppCommand);

/*
	Request a confirmation, applying automatic answer if specified on command line
*/
FlxActBool flxCtuContextConfirm(FlxCtuContext* pContext, const char *pszFormatStr, ...);

/*
	Function and macro for one-line error handling for API errors that indicate a developer issue, 
	for example an invalid handle or parameter.
*/
void flxCtuUnexpectedApiError(FlxCtuContext* pContext, const char* pszFilename, int line);
#define FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext)									\
{																					\
	flxCtuUnexpectedApiError(pContext, __FILE__, __LINE__);	/* Does not return */	\
}

void flxCtuUnexpectedApiFlexError(FlxCtuContext* pContext, const char* pszFilename, int line);
#define FLX_CTU_EXIT_UNEXPECTED_API_FLEX_ERROR(pContext)								\
{																						\
	flxCtuUnexpectedApiFlexError(pContext, __FILE__, __LINE__);	/* Does not return */	\
}

/*
	Unexpected error.
	The target function flxCtuOnUnexpectedError must be implemented by users of this module.
*/
void flxCtuOnUnexpectedError(FlxCtuContext* pContext, FlxCtuExitCode exitCode);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
