/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This file is part of an FNP sample utility that demonstrates Composite Transactions.

	It defines the functions that relate to the FlxCtuCommand (command) object. 
*/
#include "compTranUtilCommand.h"

static void sAttributeFree(FlxCtuAttribute* pAttributeList);
static void sAttributeListFree(FlxCtuAttribute** ppAttributeList, uint32_t count);

static FlxActBool sFindAttribute(const FlxCtuCommand*	pCommand, 
								 const char*			pszAttributeName, 
								 size_t*				pAttributeIndex);

static void sKeyValueListFree(FlxCtuKeyValue** ppKeyValue, uint32_t count);

/***************************************************************************************************
*		flxCtuCommandAttributeIs
*
* Returns FLX_ACT_TRUE if the attribute has the specified value (ignoring case).
***************************************************************************************************/
FlxActBool flxCtuCommandAttributeIs(const FlxCtuCommand* pCommand, 
									const char*			 pszAttributeName,
									const char*			 pszAttributeValue)
{
	FlxActBool bResult = FLX_ACT_FALSE;

	if ( flxCtuCommandHasAttribute(pCommand, pszAttributeName) )
	{
		if (0 == flxCtuStrCmpLwr(pszAttributeValue, 
								 flxCtuCommandGetAttribute(pCommand, pszAttributeName)) )
			bResult = FLX_ACT_TRUE;
	}
	return bResult;
}

/***************************************************************************************************
*		flxCtuCommandAttributeIsYes
*
* Returns FLX_ACT_TRUE if the attribute has the value "yes" (ignoring case).
***************************************************************************************************/
FlxActBool flxCtuCommandAttributeIsYes(const FlxCtuCommand* pCommand, 
									   const char*			pszAttributeName,
									   FlxActBool			bDefault)
{
	if (flxCtuCommandAttributeIs(pCommand, pszAttributeName, "yes"))
		return FLX_ACT_TRUE;
	return bDefault;
}

/***************************************************************************************************
*		flxCtuCommandDestroy
*
* Destroy the command object and all that it contains.
***************************************************************************************************/
void flxCtuCommandDestroy(const FlxCtuCommand* pCommand)
{
	size_t index;

	/* Free the name string. */
	flxCtuFree(pCommand->pszNameForMessages);

	/* Free any value. */
	flxCtuFree(pCommand->pszValue);

	/* Free any attributes. */
	sAttributeListFree(pCommand->ppAttributes, pCommand->attributeCount);

	/* Free any dictionaries.	*/
	for (index = 0; index < pCommand->dictionaryCount; index++)
		flxCtuDictionaryFree(pCommand->ppDictionaries[index]);
	/* ... and free the dictionary list. */
	flxCtuFree(pCommand->ppDictionaries);

	/* Finally free the command structure itself. */
	flxCtuFree(pCommand);
}

/***************************************************************************************************
*		flxCtuCommandHasAttribute
*
* Returns FLX_ACT_TRUE if the attribute is present for the command.
***************************************************************************************************/
FlxActBool flxCtuCommandHasAttribute(const FlxCtuCommand* pCommand, const char* pszAttributeName)
{
	return sFindAttribute(pCommand, pszAttributeName, NULL);
}

/***************************************************************************************************
*		flxCtuCommandHasValue
*
* Returns FLX_ACT_TRUE if a value was specified for the command.
***************************************************************************************************/
FlxActBool flxCtuCommandHasValue(const FlxCtuCommand* pCommand)
{
	return pCommand->pszValue != NULL;
}

/***************************************************************************************************
*		flxCtuCommandGetAttribute
*
* Returns the named attribute; only call if it exists (use flxCtuCommandHasAttribute first).
***************************************************************************************************/
const char* flxCtuCommandGetAttribute(const FlxCtuCommand* pCommand, const char* pszAttributeName)
{
	size_t attributeIndex;
	FLX_CTU_ASSERT( sFindAttribute(pCommand, pszAttributeName, &attributeIndex) );

	return pCommand->ppAttributes[attributeIndex]->pKeyValue->pszValue;
}

/***************************************************************************************************
*		flxCtuCommandGetAttributeOrDefault
*
* If the command has the attribute return its value, otherwise the default.
***************************************************************************************************/

const char* flxCtuCommandGetAttributeOrDefault(	const FlxCtuCommand* pCommand, 
												const char* pszAttributeName, 
												const char* theDefault)
{
	return flxCtuCommandHasAttribute(pCommand, pszAttributeName)? 
		flxCtuCommandGetAttribute(pCommand, pszAttributeName) : theDefault;
}


/***************************************************************************************************
*		flxCtuCommandGetName
*
* Returns the defined name of the command.
***************************************************************************************************/
const char* flxCtuCommandGetName(const FlxCtuCommand* pCommand)
{
	return pCommand->pDef->pszName;
}

/***************************************************************************************************
*		flxCtuCommandGetNameForMessages
*
* Returns the name as parsed from the arguments (may be alias, have one or two '-'s)..
***************************************************************************************************/
const char* flxCtuCommandGetNameForMessages(const FlxCtuCommand* pCommand)
{
	return pCommand->pszNameForMessages;
}

/***************************************************************************************************
*		flxCtuCommandGetValue
*
* Returns the command value; only call if it exists (use flxCtuCommandHasValue first).
***************************************************************************************************/
const char* flxCtuCommandGetValue(const FlxCtuCommand*	pCommand)
{
	FLX_CTU_ASSERT( pCommand->pszValue != NULL )
		return pCommand->pszValue;
}

/***************************************************************************************************
*		flxCtuCommandGetValue
*
* If the command has a value, return it, otherwise the default.
***************************************************************************************************/
const char* flxCtuCommandGetValueOrDefault(const FlxCtuCommand* pCommand, const char* theDefault)
{
	return flxCtuCommandHasValue(pCommand)? flxCtuCommandGetValue(pCommand) : theDefault;
}

/***************************************************************************************************
*		flxCtuCommandPrintError
*
* Prefixes the message with the display name from the command object.
***************************************************************************************************/
void flxCtuCommandPrintError(const FlxCtuCommand* pCommand, const char *pszFormatStr, ...)
{
	va_list argList;
	va_start(argList, pszFormatStr);

	/*
		Print an error message with the command name.
	*/
	flxCtuPrintError("\"%s\" ", pCommand->pszNameForMessages);
	/*
		Print the rest.
	*/
	flxCtuPrintMessageV(pszFormatStr, argList);
	va_end(argList);
}

/***************************************************************************************************
*		flxCtuDictionaryFree
*
* Free dictionary and all its entries.
***************************************************************************************************/
void flxCtuDictionaryFree(const FlxCtuDictionary* pDictionary)
{
	if (pDictionary != NULL)
	{
		/* Free the entries and entry list. */
		sKeyValueListFree(pDictionary->ppEntries, pDictionary->entryCount);

		/* Free the name string. */
		flxCtuFree(pDictionary->pszName);

		/* Free the dictionary itself. */
		flxCtuFree(pDictionary);
	}
}

/***************************************************************************************************
*		flxCtuKeyValueFree
*
***************************************************************************************************/
void flxCtuKeyValueFree(FlxCtuKeyValue* pKeyValue)
{
	if (pKeyValue != NULL)
	{
		flxCtuFree(pKeyValue->pszKey);
		flxCtuFree(pKeyValue->pszValue);
		flxCtuFree(pKeyValue);
	}
}

/***************************************************************************************************
*		sAttributeFree
*
***************************************************************************************************/
static void sAttributeFree(FlxCtuAttribute* pAttribute)
{
	if (pAttribute != NULL)
	{
		flxCtuKeyValueFree(pAttribute->pKeyValue);
		flxCtuFree(pAttribute);
	}
}

/***************************************************************************************************
*		sAttributeListFree
*
***************************************************************************************************/
static void sAttributeListFree(FlxCtuAttribute** ppAttributeList, uint32_t count)
{
	if (ppAttributeList != NULL)
	{
		uint32_t index;

		/* Free the key/values. */
		for (index = 0; index < count; index++)
			sAttributeFree(ppAttributeList[index]);

		/* .. and the list. */
		flxCtuFree(ppAttributeList);
	}
}

/***************************************************************************************************
*		sFindAttribute
*
* Return FLX_ACT_TRUE if the command has the attribute, and its index in the attribute list.
***************************************************************************************************/
FlxActBool sFindAttribute(const FlxCtuCommand*	pCommand, 
						  const char*			pszAttributeName, 
						  size_t*				pAttributeIndex)
{
	size_t attributeIndex;

	for (attributeIndex = 0; attributeIndex < pCommand->attributeCount; attributeIndex++)
	{
		if (0 == flxCtuStrCmpLwr(pszAttributeName, 
								 pCommand->ppAttributes[attributeIndex]->pKeyValue->pszKey))
		{
			if (pAttributeIndex != NULL)
				*pAttributeIndex = attributeIndex;
			return FLX_ACT_TRUE;
		}
	}
	return FLX_ACT_FALSE;
}

/***************************************************************************************************
*		sKeyValueListFree
*
***************************************************************************************************/
void sKeyValueListFree(FlxCtuKeyValue** ppKeyValue, uint32_t count)
{
	if (ppKeyValue != NULL)
	{
		uint32_t index;
		
		/* Free the key/values. */
		for (index = 0; index < count; index++)
			flxCtuKeyValueFree(ppKeyValue[index]);

		/* .. and the list. */
		flxCtuFree(ppKeyValue);
	}
}
