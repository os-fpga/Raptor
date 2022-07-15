/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions.

	It declares the functions use to load, list and cancel outstanding requests.
*/
#ifndef COMP_TRAN_UTIL_MANAGE_REQUEST_H
#define COMP_TRAN_UTIL_MANAGE_REQUEST_H

#include "FlxActTypes.h"
#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

FlxActBool flxCtuDoCommandStored(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool flxCtuLoadStoredRequest(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

FlxActBool flxCtuDoCommandListRequests(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
void	   flxCtuListRequestShort(FlxActTranRequest hTranRequest, FlxActBool bPrintHeading);

FlxActBool flxCtuDoCommandCancelAll(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool flxCtuDoCommandCancelRequest(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

FlxActBool flxCtuGetRequestBySequenceNo(FlxCtuContext*		pContext, 
										const char*			pszSequenceNo, 
										FlxActTranRequest*	phTranRequest);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
