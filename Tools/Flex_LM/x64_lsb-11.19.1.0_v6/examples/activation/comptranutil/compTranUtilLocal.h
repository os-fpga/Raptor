/**************************************************************************************************
* Copyright (c) 2017-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions and
	other functionality.  

	If declares the functions that support for local activation.
*/
#ifndef COMP_TRAN_UTIL_LOCAL_H
#define COMP_TRAN_UTIL_LOCAL_H

#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

FlxActBool	flxCtuDoCommandLocal(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool	flxCtuDoCommandLocalCheck(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool	flxCtuDoCommandLocalReset(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
