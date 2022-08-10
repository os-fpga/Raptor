/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of an FNP sample utility that demonstrates Composite Transactions.

	It declares the FlxCtuCommand (command) object and the functions that relate to it.
*/
#ifndef COMP_TRAN_UTIL_COMMAND_H
#define COMP_TRAN_UTIL_COMMAND_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "compTranUtilCommandDef.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct FlxCtuKeyValue_struct
{
	const char* pszKey;
	const char* pszValue;
} FlxCtuKeyValue;

typedef struct FlxCtuDictionary_struct
{
	const char* pszName;

	uint32_t	entryCount;
	size_t		entryIndex;
	FlxCtuKeyValue**		ppEntries;
} FlxCtuDictionary;

typedef struct FlxCtuAttribute_struct
{
	const FlxCtuAttributeDef*	pDef;
	FlxCtuKeyValue*				pKeyValue;
} FlxCtuAttribute;

typedef struct FlxCtuCommand_struct
{
	const FlxCtuCommandDef* pDef;
	const char*				pszNameForMessages;

	FlxActBool				bIsValue;
	const char*				pszValue;

	uint32_t				attributeCount;
	FlxCtuAttribute**		ppAttributes;

	size_t					dictionaryCount;
	FlxCtuDictionary**		ppDictionaries; 
} FlxCtuCommand;

FlxActBool flxCtuCommandAttributeIs(const FlxCtuCommand* pCommand, 
									const char*			 pszAttributeName,
									const char*			 pszAttributeValue);

FlxActBool flxCtuCommandAttributeIsYes(const FlxCtuCommand* pCommand, 
									   const char*			pszAttributeName,
									   FlxActBool			bDefault);

void flxCtuCommandDestroy(const FlxCtuCommand* pCommand);

const char* flxCtuCommandGetAttribute(const FlxCtuCommand* pCommand, const char* pszAttributeName);
const char* flxCtuCommandGetName(const FlxCtuCommand* pCommand);
const char* flxCtuCommandGetNameForMessages(const FlxCtuCommand* pCommand);
const char* flxCtuCommandGetValue(const FlxCtuCommand* pCommand);

/* If the command has a value, return it, otherwise the default. */
const char* flxCtuCommandGetValueOrDefault(const FlxCtuCommand* pCommand, const char* theDefault);

/* If the command has the attribute return its value, otherwise the default. */
const char* flxCtuCommandGetAttributeOrDefault(const FlxCtuCommand* pCommand, 
											   const char* pszAttributeName, 
											   const char* theDefault);

FlxActBool flxCtuCommandHasAttribute(const FlxCtuCommand* pCommand, const char* pszAttributeName);
FlxActBool flxCtuCommandHasTrait(const FlxCtuCommand* pCommand, FlxCtuCommandTrait trait);
FlxActBool flxCtuCommandHasValue(const FlxCtuCommand* pCommand);

void flxCtuCommandPrintError(const FlxCtuCommand* pCommand, const char *pszFormatStr, ...);

void flxCtuDictionaryFree(const FlxCtuDictionary* pDictionary);
void flxCtuKeyValueFree(FlxCtuKeyValue* pKeyValue);

#ifdef __cplusplus
}
#endif
#endif	/* Include guard */
