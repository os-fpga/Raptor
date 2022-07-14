/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/**
 * FlxActBorrow
 *
 * The FlxActBorrow include file contains the object and function definitions
 * used to support client side activation.
 */

#ifndef FlxActBorrow_h
#define FlxActBorrow_h  1

#include "FlxActCommon.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef enum flxActBorrowStatusAttribute
{
	kActBorrowStatusAttributeUnknown = 0,
	kActBorrowStatusAttributeEntitlementId,	/*	Entitlement ID */
	kActBorrowStatusAttributeProductId,		/*	Product ID */
	kActBorrowStatusAttributeFulfillmentId,	/*	Fulfillment ID */
	kActBorrowStatusAttributeExpiration		/*	Expiration */
} FlxActBorrowStatusAttribute;




/**
 * FlxActivation
 */
typedef struct flex_act_borrow_context *	FlxActBorrowContext;

int
flxActBorrowTSViewCreate(
	FlxActBorrowContext *	pContext,
	int *					pFulfillmentCount);

void
flxActBorrowTSViewDelete(FlxActBorrowContext context);

int
flxActBorrowTSViewFRAttributeGet(
	FlxActBorrowContext		context,
	int							index,
	FlxActBorrowStatusAttribute	attribute,
	char *						pszBuffer,
	int *						pLen);

int
flxActBorrowReturn(
	const char *	pszFulfillmentId,
	const char *	pszServer,
	FlxActError *	pError);

int
flxActBorrowActivate(
	const char *	pszEntitlementId,
	const char *	pszProductId,
	const char *	pszExpiration,
	const char *	pszServer,
	char *			pszFulfillmentId,
	int				length,
	FlxActError *	pError);

#ifdef __cplusplus
}
#endif

#endif /* FlxActivation_h */



