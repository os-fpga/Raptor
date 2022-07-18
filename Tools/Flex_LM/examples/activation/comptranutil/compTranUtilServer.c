/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This file is part of an FNP sample utility that demonstrates Composite Transactions.

	It contains the functions that are appropriate only for server trusted storage.
*/

#include "compTranUtilServer.h"
#include "FlxActSvr.h"

static void sCountPresentAndNonZero(const FlxCtuCommand* pCommand, 
                                    const char* pszAttribute, 
                                    int* pIncIfPresent, 
                                    int* pIncIfNonZero);

/***************************************************************************************************
*	flxCtuServerCommandValidation
*
* Restrict transaction commands to Standalone and 3STS Primary.
***************************************************************************************************/
FlxActBool flxCtuServerCommandValidation(FlxCtuContext* pContext)
{
	const FlxCtuCommand* pCommand;
	FlxActSvrType	serverType;
	FlxActBool		bIsMaster;
	FlxActError		error;

	/* Get server Type. */
	if ( !flxActSvrGetType(&serverType, &bIsMaster, &error))
	{
		flxCtuContextPrintApiFlexError(pContext, &error);
		return FLX_ACT_FALSE;
	}

    /* All servers... */
	/* If there are any request actions... */
	if (flxCtuGetFirstRequestAction(pContext, &pCommand))
	{
        /* For each request action... */
        do
        {
            /* Rule: if any counts are specified at least one must be non-zero. */
            int countPresent = 0;
            int countNonZero = 0;

            /* Get the count information...  
               sCountPresentAndNonZero adds 1 to countPresent and countNonZero if applicable. */
			sCountPresentAndNonZero(pCommand, CMD_ATTR_COUNT,          &countPresent, &countNonZero);
			sCountPresentAndNonZero(pCommand, CMD_ATTR_CONCURRENT,     &countPresent, &countNonZero);
			sCountPresentAndNonZero(pCommand, CMD_ATTR_CONCURRENT_OD,  &countPresent, &countNonZero);
			sCountPresentAndNonZero(pCommand, CMD_ATTR_ACTIVATABLE,    &countPresent, &countNonZero);
			sCountPresentAndNonZero(pCommand, CMD_ATTR_ACTIVATABLE_OD, &countPresent, &countNonZero);
			sCountPresentAndNonZero(pCommand, CMD_ATTR_HYBRID,         &countPresent, &countNonZero);
			sCountPresentAndNonZero(pCommand, CMD_ATTR_HYBRID_OD,      &countPresent, &countNonZero);
			sCountPresentAndNonZero(pCommand, CMD_ATTR_REPAIR_COUNT,   &countPresent, &countNonZero);

			/* If all specified counts are zero... */
            if ((countPresent != 0) && (countNonZero == 0))
			{
				flxCtuCommandPrintError(pCommand, "All the counts specified are zero.\n"); 
				return FLX_ACT_FALSE;
			}
        } while (flxCtuGetNextRequestAction(pContext, &pCommand));
    }

	/* If this is a 3-Server node... */
	if ((serverType != flxActSvrTypeStandalone))
	{
		/* Rule: only the 3STS primary node is allowed to do transactions. */
		/* If there's a transaction command...
		   Note that the transaction and request action commands don't need to be checked as they 
		   can only be specified in addition to one or more of these commands. */
		if (	flxCtuContextHasCommand(pContext, CMD_NEW)
			 ||	flxCtuContextHasCommand(pContext, CMD_PROCESS)	
			 ||	flxCtuContextHasCommand(pContext, CMD_STORED))
		{
			if (serverType != flxActSvrTypePrimary)
			{
				flxCtuCommandPrintError(pContext->pMainCommand, "Only Primary Master can perform 3-Server transactions.\n"); 
				return FLX_ACT_FALSE;
			}
		}

		/* Rule: 3STS request actions can only be created with concurrent or hybrid counts. */
		/* The Enterprise license server prevents hybrid licenses being used for activation. */
		if (flxCtuContextHasCommand(pContext, CMD_NEW))
		{
			/* Iterate over the commands */
			size_t commandNo;
			for (commandNo = 0; commandNo < pContext->commandCount; ++commandNo)
			{
				const FlxCtuCommand* pCommand = pContext->ppCommands[commandNo];

				/* Assert that no activatable count are present. */
				if (	flxCtuCommandHasAttribute(pCommand, CMD_ATTR_ACTIVATABLE)
					 || flxCtuCommandHasAttribute(pCommand, CMD_ATTR_ACTIVATABLE_OD))
				{
					flxCtuCommandPrintError(pCommand, "3-Server requests may only specify concurrent or hybrid counts.\n"); 
					return FLX_ACT_FALSE;
				}
			}
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuServerOnNewRequest
*
* Add request action vendor dictionary entries "3STS_MODE=false" for Standalone servers, else 
* "3STS_MODE=true"
***************************************************************************************************/
FlxActBool flxCtuServerOnNewRequest(FlxCtuContext*	 pContext, 
							        FlxActTranRequest hTranRequest)
{
	FlxActSvrType			serverType;
	FlxActBool				bIsMaster;
	FlxActError				error;
	const char*				pszValue;	
	uint32_t				actionCount,i;

	/* Get server Type. */
	if ( !flxActSvrGetType(&serverType, &bIsMaster, &error))
	{
		flxCtuContextPrintApiFlexError(pContext, &error);
		return FLX_ACT_FALSE;
	}

	/* Set value string false if standalone, else true (3STS) */
	pszValue = (serverType == flxActSvrTypeStandalone)? "false" : "true";

	/* Get Action Count */
	if( !flxActTranRequestGetActionCount(hTranRequest, &actionCount) )
	{
		flxCtuContextPrintApiFlexError(pContext, &error);
		return FLX_ACT_FALSE;
	}

	/* Iterate over the actions */
	for( i=0; i<actionCount; i++ )
	{
		FlxActTranReqAction		hTranReqAction;
		FlxActTranDictionary	hReqActionVendorDictionary;

		/* Get the request action */
		if( !flxActTranRequestGetAction(hTranRequest, i, &hTranReqAction) )
		{
			flxCtuContextPrintApiFlexError(pContext, &error);
			return FLX_ACT_FALSE;
		}

		/* Get a handle to the dictionary. */
		if ( !flxActTranReqActionGetVendorDictionary(hTranReqAction, &hReqActionVendorDictionary))
		{
			flxCtuContextPrintApiFlexError(pContext, &error);
			return FLX_ACT_FALSE;
		}

		/* And add the entry. */
		if ( !flxActTranDictionarySetByKey(hReqActionVendorDictionary, "3STS_MODE", pszValue))
		{
			flxCtuContextPrintApiFlexError(pContext, &error);
			return FLX_ACT_FALSE;
		}
	}
	return FLX_ACT_TRUE;
}

/*
    Helper that returns whether an attribute is present and non-zero.
    Only call for attributes with trait ATTR_VALUE_NUMERIC.
*/
static void sCountPresentAndNonZero(const FlxCtuCommand* pCommand, 
                                    const char* pszAttributeName, 
                                    int* pIncIfPresent,     /* Incrmented by 1 if present.  */
                                    int* pIncIfNonZero)     /* Incrmented by 1 if non-zero. */
{
	if (flxCtuCommandHasAttribute(pCommand, pszAttributeName))
	{
        ++(*pIncIfPresent);
		/* Command parser will have ensured the value is numeric */
		if (0 != atoi(flxCtuCommandGetAttribute(pCommand, pszAttributeName)))
            ++(*pIncIfNonZero);
    }
}

