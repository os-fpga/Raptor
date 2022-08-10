/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions.

	It parses the (command line) arguments as commands with values, attributes and dictionaries.
*/
#ifndef COMP_TRAN_UTIL_ARG_PARSER_H
#define COMP_TRAN_UTIL_ARG_PARSER_H

#include "FlxActTypes.h"
#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

FlxActBool flxCtuArgsParseToContext(FlxCtuContext* pContext, const char** ppArgs, size_t argCount);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
