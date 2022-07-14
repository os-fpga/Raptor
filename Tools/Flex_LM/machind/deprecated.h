/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/**	@file		Deprecated function prototypes.
 *	@brief		A brief description here
 *	@version	$Revision: #1 $
 *
 *	Function prototypes for deprecated functions.
 ****************************************************************************/

#ifndef INCLUDE_DEPRECATED_H
#define INCLUDE_DEPRECATED_H

#include "lmclient.h"

lm_extern char * API_ENTRY lc_ascii_date(LM_HANDLE * job, char * pszBdate);
lm_extern CONFIG * API_ENTRY lc_lookup(LM_HANDLE * job, char * feature);
lm_extern char * API_ENTRY lc_extract_date(LM_HANDLE * job, char * pszCode);


#endif /* INCLUDE_DEPRECATED_H */
