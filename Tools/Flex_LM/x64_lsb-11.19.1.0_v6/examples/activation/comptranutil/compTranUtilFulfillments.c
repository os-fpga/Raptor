/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
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

#include "FlxActCommon.h"
#include "FlxActSvr.h"
#include "FlxActError.h"

#include "compTranUtilFulfillments.h"

typedef struct FlxCtuFulfillmentFilter_struct
{
	FlxActBool	bViewTrusted;
	FlxActBool	bViewUntrusted;
	FlxActBool	bViewEnabled;
	FlxActBool	bViewDisabled;
	const char*	pszFidFilter;
	const char*	pszEidFilter;
	const char*	pszPidFilter;
	const char*	pszFeatureFilter;

	uint32_t	totalCount;
	uint32_t	passCount;
} FlxCtuFulfillmentFilter;

typedef enum FlxCtuViewType_enum
{
	flxCtuViewTypeShort	= 0,
	flxCtuViewTypeLong,
	flxCtuViewTypeLongWithCounts,
	flxCtuViewTypeFull
} FlxCtuViewType;

typedef struct FlxCtuViewFilterData_struct
{
	FlxActBool		bPrintHeading;
	FlxCtuViewType	viewType;
	const char*		pszServer;
} FlxCtuViewFilterData;

typedef FlxActBool (*FilterCallback)(FlxCtuContext*		pContext,
									 FlxActLicSpc		licSpec, 
									 FlxActProdLicSpc	prodSpec,
									 void*				pData);

static FlxActBool sAddRepairAction(FlxCtuContext*	pContext,
								   FlxActLicSpc		licSpec, 
								   FlxActProdLicSpc	prodSpec,
								   void*			pData);

static FlxActBool sDeleteFulfillment(FlxCtuContext*		pContext,
									 FlxActLicSpc		licSpec, 
									 FlxActProdLicSpc	prodSpec,
									 void*				pData);

static FlxActBool sForEachFilterPass(FlxCtuContext*				pContext, 
									 FlxCtuFulfillmentFilter*	pFilter,
									 FilterCallback				callbackFn,
									 void*						pData);

static FlxActBool sForEachFilterPassLicSpec(FlxCtuContext*				pContext, 
										    FlxActLicSpc				licSpec, 
											FlxCtuFulfillmentFilter*	pFilter,
											FilterCallback				callbackFn,
											void*						pData);

static const char* sGetFulfillmentTypeString(FlxActProdLicSpc prodSpec);
static const char* sGetFulfillmentTrustFlagsString(FlxActProdLicSpc prodSpec);
static uint32_t sGetNonOverdraftCountTotal(FlxActProdLicSpc prodSpec);
static void		  sInitFilterCounts(FlxCtuFulfillmentFilter* pFilter);
static void       sInitFilterAll(FlxCtuFulfillmentFilter* pFilter);
static void		  sSetFilterEid(FlxCtuFulfillmentFilter* pFilter, const char* pszEid);
static FlxActBool sIsFilterPass(FlxActProdLicSpc prodSpec, const FlxCtuFulfillmentFilter* pFilter);
static FlxActBool sHasThisFeatureToken(FlxActProdLicSpc prodSpec, const char* pszFeatureName);
static FlxActBool sHasThisFulfillmentId(FlxActProdLicSpc prodSpec, const char* pszFulfillmentId);
static FlxActBool sHasThisProductId(FlxActProdLicSpc prodSpec, const char* pszProductId);
static FlxActBool sHasThisEntitlementId(FlxActProdLicSpc prodSpec, const char* pszEntitlementId);
static FlxCtuViewType sGetViewType(const FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
static FlxCtuViewType sGetServerViewType(const FlxCtuCommand* pCommand, FlxCtuViewType theDefault);
static void sSetFilter(const FlxCtuCommand* pCommand, FlxCtuFulfillmentFilter* pFilter);

static FlxActBool sView(FlxCtuContext*			pContext, 
						const FlxCtuCommand*	pCommand, 
						FlxCtuViewType			viewType);

static FlxActBool sViewServer(	FlxCtuContext*			pContext, 
								const FlxCtuCommand*	pCommand, 
								FlxCtuViewType			viewType);

static void sViewFilteredFulfillments(FlxCtuContext*			pContext, 
									  FlxCtuFulfillmentFilter*	pFilter,
									  FlxCtuViewType			viewType);
static FlxActBool sViewFulfillment(FlxCtuContext*	pContext,
								   FlxActLicSpc		licSpec, 
								   FlxActProdLicSpc	prodSpec,
								   void*			pData);
static FlxActBool sViewServerFulfillment(FlxCtuContext*		pContext,
										 FlxActLicSpc		licSpec, 
										 FlxActProdLicSpc	prodSpec,
										 void*				pData);
static void sViewFulfillmentShort(FlxActProdLicSpc prodSpec);
static void sViewFulfillmentLong(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec);
static void sViewFulfillmentCounts(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec);
static void sViewDeductions(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec);
static void sViewVendorDictionary(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec);
static void sViewNameAndFeatures(const char* pszName, FlxActProdLicSpc prodSpec);
static void sViewNameAndValue(const char* pszName, const char* pszValue);
static void sViewNameAndUint32(const char* pszName, uint32_t value);
static void sViewNameAndUint32IfNonZero(const char* pszName, uint32_t value);

typedef struct FrStateCallbackData_struct
{
	FlxActBool bIsSet;
	uint32_t   trustFlags;
	FlxActBool bIsDisabled;
	uint32_t   deductionCount;
	uint32_t   validDeductionCount;	/* Excludes repairs and expired. */
} FrStateCallbackData;

static FlxActBool sGetStateData(FlxCtuContext*			pContext, 
								const char*				pszFulfillmentId,
								FrStateCallbackData*	pStateData);

static FlxActBool sFrStateCallback(FlxCtuContext*	pContext,
								   FlxActLicSpc		licSpec, 
								   FlxActProdLicSpc	prodSpec,
								   void*			pData);

static const char* sGetServerTypeText(FlxActSvrType serverType);

static FlxActBool sFrDeleteCallback(FlxCtuContext*		pContext,
								    FlxActLicSpc		licSpec, 
								    FlxActProdLicSpc	prodSpec,
								    void*				pData);

/***************************************************************************************************
*	flxCtuDoCommandDeleteFR
*
* Process a deleteFR command - delete fulfillment record(s).
***************************************************************************************************/
FlxActBool flxCtuDoCommandDeleteFR(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxCtuFulfillmentFilter	filter;

	/* Set the filtering choices from the command value and attributes. */
	sSetFilter(pCommand, &filter);

	/* Give the user a preview of the FRs that will be deleted. */
	sViewFilteredFulfillments(pContext, &filter, flxCtuViewTypeShort);

	if (filter.totalCount == 0)
		flxCtuPrintMessage("\nNo fulfillment records in trusted storage.\n");
	else if (filter.passCount == 0)
		flxCtuPrintMessage("No records to delete.\n");
	else
	{
		if (flxCtuContextConfirm(pContext,
								 "\n%u record%s to delete. Confirm?\n",
								 filter.passCount,
								 (filter.passCount == 1)? "" : "s"))
		{
			/* Call sDeleteFulfillment for each FR that meets the filter criteria. */
			if ( !sForEachFilterPass(pContext, &filter, sDeleteFulfillment, NULL))
				return FLX_ACT_FALSE;
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuDoCommandView
*
* Process a view command - view fulfillment records.
***************************************************************************************************/
FlxActBool flxCtuDoCommandView(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	/* Get the view type (short, long counts) from the command attributes. */
	FlxCtuViewType viewType = sGetViewType(pContext, pCommand);

	return sView(pContext, pCommand, viewType);
}

/***************************************************************************************************
*	flxCtuDoCommandViewLong
*
* Process a view long command - view fulfillment records.
***************************************************************************************************/
FlxActBool flxCtuDoCommandViewLong(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	/* If view type is flxCtuViewTypeShort, override with flxCtuViewTypeLongWithCounts (server)
	   or flxCtuViewTypeLong. */
	FlxCtuViewType viewType = sGetViewType(pContext, pCommand);
	if (viewType == flxCtuViewTypeShort)
		viewType = (pContext->bIsServer)? flxCtuViewTypeLongWithCounts : flxCtuViewTypeLong;

	return sView(pContext, pCommand, viewType);
}

/***************************************************************************************************
*	flxCtuDoCommandViewServer
*
* Process a view enterprise license server command - view its fulfillment records.
***************************************************************************************************/
FlxActBool flxCtuDoCommandViewServer(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxCtuViewType viewType = sGetServerViewType(pCommand, flxCtuViewTypeShort);

	return sViewServer(pContext, pCommand, viewType);
}

/***************************************************************************************************
*	flxCtuDoCommandViewServerLong
*
* Process a view enterprise license server command - view its fulfillment records.
***************************************************************************************************/
FlxActBool flxCtuDoCommandViewServerLong(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxCtuViewType viewType = sGetServerViewType(pCommand, flxCtuViewTypeLongWithCounts);

	return sViewServer(pContext, pCommand, viewType);
}

/***************************************************************************************************
*		flxCtuWarnIfEntitlementAlreadyUsed
*
* Iterates over the FRs and warns if any have the supplied RightId as their EntitlementId
* This is to avoid accidental repeat activations.
*
* User can choose to continue (automatic if --brief is specified).
* Returns FLX_ACT_TRUE for continue.
***************************************************************************************************/
FlxActBool flxCtuWarnIfEntitlementAlreadyUsed(FlxCtuContext* pContext, const char* pszEntitlementId)
{
	FlxCtuFulfillmentFilter	filter;

	/* Set the filter for the specified EntitlementId. */
	sSetFilterEid(&filter, pszEntitlementId);

	/* Give the user a preview of the FRs that have the EntitlementId. */
	sViewFilteredFulfillments(pContext, &filter, flxCtuViewTypeShort);

	if (filter.passCount > 0)
	{
		return flxCtuContextConfirm(pContext, 
									"\nThere are existing fulfillment(s) with Entitlement Id %s.\nContinue?\n",
									pszEntitlementId);
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sView
*
* Process a view or viewlong command - view fulfillment records.
***************************************************************************************************/
FlxActBool sView(FlxCtuContext* pContext, const FlxCtuCommand* pCommand, FlxCtuViewType viewType)
{
	FlxCtuFulfillmentFilter	filter;

	/* Print the trusted storage type. */
	if (pContext->bIsServer)
	{
		/* Server TS - get the server type */
		FlxActSvrType			serverType;
		FlxActBool				bIsMaster;
		FlxActError				error;

		if ( !flxActSvrGetType(&serverType, &bIsMaster, &error))
		{
			flxCtuContextPrintApiFlexError(pContext, &error);
			return FLX_ACT_FALSE;
		}

		if (serverType == flxActSvrTypeStandalone)
		{
			flxCtuPrintMessage("Viewing server trusted storage.\n");
		}
		else
		{
			/* 3-Server, print the node type and whether master. */
			flxCtuPrintMessage("Viewing 3-Server trusted storage, node type: %s %s\n", 
							   sGetServerTypeText(serverType),
							   bIsMaster? " currently MASTER" : "");
		}
	}
	else
	{
		flxCtuPrintMessage("Viewing application trusted storage.\n"); 
	}
	
	/* Get the filtering choices from the command value and attributes. */
	sSetFilter(pCommand, &filter);

	/* View the fulfillments that meet the filter criteria. */
	sViewFilteredFulfillments(pContext, &filter, viewType);

	if (filter.totalCount == 0)
		flxCtuPrintMessage("\nNo fulfillment records in trusted storage.\n");
	else
		flxCtuPrintMessage("\nViewed %d of %d fulfillment records.\n", 
						   filter.passCount,
						   filter.totalCount);

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sViewServer
*
* Get fulfillments from the server and print.
***************************************************************************************************/
static FlxActBool sViewServer(	FlxCtuContext*			pContext, 
								const FlxCtuCommand*	pCommand, 
								FlxCtuViewType			viewType)
{
	FlxActBool				bSuccess;
	FlxActLicSpc			licSpec		= NULL;		/* A container for FlxActProdLicSpc's. */
	FlxActProdLicSpc		prodSpec	= NULL;		/* Represents one FR. */
	FlxCtuFulfillmentFilter	filter;
	FlxCtuViewFilterData	filterData;
	const char*				pszServer	= flxCtuCommandGetValueOrDefault(pCommand, "@localhost");

	/* Reset the interval timer */
	flxCtuTimeResetInterval();

	/* Get the FR Counts from the server */

	/* Set up the FR filter from the command attributes;
	   the FID needs setting additionally as it is an attribute of this command rather than the value. 
	  */
	sSetFilter(pCommand, &filter);
	filter.pszFidFilter = flxCtuCommandGetAttributeOrDefault(pCommand, CMD_ATTR_FID, NULL);

	/* Create the container. */
	if ( !flxActCommonLicSpcCreate(pContext->hApi, &licSpec) )
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}

	/* Main heading. */
	flxCtuPrintMessage("Reading fulfillments from server %s\n", pszServer);
	if (filter.pszFidFilter != NULL)
		flxCtuPrintMessage("    Fulfillment Id: %s\n", filter.pszFidFilter);
	else 
	{	
		if (filter.pszEidFilter != NULL)
			flxCtuPrintMessage("    Entitlement Id: %s\n", filter.pszEidFilter);
		if (filter.pszPidFilter != NULL)
			flxCtuPrintMessage("        Product Id: %s\n", filter.pszPidFilter);
	}

	/* Populate the container with server FRs (limited data) */
	if ( !flxActCommonLicSpcPopulateAllFromServerTS(licSpec, 
													filter.pszEidFilter, 
													filter.pszPidFilter, 
													pszServer))
	{
		flxCtuPrintError("Can't read fulfillments from server %s\n", pszServer);
		flxCtuContextPrintLastApiFlexError(pContext);
		flxActCommonLicSpcDelete(licSpec);
		return FLX_ACT_FALSE;
	}

	/* Print the time taken */
	flxCtuTimePrintInterval("load server fulfillments");

	flxCtuPrintMessage("%u fulfillments read\n", flxActCommonLicSpcGetNumberProducts(licSpec));

	/* Set up filter data - if short view, print headings. If FID or feature specified, force viewType FULL */
	filterData.viewType = viewType;
	if (filter.pszFidFilter != NULL || filter.pszFeatureFilter != NULL)
		filterData.viewType	= flxCtuViewTypeFull;
	filterData.bPrintHeading = (filterData.viewType == flxCtuViewTypeShort)? FLX_ACT_TRUE : FLX_ACT_FALSE;
	filterData.pszServer	 = pszServer;

	/* If flxCtuViewTypeFull, request full details of each fulfillment */
	if (filterData.viewType	== flxCtuViewTypeFull)
	{
		FlxActProdLicSpc	prodSpec	= NULL;		/* Represents one FR. */
		uint32_t			index;

		/* Iterate over the fulfillments. */
		for (index = 0; index < flxActCommonLicSpcGetNumberProducts(licSpec); index++)
		{
			prodSpec = flxActCommonLicSpcGet(licSpec, index);
			flxCtuPrintMessage("Reading full details for fulfillment id %s\n", 
								flxActCommonProdLicSpcFulfillmentIdGet(prodSpec));
			if ( !flxActCommonProdLicSpcPopulateFRFromServerTS(prodSpec, pszServer))
			{
				flxCtuPrintError(	"Can't read fulfillment %s from server %s\n", 
									flxActCommonProdLicSpcFulfillmentIdGet(prodSpec),
									pszServer);
				flxCtuContextPrintLastApiFlexError(pContext);
				flxActCommonLicSpcDelete(licSpec);
				return FLX_ACT_FALSE;
			}
			flxCtuTimePrintInterval("load full server fulfillment");
		}
	}

	/* Call sViewFulfillment for each FR that meets the filter criteria. */
	bSuccess = sForEachFilterPassLicSpec(pContext, licSpec, &filter, sViewFulfillment, &filterData);

	flxCtuPrintMessage("\n%u of %u fulfillments viewed\n", filter.passCount, filter.totalCount);

	flxActCommonLicSpcDelete(licSpec);
	return bSuccess;
}

/***************************************************************************************************
*	flxCtuFulfillmentsAddRepairActions
*
* Add a repair action to the current response for each fulfillment record that is not fully trusted.
***************************************************************************************************/
FlxActBool flxCtuFulfillmentsAddRepairActions(FlxCtuContext* pContext, uint32_t* pCount)
{
	FlxActTranRequest		hTranRequest;
	FlxCtuFulfillmentFilter filter;

	/* Get the current request. */
	if ( !flxActTransactionGetRequest(pContext->hTransaction, &hTranRequest) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	/* Create a filter for untrusted FRs (exclude trusted ones). */
	sInitFilterAll(&filter);
	filter.bViewTrusted = FLX_ACT_FALSE;

	/* Call sAddRepairAction for each untrusted FR. */
	if ( !sForEachFilterPass(pContext, &filter, sAddRepairAction, NULL))
		return FLX_ACT_FALSE;

	/* Return the pass count to the caller. */
	*pCount = filter.passCount;
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuFulfillmentGetState
*
* Return the trusted and enabled state of an FR.
***************************************************************************************************/
FlxActBool flxCtuFulfillmentGetState(FlxCtuContext* pContext, 
									 const char*	pszFulfillmentId,
									 FlxActBool*	pbIsTrusted, 
									 FlxActBool*	pbIsEnabled)
{
	FlxCtuFulfillmentFilter filter;
	FrStateCallbackData     stateData;

	/* Set up a filter for the required FulfillmentId. */
	sInitFilterAll(&filter);
	filter.pszFidFilter = pszFulfillmentId;

	/* Initialise the state data in case the Fr is not found. */
	stateData.bIsSet = FLX_ACT_FALSE;

	/* Get the state for that Fulfillment. */
	sForEachFilterPass(pContext, &filter, sFrStateCallback, (void*)&stateData);

	/* Check the Fr was found. */
	if ( !stateData.bIsSet)
		return FLX_ACT_FALSE;

	/* Return the state. */
	*pbIsEnabled = (stateData.bIsDisabled)? FLX_ACT_FALSE : FLX_ACT_TRUE;
	*pbIsTrusted = (stateData.trustFlags == FLAGS_FULLY_TRUSTED)? FLX_ACT_TRUE : FLX_ACT_FALSE;
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuFulfillmentGetDeductionCount
*
* Return the deduction count of an FR (all deductions).
***************************************************************************************************/
FlxActBool flxCtuFulfillmentGetDeductionCount(FlxCtuContext* pContext, 
											  const char*	 pszFulfillmentId,
											  uint32_t*		 pDeductionCount)
{
	FrStateCallbackData     stateData;

	/* Get the state data for the Fulfillment. */
	if ( !sGetStateData(pContext, pszFulfillmentId, &stateData))
		return FLX_ACT_FALSE;	/* FR not found. */

	/* Return the count. */
	*pDeductionCount = stateData.deductionCount;
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuFulfillmentGetValidDeductionCount
*
* Return the "valid" deduction count of an FR (excludes repairs and expired).
***************************************************************************************************/
FlxActBool flxCtuFulfillmentGetValidDeductionCount(FlxCtuContext*	pContext, 
												   const char*		pszFulfillmentId,
												   uint32_t*		pValidDeductionCount)
{
	FrStateCallbackData     stateData;

	/* Get the state data for the Fulfillment. */
	if ( !sGetStateData(pContext, pszFulfillmentId, &stateData))
		return FLX_ACT_FALSE;	/* FR not found. */

	/* Return the count. */
	*pValidDeductionCount = stateData.validDeductionCount;
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	flxCtuFulfillmentViewVendorDictionary
*
* Display any vendor dictionary entries.
***************************************************************************************************/
void flxCtuFulfillmentViewVendorDictionary(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec)
{
	sViewVendorDictionary(pContext, prodSpec);
}

/***************************************************************************************************
*	flxCtuFulfillmentDelete
*
* Delete an FR given its FID.
***************************************************************************************************/
FlxActBool flxCtuFulfillmentDelete(FlxCtuContext* pContext, const char*	 pszFulfillmentId)
{
	FlxActBool bSuccess = FLX_ACT_TRUE;	/* Callback sets false if delete fails. */

	/* Set up a filter for the FID */
	FlxCtuFulfillmentFilter	filter;
	sInitFilterAll(&filter);
	filter.pszFidFilter = pszFulfillmentId;

	/* Find and delete the Fulfillment. */
	sForEachFilterPass(pContext, &filter, sFrDeleteCallback, (void*)&bSuccess);
	return bSuccess;
}

/***************************************************************************************************
*	sAddRepairAction
*
* Add a repair action for this fulfillment to the current request.
***************************************************************************************************/
static FlxActBool sAddRepairAction(FlxCtuContext*	pContext,
								   FlxActLicSpc		licSpec, 
								   FlxActProdLicSpc	prodSpec,
								   void*			pData)
{
	FlxActTranRequest	hTranRequest;
	const char*			pszFulfillmentId;
	FlxActTranReqAction	hAction;

	/* Get the current request. */
	if ( !flxActTransactionGetRequest(pContext->hTransaction, &hTranRequest) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	/* Get the fulfillment id. */
	pszFulfillmentId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);

	/* Create a new repair action and add it to the request. */
	if ( !flxActTranRequestAddAction(hTranRequest, FLX_ACT_TRAN_REQ_ACTION_REPAIR, &hAction) )
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);

	/* Specify this FR by setting the action's FulfillmentId attribute. */
	if ( !flxActTranReqActionSetAttribute(hAction, 
										  FLX_ACT_TRAN_REQ_ACT_FULFILLMENT_ID, 
										  pszFulfillmentId) )
	{
		FLX_CTU_EXIT_UNEXPECTED_API_ERROR(pContext);
	}

	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sDeleteFR
*
* Delete an FR (by deleting the prodSpec from its licSpec container).
***************************************************************************************************/
static FlxActBool sDeleteFulfillment(FlxCtuContext*		pContext,
									 FlxActLicSpc		licSpec, 
									 FlxActProdLicSpc	prodSpec,
									 void*				pData)
{
	flxCtuPrintMessage("Deleting fulfillment %s\n", 
					   flxActCommonProdLicSpcFulfillmentIdGet(prodSpec));

	if ( !flxActCommonLicSpcProductDelete(licSpec, prodSpec))
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sForEachFilterPass
*
* Call callbackFn for each FR in trusted storage that passes the filter criteria.
***************************************************************************************************/
static FlxActBool sForEachFilterPass(FlxCtuContext*				pContext, 
									 FlxCtuFulfillmentFilter*	pFilter,
									 FilterCallback				callbackFn,
									 void*						pData)
{
	FlxActLicSpc	licSpec		= NULL;		/* A container for FlxActProdLicSpc's. */
	FlxActBool		bSuccess;

	/* Reset the interval timer */
	flxCtuTimeResetInterval();

	/* Create the container. */
	if ( !flxActCommonLicSpcCreate(pContext->hApi, &licSpec) )
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return FLX_ACT_FALSE;
	}
	
	/* Fill the container with all FRs. */
	if ( !flxActCommonLicSpcPopulateAllFromTS(licSpec) )
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		flxActCommonLicSpcDelete(licSpec);
		return FLX_ACT_FALSE;
	}

	/* Print the time taken */
	flxCtuTimePrintInterval("load fulfillments");

	bSuccess = sForEachFilterPassLicSpec(pContext, licSpec, pFilter,callbackFn, pData);

	flxActCommonLicSpcDelete(licSpec);
	return bSuccess;
}

/***************************************************************************************************
*	sForEachFilterPassLicSpec
*
* Call callbackFn for each FR (FlxActProdLicSpc) in the FlxActLicSpc container supplied.
***************************************************************************************************/

static FlxActBool sForEachFilterPassLicSpec(FlxCtuContext*				pContext, 
										    FlxActLicSpc				licSpec, 
											FlxCtuFulfillmentFilter*	pFilter,
											FilterCallback				callbackFn,
											void*						pData)
{
	FlxActProdLicSpc	prodSpec	= NULL;		/* Represents one FR. */
	uint32_t			index;

	/* Initialise the counts. */
	pFilter->totalCount = flxActCommonLicSpcGetNumberProducts(licSpec);
	pFilter->passCount  = 0;

	/* Iterate over the fulfillments. */
	for (index = 0; index < pFilter->totalCount; index++)
	{
		prodSpec = flxActCommonLicSpcGet(licSpec, index);
		if (sIsFilterPass(prodSpec, pFilter))
		{	/* Meets filter criteria - call callbackFn.	*/
			if ( !(callbackFn)(pContext, licSpec, prodSpec, pData) )
			{
				return FLX_ACT_FALSE;
			}
			pFilter->passCount++;
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sGetStateData
*
* Return the state data of an FR.
***************************************************************************************************/
FlxActBool sGetStateData(FlxCtuContext*			pContext, 
						 const char*			pszFulfillmentId,
						 FrStateCallbackData*	pStateData)
{
	FlxCtuFulfillmentFilter filter;

	/* Set up a filter for the required FulfillmentId. */
	sInitFilterAll(&filter);
	filter.pszFidFilter = pszFulfillmentId;

	/* Initialise the state data in case the Fr is not found. */
	pStateData->bIsSet = FLX_ACT_FALSE;

	/* Set the state for that Fulfillment. */
	sForEachFilterPass(pContext, &filter, sFrStateCallback, (void*)pStateData);

	return pStateData->bIsSet;	/*FLX_ACT_TRUE if FR found */
}

/***************************************************************************************************
*	sFrStateCallback
*
* Sets the FR state in the pData (FrStateCallbackData).
***************************************************************************************************/
static FlxActBool sFrStateCallback(FlxCtuContext*	pContext,
								   FlxActLicSpc		licSpec, 
								   FlxActProdLicSpc	prodSpec,
								   void*			pData)
{
	FrStateCallbackData* pStateData = (FrStateCallbackData*)pData;

	pStateData->trustFlags			= flxActCommonProdLicSpcTrustFlagsGet(prodSpec);
	pStateData->bIsDisabled			= flxActCommonProdLicSpcIsDisabled(prodSpec);
	pStateData->deductionCount		= flxActCommonProdLicSpcNumberDedSpcGet(prodSpec);
	pStateData->validDeductionCount	= flxActCommonProdLicSpcNumberValidDedSpcGet(prodSpec);
	pStateData->bIsSet				= FLX_ACT_TRUE;
	return FLX_ACT_FALSE;	/* No need to continue the iteration. */
}

/***************************************************************************************************
*	sGetFulfillmentTrustFlagsString
*
* Return a printable string for the trust flags of a fulfillment record.
***************************************************************************************************/
static const char* sGetFulfillmentTrustFlagsString(FlxActProdLicSpc prodSpec)
{
	uint32_t trustFlags = flxActCommonProdLicSpcTrustFlagsGet(prodSpec);
	static char szTrustString[64];

	if (trustFlags == FLAGS_FULLY_TRUSTED)
	{
		sprintf(szTrustString, "0x%X Fully Trusted", trustFlags);
	}
	else
	{
		sprintf(szTrustString, 
			"0x%X **BROKEN** %s%s%s", 
			trustFlags,
			((trustFlags & FLAGS_TRUST_RESTORE) == 0)? "Restore " : "",
			((trustFlags & FLAGS_TRUST_HOST)    == 0)? "Host "    : "",
			((trustFlags & FLAGS_TRUST_TIME)    == 0)? "Time "    : ""
			);
	}
	return szTrustString;
}

/***************************************************************************************************
*	sGetFulfillmentTypeString
*
* Return a printable string for the type of a fulfillment record.
***************************************************************************************************/
static const char* sGetFulfillmentTypeString(FlxActProdLicSpc prodSpec)
{
	static const char *szType[] = {	"0 Unknown", 
									"1 Trial", 
									"2 Shortcode",  
									"3 Emergency", 
									"4 Served Activation", 
									"5 Served Overdraft Activation",
									"6 Publisher Activation", 
									"7 Publisher Overdraft Activation"};

	FlxFulfillmentType fulfillmentType = flxActCommonProdLicSpcFulfillmentTypeGet(prodSpec);
	if ( (unsigned int)fulfillmentType >= (sizeof(szType) / sizeof(szType[0])) )
		fulfillmentType = FULFILLMENT_TYPE_UNKNOWN;
	return szType[fulfillmentType];
}

/***************************************************************************************************
*	sGetNonOverdraftCountTotal
*
* Return the sum of the basic counts.
***************************************************************************************************/
static uint32_t sGetNonOverdraftCountTotal(FlxActProdLicSpc prodSpec)
{
	return	flxActCommonProdLicSpcCountGet(prodSpec, CONCURRENT)
		  +	flxActCommonProdLicSpcCountGet(prodSpec, ACTIVATABLE)
		  +	flxActCommonProdLicSpcCountGet(prodSpec, HYBRID);
}

/***************************************************************************************************
*	sGetViewType
*
* Returns the format of the view output from the command attributes.
***************************************************************************************************/
static FlxCtuViewType sGetViewType(const FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	FlxCtuViewType viewType = flxCtuViewTypeShort;
	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_FORMAT))
	{
		if (flxCtuCommandAttributeIs(pCommand, CMD_ATTR_FORMAT, ATTR_VALUE_LONG))
		{
			viewType = flxCtuViewTypeLong;

			/* Shows counts if viewing server TS or if command attribute "counts=yes" */
			if (flxCtuCommandAttributeIsYes(pCommand, "counts", pContext->bIsServer))
				viewType = flxCtuViewTypeLongWithCounts;
		}
	}
	return viewType;
}

/***************************************************************************************************
*	sGetServerViewType
*
* Returns the format of the server view output from the command attributes.
***************************************************************************************************/
static FlxCtuViewType sGetServerViewType(const FlxCtuCommand* pCommand, FlxCtuViewType theDefault)
{
	FlxCtuViewType viewType = theDefault;
	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_FORMAT))
	{
		if (flxCtuCommandAttributeIs(pCommand, CMD_ATTR_FORMAT, ATTR_VALUE_SHORT))
			viewType = flxCtuViewTypeLong;
		else if (flxCtuCommandAttributeIs(pCommand, CMD_ATTR_FORMAT, ATTR_VALUE_LONG))
			viewType = flxCtuViewTypeLongWithCounts;
		else if (flxCtuCommandAttributeIs(pCommand, CMD_ATTR_FORMAT, ATTR_VALUE_FULL))
			viewType = flxCtuViewTypeFull;
	}
	return viewType;
}

/***************************************************************************************************
*	sInitFilterAll
*
* Initialise the filter so that all records will pass.
***************************************************************************************************/
static void sInitFilterAll(FlxCtuFulfillmentFilter* pFilter)
{
	pFilter->bViewTrusted		= FLX_ACT_TRUE;
	pFilter->bViewUntrusted		= FLX_ACT_TRUE;
	pFilter->bViewEnabled		= FLX_ACT_TRUE;
	pFilter->bViewDisabled		= FLX_ACT_TRUE;

	pFilter->pszFidFilter		= NULL;
	pFilter->pszPidFilter		= NULL;
	pFilter->pszEidFilter		= NULL;
	pFilter->pszFeatureFilter	= NULL;

	sInitFilterCounts(pFilter);
}

/***************************************************************************************************
*	sInitFilterCounts
*
* Initialise the pass and total counts.
***************************************************************************************************/
static void sInitFilterCounts(FlxCtuFulfillmentFilter* pFilter)
{
	pFilter->totalCount = 0;
	pFilter->passCount  = 0;
}

/***************************************************************************************************
*	sIsFilterPass
*
* Returns FLX_ACT_TRUE if the record passes the filter criteria.
***************************************************************************************************/
static FlxActBool sIsFilterPass(FlxActProdLicSpc prodSpec, const FlxCtuFulfillmentFilter* pFilter)
{
	FlxActBool bIsPass = FLX_ACT_FALSE;

	if ((pFilter->pszFidFilter == NULL) || sHasThisFulfillmentId(prodSpec, pFilter->pszFidFilter))
	{
		/* Passes FulfillmentId filter. */
		if ((pFilter->pszPidFilter == NULL) || sHasThisProductId(prodSpec, pFilter->pszPidFilter))
		{
			/* Passes ProductId filter. */
			if (   (pFilter->pszEidFilter == NULL)
				|| sHasThisEntitlementId(prodSpec, pFilter->pszEidFilter))
			{
				/* Passes Entitlement filter. */
				if (   (pFilter->pszFeatureFilter == NULL)
					|| sHasThisFeatureToken(prodSpec, pFilter->pszFeatureFilter))
				{
					/* Passes Feature filter. */
					uint32_t trustFlags = flxActCommonProdLicSpcTrustFlagsGet(prodSpec);
					if (   (pFilter->bViewTrusted   && (trustFlags == FLAGS_FULLY_TRUSTED))
						|| (pFilter->bViewUntrusted && (trustFlags != FLAGS_FULLY_TRUSTED)) )
					{
						/* Passes Trusted filter. */
						FlxActBool bIsDisabled = flxActCommonProdLicSpcIsDisabled(prodSpec);
						if (   (pFilter->bViewEnabled  && !bIsDisabled)
							|| (pFilter->bViewDisabled &&  bIsDisabled) )
						{
							/* Passes Disabled filter. */
							bIsPass = FLX_ACT_TRUE;
						}
					}
				}
			}
		}
	}
	return bIsPass;
}

/***************************************************************************************************
*	sHasThisFeatureToken 
*
* Returns FLX_ACT_TRUE if the token appears anywhere in the feature line. 
***************************************************************************************************/
static FlxActBool sHasThisFeatureToken(FlxActProdLicSpc prodSpec, const char* pszFeatureToken)
{
	const char* pszFeatureLine = flxActCommonProdLicSpcFeatureLineGet(prodSpec);

	if (pszFeatureLine == NULL)
		return FLX_ACT_FALSE;

	return (strstr(pszFeatureLine, pszFeatureToken) == NULL)? FLX_ACT_FALSE : FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sHasThisFulfillmentId
*
* Returns FLX_ACT_TRUE if the record has the supplied FulfillmentId. 
***************************************************************************************************/
static FlxActBool sHasThisFulfillmentId(FlxActProdLicSpc prodSpec, const char* pszFulfillmentId)
{
	return (0 == strcmp(pszFulfillmentId, flxActCommonProdLicSpcFulfillmentIdGet(prodSpec)));
}

/***************************************************************************************************
*	sHasThisProductId
*
* Returns FLX_ACT_TRUE if the record has the supplied ProductId. 
***************************************************************************************************/
static FlxActBool sHasThisProductId(FlxActProdLicSpc prodSpec, const char* pszProductId)
{
	const char* pszFrProductId = flxActCommonProdLicSpcProductIdGet(prodSpec);

	if (pszFrProductId == NULL)
		return FLX_ACT_FALSE;

	return (0 == strcmp(pszProductId, pszFrProductId));
}

/***************************************************************************************************
*	sHasThisEntitlementId
*
* Returns FLX_ACT_TRUE if the record has the supplied EntitlementId. 
***************************************************************************************************/
static FlxActBool sHasThisEntitlementId(FlxActProdLicSpc prodSpec, const char* pszEntitlementId)
{
	const char* pszFrEntitlementId = flxActCommonProdLicSpcEntitlementIdGet(prodSpec);

	if (pszFrEntitlementId == NULL)
		return FLX_ACT_FALSE;

	return (0 == strcmp(pszEntitlementId, pszFrEntitlementId));
}

/***************************************************************************************************
*	sSetFilter
*
* Set the filter criteria from the command value and attributes. 
***************************************************************************************************/
static void sSetFilter(const FlxCtuCommand* pCommand, FlxCtuFulfillmentFilter* pFilter)
{
	sInitFilterAll(pFilter);

	pFilter->bViewTrusted		= !flxCtuCommandAttributeIs(pCommand, CMD_ATTR_TRUSTED, "no");
	pFilter->bViewUntrusted		= !flxCtuCommandAttributeIs(pCommand, CMD_ATTR_TRUSTED, "yes");
	pFilter->bViewEnabled		= !flxCtuCommandAttributeIs(pCommand, CMD_ATTR_ENABLED, "no");
	pFilter->bViewDisabled		= !flxCtuCommandAttributeIs(pCommand, CMD_ATTR_ENABLED, "yes");

	if (flxCtuCommandHasValue(pCommand))
		pFilter->pszFidFilter = flxCtuCommandGetValue(pCommand);

	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_PID))
		pFilter->pszPidFilter = flxCtuCommandGetAttribute(pCommand, CMD_ATTR_PID);

	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_EID))
		pFilter->pszEidFilter = flxCtuCommandGetAttribute(pCommand, CMD_ATTR_EID);

	if (flxCtuCommandHasAttribute(pCommand, CMD_ATTR_FEATURE))
		pFilter->pszFeatureFilter = flxCtuCommandGetAttribute(pCommand, CMD_ATTR_FEATURE);
}

/***************************************************************************************************
*	sSetFilterEid
*
* Set the filter criteria for supplied Entitlement Id. 
***************************************************************************************************/
static void sSetFilterEid(FlxCtuFulfillmentFilter* pFilter, const char* pszEid)
{
	sInitFilterAll(pFilter);
	pFilter->pszEidFilter = pszEid;
}

/***************************************************************************************************
*	sViewFilteredFulfillments
*
* Print details of the fulfillments that pass the filter criteria.
***************************************************************************************************/
static void sViewFilteredFulfillments(FlxCtuContext*			pContext, 
									  FlxCtuFulfillmentFilter*	pFilter,
									  FlxCtuViewType			viewType)
{
	FlxCtuViewFilterData viewFilterData;

	/* Set up filter data - if short view, print headings. */
	viewFilterData.viewType		 = viewType;
	viewFilterData.bPrintHeading = (viewType == flxCtuViewTypeShort)? FLX_ACT_TRUE : FLX_ACT_FALSE;

	/* Call sViewFulfillment for each FR that meets the filter criteria. */
	(void)sForEachFilterPass(pContext, pFilter, sViewFulfillment, (void*)&viewFilterData);
}

/***************************************************************************************************
*	sViewFulfillment
*
* Print details of one fulfillment.
***************************************************************************************************/
static FlxActBool sViewFulfillment(FlxCtuContext*	pContext,
								   FlxActLicSpc		licSpec, 
								   FlxActProdLicSpc	prodSpec,
								   void*			pData)
{
	FlxCtuViewFilterData* viewFilterData = (FlxCtuViewFilterData*)pData;

	if (viewFilterData->bPrintHeading)
	{
		flxCtuPrintMessage(
			"\n"
			"U if untrusted\n"
			". D if disabled\n"
			". . fulfillmentType\n" 
			". . . Expiry    Count FulfillmentId                   ProductId\n"
			"\n");

		viewFilterData->bPrintHeading = FLX_ACT_FALSE;	/* Just once */
	}

	switch (viewFilterData->viewType)
	{
	case flxCtuViewTypeLong:
		sViewFulfillmentLong(pContext, prodSpec);
		break;
	case flxCtuViewTypeLongWithCounts:
	case flxCtuViewTypeFull:
		sViewFulfillmentCounts(pContext, prodSpec);
		sViewDeductions(pContext, prodSpec);
		break;

	case flxCtuViewTypeShort:
		/* Drop through. */
	default:
		sViewFulfillmentShort(prodSpec);
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sViewServerFulfillment
*
* Print details of one server fulfillment.
***************************************************************************************************/
static FlxActBool sViewServerFulfillment(FlxCtuContext*		pContext,
									     FlxActLicSpc		licSpec, 
									     FlxActProdLicSpc	prodSpec,
									     void*				pData)
{
	FlxCtuViewFilterData* viewFilterData = (FlxCtuViewFilterData*)pData;

	if (viewFilterData->bPrintHeading)
	{
		flxCtuPrintMessage(
			"\n"
			"U if untrusted\n"
			". D if disabled\n"
			". . fulfillmentType\n" 
			". . . Expiry    Count FulfillmentId                   ProductId\n"
			"\n");

		viewFilterData->bPrintHeading = FLX_ACT_FALSE;	/* Just once */
	}

	switch (viewFilterData->viewType)
	{
	case flxCtuViewTypeLong:
		sViewFulfillmentLong(pContext, prodSpec);
		break;
	case flxCtuViewTypeLongWithCounts:
		sViewFulfillmentCounts(pContext, prodSpec);
		sViewDeductions(pContext, prodSpec);
		break;
	case flxCtuViewTypeFull:
		flxCtuTimePrintInterval("load full server fulfillment");
		sViewFulfillmentCounts(pContext, prodSpec);
		sViewDeductions(pContext, prodSpec);
		break;
	case flxCtuViewTypeShort:
		/* Drop through. */
	default:
		sViewFulfillmentShort(prodSpec);
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sViewFulfillmentCounts
*
* Print a fulfillment's individual counts.
***************************************************************************************************/
void sViewFulfillmentCounts(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec)
{
	sViewFulfillmentLong(pContext, prodSpec);

	flxCtuPrintOutput("\n");
	sViewNameAndUint32("Concurrent", flxActCommonProdLicSpcCountGet(prodSpec, CONCURRENT));
	sViewNameAndUint32("Activatable", flxActCommonProdLicSpcCountGet(prodSpec, ACTIVATABLE));
	sViewNameAndUint32("Hybrid", flxActCommonProdLicSpcCountGet(prodSpec, HYBRID));
	sViewNameAndUint32("Concurrent overdraft", flxActCommonProdLicSpcCountGet(prodSpec, 
																			   CONCURRENT_OD));
	sViewNameAndUint32("Activatable overdraft", flxActCommonProdLicSpcCountGet(prodSpec, 
																			    ACTIVATABLE_OD));
	sViewNameAndUint32("Hybrid overdraft", flxActCommonProdLicSpcCountGet(prodSpec, HYBRID_OD));
	sViewNameAndUint32("Repair", flxActCommonProdLicSpcCountGet(prodSpec, REPAIRS));
}

/***************************************************************************************************
*	sViewDeductions
*
* Print all a fulfillment's deduction records.
***************************************************************************************************/
void sViewDeductions(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec)
{
	uint32_t index;
	uint32_t deductionCount = flxActCommonProdLicSpcNumberDedSpcGet(prodSpec);
	sViewNameAndUint32("\nDeduction count", deductionCount);

	for (index = 0; index < deductionCount; index++)
	{
		FlxActDedSpc dedSpec = flxActCommonProdLicSpcDedSpcGet(prodSpec, index);

		flxCtuPrintOutput("\nDeduction %u (only non-zero counts shown)\n", index);

		sViewNameAndValue("Fulfillment ID", flxActCommonDedSpcDestinationFulfillmentIdGet(dedSpec));
		sViewNameAndValue("System name", flxActCommonDedSpcDestinationSystemNameGet(dedSpec));
		sViewNameAndValue("Expiry", flxActCommonDedSpcExpDateGet(dedSpec));
		sViewNameAndUint32("Type", flxActCommonDedSpcTypeGet(dedSpec));

		sViewNameAndUint32IfNonZero("Concurrent", flxActCommonDedSpcCountGet(dedSpec, CONCURRENT));
		sViewNameAndUint32IfNonZero("Activatable", flxActCommonDedSpcCountGet(dedSpec, ACTIVATABLE));
		sViewNameAndUint32IfNonZero("Hybrid", flxActCommonDedSpcCountGet(dedSpec, HYBRID));
		sViewNameAndUint32IfNonZero("Concurrent overdraft", flxActCommonDedSpcCountGet(dedSpec, CONCURRENT_OD));
		sViewNameAndUint32IfNonZero("Activatable overdraft", flxActCommonDedSpcCountGet(dedSpec, ACTIVATABLE_OD));
		sViewNameAndUint32IfNonZero("Hybrid overdraft", flxActCommonDedSpcCountGet(dedSpec, HYBRID_OD));
		sViewNameAndUint32IfNonZero("Repairs", flxActCommonDedSpcCountGet(dedSpec, REPAIRS));
	}
}

/***************************************************************************************************
*	sViewVendorDictionary
*
* Print a fulfillment's vendor dictionary entries.
***************************************************************************************************/
void sViewVendorDictionary(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec)
{
	uint32_t		count;
	uint32_t		index;
	const char*		pszKey;
	const char*		pszValue;

	if ( !flxActCommonProdLicSpcVendorDataGetCount(prodSpec, &count))
	{
		flxCtuContextPrintLastApiFlexError(pContext);
		return;
	}

	if (count == 0)
	{
		/* No entries. */
		return;
	}

	flxCtuPrintOutput("Vendor dictionary:\n");

	for (index = 0; index < count; ++index)
	{
		if ( !flxActCommonProdLicSpcVendorDataGetByIndex(prodSpec, index, &pszKey, &pszValue))
		{
			flxCtuContextPrintLastApiFlexError(pContext);
			return;
		}
		sViewNameAndValue(pszKey, pszValue);
	}
}

/***************************************************************************************************
*	sViewFulfillmentLong
*
* Print a fulfillment's details multi-line.
***************************************************************************************************/
static void sViewFulfillmentLong(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec)
{
	flxCtuPrintOutput(
		"-------------------------------------------------------------------------------\n");
	sViewNameAndValue("Fulfillment Id", flxActCommonProdLicSpcFulfillmentIdGet(prodSpec));
	sViewNameAndValue("Fulfillment Type", sGetFulfillmentTypeString(prodSpec));
	sViewNameAndValue("Trust Flags", sGetFulfillmentTrustFlagsString(prodSpec));
	sViewNameAndValue("Status", flxActCommonProdLicSpcIsDisabled(prodSpec)? "Disabled" : "Enabled");
	sViewNameAndValue("Entitlement Id", flxActCommonProdLicSpcEntitlementIdGet(prodSpec));
	sViewNameAndValue("Product Id", flxActCommonProdLicSpcProductIdGet(prodSpec));
	sViewNameAndValue("Expiration Date", flxActCommonProdLicSpcExpDateGet(prodSpec));
	sViewNameAndValue("Server chain", flxActCommonProdLicSpcActServerChainGet(prodSpec));
	sViewNameAndFeatures("Feature Line(s)", prodSpec);
	sViewVendorDictionary(pContext, prodSpec);
}

/***************************************************************************************************
*	sViewFulfillmentShort
*
* Print a fulfillment's main details on a single line.
***************************************************************************************************/
void sViewFulfillmentShort(FlxActProdLicSpc prodSpec)
{
	FlxActBool bIsTrusted  = (flxActCommonProdLicSpcTrustFlagsGet(prodSpec) == FLAGS_FULLY_TRUSTED);
	FlxActBool bIsDisabled = flxActCommonProdLicSpcIsDisabled(prodSpec);

	FlxFulfillmentType fulfillmentType = flxActCommonProdLicSpcFulfillmentTypeGet(prodSpec);

	const char* pszFulfillmentId = flxActCommonProdLicSpcFulfillmentIdGet(prodSpec);
	const char* pszProductId	 = flxActCommonProdLicSpcProductIdGet(prodSpec);
	const char* pszExpiry		 = flxActCommonProdLicSpcExpDateGet(prodSpec);
	uint32_t nonOverdraftCount   = sGetNonOverdraftCountTotal(prodSpec);

	if (pszFulfillmentId == NULL)
		pszFulfillmentId = "<NoFid>";

	if (pszProductId == NULL)
		pszProductId = "<NoPid>";

	if (pszExpiry == NULL)
		pszExpiry = "<NoExp>";
	
	flxCtuPrintOutput("%c %c %u %11.11s %3u %-31s %s\n",
					  bIsTrusted?  '.' : 'U',
					  bIsDisabled? 'D' : '.',
					  fulfillmentType,
					  pszExpiry,
					  nonOverdraftCount,
					  pszFulfillmentId,
					  pszProductId
				     );
}

/***************************************************************************************************
*	sViewNameAndFeatures
*
* Print a fulfillment's feature line.
***************************************************************************************************/
void sViewNameAndFeatures(const char* pszName, FlxActProdLicSpc prodSpec)
{
	const char* pszFeatureLine = flxActCommonProdLicSpcFeatureLineGet(prodSpec);

	sViewNameAndValue(pszName, "");
	if (pszFeatureLine != NULL)
	{
		flxCtuPrintOutput(pszFeatureLine);
	}
	flxCtuPrintOutput("\n");
}

/***************************************************************************************************
*	sViewNameAndValue
*
* Print name : string.
***************************************************************************************************/
void sViewNameAndValue(const char* pszName, const char* pszValue)
{
	flxCtuPrintOutput("%22.22s : %s\n", 
					  pszName,
					  (pszValue == NULL)? "" : pszValue
					 );
}

/***************************************************************************************************
*	sViewNameAndUint32
*
* Print name : number.
***************************************************************************************************/
void sViewNameAndUint32(const char* pszName, uint32_t value)
{
	char pszValue[sizeof(value) * 3 + 1];
	sprintf(pszValue, "%u", value);
	sViewNameAndValue(pszName, pszValue);
}

/***************************************************************************************************
*	sViewNameAndUint32IfNonZero
*
* Print name : number.
***************************************************************************************************/
void sViewNameAndUint32IfNonZero(const char* pszName, uint32_t value)
{
	if (value != 0)
		sViewNameAndUint32(pszName, value);
}

/***************************************************************************************************
*	sGetServerTypeText
*
* Return a text string for the type.
***************************************************************************************************/
const char* sGetServerTypeText(FlxActSvrType serverType)
{
	/* Translate to text. */
	switch (serverType)
	{
	case flxActSvrTypeStandalone:	return "Standalone";
	case flxActSvrTypePrimary:		return "Primary";
	case flxActSvrTypeSecondary:	return "Secondary";
	case flxActSvrTypeTertiary:		return "Tertiary";
	default:						return "Unknown";
	}
}

/***************************************************************************************************
*	sFrDeleteCallback
*
* Delete the FR supplied to this callback. *pData should be initialised FLX_ACT_TRUE and will 
* remain so if all deletes succeed.
***************************************************************************************************/
static FlxActBool sFrDeleteCallback(FlxCtuContext*		pContext,
								    FlxActLicSpc		licSpec, 
								    FlxActProdLicSpc	prodSpec,
								    void*				pData)
{
	*(FlxActBool*)pData &= flxActCommonLicSpcProductDelete(licSpec, prodSpec);
	return FLX_ACT_TRUE;
}
