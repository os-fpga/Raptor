/**************************************************************************************************
* Copyright (c) 2007-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
#ifndef FLXACT3SERVER_H 
#define FLXACT3SERVER_H

#include <stdlib.h>

#include "FlxActSvr.h"
#include "FlxActTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------------------------------------------------------------
 * These two functions, flxAct3SvrGetConfigString & flxAct3SvrConfigNode, will eventually appear in FlxActSvr.h.
 * --------------------------------------------------------------------------------------------------------------------------------*/
/*
 * flxAct3SvrGetConfigString
 *
 * Obtain a configuration string for a particular role and network address
 */

FlxActBool
flxAct3SvrGetConfigString( FlxAct3ServerRole role, const char* hostname, char* result, size_t* size, FlxActError* pError );


/*
 * flxAct3SvrConfigNode
 *
 * Configure node as part of 3-server group
 */

FlxActBool
flxAct3SvrConfigNode( const char* primary, const char* secondary, const char* tertiary, FlxActError* pError );

#ifdef __cplusplus
}
#endif

#endif
