/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of an FNP sample utility that demonstrates Composite Transactions.

	It declares the functions that are appropriate only for server trusted storage.
*/
#ifndef COMP_TRAN_UTIL_SERVER_H
#define COMP_TRAN_UTIL_SERVER_H

#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	Called after the commands have been processed and set in *pContext.
	Apply policies locally, e.g. by server role.
	Return FLX_ACT_TRUE to continue, FLX_ACT_FALSE to exit.
*/
FlxActBool flxCtuServerCommandValidation(FlxCtuContext* pContext);

/*
	Called when a new request is created for server TS.
	Typical use would be to add vendor data.
*/
FlxActBool flxCtuServerOnNewRequest(FlxCtuContext*	 pContext, 
									FlxActTranRequest hTranRequest);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
