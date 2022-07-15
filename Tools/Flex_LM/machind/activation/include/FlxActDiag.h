/**************************************************************************************************
* Copyright (c) 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
#if !defined( FLXACTDIAG_H_INCLUDED )
#define FLXACTDIAG_H_INCLUDED 

#include "FlxActCommon.h"

/*
 *	Declarations for diagnostic functions.
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *	flxActDiagTrustedStorageHealthCheck
 *
 * Runs a series of checks (defined in FlxActDiagTypes.h) on trusted storage and its dependent
 * functionality.
 * The function will return FLX_ACT_TRUE if all checks succeed, otherwise FLX_ACT_FALSE.  
 * If pResultArray is non-NULL, the results of check items 0 to (pResultArrayLength - 1) 
 * are returned in the array - LM_TSSE_NO_ERROR on success, otherwise a system error code.
 * To obtain all results, result array should have length flxTsHcCount.  
 *************************************************************************************************/
FlxActBool flxActDiagTrustedStorageHealthCheck(uint32_t* pResultArray, uint32_t resultArrayLength);
#ifdef __cplusplus
}
#endif


#endif /* FLXACTDIAG_H_INCLUDED */
