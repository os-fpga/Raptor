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

#include "version_info.h"
#include "version_hotfix.h"

#include "FlxActCommon.h"
#include "FlxActError.h"
#include "FlxActDiagTypes.h"
#include "FlxActDiag.h"

#include "compTranUtilHelper.h"
#include "compTranUtilMisc.h"

typedef FlxActBool(*VmAccessor)(FlxActHandle handle, const char **ppszAttributeValue);
static FlxActBool sPrintVmAttribute(FlxCtuContext *pContext, 
									const char* pszAttributeName, 
									VmAccessor pfnVmAccessor);
static const char* sGetCheckName(uint32_t checkNo);

/***************************************************************************************************
*	flxCtuDoCommandLocalRepair
*
* Process a "localRepair" command - perform a local repair on trusted storage.
***************************************************************************************************/
FlxActBool	flxCtuDoCommandLocalRepair(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	flxCtuPrintMessage("Performing local repair of trusted storage...\n");

	if (flxActCommonRepairLocalTrustedStorage(pContext->hApi))
	{
		flxCtuPrintMessage("Success\n");
	}
	else
	{
		FLX_CTU_EXIT_UNEXPECTED_API_FLEX_ERROR(pContext);	/* Does not return */
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuDoCommandUnique
*
* Process a "unique" command - print Unique Machine Number values.
***************************************************************************************************/
FlxActBool	flxCtuDoCommandUnique(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	struct UmnNoAndName
	{
		FlxUMNType UmnNo;
		const char* UmnName;
	} UMNs[] =	{	{ flxUMNTypeOne,	"1" },
					{ flxUMNTypeTwo,	"2" },
					{ flxUMNTypeThree,	"3" },	/* Virtual environments only		*/
					{ flxUMNTypeFive,   "5" }	/* Generation ID - Virtual environments & currently Windows only */
				};

	size_t		index;
	const char* pszUMN;
	FlxActBool	bSuccess = FLX_ACT_TRUE;

	flxCtuPrintMessage("Unique Machine Numbers (blank if not available in this environment):\n");
	for (index = 0; index < sizeof(UMNs)/sizeof(UMNs[0]); index ++)
	{
		flxCtuPrintMessage(	"  UMN%s = ", UMNs[index].UmnName);
		pszUMN = flxActCommonHandleGetUniqueMachineNumber(pContext->hApi, UMNs[index].UmnNo);
		if (pszUMN != NULL)
		{
			flxCtuPrintMessage(pszUMN);
		}
		else
		{
			FlxActError error;
			FlxActBool	bNotAvailable;
			flxActCommonHandleGetError(pContext->hApi, &error);
			bNotAvailable = (error.majorErrorNo == LM_TS_CANT_FIND);
			if ( !bNotAvailable)
			{
				flxCtuPrintMessage(	"  ERROR (%d,%d,%d)", 
									error.majorErrorNo, error.minorErrorNo, error.sysErrorNo);
				bSuccess = FLX_ACT_FALSE;
			}
		}
		flxCtuPrintMessage(	"\n");
	}
	return bSuccess;
}

/***************************************************************************************************
*	flxCtuDoCommandVersion
*
* Process a "version" command - print application and licensing service versions.
***************************************************************************************************/
FlxActBool	flxCtuDoCommandVersion(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	uint32_t Major, Minor, Maint, Hotfix, BuildNo, BuildYear, BuildMonth, BuildDay;
	uint32_t SvcMajor, SvcMinor, SvcMaint, SvcHotfix, SvcBuildNo, SvcBuildYear, SvcBuildMonth, SvcBuildDay;

	/* Activation libraries used to prep this application */
	if (	flxActCommonHandleGetVersion(pContext->hApi, &Major, &Minor, &Maint, &Hotfix)
		 && flxActCommonHandleGetBuildInfo(pContext->hApi, &BuildNo, &BuildYear, &BuildMonth, &BuildDay))
	{
		flxCtuPrintMessage("%20.20s version %u.%u.%u.%u build %u %u/%02u/%02u\n", pContext->pszProgramName, 
							Major, Minor, Maint, Hotfix, BuildNo, BuildYear, BuildMonth, BuildDay);
	}
	else
	{
		FLX_CTU_EXIT_UNEXPECTED_API_FLEX_ERROR(pContext);	/* Does not return */
	}

	/* Licensing service that is running on this system */
	if (	flxActCommonHandleGetServiceVersion(pContext->hApi, &SvcMajor, &SvcMinor, &SvcMaint, &SvcHotfix)
		 && flxActCommonHandleGetServiceBuildInfo(pContext->hApi, &SvcBuildNo, &SvcBuildYear, &SvcBuildMonth, &SvcBuildDay))
	{
		flxCtuPrintMessage("   Licensing Service version %u.%u.%u.%u build %u %u/%02u/%02u\n", 
							SvcMajor, SvcMinor, SvcMaint, SvcHotfix, SvcBuildNo, SvcBuildYear, SvcBuildMonth, SvcBuildDay);
	}
	else
	{
		FlxActError error;
		flxActCommonHandleGetError(pContext->hApi, &error);
		
		/* LM_TSSE_SERVICE_NONE means this platform doesn't have a service so just print nothing, otherwise an error */
		if ( error.sysErrorNo != LM_TSSE_SERVICE_NONE)
			FLX_CTU_EXIT_UNEXPECTED_API_FLEX_ERROR(pContext);	/* Does not return */
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuDoCommandVirtual
*
* Process a "virtual" command - print virtualization information.
***************************************************************************************************/
FlxActBool	flxCtuDoCommandVirtual(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxActBool	bSuccess   = FLX_ACT_TRUE;
	FlxActBool	bIsVirtual = FLX_ACT_FALSE;
	
	if ( !flxActCommonVirtualStatusGet(pContext->hApi, &bIsVirtual) )
	{
		FlxActError error;
		flxActCommonHandleGetError(pContext->hApi, &error);
		if (error.majorErrorNo == LM_TS_NOT_THIS_PLATFORM)
		{
			flxCtuPrintMessage("Virtualization detection not supported on this platform, assuming physical.\n");
			bIsVirtual = FLX_ACT_FALSE;
		}
		else
			FLX_CTU_EXIT_UNEXPECTED_API_FLEX_ERROR(pContext);
	}

	if (bIsVirtual)
	{
		flxCtuPrintMessage("This is a virtual platform.\n");
		flxCtuPrintMessage("Attributes:\n");

		bSuccess &= sPrintVmAttribute(pContext,"FAMILY",flxActCommonVirtualFamilyGet);
		bSuccess &= sPrintVmAttribute(pContext,"NAME  ",flxActCommonVirtualNameGet);
		bSuccess &= sPrintVmAttribute(pContext,"UUID  ",flxActCommonVirtualUuidGet);
		bSuccess &= sPrintVmAttribute(pContext,"GENID ",flxActCommonVirtualGenidGet);
	}
	else
	{
		flxCtuPrintMessage("This is a physical platform.\n");
	}
	return bSuccess;
}

/***************************************************************************************************
*	flxCtuDoCommandHealthCheck
*
* Process a "healthCheck" command - perform a health check.
***************************************************************************************************/
FlxActBool	flxCtuDoCommandHealthCheck(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	uint32_t	resultLength = flxTsHcCount;
	uint32_t*	pResults = (uint32_t*)flxCtuMalloc(sizeof(uint32_t) * resultLength); 
	uint32_t	checkNo ;
	FlxActBool	bSuccess;

	flxCtuPrintMessage("Trusted storage health check....\n");

	bSuccess = flxActDiagTrustedStorageHealthCheck(pResults, flxTsHcCount);

	for (checkNo = 0; checkNo < resultLength; ++checkNo)
	{
		uint32_t result			= pResults[checkNo];
		const char* pCheckName	= sGetCheckName(checkNo);

		flxCtuPrintMessage("%20.20s %u: ", sGetCheckName(checkNo), checkNo);
		if (pResults[checkNo] == LM_TSSE_NO_ERROR)
			flxCtuPrintMessage("Pass\n");
		else if (pResults[checkNo] == LM_TSSE_CHECK_PLATFORM)
			flxCtuPrintMessage("Not implemented on this platform\n");
		else if (pResults[checkNo] == LM_TSSE_CHECK_NOT_RUN)
			flxCtuPrintMessage("Not run\n");
		else
			flxCtuPrintMessage("FAIL %u %s\n", pResults[checkNo], flxCtuGetTextForSystemError(pResults[checkNo]));
	}

	pContext->exitCode = bSuccess? flxCtuExitSuccess : flxCtuExitApiError;

	return bSuccess;
}


/***************************************************************************************************
* Helper to print a virtual attribute, given its name and accessor function.
***************************************************************************************************/
static FlxActBool sPrintVmAttribute(FlxCtuContext *pContext, 
									const char* pszAttributeName, 
									VmAccessor pfnVmAccessor)
{
	FlxActBool  bSuccess			= FLX_ACT_TRUE;
	const char* pszVmAttributeValue = NULL;

	/* Get the attribute from the accessor function */
	if ( (pfnVmAccessor)(pContext->hApi, &pszVmAttributeValue ) )
	{
		flxCtuPrintMessage(	"  %s = %s\n", pszAttributeName, pszVmAttributeValue);

		/* We are responsible for freeing the memory allocated by the accessor... */
		free((char*)pszVmAttributeValue);
	}
	else
	{
		FlxActError error;
		flxActCommonHandleGetError(pContext->hApi, &error);

		if (error.majorErrorNo == LM_TS_NOT_THIS_PLATFORM)
		{
			/* Not available on this platform */
			flxCtuPrintMessage("  %s   Not available on this platform\n", pszAttributeName);
			/* Not an error */
		}
		else
		{
			const char* pszErrorText =	
				(error.majorErrorNo == LM_TS_VM_ATTRIBUTE_PRIVILEGE) ?	"Insufficient privilege." :
				(error.majorErrorNo == LM_TS_VM_ATTRIBUTE_PROTECTED) ?	"Protected resource encountered." :
																		"";
			flxCtuPrintMessage(	"  %s   ERROR - (%d,%d,%d) %s\n", 
								pszAttributeName, 
								error.majorErrorNo, error.minorErrorNo, error.sysErrorNo,
								pszErrorText);

			pContext->exitCode = flxCtuExitApiError;
			bSuccess = FLX_ACT_FALSE;
		}
	}

	return bSuccess;
}

/***************************************************************************************************
* Helper to return the name of a Health Check item
***************************************************************************************************/
static const char* sGetCheckName(uint32_t checkNo)
{
	switch (checkNo)
	{
	case flxTsHcEventLog:			return "Event log";
	case flxTsHcLicensingService:	return "Licensing service";
	case flxTsHcAnchoring:			return "Anchoring";
	case flxTsHcBinding:			return "Binding";
	case flxTsHcFile:				return "TS file";
	case flxTsHcSection:			return "TS section";
	default:						return "Unkown";
	}
}

