/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This sample utility demonstrates Composite Transactions.

	If defines the stub custom functions (no custom extensions).

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FlxActCommon.h"
#include "FlxActError.h"

#include "compTranUtilCustom.h"

/* 
    Called to allow the "custom" command definition to be modified (e.g. to add the 
	flxCtuCommandTraitCanHaveValue trait) and/or to add attribute definitions using
	flxCtuCommandDefAddAttribute()
*/
void flxCtuCustomCommandDef(FlxCtuCommandDef* pCommandDef)
{
	/* Use default definition. */
}

/* 
    Called to action the custom command.
	Return FLX_ACT_FALSE to continue with any other commands, otherwise set pCustom->exitCode 
	and return FLX_ACT_TRUE. 
*/
void flxCtuDoCommandCustom(FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	flxCtuCommandPrintError(pCommand, "No custom extensions are defined.\n");
	pContext->exitCode = flxCtuExitInputError;
}

/* 
    Called on exit, just before context is destroyed. 
*/
void flxCtuCustomOnExit(FlxCtuContext* pContext)
{
}

/* 
    Called to return a help string summary string for the custom command.
*/
const char* flxCtuCustomGetHelpSummary()
{
	return "";
}

/* 
    Called to print help details for the custom command.
*/

void flxCtuCustomHelpDetails(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(
	"\n"
	"custom command: no custom extensions are defined.\n"
	"======\n"
	);
}


