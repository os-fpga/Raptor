/**************************************************************************************************
* Copyright (c) 2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
	This header file is part of a sample utility that demonstrates Composite Transactions and
	other functionality.  

	If declares the functions used for short code transactions.
*/
#ifndef COMP_TRAN_UTIL_SHORTCODE_H
#define COMP_TRAN_UTIL_SHORTCODE_H

#include "compTranUtilContext.h"

#ifdef __cplusplus
extern "C" {
#endif

void		flxCtuHelpShortCode(const FlxCtuContext* pContext);
void		flxCtuHelpShortCodeResponse(const FlxCtuContext* pContext);
FlxActBool	flxCtuDoCommandShortCode(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);
FlxActBool	flxCtuDoCommandShortCodeResponse(FlxCtuContext* pContext, const FlxCtuCommand* pCommand);

#ifdef __cplusplus
}
#endif

#endif	/* Include guard */
