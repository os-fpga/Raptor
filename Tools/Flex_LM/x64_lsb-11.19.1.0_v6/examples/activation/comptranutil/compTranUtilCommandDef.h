/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions.

	It defines command and attribute names and traits used by FlxCtuCommandDef.c to
	define the valid command and attribute combinations that are allowed.

*/
#ifndef COMP_TRAN_UTIL_COMMAND_DEF_H
#define COMP_TRAN_UTIL_COMMAND_DEF_H

#include "compTranUtilHelper.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
	The command name strings; case is ignored so cancelall==cancelAll==CANCELALL etc.
*/
#define CMD_ACTIVATE		"activate"
#define CMD_BRIEF			"brief"
#define CMD_CANCEL_ALL		"cancelall"
#define CMD_CANCEL_REQUEST	"cancelrequest"
#define CMD_CANCEL_SHORTCODE "cancelshortcoderequest"
#define CMD_CUSTOM			"custom"
#define CMD_DELETE_FR		"deletefr"
#define CMD_HEALTH_CHECK	"healthcheck"
#define CMD_HELP			"help"
#define CMD_LIST_REQUEST	"listrequests"
#define CMD_LOCAL			"local"
#define CMD_LOCAL_CHECK		"localcheck"
#define CMD_LOCAL_RESET		"localreset"
#define CMD_LOCAL_REPAIR	"localrepair"
#define CMD_LOG_ERRORS		"logerrors"
#define CMD_NEW				"new"
#define CMD_NO				"no"
#define CMD_PROCESS			"process"
#define CMD_REPAIR			"repair"
#define CMD_REPAIR_ALL		"repairall"
#define CMD_RETURN			"return"
#define CMD_SERVER_VIEW		"serverview"
#define CMD_SERVER_VIEW_LONG	"serverviewlong"
#define CMD_SHORTCODE		"shortcode"
#define CMD_SHORTCODE_RESP	"shortcoderesponse"
#define CMD_STORED			"stored"
#define CMD_TIME			"time"
#define CMD_TRANSACTION		"transaction"
#define CMD_UNIQUE			"unique"
#define CMD_VIEW			"view"
#define CMD_VIEW_LONG		"viewlong"
#define CMD_VERBOSE			"verbose"
#define CMD_VERSION			"version"
#define CMD_VIRTUAL			"virtual"
#define CMD_YES				"yes"

/*
	The attribute name strings.
*/
#define CMD_ATTR_ACTIVATABLE	"activatable"
#define CMD_ATTR_ACTIVATABLE_OD	"activatableod"
#define CMD_ATTR_COMMS_TYPE		"commstype"
#define CMD_ATTR_CONCURRENT		"concurrent"
#define CMD_ATTR_CONCURRENT_OD	"concurrentod"
#define CMD_ATTR_COUNT			"count"
#define CMD_ATTR_DURATION_DAYS	"durationdays"
#define CMD_ATTR_EID			"entitlementid"
#define CMD_ATTR_ENABLED		"enabled"
#define CMD_ATTR_EXP_VALUE		"expiration"
#define CMD_ATTR_EXP_TYPE		"expirationtype"
#define CMD_ATTR_FR				"fulfillmentrecord"
#define CMD_ATTR_FROMBUFFER		"frombuffer"
#define CMD_ATTR_FEATURE		"feature"
#define CMD_ATTR_FID			"fulfillmentid"
#define CMD_ATTR_FORMAT			"format"
#define CMD_ATTR_HYBRID			"hybrid"
#define CMD_ATTR_HYBRID_OD		"hybridod"
#define CMD_ATTR_INTERVAL		"interval"
#define CMD_ATTR_PID			"productid"
#define CMD_ATTR_POLL			"pollinterval"
#define CMD_ATTR_PROXY			"proxy"
#define CMD_ATTR_REASON			"reason"
#define CMD_ATTR_REFERENCE		"reference"
#define CMD_ATTR_REPAIR_COUNT	"repairCount"
#define CMD_ATTR_REQUEST		"request"
#define CMD_ATTR_RETRIES		"retries"
#define CMD_ATTR_SERVER			"server"
#define CMD_ATTR_SSL			"ssl"
#define CMD_ATTR_TIMEOUT		"timeout"
#define CMD_ATTR_RID_TYPE		"rightstype"
#define CMD_ATTR_TRUSTED		"trusted"

/*
	The attribute value strings.
*/
#define ATTR_VALUE_ALL		"all"
#define ATTR_VALUE_FLEX		"flex"
#define ATTR_VALUE_FULL		"full"
#define ATTR_VALUE_LONG		"long"
#define ATTR_VALUE_NO		"no"
#define ATTR_VALUE_NONE		"none"
#define ATTR_VALUE_NUMERIC	"#"		/* Special case - any numeric string. */
#define ATTR_VALUE_DATE		"_flexdate"	/* Special case - flex date string. */
#define ATTR_VALUE_SOAP		"soap"
#define ATTR_VALUE_SHORT	"short"
#define ATTR_VALUE_XML		"xml"
#define ATTR_VALUE_YES		"yes"

/*
	The dictionary names.
*/
#define CMD_DICT_FLEX			"flexnet"
#define CMD_DICT_VENDOR			"vendor"
#define CMD_DICT_GLOBAL_FLEX	"gflexnet"
#define CMD_DICT_GLOBAL_VENDOR	"gvendor"

typedef enum FlxCtuCommandTrait_enum
{
	flxCtuCommandTraitNone					= (0),
	flxCtuCommandTraitMain					= (0x1 << 0),

	flxCtuCommandTraitCanHaveValue			= (0x1 << 2),
	flxCtuCommandTraitCanHaveAttributes		= (0x1 << 3),
	flxCtuCommandTraitCanHaveDictionaries	= (0x1 << 4),
	flxCtuCommandTraitIsRequestAction		= (0x1 << 5),
	flxCtuCommandTraitDuplicatesOK			= (0x1 << 6),

	flxCtuCommandTraitIsVerbose				= (0x1 << 7),
	flxCtuCommandTraitIsCustom				= (0x1 << 8),

	/* Aggregates */
	flxCtuCommandTraitRequestActionActivate	= (flxCtuCommandTraitIsRequestAction
											| flxCtuCommandTraitDuplicatesOK
											| flxCtuCommandTraitCanHaveValue
											| flxCtuCommandTraitCanHaveAttributes
											| flxCtuCommandTraitCanHaveDictionaries),

	flxCtuCommandTraitCommandCancel			= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue),

	flxCtuCommandTraitCommandList			= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue),
	
	flxCtuCommandTraitCommandLocal			= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue
											| flxCtuCommandTraitCanHaveAttributes),
	
	flxCtuCommandTraitCommandHelp			= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue),

	flxCtuCommandTraitRequestActionReturn	= (flxCtuCommandTraitIsRequestAction
											| flxCtuCommandTraitDuplicatesOK
											| flxCtuCommandTraitCanHaveValue	
											| flxCtuCommandTraitCanHaveAttributes
											| flxCtuCommandTraitCanHaveDictionaries),

	flxCtuCommandTraitRequestActionRepair	= (flxCtuCommandTraitIsRequestAction
											| flxCtuCommandTraitDuplicatesOK
											| flxCtuCommandTraitCanHaveValue	
											| flxCtuCommandTraitCanHaveDictionaries),

	flxCtuCommandTraitCommandProcess		= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue),

	flxCtuCommandTraitCommandTransaction	= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue	
											| flxCtuCommandTraitCanHaveAttributes),

	flxCtuCommandTraitCommandView			= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue	
											| flxCtuCommandTraitCanHaveAttributes),

	flxCtuCommandTraitCommandDelete			= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue	
											| flxCtuCommandTraitCanHaveAttributes),

	flxCtuCommandTraitCommandShortCode		= (flxCtuCommandTraitMain
											| flxCtuCommandTraitCanHaveValue	
											| flxCtuCommandTraitCanHaveAttributes),

	flxCtuCommandTraitCommandShortCodeResponse	= (flxCtuCommandTraitMain
												| flxCtuCommandTraitCanHaveValue)

} FlxCtuCommandTrait;

typedef struct FlxCtuAttributeDef_struct
{
	const char*			pszName;
	const char*			pszAlias;
	size_t				valueCount;
	const char**		ppszValues;
} FlxCtuAttributeDef;

typedef struct FlxCtuCommandDef_struct
{
	const char*				pszName;
	const char*				pszAlias;
	FlxCtuCommandTrait		traits;
	size_t					attributeDefCount;
	FlxCtuAttributeDef**	ppAttributeDefs;
} FlxCtuCommandDef;

/*
	Functions relating to command definitions.
*/
size_t flxCtuCommandDefsCreate(const FlxCtuCommandDef* ppCommandDefs[]);
void flxCtuCommandDefsDestroy(FlxCtuCommandDef pCommandDefs[], size_t count);

void flxCtuCommandDefAddAttribute(FlxCtuCommandDef*	pCommandDef, 
								  const char*		pszName, 
								  const char*		pszAlias,
								  ...);				/* Allowed value strings terminated by NULL. */

/*
	Provide usage information (based on the command definitions).
*/
struct FlxCtuContext_struct;
struct FlxCtuCommand_struct;

void flxCtuCommandHelp(
		   const struct FlxCtuContext_struct* pContext,		/* const FlxCtuContext* pContext */
		   const struct FlxCtuCommand_struct* pCommand);	/* const FlxCtuCommand* pCommand */

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
