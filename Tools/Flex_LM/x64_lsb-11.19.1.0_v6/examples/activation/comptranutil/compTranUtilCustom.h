/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of an FNP sample utility that demonstrates Composite Transactions.

	It declares the functions that support custom extentions.
*/
#ifndef COMP_TRAN_UTIL_CUSTOM_H
#define COMP_TRAN_UTIL_CUSTOM_H

#include <stdlib.h>
#include <stdarg.h>

#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
    Called to allow the "custom" command definition to be modified (e.g. to add the 
	flxCtuCommandTraitCanHaveValue trait) and/or to add attribute definitions using
	flxCtuCommandDefAddAttribute()
*/
void flxCtuCustomCommandDef(FlxCtuCommandDef* pCommandDef);

/* 
    Called to action the custom command.
*/
void flxCtuDoCommandCustom(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

/* 
    Called on exit, just before context is destroyed. 
*/
void flxCtuCustomOnExit(FlxCtuContext* pContext);

/* 
    Called to return a help string summary for the custom command.
*/
const char* flxCtuCustomGetHelpSummary();

/* 
    Called to print help details for the custom command.
*/
void flxCtuCustomHelpDetails(const FlxCtuContext* pContext);

#ifdef __cplusplus 
}
#endif

#endif	/* Include guard */
