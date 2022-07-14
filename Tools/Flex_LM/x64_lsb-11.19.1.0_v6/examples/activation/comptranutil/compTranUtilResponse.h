/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions.

	If declares the functions for response processing.
*/
#ifndef COMP_TRAN_UTIL_RESPONSE_H
#define COMP_TRAN_UTIL_RESPONSE_H
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
	
#include "FlxActTransaction.h"

#ifdef __cplusplus
extern "C" {
#endif

FlxActBool flxCtuDoCommandProcess(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

void flxCtuResponseOnProcessSuccess(FlxCtuContext* pContext);
void flxCtuResponseOnProcessError(FlxCtuContext* pContext);

void flxCtuResponsePrint(FlxCtuContext* pContext, FlxActTranResponse hTranResponse);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
