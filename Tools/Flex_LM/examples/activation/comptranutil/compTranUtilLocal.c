/**************************************************************************************************
* Copyright (c) 2017-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This source file is part of a sample utility that demonstrates Composite Transactions.

	It declares the functions that operate on fulfillment records (view, delete, repair).

	Access to fullfillments is supported by the common API and is not specific to composite 
	transactions; the fulfillments may also have been created by local activations, single action
	trnsactions or short code transactions.

	The common API uses two objects for representing data in trusted storage:

		FlxActProdLicSpc	One fulfillment record.
		FlxActLicSpc		A collection of FlxActProdLicSpc's 

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "FlxActCommon.h"
#include "FlxActSvr.h"
#include "FlxActError.h"

#include "compTranUtilHelper.h"
#include "compTranUtilLocal.h"

typedef struct FlxCtuContextLocalStruct
{
	const char*		pszAsrFilename;
	FlxActBool		bFromBuffer;
	char*			pszAsrXml;
	FlxActLicSpc	licSpc;
} FlxCtuContextLocal;

static FlxActBool sCreateLocalContext(FlxCtuContext*	pContext, 
									  const FlxCtuCommand*	pCommand, 
									  FlxCtuContextLocal**	ppLocalContext);
static void			sDestroyLocalContext(FlxCtuContextLocal* pLocalContext);
static FlxActBool	sIsADirectory(const char * pszFilePath);

/***************************************************************************************************
*	flxCtuDoCommandLocal
*
* Process a "local" command - load an ASR, activating the fulfillment record it contains.
***************************************************************************************************/
FlxActBool flxCtuDoCommandLocal(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxCtuContextLocal*	pLocalContext = NULL;
	FlxActBool			bSuccess;

	/* Initailise the context for local activation */
	if ( !sCreateLocalContext( pContext, pCommand, &pLocalContext) )
		return FLX_ACT_FALSE;

	flxCtuPrintMessage(	"Local activation, ASR %s %s\n", 
						pLocalContext->pszAsrFilename,
						(pLocalContext->bFromBuffer)? "(from buffer)" : "");
						

	/* Either add the ASR using the XML read into the local context, or directly using the filename */
	if (pLocalContext->bFromBuffer)
	{
		bSuccess = flxActCommonLicSpcAddASRFromBuffer(pLocalContext->licSpc, 
													  pLocalContext->pszAsrXml);
	}
	else
	{
		bSuccess = flxActCommonLicSpcAddASRs(pLocalContext->licSpc, 
											  pLocalContext->pszAsrFilename);
	}

	if (!bSuccess)
	{
		flxCtuContextPrintLastApiFlexError(pContext);
	}
	else
	{
		flxCtuPrintMessage(	"Local activation successful\n");
	}

	sDestroyLocalContext(pLocalContext);
	return bSuccess;	
}

/***************************************************************************************************
*	flxCtuDoCommandLocalCheck
*
* Process a "localCheck" command - check whether an ASR has already been loaded.
***************************************************************************************************/
FlxActBool flxCtuDoCommandLocalCheck(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxCtuContextLocal*	pLocalContext = NULL;
	FlxActBool			bSuccess;
	uint32_t			trialId;	/* Will be set to the ASR's TrialId, or zero if none */
	time_t				startTime;	/* If the ASR has a TrialId, this will be set to the time 
										that the ASR was added, or zero if it was never added */

	/* Initailise the context for local activation */
	if ( !sCreateLocalContext( pContext, pCommand, &pLocalContext) )
		return FLX_ACT_FALSE;

	flxCtuPrintMessage(	"Local activation check, ASR %s %s\n", 
						pLocalContext->pszAsrFilename,
						(pLocalContext->bFromBuffer)? "(from buffer)" : "");
						
	/* Either check the ASR using the XML read into the local context, or directly using the filename */
	if (pLocalContext->bFromBuffer)
	{
		bSuccess = flxActCommonLicSpcCheckASRFromBuffer(pLocalContext->licSpc, 
														pLocalContext->pszAsrXml, 
														&trialId, 
														&startTime);
	}
	else
	{
		bSuccess = flxActCommonLicSpcCheckASR(pLocalContext->licSpc, 
											  pLocalContext->pszAsrFilename, 
											  &trialId, 
											  &startTime);
	}
	if (!bSuccess)
	{
		flxCtuContextPrintLastApiFlexError(pContext);
	}
	else
	{
		if (trialId != 0)
		{
			if (startTime == 0)
			{
				flxCtuPrintMessage("The ASR's trial (Id %u) has not been started.\n", trialId);
			}
			else
			{
				/*	Only the day of creation is recorded so the time returned by 
					API function flxActCommonLicSpcCheckASR is 0:00 (12am) on that day.
					We do not know explicitly what the local date of creation is.
					We could assume that the offset from UTC time now is the same as it was then, 
					but if it is not (e.g. because of daylight saving) then the date may be 1 day out.
					Rather than risk this we display it in UTC. 
				*/
				struct tm* startTm = gmtime(&startTime);
				flxCtuPrintMessage("The ASR's trial (Id %u) was started on (ymd) %04d/%02d/%02d UTC (Coordinated Universal Time).\n",
					trialId, startTm->tm_year + 1900, startTm->tm_mon + 1, startTm->tm_mday  );
			}
		}
		/*	If the ASR's fullfillment record exists in trusted storage, it will have been 
			added to licSpc
		*/
		if (flxActCommonLicSpcGetNumberProducts(pLocalContext->licSpc) == 0)
		{
			flxCtuPrintMessage("No fulfillment record for this ASR exists in Trusted Storage.\n");
		}
		else
		{
			/* The fulfillment created when the ASR was loaded exists in TS */
			flxCtuPrintMessage("The fulfillment record created when the ASR was loaded exists.\n");
		}
	}

	sDestroyLocalContext(pLocalContext);
	return bSuccess;	
}

/***************************************************************************************************
*	flxCtuDoCommandLocalReset
*
* Process a "localReset" command - reset an ASR, allowing it to be loaded again in the future.
***************************************************************************************************/
FlxActBool flxCtuDoCommandLocalReset(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxCtuContextLocal*	pLocalContext = NULL;
	FlxActBool			bSuccess;

	/* Initailise the context for local activation */
	if ( !sCreateLocalContext( pContext, pCommand, &pLocalContext) )
		return FLX_ACT_FALSE;

	flxCtuPrintMessage(	"Local activation reset, ASR %s %s\n", 
						pLocalContext->pszAsrFilename,
						(pLocalContext->bFromBuffer)? "(from buffer)" : "");
						

	/* Either reset the ASR using the XML read into the local context, or directly using the filename */
	if (pLocalContext->bFromBuffer)
	{
		bSuccess = flxActCommonResetTrialASRFromBuffer(pLocalContext->licSpc, 
													   pLocalContext->pszAsrXml);
	}
	else
	{
		bSuccess = flxActCommonResetTrialASRs(pLocalContext->licSpc, 
											  pLocalContext->pszAsrFilename);
	}

	if (!bSuccess)
	{
		flxCtuContextPrintLastApiFlexError(pContext);
	}
	else
	{
		flxCtuPrintMessage(	"Local reset successful\n");
	}

	sDestroyLocalContext(pLocalContext);
	return bSuccess;	
}

/***************************************************************************************************
*	sCreateLocalContext
*
* Create and initialise a context for use by all loacal activation commands.
***************************************************************************************************/
static FlxActBool sCreateLocalContext(FlxCtuContext*	pContext, 
									  const FlxCtuCommand*	pCommand, 
									  FlxCtuContextLocal**	ppLocalContext)
{
	FlxCtuContextLocal*	pLocalContext = (FlxCtuContextLocal*)flxCtuMallocAndZeroise(sizeof(FlxCtuContextLocal));

	if ( !flxCtuCommandHasValue(pCommand))
	{
		flxCtuCommandPrintError(pCommand, "ASR filename is missing.\n");
		sDestroyLocalContext(pLocalContext);
		return FLX_ACT_FALSE;
	}
	pLocalContext->pszAsrFilename = flxCtuCommandGetValue(pCommand);

	/* Loading or resetting a directory of ASRs is deprecated */
	if ( sIsADirectory(pLocalContext->pszAsrFilename) )
	{
		flxCtuCommandPrintError(pCommand, 
								"%s is a directory, an ASR filename should be specified.\n",
								pLocalContext->pszAsrFilename);
		sDestroyLocalContext(pLocalContext);
		return FLX_ACT_FALSE;
	}

	pLocalContext->bFromBuffer = flxCtuCommandAttributeIsYes(pCommand, CMD_ATTR_FROMBUFFER, FLX_ACT_FALSE);
	if (pLocalContext->bFromBuffer)
	{
		/* Read the ASR XML into memory */
		if ( !flxCtuContextFileLoad(pContext, pLocalContext->pszAsrFilename, &pLocalContext->pszAsrXml, NULL))
		{
			sDestroyLocalContext(pLocalContext);
			return FLX_ACT_FALSE;	/* Error has been reported */
		}
	}

	/* Create a FlxActLicSpc - will contain a FlxActProdLicSpc for the ASR's fulfillment once it is loaded. */ 
	if ( !flxActCommonLicSpcCreate(pContext->hApi, &pLocalContext->licSpc))
	{
		flxCtuCommandPrintError(pCommand, "no memory for licspc\n");
		sDestroyLocalContext(pLocalContext);
		return FLX_ACT_FALSE;
	}

	*ppLocalContext = pLocalContext;
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sDestroyLocalContext
*
***************************************************************************************************/
static void sDestroyLocalContext(FlxCtuContextLocal* pLocalContext)
{
	if (pLocalContext != NULL)
	{
		if (pLocalContext->licSpc != NULL)
			flxActCommonLicSpcDelete(pLocalContext->licSpc);

		if (pLocalContext->bFromBuffer)
			flxCtuFree(pLocalContext->pszAsrXml); /* Copes with NULL */

		flxCtuFree(pLocalContext); /* Copes with NULL */
	}
}

/***************************************************************************************************
*	sIsADirectory
*
***************************************************************************************************/
static FlxActBool sIsADirectory(const char * pszFilePath)
{
	FlxActBool	bIsADir = FLX_ACT_FALSE;	/* default to false */
	struct stat theStat;
	if (pszFilePath != NULL)
	{
		if (	(0 == stat(pszFilePath, &theStat))
			 && ((theStat.st_mode & S_IFDIR) != 0) )
			{
				bIsADir = FLX_ACT_TRUE;
			}
	}
	return bIsADir;
}
