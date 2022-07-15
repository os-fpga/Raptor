/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
	This source file is part of a sample utility that demonstrates Composite Transactions.

	It defines the program arguments.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "version_info.h"
#include "version_hotfix.h"

#include "compTranUtilCommandDef.h"
#include "compTranUtilCommand.h"
#include "compTranUtilContext.h"
#include "compTranUtilCustom.h"
#include "compTranUtilShortCode.h"

#include "FlxActCommon.h"

static FlxCtuCommandDef* sFindCommandDef(const char*pszNameOrAlias);
static FlxCtuCommandDef* sGetCommandDef( const char*pszNameOrAlias);
static void sDefineAttributes();
static void sFreeAttributesDefs(FlxCtuCommandDef* pCommandDef);
static void sFreeAttributesDef(FlxCtuAttributeDef* pAttributeDef);

static FlxCtuCommandDef sCommandDefs[] =
{
	{ CMD_ACTIVATE,			"a", flxCtuCommandTraitRequestActionActivate, 0, NULL },
	{ CMD_BRIEF,			"b", flxCtuCommandTraitNone, 0, NULL },
	{ CMD_CANCEL_REQUEST,	"c", flxCtuCommandTraitCommandCancel, 0, NULL },
	{ CMD_CANCEL_SHORTCODE,	"cs", flxCtuCommandTraitNone, 0, NULL },
	{ CMD_REPAIR,			"e", flxCtuCommandTraitRequestActionRepair, 0, NULL },
	{ CMD_LOG_ERRORS,		"g", flxCtuCommandTraitNone, 0, NULL },
	{ CMD_HELP,				"h", flxCtuCommandTraitCommandHelp, 0, NULL },
	{ CMD_LIST_REQUEST,		"l", flxCtuCommandTraitCommandList, 0, NULL },
	{ CMD_NEW,				"n", flxCtuCommandTraitCanHaveValue, 0, NULL },
	{ CMD_VERBOSE,			"o", flxCtuCommandTraitIsVerbose, 0, NULL },
	{ CMD_PROCESS,			"p", flxCtuCommandTraitCommandProcess, 0, NULL },
	{ CMD_RETURN,			"r", flxCtuCommandTraitRequestActionReturn, 0, NULL },
	{ CMD_SERVER_VIEW,		"sv", flxCtuCommandTraitCommandView, 0, NULL },
	{ CMD_SERVER_VIEW_LONG,	"svl", flxCtuCommandTraitCommandView, 0, NULL },
	{ CMD_SHORTCODE,		"sc", flxCtuCommandTraitCommandShortCode, 0, NULL },
	{ CMD_SHORTCODE_RESP,	"scr", flxCtuCommandTraitCommandShortCodeResponse, 0, NULL },
	{ CMD_STORED,			"s", flxCtuCommandTraitCanHaveValue, 0, NULL },
	{ CMD_TRANSACTION,		"t", flxCtuCommandTraitCommandTransaction, 0, NULL },
	{ CMD_VIEW,				"v", flxCtuCommandTraitCommandView, 0, NULL },
	{ CMD_VIEW_LONG,		"vl", flxCtuCommandTraitCommandView, 0, NULL },

	{ CMD_CANCEL_ALL,		"",  flxCtuCommandTraitMain, 0, NULL },
	{ CMD_CUSTOM,			"",  flxCtuCommandTraitIsCustom, 0, NULL },
	{ CMD_DELETE_FR,		"",  flxCtuCommandTraitCommandDelete, 0, NULL },
	{ CMD_HEALTH_CHECK,		"", flxCtuCommandTraitNone, 0, NULL },
	{ CMD_LOCAL,			"", flxCtuCommandTraitCommandLocal, 0, NULL },
	{ CMD_LOCAL_CHECK,		"", flxCtuCommandTraitCommandLocal, 0, NULL },
	{ CMD_LOCAL_RESET,		"reset", flxCtuCommandTraitCommandLocal, 0, NULL },
	{ CMD_LOCAL_REPAIR,		"", flxCtuCommandTraitNone, 0, NULL },
	{ CMD_NO,				"",  flxCtuCommandTraitNone, 0, NULL },
	{ CMD_REPAIR_ALL,		"",  flxCtuCommandTraitIsRequestAction, 0, NULL },
	{ CMD_TIME,				"",  flxCtuCommandTraitNone, 0, NULL },
	{ CMD_UNIQUE,			"umn",  flxCtuCommandTraitNone, 0, NULL },
	{ CMD_VERSION,			"ver",  flxCtuCommandTraitNone, 0, NULL },
	{ CMD_VIRTUAL,			"",  flxCtuCommandTraitNone, 0, NULL },
	{ CMD_YES,				"",  flxCtuCommandTraitNone, 0, NULL }
};
static size_t sCommandDefsCount = sizeof(sCommandDefs) / sizeof(sCommandDefs[0]);

static FlxActBool sbIsAttributeDefsDefined = FLX_ACT_FALSE;

size_t flxCtuCommandDefsCreate(const FlxCtuCommandDef* ppCommandDefs[] )
{
	if (!sbIsAttributeDefsDefined)
	{
		sDefineAttributes();
		sbIsAttributeDefsDefined = FLX_ACT_TRUE;
	}
	*ppCommandDefs = sCommandDefs;
	return sCommandDefsCount;
}

void flxCtuCommandDefsDestroy(FlxCtuCommandDef pCommandDefs[], size_t count)
{
	FLX_CTU_ASSERT(pCommandDefs == sCommandDefs);
	FLX_CTU_ASSERT(count == sCommandDefsCount);

	if (sbIsAttributeDefsDefined)
	{
		size_t index;
		for (index = 0; index < sCommandDefsCount; index++)
			sFreeAttributesDefs(&pCommandDefs[index]);
		
		sbIsAttributeDefsDefined = FLX_ACT_FALSE;
	}
}

static void sHelpAliases(const FlxCtuContext* pContext);
static void sHelpCancel(const FlxCtuContext* pContext);
static void sHelpDeleteFr(const FlxCtuContext* pContext);
static void sHelpDictionary(const FlxCtuContext* pContext);
static void sHelpHealthCheck(const FlxCtuContext* pContext);
static void sHelpList(const FlxCtuContext* pContext);
static void sHelpLocal(const FlxCtuContext* pContext);
static void sHelpNew(const FlxCtuContext* pContext);
static void sHelpOptions(const FlxCtuContext* pContext);
static void sHelpProcess(const FlxCtuContext* pContext);
static void sHelpRepairAll(const FlxCtuContext* pContext);
static void sHelpStored(const FlxCtuContext* pContext);
static void sHelpSummary(const FlxCtuContext* pContext);
static void sHelpTransaction(const FlxCtuContext* pContext);
static void sHelpView(const FlxCtuContext* pContext);
static void sHelpServerView(const FlxCtuContext* pContext);
static void sHelpVersion(const FlxCtuContext* pContext);
static void sHelpVirtual(const FlxCtuContext* pContext);
static void sHelpUnique(const FlxCtuContext* pContext);
static void sHelpLocalRepair(const FlxCtuContext* pContext);
static void sHelpTopics(const FlxCtuContext* pContext);

/***************************************************************************************************
*	flxCtuCommandHelp
*
* -help							Summary of commands and topics
* -help <command or topic>		e.g "-help new"
* -help all						All help as continuous output.
***************************************************************************************************/
void flxCtuCommandHelp(const FlxCtuContext* pContext, const FlxCtuCommand* pCommand)
{
	const char *pszHelpContext = NULL;
	FlxActBool bShowAll;
	FlxCtuCommandDef* pDef;

	flxCtuPrintMessage("Composite Transaction Utility %u.%u.%u.%u\n", FNP_VER_MAJOR,
																	  FNP_VER_MINOR,
																	  FNP_VER_MAINT,
																	  FNP_VER_HOTFIX);

	if ( (pCommand == NULL) || !flxCtuCommandHasValue(pCommand) )
	{
		sHelpSummary(pContext);
		return;
	}

	pszHelpContext  = flxCtuCommandGetValue(pCommand);
	bShowAll		= (0 == flxCtuStrCmpLwr(pszHelpContext, "all"))? FLX_ACT_TRUE : FLX_ACT_FALSE;

	/* If the context is a command alias, replace it with the command name */
	pDef = sFindCommandDef(pszHelpContext);
	if (pDef != NULL)
		pszHelpContext = pDef->pszName;

	if (bShowAll)
	{
		sHelpSummary(pContext);
		sHelpTopics(pContext);
		sHelpAliases(pContext);
		sHelpTransaction(pContext);
		sHelpNew(pContext);
		sHelpStored(pContext);
		sHelpProcess(pContext);
		sHelpHealthCheck(pContext);
		sHelpList(pContext);
		sHelpLocal(pContext);
		sHelpCancel(pContext);
		sHelpView(pContext);
		sHelpServerView(pContext);
		sHelpDeleteFr(pContext);
		sHelpDictionary(pContext);
		sHelpRepairAll(pContext);
		flxCtuHelpShortCode(pContext);
		flxCtuHelpShortCodeResponse(pContext);
		sHelpUnique(pContext);
		sHelpVersion(pContext);
		sHelpVirtual(pContext);
		sHelpOptions(pContext);
		flxCtuCustomHelpDetails(pContext);
	}
	else if (0 == flxCtuStrCmpLwr(pszHelpContext, "topics"))
		sHelpTopics(pContext);

	else if (	(0 == flxCtuStrCmpLwr(pszHelpContext, "aliases"))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, "commands")) )
		sHelpAliases(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_TRANSACTION))
		sHelpTransaction(pContext);

	else if (	(0 == flxCtuStrCmpLwr(pszHelpContext, CMD_NEW))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_ACTIVATE))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_RETURN))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_REPAIR)) ) 
		sHelpNew(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_STORED))
		sHelpStored(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_PROCESS))
		sHelpProcess(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_HEALTH_CHECK))
		sHelpHealthCheck(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_LIST_REQUEST))
		sHelpList(pContext);

	else if (	(0 == flxCtuStrCmpLwr(pszHelpContext, CMD_LOCAL))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_LOCAL_CHECK))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_LOCAL_RESET)) )
		sHelpLocal(pContext);

	else if (	(0 == flxCtuStrCmpLwr(pszHelpContext, CMD_CANCEL_REQUEST))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_CANCEL_ALL)) )
		sHelpCancel(pContext);

	else if (	(0 == flxCtuStrCmpLwr(pszHelpContext, CMD_SHORTCODE))
			 ||	(0 == flxCtuStrCmpLwr(pszHelpContext, CMD_CANCEL_SHORTCODE)) )
		flxCtuHelpShortCode(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_SHORTCODE_RESP))
		flxCtuHelpShortCodeResponse(pContext);

	else if (	(0 == flxCtuStrCmpLwr(pszHelpContext, CMD_VIEW))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_VIEW_LONG)) )
		sHelpView(pContext);

	else if (	(0 == flxCtuStrCmpLwr(pszHelpContext, CMD_SERVER_VIEW))
			 || (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_SERVER_VIEW_LONG)) )
		sHelpServerView(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_DELETE_FR))
		sHelpDeleteFr(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_REPAIR_ALL))
		sHelpRepairAll(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_CUSTOM))
		flxCtuCustomHelpDetails(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_VERSION))
		sHelpVersion(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_VIRTUAL))
		sHelpVirtual(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_UNIQUE))
		sHelpUnique(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, CMD_LOCAL_REPAIR))
		sHelpLocalRepair(pContext);

	else if (	0 == flxCtuStrCmpLwr(pszHelpContext, CMD_BRIEF)
			 || 0 == flxCtuStrCmpLwr(pszHelpContext, CMD_VERBOSE)
			 || 0 == flxCtuStrCmpLwr(pszHelpContext, CMD_YES)
			 || 0 == flxCtuStrCmpLwr(pszHelpContext, CMD_NO)
			 || 0 == flxCtuStrCmpLwr(pszHelpContext, CMD_TIME)
			 || 0 == flxCtuStrCmpLwr(pszHelpContext, CMD_LOG_ERRORS)
			 || 0 == flxCtuStrCmpLwr(pszHelpContext, "options"))
		sHelpOptions(pContext);

	else if (0 == flxCtuStrCmpLwr(pszHelpContext, "topics"))
		sHelpTopics(pContext);
	else if (0 == flxCtuStrCmpLwr(pszHelpContext, "summary"))
		sHelpSummary(pContext);
	else if (	0 == flxCtuStrCmpLwr(pszHelpContext, "dictionary")
			 || 0 == flxCtuStrCmpLwr(pszHelpContext, "vendorData")
			 || 0 == flxCtuStrCmpLwr(pszHelpContext, "vendor"))
		sHelpDictionary(pContext);
	else
	{
		flxCtuPrintMessage("\nNo specific help available for \"%s\"\n\n", pszHelpContext);
		sHelpSummary(pContext);
	}
}

/***************************************************************************************************
*	sDefineAttributes
*
* Adds the attribute definitions to the command definitions.
***************************************************************************************************/
void sDefineAttributes()
{
	FlxCtuCommandDef* pCommandDef;

	/* Attributes for CMD_ACTIVATE */
	pCommandDef = sGetCommandDef(CMD_ACTIVATE);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_COUNT,	 "", ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_DURATION_DAYS,"days", ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_EXP_TYPE, "exptype", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_EXP_VALUE,"exp", ATTR_VALUE_DATE, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FID,		 "fid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_PID,		 "pid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FR,		 "fr", ATTR_VALUE_SHORT, ATTR_VALUE_LONG, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_REASON,	 "", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_RID_TYPE, "type", NULL);

	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ACTIVATABLE,   "", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ACTIVATABLE_OD,"activatableo", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_CONCURRENT,    "", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_CONCURRENT_OD,  "concurrento", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_HYBRID,		  "", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_HYBRID_OD,	  "hybrido", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_REPAIR_COUNT,  "", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);

	/* Attributes for CMD_DELETE_FR */
	pCommandDef = sGetCommandDef(CMD_DELETE_FR);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FEATURE, "", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ENABLED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_PID,		"pid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_TRUSTED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_LIST_REQUEST */
	pCommandDef = sGetCommandDef(CMD_LIST_REQUEST);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FORMAT,	  "", ATTR_VALUE_SHORT, ATTR_VALUE_LONG, 
															 ATTR_VALUE_XML, NULL);

	/* Attributes for CMD_LOCAL */
	pCommandDef = sGetCommandDef(CMD_LOCAL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FROMBUFFER,	"", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_LOCAL_CHECK */
	pCommandDef = sGetCommandDef(CMD_LOCAL_CHECK);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FROMBUFFER,	"", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_LOCAL_RESET */
	pCommandDef = sGetCommandDef(CMD_LOCAL_RESET);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FROMBUFFER,	"", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_NEW */
	pCommandDef = sGetCommandDef(CMD_NEW);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_REFERENCE, "ref", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FR,		  "fr", ATTR_VALUE_SHORT, ATTR_VALUE_LONG, 
															ATTR_VALUE_NONE, NULL);

	/* Attributes for CMD_REPAIR */
	pCommandDef = sGetCommandDef(CMD_REPAIR);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FR, "fr", ATTR_VALUE_SHORT, ATTR_VALUE_LONG, NULL);

	/* Attributes for CMD_RETURN */
	pCommandDef = sGetCommandDef(CMD_RETURN);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_COUNT,	"", ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FR,		"fr", ATTR_VALUE_SHORT, ATTR_VALUE_LONG, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_REASON,	"", NULL);

	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ACTIVATABLE,   "", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ACTIVATABLE_OD,"activatableo", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_CONCURRENT,    "", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_CONCURRENT_OD,  "concurrento", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_HYBRID,		  "", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_HYBRID_OD,	  "hybrido", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_REPAIR_COUNT,  "", 
														  ATTR_VALUE_ALL, ATTR_VALUE_NUMERIC, NULL);

	/* Attributes for CMD_STORED */
	pCommandDef = sGetCommandDef(CMD_STORED);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_REQUEST, "", "#", NULL);

	/* Attributes for CMD_TRANSACTION */
	pCommandDef = sGetCommandDef(CMD_TRANSACTION);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_POLL,	"poll", "#", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_PROXY,	"", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_SSL,		"", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_TIMEOUT, "time", "#", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_COMMS_TYPE, "type", ATTR_VALUE_FLEX, ATTR_VALUE_SOAP, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_RETRIES, "", "#", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_INTERVAL, "", "#", NULL);

	/* Attributes for CMD_VIEW */
	pCommandDef = sGetCommandDef(CMD_VIEW);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FEATURE, "", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ENABLED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FORMAT,	"", ATTR_VALUE_SHORT, ATTR_VALUE_LONG, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_EID,		"eid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_PID,		"pid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_TRUSTED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_VIEW_LONG */
	pCommandDef = sGetCommandDef(CMD_VIEW_LONG);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FEATURE, "", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ENABLED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_EID,		"eid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_PID,		"pid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_TRUSTED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_SERVER_VIEW */
	pCommandDef = sGetCommandDef(CMD_SERVER_VIEW);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FEATURE, "", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ENABLED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FORMAT,	"", ATTR_VALUE_SHORT, ATTR_VALUE_LONG, ATTR_VALUE_FULL, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_EID,		"eid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FID,		"fid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_PID,		"pid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_SERVER,	"svr", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_TRUSTED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_SERVER_VIEW_LONG */
	pCommandDef = sGetCommandDef(CMD_SERVER_VIEW_LONG);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FEATURE, "", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_ENABLED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FORMAT,	"", ATTR_VALUE_SHORT, ATTR_VALUE_LONG, ATTR_VALUE_FULL, NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_EID,		"eid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FID,		"fid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_PID,		"pid", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_SERVER,	"svr", NULL);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_TRUSTED, "", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_SHORTCODES */
	pCommandDef = sGetCommandDef(CMD_SHORTCODE);
	flxCtuCommandDefAddAttribute(pCommandDef, CMD_ATTR_FROMBUFFER,	"", ATTR_VALUE_YES, ATTR_VALUE_NO, NULL);

	/* Attributes for CMD_CUSTOM */
	flxCtuCustomCommandDef(sGetCommandDef(CMD_CUSTOM));
}

/***************************************************************************************************
*	flxCtuCommandDefAddAttribute
*
* Adds an attribute definition to a command definition.
* Global scope so can be called by the custom extensions.
***************************************************************************************************/
void flxCtuCommandDefAddAttribute(FlxCtuCommandDef*	pCommandDef, 
								  const char*		pszName, 
								  const char*		pszAlias,
								  ...)				/* Allowed value strings terminated by NULL. */
{
	const char*			pszValue	  = NULL;
	FlxCtuAttributeDef* pAttributeDef = NULL;
	va_list				argList;

	/* Create a new attribute definition. */
	pAttributeDef = (FlxCtuAttributeDef*)flxCtuMallocAndZeroise(sizeof(FlxCtuAttributeDef));
	pAttributeDef->pszName  = pszName;
	pAttributeDef->pszAlias = pszAlias;

	/* Add the allowed values from the variable argument list, which is terminated by NULL. */
	va_start(argList, pszAlias);
	while (NULL != (pszValue = va_arg(argList, const char*)))
	{
		/* Extend the list. */
		size_t newSize = (pAttributeDef->valueCount + 1) * sizeof(const char*);
		pAttributeDef->ppszValues = (const char**)flxCtuRealloc(pAttributeDef->ppszValues, 
																newSize);
		/* Set the allowed value as the last element. */
		pAttributeDef->ppszValues[pAttributeDef->valueCount++] = pszValue;
	}
	va_end(argList);

	/* Append the attribute definition to the list in the command definition. */
	{
		/* Extend the list. */
		size_t newSize = (pCommandDef->attributeDefCount + 1) * sizeof(FlxCtuAttributeDef*);
		pCommandDef->ppAttributeDefs = (FlxCtuAttributeDef**)
									   flxCtuRealloc(pCommandDef->ppAttributeDefs, newSize);
		/* Set the definition as the last element. */
		pCommandDef->ppAttributeDefs[pCommandDef->attributeDefCount++] = pAttributeDef;
	}


}

/***************************************************************************************************
*	sFreeAttributesDef
*
* Frees an attribute definitions.
***************************************************************************************************/
static void sFreeAttributesDef(FlxCtuAttributeDef* pAttributeDef)
{
	/* Free the value list. */
	flxCtuFree(pAttributeDef->ppszValues);

	/* Free the definition itself. */
	flxCtuFree(pAttributeDef);
}

/***************************************************************************************************
*	sFreeAttributesDefs
*
* Frees the attribute definitions of a command definition.
***************************************************************************************************/
static void sFreeAttributesDefs(FlxCtuCommandDef* pCommandDef)
{
	if (pCommandDef->attributeDefCount > 0)
	{
		size_t attrDefIndex;

		/* Free each attribute definition. */
		for (attrDefIndex = 0; attrDefIndex < pCommandDef->attributeDefCount; attrDefIndex++)
			sFreeAttributesDef(pCommandDef->ppAttributeDefs[attrDefIndex]);

		/* Free the list. */
		flxCtuFree(pCommandDef->ppAttributeDefs);
	}	
}

/***************************************************************************************************
*	sFindCommandDef
*
* Return the command definition for a given command name or alias.
***************************************************************************************************/
static FlxCtuCommandDef* sFindCommandDef(const char* pszNameOrAlias)
{
	size_t index;

	for (index = 0; index < sCommandDefsCount; index++)
	{
		if (	(0 == strcmp(sCommandDefs[index].pszName,  pszNameOrAlias))
			 || (0 == strcmp(sCommandDefs[index].pszAlias, pszNameOrAlias)))
			return &sCommandDefs[index];
	}
	return NULL;
}

/***************************************************************************************************
*	sGetCommandDef
*
* Return the command definition for a given command name, asserts if not found
***************************************************************************************************/
static FlxCtuCommandDef* sGetCommandDef(const char* pszNameOrAlias)
{
	FlxCtuCommandDef* pDef = sFindCommandDef(pszNameOrAlias);

	FLX_CTU_ASSERT(pDef != NULL);
	return pDef;
}

/***************************************************************************************************
*	sHelpSummary
*
* Show a summary of the usage, and how to get more details.
***************************************************************************************************/
static void sHelpSummary(const FlxCtuContext* pContext)
{
	const char* pszProgramName = pContext->pszProgramName;

	flxCtuPrintOutput(
		"\n"
		"  %s is a sample activation utility that demonstrates\n"
		"    composite transactions and supporting functionality.\n", pContext->pszProgramName);
	flxCtuPrintOutput(	
		"\n"
		"Syntax:\n"
		"  -<command> [value] [attribute=value...] [dictionary] [<action>...] [option...]\n"
		"\n"
		"  <action> = -<action> [value] [attribute=value...] [dictionary]/n"
		"\n"
		"All command, attribute, action and option keywords are case insensitive, many\n"
		"have short aliases and '--' is accepted for '-'. So these are all equivalent:\n"
		"  -serverViewLong -serverviewlong -svl --svl\n"
		"\n"
	);
	flxCtuPrintOutput("  %s -help topics          For a list of topics\n", pszProgramName);
	flxCtuPrintOutput("  %s -help <topic>         For help on a topic\n", pszProgramName);
	flxCtuPrintOutput("  %s -help all             For help on all topics\n", pszProgramName);
	flxCtuPrintOutput("  %s -help commands        For a list of commands, attributes and options\n", pszProgramName);
}

/***************************************************************************************************
*	sHelpTopics
*
* Show a list of help topics.
***************************************************************************************************/
static void sHelpTopics(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(
		"\n"
		"Transaction topics:\n"
		"  new             Create a request to start a manual transaction\n"
		"  process         Process a response to complete a manual transaction\n"
		"  transaction     Perform an online composite transaction\n"
		"  shortCode            Create and manage shortcode requests\n"
		"  shortCodeResponse    Process a shortcode response\n"
		"\n" 
		"Manage stored request topics:\n"
		"  stored listRequests cancelRequest cancelAll\n"
		"\n"
		"Local activation topics:\n"
		"  local localCheck localReset\n"
		"\n"
		"Manage fulfillment record topics:\n"
		"  view viewLong deleteFR serverView serverViewLong\n"
		"\n"
		"Other topics:\n"
		"  aliases     Command, attribute and option names and aliases\n"
		"  commands    Command, attribute and option names and aliases\n"
		"  healthCheck Run the trusted storage health check\n"
		"  options     General options: brief verbose logReport\n"
		"  unique      UMNs (Unique Machine Numbers)\n"
		"  version     Version information\n"
		"  virtual     Virtualization information\n"
	);
	return;
}

/***************************************************************************************************
*	sHelpAliases
*
* Show command, attribute and option names and aliases.
***************************************************************************************************/
static void sHelpAliases(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(
		"\n"
		"Commands and their aliases:\n"
		"  cancelAll\n"
		"  cancelRequest           c     \n"
		"  cancelShortcodeRequest  cs    \n"
		"  custom\n"
		"  deleteFR\n"
		"  healthCheck\n"
		"  help                    h     \n"
		"  listRequests            l     \n"
		"  local                   lo    \n"
		"  localCheck              lc    \n"
		"  localReset              lr    \n"
		"  localRepair\n"
		"  new                     n     \n"
		"  process                 p     \n"
		"  serverView              sv    \n"
		"  serverViewLong          svl   \n"
		"  shortcode               sc    \n"
		"  shortcodeResponse       scr   \n"
		"  stored                  s     \n"
		"  transaction             t     \n"
		"  unique                  umn   \n"
		"  version                 ver   \n"
		"  view                    v     \n"
		"  viewlong                vl    \n"
		"  virtual\n"
		"\n"
		"Request actions and their aliases:\n"
		"  activate                a     \n"
		"  repair                  e     \n"
		"  repairAll\n"
		"  return                  r     \n"
		"\n"
		"Options and their aliases:\n"
		"  brief                   b     \n"
		"  logErrors               g     \n"
		"  no                            \n"
		"  time                          \n"
		"  verbose                 o     \n"
		"  yes                           \n"
		"\n"
		"Attributes and their aliases:   \n"
		"  activatable\n"
		"  activatableOd\n"
		"  commsType\n"
		"  concurrent\n"
		"  concurrentOd\n"
		"  count\n"
		"  durationDays            days  \n"
		"  entitlementId           eid   \n"
		"  enabled\n"
		"  expiration              exp   \n"
		"  expirationType\n"
		"  fulfillmentRecord       fr    \n"
		"  frombuffer\n"
		"  feature\n"
		"  fulfillmentId           fid   \n"
		"  format\n"
		"  hybrid\n"
		"  hybridOd\n"
		"  interval\n"
		"  productId               pid  \n"
		"  pollInterval            poll \n"
		"  proxy\n"
		"  reason\n"
		"  reference               ref  \n"
		"  repairCount\n"
		"  request\n"
		"  retries\n"
		"  ssl\n"
		"  timeout\n"
		"  rightsType\n"
		"  trusted\n"
	);
}

/***************************************************************************************************
*	sHelpOptions
*
* Show the command options.
***************************************************************************************************/
static void sHelpOptions(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"Options - add to any command.\n"
		"=======\n"
		"\n"
		"  -time          Show timings for execution steps\n"
		"  -yes           Respond 'yes' to confimation prompts\n"
		"  -no            Respond 'no' to confimation prompts\n"
		"  -brief     -b  Minimize message output\n"
		"  -verbose   -o  Maximize message output\n"
		"  -logErrors -g  Call API function flxActTransactionLogError \n"
		"                 when a flxActTranXXX function fails unexpectedly.\n"
		);
	return;
}

/***************************************************************************************************
*	sHelpCancel
*
* Show the command line usage for the -cancel and -cancelAll commands.
***************************************************************************************************/
static void sHelpCancel(const FlxCtuContext* pContext)
{
	flxCtuPrintMessage(
		"\n"
		"cancelRequest command: cancel a stored request because no response\n"
		"=============          is expected.\n"
		"   The -listRequests command shows requests and their sequence numbers\n"
		"\n"
		"      -cancelRequest|-c <sequenceNo>]\n"	
		"\n"
		"cancelAll command: cancel all stored requests.\n"
		"=========\n"
		"      -cancelAll\n"	
		"\n"
		"Examples:\n");
	flxCtuPrintOutput(	"  %s -cancelRequest 23\n", pContext->pszProgramName);
	flxCtuPrintOutput(	"  %s -c 23\n", pContext->pszProgramName);
	flxCtuPrintOutput(	"  %s -cancelAll\n", pContext->pszProgramName);

	return;
}

/***************************************************************************************************
*	sHelpDeleteFr
*
* Show the command line usage for the -deleteFR command.
***************************************************************************************************/
static void sHelpDeleteFr(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(
		"\n"
		"deleteFR command: delete fulfillment record(s) from trusted storage.\n"
		"========\n"
		"    -deleteFR [<fulfillmentid>] [<filters>]\n"	
		"\n"
		"  Filters:\n"
		"    productId|pid=<pid>  Only records with this ProductId.\n"
		"    feature=<string>     Only records with <string> in the feature line.\n"
		"    trusted=yes|no       Only trusted|untrusted records.\n"
		"    enabled=yes|no       Only enabled|disabled records.\n"
		"\n"
		"  The fulfillments to be deleted are displayed and confirmation requested\n"
		"  (use -yes for automatic coonfirmation).\n"
		"\n"
	);
	flxCtuPrintOutput("\n");
	flxCtuPrintOutput("Examples: delete fulfillment records (for syntax and shortcuts use \"-help deleteFR\")\n");
	flxCtuPrintOutput("  %s -deleteFR MyFulfillmentId\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -deleteFR productId=MyProductId\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -deleteFR trusted=no\n", pContext->pszProgramName);

	return;
}

/***************************************************************************************************
*	sHelpDictionary
*
* Show the use of dictionaries to send vendor data.
***************************************************************************************************/
static void sHelpDictionary(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(
		"\n"
		"dictionary help.  Dictionaries are used to include custom (vendor) data in requests\n"
		"==========\n"
		"  Vendor data can be included using the gVendor dictionary.\n"
		"    -gVendor{ <key>=<value>...}\n"	
		"  \n"	
		"Examples:\n"	
	);
	flxCtuPrintOutput("  %s -new gVendor{MyKey1=MyValue1 MyKey2=MyValue2} -activate MyActId\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -transaction http://myfnoserver.com:7777/flexnet/services/ActivationService gVendor{MyKey1=MyValue1 MyKey2=MyValue2} -activate MyActId\n", pContext->pszProgramName);

	flxCtuPrintOutput(
		"  \n"	
		"  It can also be included for each action but this is not supported by FlexNet servers.\n"	
		"    -<action> <value> [attributes] vendor{ <key>=<value>...}\n"	
		"  \n"	
		"Example:\n"	
	);
	flxCtuPrintOutput("  %s -new -return MyFid vendor{MyKey1=MyValue1} -activate MyActId days=90 vendor{MyKey2=MyValue2}\n", pContext->pszProgramName);

	flxCtuPrintOutput(
		"  \n"	
		"  A second dictionary type (gFlexNet/FlexNet) is also defined; this is reserved for future use.\n"	
	);
	return;
}

/***************************************************************************************************
*	sHelpHealthCheck
*
* Runs the trusted storage health check and resports any isses.
***************************************************************************************************/
static void sHelpHealthCheck(const FlxCtuContext* pContext)
{
	flxCtuPrintMessage(
		"\n"
		"healthCheck command: Runs the trusted storage health check and reports any issues.\n"
		"===========          If any checks fail, see the event log for more details.\n"
		"\n"
		"      -healthCheck\n"
		"\n"
	);
	flxCtuPrintOutput("Examples: \n");
	flxCtuPrintOutput("  %s -healthCheck\n", pContext->pszProgramName);
}

/***************************************************************************************************

*	sHelpList
*
* Show the command line usage for the listRequests command.
***************************************************************************************************/
static void sHelpList(const FlxCtuContext* pContext)
{
	flxCtuPrintMessage(
		"\n"
		"listRequests command: list the request(s) that are stored\n"
		"============          awaiting a response.\n"
		"\n"
		"      -listRequests|-l [<seqNo>] [format=<formatType>]\n"
		"\n"
		"  seqNo       The request with this sequence number, default all\n"
		"\n"
		"  Format types (default is \"short\"):\n"
		"    short     <seqNo> <status> <time created> <reference>\n"
		"    long      Multi-line with format <name>=<value>\n"
		"    xml       The full xml of the request(s)\n"
		"\n"
	);
	flxCtuPrintOutput("Examples: \n");
	flxCtuPrintOutput("  %s -listRequests\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -l 23 format=long\n", pContext->pszProgramName);
}

/***************************************************************************************************
*	sHelpLocal
*
* Show the usage of the "localCheck", "local" and "localReset" commands.
***************************************************************************************************/
static void sHelpLocal(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"localCheck, local and localReset commands: local activation (trials).\n"
		"================================\n"
		"\n"
		"  Local activation requires an ASR XML document.  The API supports \n"
		"  processing the ASR directly from a file or from a memory buffer.\n"
		"  The \"fromBuffer\" attribute is used to choose - the default is \"no\".\n"
		"\n"
		"  -localCheck <MyAsrpath> [fromBuffer=yes|no]       Check whether the ASR has been added to TS\n"
		"  -local      <MyAsrpath> [fromBuffer=yes|no]       Add the ASR to TS\n"
		"  -localReset <MyAsrpath> [fromBuffer=yes|no]       Perform a reset so the ASR can be added again\n"
		"\n"
		"Examples:\n");
		if (pContext->bIsServer)
			flxCtuPrintOutput(	"  %s -localCheck ../examples/ezcalc/30day_trial_server/30day_trial_server.asr\n"
								"  %s -local ../examples/ezcalc/30day_trial_server/30day_trial_server.asr\n"
								"  %s -local ../examples/ezcalc/30day_trial_server/30day_trial_server.asr fromBuffer=yes\n"
								"  %s -localReset ../examples/ezcalc/30day_trial_server/30day_reset_server.asr\n",
								pContext->pszProgramName, pContext->pszProgramName, pContext->pszProgramName, pContext->pszProgramName);
		else
			flxCtuPrintOutput(	"  %s -localCheck ../examples/ezcalc/30day_acct/30day_acct.asr\n"
								"  %s -local ../examples/ezcalc/30day_acct/30day_acct.asr\n"
								"  %s -local ../examples/ezcalc/30day_acct/30day_acct.asr fromBuffer=yes\n"
								"  %s -localReset ../examples/ezcalc/30day_acct_reset/30day_reset.asr\n",
								pContext->pszProgramName, pContext->pszProgramName, pContext->pszProgramName, pContext->pszProgramName);
	return;
}


/***************************************************************************************************
*		sHelpNew
*
*	Show the usage for the "new" command.
***************************************************************************************************/
static void sHelpNew(const FlxCtuContext* pContext)
{
	const char* pszServerCounts = pContext->bIsServer?
		"                 activatable activatableOd concurrent concurrentOd\n"
		"                 hybrid hybridOd repairCount\n"
	:
		"";

	flxCtuPrintOutput(
		"\n"
		"new command: create a new request.\n"
		"===\n"
		"  -new|-n [<filename>] [fulfillmentRecord|fr=short|long|none]\n"
		"          [reference|ref=<ref>] [<gDictionary>...] [<action> ...]\n"
		"\n"
		"  <filename>   Request is written to this file; optional, default stdout.\n"
		"  <ref>        A reference that can be used to identify the request later.\n"
		"\n"
		"  Global dictionaries <gDictionary>:\n"
		"    gFlexNet|gVendor{<key1>=<value1> ...}\n"
		"\n"
		"  Actions:\n"
		"   -activate|-a <RightsId> [<attribute>=<value> ...] [<dictionary>...]\n"
		"     attributes: count rightsType|type reason\n"
		"                 durationDays|days|expiration|exp  expirationType|expType\n"
		"                 fulfillmentId|fid fulfillmentRecord|fr\n"
		"%s"
		"   -return|-r <FulfillmentId> [<attribute>=<value> ...] [<dictionary>...]\n"
		"     attributes: count reason fulfillmentRecord|fr\n"
		"%s"
		"   -repair|-e <FulfillmentId> [<dictionary>...]\n"
		"     attributes: fulfillmentRecord|fr\n"
		"   -repairAll\n"
		"\n"
		"  Action dictionaries <dictionary>:\n"
		"    FlexNet|Vendor{<key1>=<value1> ...}\n"
		"\n"
		"Notes:\n"
		"  'RightsId' is the entitlement or activation id provided by the server.\n"
		"  'fr' controls the amount of existing fulfilment information included.\n"
		"  'count' is not used by FlexNet servers.\n"
		"  'expirationType' is not used by FlexNet servers.\n"
		"  'fid' with -activate is not used by FlexNet servers.\n"
		"  Only the 'gVendor' dictionary is used by FlexNet servers.\n"
		"\n"
		, pszServerCounts, pszServerCounts);

	flxCtuPrintOutput("Examples: create request XML\n");
	flxCtuPrintOutput("  %s -new request.xml -activate MyActId\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -new request.xml -activate MyActId expiration=31-dec-2022\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -new request.xml -a MyActId1 days=7 -a MyActId2 days=30\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -new request.xml -return MyFulfillmentId\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -new request.xml -r MyFid -a MyNewActId\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -new request.xml -repair MyFid1 -repair MyFid2\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -new request.xml -repairAll\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -new request.xml gVendor{MyKey1=value1 MyKey2=value2} -a MyActId\n", pContext->pszProgramName);
}

/***************************************************************************************************
*	sHelpProcess
*
* Show the usage of the "process" command.
***************************************************************************************************/
static void sHelpProcess(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"process command: process a response.\n"
		"=======\n"
		"      -process <filename>\n"
		"\n"
		"  <filename> Response is read from this file.\n"
		"\n"
		);
	flxCtuPrintOutput("Example: process response XML\n");
	flxCtuPrintOutput("  %s -process responsefilename.xml\n", pContext->pszProgramName);
	return;
}

/***************************************************************************************************
*	sHelpRepairAll
*
* Show the usage of the "repairAll" command.
***************************************************************************************************/
static void sHelpRepairAll(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"repairAll action: adds repair actions to a request for every\n"
		"=========         fulfillment record that is not fully trusted.\n"
		"\n"
		);
	flxCtuPrintOutput("  %s -new request.xml -repairAll\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -transaction 27000@192.168.1.123 -repairAll\n", pContext->pszProgramName);
	return;
}

/***************************************************************************************************
*	sHelpStored
*
* Show the usage of the "stored" command.
***************************************************************************************************/
static void sHelpStored(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"stored command: retrieve a previously created request from Trusted Storage.\n"
		"======\n"
		"      -stored|-s <filename> [request=<sequenceNo>]\n"
		"\n"
		"  <filename>   Request is written to this file; default stdout.\n"
		"  <sequenceNo> The sequence number of the request; default is most recent.\n"
		"               (use -listRequest to show requests and their sequence no).\n"
		"\n"
	);
	flxCtuPrintOutput("Examples: get a stored request XML\n");
	flxCtuPrintOutput("  %s -stored\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -stored request.xml\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -s request.xml 23\n", pContext->pszProgramName);
	flxCtuPrintOutput("Example: resend latest request to server\n");
	flxCtuPrintOutput("  %s -transaction 27000@192.168.1.123 -s\n", pContext->pszProgramName);
	return;
}

/***************************************************************************************************
*	sHelpTransaction
*
* Show the usage of the "transaction" command.
***************************************************************************************************/
static void sHelpTransaction(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"transaction command: perform a composite transaction with a server.\n"
		"===========\n"
		"      -transaction <Server> [<attribute>...] <new>|<stored>\n"
		"\n"
		"  Attributes (all optional):\n"
		"    commsType=SOAP|FLEX\n"
		"    proxy = \"<host> <port> [<userId> <passwd>]\"\n"
		"    ssl = \"<CaCert> <CaPath>\"\n"
		"    timeout|time = <seconds>\n"
		"    pollInterval|poll = <seconds>\n"
		"    retries = <numberOfRetries>\n"
		"\n"
		"  The commsType is defaulted to FLEX for addreses of form port@server, otherwise SOAP.\n"
		"  <new> specifies a new request to create and send (see the 'new' help topic);\n"
		"  the '-new' tag is only needed if you wish to specify a request reference.\n"
		"  <stored> is used to re-send an existing request, e.g. after comms failure.\n"
		"  The request is sent to the server and the response processed when it is received.\n"
		"  Default retry count is 3. To automatically confirm each retry, add the -yes option.\n"
		"\n"
		);
		flxCtuPrintOutput("Examples: transactions with an Operations Server (new request)\n");
		flxCtuPrintOutput("  %s -transaction http://myfnoserver.com:7777/flexnet/services/ActivationService -new MyRef -activate MyActId\n", pContext->pszProgramName);
		flxCtuPrintOutput("  %s -transaction http://myfnoserver.com:7777/flexnet/services/ActivationService -activate MyActId [durationDays | days]=90 -yes\n", pContext->pszProgramName);
		flxCtuPrintOutput("  %s -t http://myfnoserver.com:7777/flexnet/services/ActivationService -return MyFulfillmentId\n", pContext->pszProgramName);
		flxCtuPrintOutput("  %s -t http://myfnoserver.com:7777/flexnet/services/ActivationService -repair MyFulfillmentId\n", pContext->pszProgramName);
		if (pContext->bIsServer)
		{
			flxCtuPrintOutput("  %s -t http://myfnoserver.com/flexnet/services/ActivationService -activate MyActId hybrid=10\n", pContext->pszProgramName);
		}
		flxCtuPrintOutput("Example: transaction with an Operations Server (reuse latest stored request)\n");
		flxCtuPrintOutput("  %s -t http://myfnoserver.com/flexnet/services/ActivationService -stored\n", pContext->pszProgramName);

		if (pContext->bIsServer)
		{
			flxCtuPrintOutput("Examples: transfers from/to another Enterprise License Server\n");
			flxCtuPrintOutput("  %s -t 27000@10.81.1.34 -activate MyEntitlementId hybrid=10 repairCount=2\n", pContext->pszProgramName);
			flxCtuPrintOutput("  %s -t 27000@10.81.1.23 -return MyFulfillmentId\n", pContext->pszProgramName);
		}
		else
		{
			flxCtuPrintOutput("Examples: transactions with an Enterprise License Server\n");
			flxCtuPrintOutput("  %s -t 27000@10.81.1.34 -activate MyEntitlementId\n", pContext->pszProgramName);
			flxCtuPrintOutput("  %s -t 27000@10.81.1.34 -return MyFulfillmentId\n", pContext->pszProgramName);
		}
	return;
}

/***************************************************************************************************
*	sHelpView
*
* Show the command line usage for the -view command.
***************************************************************************************************/
static void sHelpView(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(
		"\n"
		"view command: view the fulfillment records in trusted storage, including those\n"
		"====          created by means other than composite transactions.\n"
		"\n"
		"    -view|-v [<fulfillmentiId>] [<filters>] [format=<formatType>]\n"	
		"\n"
		"  Filters:\n"
		"    productId|pid=<pid>      Only records with this Product Id.\n"
		"    entitlementId|eid=<eid>  Only records with this Entitlement Id.\n"
		"    feature=<string>         Only records with <string> in the feature line(s).\n"
		"    trusted=yes|no           Only trusted|untrusted records.\n"
		"    enabled=yes|no           Only enabled|disabled records.\n"
		"\n"
		"  Format types (default is short):\n"
		"    short     U D FulfillmentType Expiration Count FulfillmentId ProductId\n"
		"              where U is replaced by '.' if trusted,  D by '.' if enabled.\n"
		"    long      Multi-line with format <name>=<value>\n"
		"\n"
		"viewlong command: as -view but defaults to format=long\n"
		"========\n"
		"\n"
		);

	flxCtuPrintOutput("Examples: view fulfillment record(s)\n");
	flxCtuPrintOutput("    %s -view\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -view trusted=no\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -view enabled=no\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -view feature=ADD enabled=yes\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -viewlong\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -viewlong MyFulfillmentId\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -viewlong productId=MyPid\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -vl pid=MyPid\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -viewlong feature=ADD enabled=yes\n", pContext->pszProgramName);
	
	return;
}

/***************************************************************************************************
*	sHelpServerView
*
* Show the command line usage for the -serverview command.
***************************************************************************************************/
static void sHelpServerView(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(
		"\n"
		"serverView command: view the fulfillment records from a server.\n"
		"==========\n"
		"\n"
		"    -serverView|-sv <serverAddress> [<filters>] [format=<formatType>]\n"	
		"\n"
		"  Filters:\n"
		"    fulfillmentId=<fid>      The record with this Fulfillment Id.\n"
		"    productId|pid=<pid>      Only records with this Product Id.\n"
		"    entitlementId|eid=<eid>  Only records with this Entitlement Id.\n"
		"    feature=<string>         Only records with <string> in the feature line.\n"
		"    trusted=yes|no           Only trusted|untrusted records.\n"
		"    enabled=yes|no           Only enabled|disabled records.\n"
		"\n"
		"  Server address defaults to \"@localhost\"\n"
		"\n"
		"  Format types (default is full if fid or feature given, otherwise short):\n"
		"    short     U D FulfillmentType Expiration Count FulfillmentId ProductId\n"
		"              where U is replaced by . if trusted,  D by . if enabled.\n"
		"    long      Multi-line (but no feature or deduction information)\n"
		"    full      Multi-line including feature and deduction information)\n"
		"\n"
		"serverViewLong command: as -serverview but with default to format=long\n"
		"==============\n"
		"\n"
		);

	flxCtuPrintOutput("Examples: view server fulfillment record(s)\n");
	flxCtuPrintOutput("    %s -serverView\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -serverView format=full\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -serverView 27000@192.168.1.123 productId=MyProductId\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -serverViewLong 27000@192.168.1.123 fulfillmentId=MyFulfillmentId\n", pContext->pszProgramName);
	flxCtuPrintOutput("    %s -serverViewLong 27000@192.168.1.123 entitlementId=MyEntitlementId\n", pContext->pszProgramName);

	return;
}

/***************************************************************************************************
*	sHelpVersion
*
* Show the usage of the "version" command.
***************************************************************************************************/
static void sHelpVersion(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"version command: print version details.\n"
		"=======\n"
		);
	flxCtuPrintOutput("  %s -version\n", pContext->pszProgramName);
	return;
}

/***************************************************************************************************
*	sHelpVirtual
*
* Show the usage of the "virtual" command.
***************************************************************************************************/
static void sHelpVirtual(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"virtual command: print virtualization details for this environment.\n"
		"=======\n"
		);
	flxCtuPrintOutput("  %s -virtual\n", pContext->pszProgramName);
	return;
}

/***************************************************************************************************
*	sHelpUnique
*
* Show the usage of the "unique" command.
***************************************************************************************************/
static void sHelpUnique(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"unique command: print Unique Machine Number (UMN) details for this environment.\n"
		"======\n"
	);
	flxCtuPrintOutput("  %s -unique\n", pContext->pszProgramName);
	flxCtuPrintOutput("  %s -umn\n", pContext->pszProgramName);
	return;
}

/***************************************************************************************************
*	sHelpLocalRepair
*
* Show the usage of the "localRepair" command.
***************************************************************************************************/
static void sHelpLocalRepair(const FlxCtuContext* pContext)
{
	flxCtuPrintOutput(	
		"\n"
		"localRepair command: latches breaks in individual fulfillment records so that life cycle\n"
		"===========          operations such as local activation and creating requests can be done.\n"
		"                     Provided for API testing, it's done by other commands when needed.\n"
		);
	flxCtuPrintOutput("  %s -localRepair\n", pContext->pszProgramName);
	return;
}

