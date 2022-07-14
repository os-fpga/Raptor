/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions.

	If declares the function used for online transactions.
*/
#ifndef COMP_TRAN_UTIL_SEND_H
#define COMP_TRAN_UTIL_SEND_H

#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

FlxActBool flxCtuDoCommandTransaction(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
