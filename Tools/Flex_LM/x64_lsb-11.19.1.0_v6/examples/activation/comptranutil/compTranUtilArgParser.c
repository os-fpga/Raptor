/**************************************************************************************************
* Copyright (c) 1997-2021 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This source file is part of an FNP sample utility that demonstrates Composite Transactions.

	It parses the program arguments into commands and options.

	The general syntx for commands is

		--commandName commandValue attribute1=attributeValue1 dictionaryName{key1=value1}

	where
		-commandName and -singleLetterAlias are equivalent to --commandName.
		there can be multiple attributes
		there can be multiple entries within a dictionary
		there can be multiple dictionaries

	Each command is defined by a FlxCtuCommandDef object within the context object - whether it can
	have a value, attributes, dictionaries, is a "main" command, is repeatable, etc.

	Multiple commands are allowed but there can only be one main command. 

	Case of command, attribute and dictionary names is ignored.

*/
#include <stdio.h>
#include <stdlib.h>
/*#include <string.h>*/

#include "compTranUtilArgParser.h"
/*#include "compTranUtilHelper.h"*/

#define CHAR_COMMAND_PREXIX			'-'
#define CHAR_KEY_VALUE_SEPARATOR	'='
#define CHAR_DICTIONARY_START		'{'
#define CHAR_DICTIONARY_END			'}'

typedef enum FlxCtuArgumentType_enum
{
	FlxCtuArgumentTypeNone,
	FlxCtuArgumentTypeCommand,
	FlxCtuArgumentTypeValue,
	FlxCtuArgumentTypeKeyValue,
	FlxCtuArgumentTypeDictionaryStart,
	FlxCtuArgumentTypeDictionaryEnd,
} FlxCtuArgumentType;

typedef struct FlxCtuArgs_struct
{
	const char**				ppArgs;
	size_t						count;
	size_t						indexOfNext;
} FlxCtuArgs;

const char* gEmptyString = "";


static const char* sArgumentGet(FlxCtuArgs* pArgs);
static const char* sArgumentGetPop(FlxCtuArgs* pArgs);
static FlxCtuArgumentType sArgumentGetType(FlxCtuArgs* pArgs);

static FlxActBool  sArgumentPop(FlxCtuArgs* pArgs);

static FlxCtuArgumentType sArgumentStringGetType(const char* pszArg);

static void sCommandCreate(FlxCtuContext*			pContext, 
						   const FlxCtuCommandDef*	pCommandDef, 
						   const char*				pszNameForMessages,
						   FlxCtuCommand**			ppCommand);

static FlxActBool sGetAttributeDef(const FlxCtuCommandDef*		pCommandDef,
								   const char*					pszAttributeName,
								   const FlxCtuAttributeDef**	ppAttributeDef);

static FlxActBool sbValidateAttributeValue(	const FlxCtuCommand*		pCommand, 
											const char*                 pszArg,
											const FlxCtuAttributeDef*	pAttributeDef, 
											const char*					pszValue);

static FlxActBool sParseAttributes(FlxCtuCommand*	pCommand, FlxCtuArgs* pArgs);
static FlxActBool sParseCommand(FlxCtuContext*	pContext, FlxCtuArgs*	pArgs);
static FlxActBool sParseDictionaries(FlxCtuCommand*	pCommand, FlxCtuArgs* pArgs);

static FlxActBool sParseDictionary(FlxCtuCommand*		pCommand,
								   FlxCtuArgs*			pArgs,
								   FlxCtuDictionary**	ppDictionary);

static FlxActBool sParseKeyValue(FlxCtuCommand*		pCommand,
								 const char*		pszArg,
								 FlxCtuKeyValue**	ppKeyValue);

typedef enum FlxCtuParseDictionaryResultEnum
{
	flxCtuParseDictionaryContinue,
	flxCtuParseDictionaryEnd,
	flxCtuParseDictionaryError,
} FlxCtuParseDictionaryResult;

static FlxCtuParseDictionaryResult sParseDictionaryEntry(FlxCtuCommand*		pCommand, 
														 FlxCtuDictionary*	pDictionary,
														 const char*			pszArg);

/***************************************************************************************************
*	flxCtuArgsParseToContext
*
* Parses the command line arguments and creates a set of FlxCtuCommand objects in the context.
*
* pContext already defines the commands and their attributes.
* ppArgs[argCount] are the command line arguments.
***************************************************************************************************/
FlxActBool flxCtuArgsParseToContext(FlxCtuContext* pContext, const char** ppArgs, size_t argCount)
{
	FlxCtuArgs	args;
	FlxCtuArgs*	pArgs = &args;

	/* Initialise the FlxCtuArgs object used by the parsing functions to iterate over 
	   the arguments. */
	pArgs->ppArgs		= ppArgs;
	pArgs->count		= argCount;
	pArgs->indexOfNext	= 0;

	/* Parse the commands from the argument list. */
	while (sArgumentGetType(pArgs) == FlxCtuArgumentTypeCommand)
	{
		if (!sParseCommand(pContext, pArgs))
		{
			return FLX_ACT_FALSE;
		}
	}
	/* Check that the arguments have all been consumed. */
	if (sArgumentGetType(pArgs) != FlxCtuArgumentTypeNone)
	{
		flxCtuPrintError("%s is not a command or option.\n", sArgumentGet(pArgs));
		return FLX_ACT_FALSE;
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sArgumentGet
*
* Return the next argument without removing it from the list.
* Use sArgumentGetType first to check there is one; an empty sting is returned if no arguments left.
***************************************************************************************************/
const char* sArgumentGet(FlxCtuArgs* pArgs)
{
	if (pArgs->indexOfNext >= pArgs->count)
		return gEmptyString;

	return pArgs->ppArgs[pArgs->indexOfNext];
}

/***************************************************************************************************
*	sArgumentGetType
*
* Return the type of the next argument in the list.
***************************************************************************************************/
FlxCtuArgumentType sArgumentGetType(FlxCtuArgs* pArgs)
{
	if (pArgs->indexOfNext >= pArgs->count)
		return FlxCtuArgumentTypeNone;

	return sArgumentStringGetType(sArgumentGet(pArgs));
}

/***************************************************************************************************
*	sArgumentGetPop
*
* Remove and return the first argument from the list.
***************************************************************************************************/
const char* sArgumentGetPop(FlxCtuArgs* pArgs)
{
	const char* pArg = sArgumentGet(pArgs);
	sArgumentPop(pArgs);
	return pArg;
}

/***************************************************************************************************
*	sArgumentPop
*
* Remove the first argument from the list; error if none to remove.
***************************************************************************************************/
static FlxActBool sArgumentPop(FlxCtuArgs* pArgs)
{
	if (pArgs->indexOfNext >= pArgs->count)
		return FLX_ACT_FALSE;
	pArgs->indexOfNext++;
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
 *	sArgumentStringGetType
 *
 * Return the type of an argument.
 ***************************************************************************************************/
static FlxCtuArgumentType sArgumentStringGetType(const char* pszArg)
{
	if (strlen(pszArg) == 0)
		return FlxCtuArgumentTypeNone;

	if (*pszArg == CHAR_COMMAND_PREXIX)
		return FlxCtuArgumentTypeCommand;

	if (strchr(pszArg, CHAR_DICTIONARY_START) != NULL)
		return FlxCtuArgumentTypeDictionaryStart;

	if (strchr(pszArg, CHAR_DICTIONARY_END) != NULL)
		return FlxCtuArgumentTypeDictionaryEnd;

	if (strchr(pszArg, CHAR_KEY_VALUE_SEPARATOR) != NULL)
		return FlxCtuArgumentTypeKeyValue;

	return FlxCtuArgumentTypeValue;
}

/***************************************************************************************************
*	sCommandCreate
*
* Create a new FlxCtuCommand object and add it to the context.
***************************************************************************************************/
static void sCommandCreate(FlxCtuContext*			pContext, 
						   const FlxCtuCommandDef*	pCommandDef, 
						   const char*				pszNameForMessages,
						   FlxCtuCommand**			ppCommand  )
{
	/* Create an empty command. */
	FlxCtuCommand* pCommand  = (FlxCtuCommand*)flxCtuMallocAndZeroise(sizeof(FlxCtuCommand));

	/* Increase the command list size in the context and append the new command to the list. */
	size_t newListSize	 = (pContext->commandCount + 1) * sizeof(FlxCtuCommand*);
	pContext->ppCommands = (const FlxCtuCommand **)flxCtuRealloc(pContext->ppCommands, newListSize);
	pContext->ppCommands[pContext->commandCount++] = pCommand;

	/* Add the definition and the name string to be used for identifying the command in messages. */
	pCommand->pDef = pCommandDef;
	flxCtuStrNDupReplace(&pCommand->pszNameForMessages, pszNameForMessages, strlen(pszNameForMessages));

	/* If it's an action, bump the count */
	if ((pCommand->pDef->traits & flxCtuCommandTraitIsRequestAction) != 0)
		pContext->actionCount++;

	flxCtuPrintVerbose("Command: %s (%s)\n", pCommand->pszNameForMessages, pCommand->pDef->pszName);

	/* Return the command object as a convenience (it will be destroyed with the context). */
	*ppCommand = pCommand;
}

/***************************************************************************************************
*	sGetAttributeDef
*
* Returns the attribute definition for the command attribute.
***************************************************************************************************/
static FlxActBool sGetAttributeDef(const FlxCtuCommandDef*		pCommandDef,
								   const char*					pszAttributeName,
								   const FlxCtuAttributeDef**	ppAttributeDef)
{
	size_t index;

	if (pCommandDef->ppAttributeDefs == NULL)
	{
		/* No attributes defined for this command. */
		return FLX_ACT_FALSE;
	}

	/* Iterate over the attribute definitions. */
	for (index = 0; index < pCommandDef->attributeDefCount; index++)
	{
		const FlxCtuAttributeDef* pAttributeDef = pCommandDef->ppAttributeDefs[index];

		if (   (0 == flxCtuStrCmpLwr(pAttributeDef->pszName, pszAttributeName))
			|| (0 == flxCtuStrCmpLwr(pAttributeDef->pszAlias, pszAttributeName)))
		{
			*ppAttributeDef = pAttributeDef;
			return FLX_ACT_TRUE;	/* Found. */
		} 
	}
	return FLX_ACT_FALSE;			/* Attribute not defined for this command. */
}

/***************************************************************************************************
*	sbValidateAttributeValue
*
* Checks that the attribute value is allowed by the definition.
***************************************************************************************************/
static FlxActBool sbValidateAttributeValue(	const FlxCtuCommand*		pCommand, 
											const char*                 pszArg,
											const FlxCtuAttributeDef*	pAttributeDef, 
											const char*					pszValue)
{
	size_t index;

	if (pAttributeDef->ppszValues == NULL)
		return FLX_ACT_TRUE;	/* Any value allowed. */

	for (index = 0; index < pAttributeDef->valueCount; index++)
	{
		/* Check for special cases. */
		if (0 == flxCtuStrCmpLwr(pAttributeDef->ppszValues[index], ATTR_VALUE_NUMERIC))
		{
			/* Any numeric value. */
			if (flxCtuStrIsNumeric(pszValue))
				return FLX_ACT_TRUE;	/* Numeric. */
		}
		else if (0 == flxCtuStrCmpLwr(pAttributeDef->ppszValues[index], ATTR_VALUE_DATE))
		{
			/* Any flex date string. */
			FlxActBool bIsValid;
			if (	flxActCommonValidateDateString(pszValue, &bIsValid)
				 && bIsValid)
			{
				return FLX_ACT_TRUE;			/* Valid date. */
			}
		}
		else
		{
			/* Check specific value, ignoring case. */
			if (0 == flxCtuStrCmpLwr(pAttributeDef->ppszValues[index], pszValue))
				return FLX_ACT_TRUE;	/* Value found. */
		}
	}

	/* Here if value is not one specified in the definition, */
	flxCtuCommandPrintError(pCommand, "has invalid attribute \"%s\".\n", pszArg);
	flxCtuPrintMessage("       \"%s\" must be ", pszValue);
	for (index = 0; index < pAttributeDef->valueCount; index++)
	{
		const char* pszValidValue = pAttributeDef->ppszValues[index];
		if (0 == flxCtuStrCmpLwr(pszValidValue, ATTR_VALUE_NUMERIC))
			flxCtuPrintMessage("numeric");
		else if (0 == flxCtuStrCmpLwr(pszValidValue, ATTR_VALUE_DATE))
			flxCtuPrintMessage("a date with format dd-mmm-yyyy (e.g. 31-dec-2030) or \"permanent\"");
		else
			flxCtuPrintMessage("\"%s\"", pszValidValue);

		flxCtuPrintMessage("%s", (index < pAttributeDef->valueCount - 1)? " or " : ".\n");
	}

	return FLX_ACT_FALSE;			/* Value not found. */
}
/***************************************************************************************************
*	sParseAttributes
*
* Parse the attributes from the arguments and add them to the command.
***************************************************************************************************/
static FlxActBool sParseAttributes(FlxCtuCommand*	pCommand,
								   FlxCtuArgs*		pArgs)
{
	while (	sArgumentGetType(pArgs) == FlxCtuArgumentTypeKeyValue )
	{
		/* Parse the key and value to a new FlxCtuKeyValue. */
		FlxCtuKeyValue* pAttributeKeyValue;
		const char* pszArg = sArgumentGetPop(pArgs);
		if (sParseKeyValue(pCommand, pszArg, &pAttributeKeyValue))
		{
			FlxCtuAttributeDef* pAttributeDef;
			FlxCtuAttribute*	pAttribute;
			size_t				newListSize;

			/* Get the attribute definition. */
			if ( !sGetAttributeDef(	pCommand->pDef, 
									pAttributeKeyValue->pszKey, 
									(const FlxCtuAttributeDef**)&pAttributeDef) )
			{
				flxCtuCommandPrintError(pCommand, "has undefined attribute \"%s\".\n", pszArg);
				flxCtuKeyValueFree(pAttributeKeyValue);
				return FLX_ACT_FALSE;
			}

			/* Check attribute value is valid. */
			if ( !sbValidateAttributeValue(pCommand, pszArg, pAttributeDef, pAttributeKeyValue->pszValue))
			{
				flxCtuKeyValueFree(pAttributeKeyValue);
				return FLX_ACT_FALSE;
			}

			/* Attribute is valid - create a new attribute entry. Note that the attribute name is 
			   taken from the definition as the parsed name may have been the alias. */
			pAttribute						= (FlxCtuAttribute*)flxCtuMalloc(sizeof(FlxCtuAttribute));
			pAttribute->pDef				= pAttributeDef;
			pAttribute->pKeyValue			= (FlxCtuKeyValue*)flxCtuMalloc(sizeof(FlxCtuKeyValue));
			pAttribute->pKeyValue->pszKey	= flxCtuStrDup(pAttribute->pDef->pszName);
			pAttribute->pKeyValue->pszValue	= flxCtuStrDup(pAttributeKeyValue->pszValue);

			/* Increase the argument list size in the command. */
			newListSize				= (pCommand->attributeCount + 1) * sizeof(FlxCtuKeyValue*);
			pCommand->ppAttributes	= (FlxCtuAttribute**)flxCtuRealloc(pCommand->ppAttributes, 
																		newListSize);
			
			/* Append the new attribute to the list. */
			pCommand->ppAttributes[pCommand->attributeCount++] = pAttribute;
		
			flxCtuPrintVerbose("  Attribute: %s = \"%s\"\n", 
							   pAttribute->pKeyValue->pszKey, 
							   pAttribute->pKeyValue->pszValue);

			/* The parsed attribute has been duplicated so it can be freed. */
			flxCtuKeyValueFree(pAttributeKeyValue);
		}
		else
		{
			flxCtuCommandPrintError(pCommand, "- \"%s\" is not a valid attribute\n", pszArg);
			return FLX_ACT_FALSE;
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
*	sParseCommand
*
* Parse the next command object from the argument list and add it to the context.
***************************************************************************************************/
static FlxActBool sParseCommand(FlxCtuContext*	pContext, 
						 FlxCtuArgs*	pArgs)
{
	const char*				pszCommandOrAlias = sArgumentGet(pArgs);
	const char*				pszNameForMessages = pszCommandOrAlias;
	const FlxCtuCommandDef*	pCommandDef;

	FlxCtuCommand*			pCommand;

	/*	Discard leading command prefixes - allow "--command" as well as "-command" providing the
		command is not a single character alias. */
	if (pszCommandOrAlias[0] == CHAR_COMMAND_PREXIX)
		pszCommandOrAlias++;
	if ( (pszCommandOrAlias[0] == CHAR_COMMAND_PREXIX) && (strlen(pszCommandOrAlias) > 2) )
		pszCommandOrAlias++;

	/* Find the definition of the command. */
	if ( !flxCtuContextFindCommandDef(pContext, pszCommandOrAlias, &pCommandDef) )
	{
		flxCtuPrintError("\"%s\" is not a valid command or option.\n", sArgumentGet(pArgs));
		return FLX_ACT_FALSE;
	}

	/* If it is the "verbose" command update the context now so that subsequent parsing
	   reports verbosely. */
	if ( (pCommandDef->traits & flxCtuCommandTraitIsVerbose) != 0)
	{
		pContext->bIsVerbose = FLX_ACT_TRUE;
		pContext->bprintResponseXml = pContext->bIsVerbose;
		flxCtuSetVerbose(FLX_ACT_TRUE);
	}

	/* If duplicates not allowed, check this is the first occurrence. */
	if (0 == (pCommandDef->traits & flxCtuCommandTraitDuplicatesOK))
	{
		FlxCtuCommand* pCommandFirst;
		if (flxCtuContextGetCommandByName(	pContext, 
											pCommandDef->pszName, 
											(const FlxCtuCommand**)&pCommandFirst) )
		{
			flxCtuPrintError("\"%s\" and \"%s\" cannot both be present.\n", 
							 pCommandFirst->pszNameForMessages,
							 pszNameForMessages);
			return FLX_ACT_FALSE;
		}
	}

	/* Create a new command object and add it to the list in the context. */
	sCommandCreate(pContext, pCommandDef, pszNameForMessages, &pCommand);
	sArgumentPop(pArgs);

	/* If it is a main command, check there is not one already set. */
	if ( (pCommandDef->traits & flxCtuCommandTraitMain) != 0)
	{
		if (pContext->bIsMainCommandSet)
		{
			/* Second main command. */
			flxCtuCommandPrintError(pCommand, "and \"%s\" cannot both be present.\n", 
									pContext->pMainCommand->pszNameForMessages);
			return FLX_ACT_FALSE;
		}
		/* Set as main command. */
		pContext->bIsMainCommandSet = FLX_ACT_TRUE;
		pContext->pMainCommand		= pCommand;
	}

	/* Parse any value. */
	if ( sArgumentGetType(pArgs) == FlxCtuArgumentTypeValue )
	{
		/* Value present - check it's allowed for this command. */
		const char* pszValue = sArgumentGet(pArgs);
		if ( (pCommandDef->traits & flxCtuCommandTraitCanHaveValue) == 0 )
		{
			flxCtuCommandPrintError(pCommand, "does not require a value (%s).\n", pszValue);
			return FLX_ACT_FALSE;
		}
		flxCtuStrNDupReplace(&pCommand->pszValue, pszValue, strlen(pszValue));
		sArgumentPop(pArgs);

		flxCtuPrintVerbose("  Value: \"%s\"\n", pszValue);
	}

	/* Parse any attributes. */
	if ( !sParseAttributes(pCommand, pArgs) )
		return FLX_ACT_FALSE;

	/* Parse any dictionaries. */
	return sParseDictionaries(pCommand, pArgs);
}

/***************************************************************************************************
 *	sParseDictionaries
 *
 * Parse any dictionaries from the arguments and add them to the command.
 ***************************************************************************************************/
static FlxActBool sParseDictionaries(FlxCtuCommand*	pCommand,
							  FlxCtuArgs*		pArgs)
{
	while (	sArgumentGetType(pArgs) == FlxCtuArgumentTypeDictionaryStart )
	{
		FlxCtuDictionary* pDictionary;

		if ( !sParseDictionary(pCommand, pArgs, &pDictionary) )
		{
			return FLX_ACT_FALSE;
		}
		else
		{
			/* Increase the dictionary list size in the command. */
			size_t newListSize		 = (pCommand->dictionaryCount + 1) * sizeof(FlxCtuKeyValue*);
			pCommand->ppDictionaries = (FlxCtuDictionary**)flxCtuRealloc(pCommand->ppDictionaries, 
																	     newListSize);
			/* Append the new dictionary to the list in the command (it will be destroyed with
			   the command). */
			pCommand->ppDictionaries[pCommand->dictionaryCount++] = pDictionary;
		}
	}
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
 *	sParseDictionary
 *
 * Parse a dictionary from the arguments.
 ***************************************************************************************************/
static FlxActBool sParseDictionary(FlxCtuCommand*		pCommand,
								   FlxCtuArgs*			pArgs,
								   FlxCtuDictionary**	ppDictionary)
{
	FlxCtuDictionary* pDictionary = (FlxCtuDictionary*)
									flxCtuMallocAndZeroise(sizeof(FlxCtuDictionary));
	const char* pszEntry = NULL;
	FlxCtuParseDictionaryResult parseResult;

	/* Get the dictionary name (and possibly the first entry) from the next argument. */
	flxCtuStrDupSeparate(sArgumentGetPop(pArgs), 
						 CHAR_DICTIONARY_START, 
						 &pDictionary->pszName, 
						 &pszEntry);
	
	/* Don't allow a null name. */
	if (strlen(pDictionary->pszName) == 0)
	{
		flxCtuCommandPrintError(pCommand, "the dictionary name must precede the opener %c\n",
										  CHAR_DICTIONARY_START);
		flxCtuFree(pDictionary);
		flxCtuFree(pszEntry);
		return FLX_ACT_FALSE;
	}
	flxCtuPrintVerbose("  Dictionary: %s\n", pDictionary->pszName);

	/* Process any entry that follows immediately (no space after CHAR_DICTIONARY_START). */
	parseResult = sParseDictionaryEntry(pCommand, pDictionary, pszEntry);
	flxCtuFree(pszEntry);

	/* Add entries until closer character found. */
	while( parseResult == flxCtuParseDictionaryContinue) 
		parseResult = sParseDictionaryEntry(pCommand, pDictionary, sArgumentGetPop(pArgs));

	if (parseResult == flxCtuParseDictionaryError)
	{
		flxCtuDictionaryFree(pDictionary);
		return FLX_ACT_FALSE;
	}
	
	/* Return the dictionary, and success. */
	*ppDictionary = pDictionary;
	return FLX_ACT_TRUE;
}

/***************************************************************************************************
 *	sParseDictionaryEntry
 *
 * Parse a dictionary entry from the supplied argument string and add it to the dictionary.
 ***************************************************************************************************/
static FlxCtuParseDictionaryResult sParseDictionaryEntry(FlxCtuCommand*		pCommand, 
														 FlxCtuDictionary*	pDictionary,
														 const char*		pszArg)
{
	FlxCtuParseDictionaryResult parseResult;
	
	if (strlen(pszArg) == 0)
	{
		parseResult = flxCtuParseDictionaryContinue;
	}
	else if (sArgumentStringGetType(pszArg) == FlxCtuArgumentTypeDictionaryEnd)
	{
		/* The argument contains the dictionary end char, with possibly the last entry before it. */
		const char* pszBefore;
		const char* pszAfter;
		flxCtuStrDupSeparate(pszArg, 
							 CHAR_DICTIONARY_END, 
							 &pszBefore, 
							 &pszAfter);

		if (strlen(pszAfter) > 0)
		{	/* The closer is not the last character of the argument. */ 
			flxCtuCommandPrintError(pCommand, "dictionary %s closer %c must be followed by a space\n",
											  pDictionary->pszName,
											  CHAR_DICTIONARY_END);
			parseResult = flxCtuParseDictionaryError;
		}
		else
		{
			/* Add any entry that precedes the closer character. */
			if (sParseDictionaryEntry(pCommand, pDictionary, pszBefore) == flxCtuParseDictionaryError)
				parseResult = flxCtuParseDictionaryError;
			else
				parseResult = flxCtuParseDictionaryEnd;
		}
		flxCtuFree(pszBefore);
		flxCtuFree(pszAfter);
	}
	else if (sArgumentStringGetType(pszArg) == FlxCtuArgumentTypeKeyValue)
	{
		/* Parse the key and value to a new FlxCtuKeyValue. */
		FlxCtuKeyValue* pKeyValue;
		if ( !sParseKeyValue(pCommand, pszArg, &pKeyValue) )
		{
			flxCtuCommandPrintError(pCommand, "dictionary %s - \"%s\" is not a dictionary entry\n",
											  pDictionary->pszName,
											  pszArg);
			parseResult = flxCtuParseDictionaryError;
		}
		else
		{
			/* Increase the entry list size in the dictionary. */
			size_t newListSize		= (pDictionary->entryCount + 1) * sizeof(FlxCtuKeyValue*);
			pDictionary->ppEntries	= (FlxCtuKeyValue**)flxCtuRealloc(pDictionary->ppEntries, 
																	  newListSize);
			/* Append the new entry to the list. */
			pDictionary->ppEntries[pDictionary->entryCount++] = pKeyValue;

			flxCtuPrintVerbose("       Entry: %s%c%s\n", pKeyValue->pszKey, 
														 CHAR_KEY_VALUE_SEPARATOR,
														 pKeyValue->pszValue);
			parseResult = flxCtuParseDictionaryContinue;
		}
	}
	else
	{
		flxCtuCommandPrintError(pCommand, "dictionary %s - \"%s\" is not a dictionary entry\n",
										  pDictionary->pszName,
										  pszArg);
		parseResult = flxCtuParseDictionaryError;
	}
	return parseResult;
}

/***************************************************************************************************
*	sParseKeyValue
*
* Parse "key=value" from the string, creating and returning a FlxCtuKeyValue.
***************************************************************************************************/
static FlxActBool sParseKeyValue(FlxCtuCommand*	pCommand,
						  const char*		pszArg,
						  FlxCtuKeyValue**  ppKeyValue)
{
	/* Allocate a new key/value structure. */
	FlxCtuKeyValue* pKeyValue = (FlxCtuKeyValue*)flxCtuMallocAndZeroise(sizeof(FlxCtuKeyValue));

	/* Duplicate the key and value from the argument. */
	flxCtuStrDupSeparate(pszArg, 
						 CHAR_KEY_VALUE_SEPARATOR, 
						 &pKeyValue->pszKey, 
						 &pKeyValue->pszValue);
	/* Don't allow an empty key. */
	if ( strlen(pKeyValue->pszKey) == 0)
	{
		flxCtuKeyValueFree(pKeyValue);
		return FLX_ACT_FALSE;
	}

	/* Return the key/value. */
	*ppKeyValue = pKeyValue;
	return FLX_ACT_TRUE;
}

