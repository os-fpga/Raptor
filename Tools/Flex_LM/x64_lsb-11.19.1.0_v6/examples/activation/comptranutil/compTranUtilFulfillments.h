/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions.

	It declares the functions that operate on fulfillment records (view, delete, repair).
*/
#ifndef COMP_TRAN_UTIL_FULFILLMENTS_H
#define COMP_TRAN_UTIL_FULFILLMENTS_H

#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

FlxActBool flxCtuDoCommandDeleteFR(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool flxCtuDoCommandView(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool flxCtuDoCommandViewLong(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool flxCtuDoCommandViewServer(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool flxCtuDoCommandViewServerLong(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

FlxActBool flxCtuFulfillmentsAddRepairActions(FlxCtuContext* pContext, uint32_t* pCount);

FlxActBool flxCtuFulfillmentGetState(FlxCtuContext* pContext, 
									 const char*	pszFulfillmentId,
									 FlxActBool*	pbIsTrusted, 
									 FlxActBool*	pbIsEnabled);

/* Count of all deductions of an FR. */
FlxActBool flxCtuFulfillmentGetDeductionCount(FlxCtuContext* pContext, 
											  const char*	 pszFulfillmentId,
											  uint32_t*		 pDeductionCount);

/* Count of "valid" deductions of an FR (excludes repairs and expired). */
FlxActBool flxCtuFulfillmentGetValidDeductionCount(FlxCtuContext*	pContext, 
												   const char*		pszFulfillmentId,
												   uint32_t*		pDeductionCount);

FlxActBool flxCtuWarnIfEntitlementAlreadyUsed(FlxCtuContext* pContext, const char* pszEntitlementId);

void flxCtuFulfillmentViewVendorDictionary(FlxCtuContext* pContext, FlxActProdLicSpc prodSpec);

FlxActBool flxCtuFulfillmentDelete(FlxCtuContext* pContext, const char*	 pszFulfillmentId);

#ifdef __cplusplus
}
#endif


#endif	/* Include guard */
