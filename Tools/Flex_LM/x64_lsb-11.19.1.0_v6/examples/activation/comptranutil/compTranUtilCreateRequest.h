/**************************************************************************************************
* Copyright (c) 1997-2016, 2018-2020 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions.

	If declares the function that implements the -new command and supporting functions used
	by that and other commands for request creation and output. 
*/
#ifndef COMP_TRAN_UTIL_CREATE_REQUEST_H
#define COMP_TRAN_UTIL_CREATE_REQUEST_H

#include "FlxActTransaction.h"
#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

FlxActBool flxCtuDoCommandNew(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

FlxActBool flxCtuRequestCreate(FlxCtuContext*		pContext, 
							   const FlxCtuCommand*	pCommand, 
							   FlxActTranRequest*	phTranRequest, FlxActBool pReinstallFlag);

FlxActBool flxCtuRequestMatchOrSave(FlxCtuContext* pContext, FlxActTranRequest* phTranRequest);

FlxActBool flxCtuRequestOutputXml(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
