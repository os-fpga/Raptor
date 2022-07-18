/**************************************************************************************************
* Copyright (c) 2017-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions and
	other functionality.  

	If declares function that support a number of minor commands.
*/
#ifndef COMP_TRAN_UTIL_MISC_H
#define COMP_TRAN_UTIL_MISC_H

#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

FlxActBool	flxCtuDoCommandLocalRepair(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool	flxCtuDoCommandUnique(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool	flxCtuDoCommandVersion(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool	flxCtuDoCommandVirtual(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool	flxCtuDoCommandHealthCheck(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
