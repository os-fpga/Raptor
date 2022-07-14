/**************************************************************************************************/
/*!@file
 * @brief A brief description goes here
 *
 * Detailed description goes here
 *
 * @version \$Revision: #1 $
 */
/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
#if !defined( FNP_STDTYPES_H )
#define FNP_STDTYPES_H

#include "fnpplatformtypes.h"

#if FNPPUB_HAVE_STDINT_H
#include <stdint.h>
#elif FNPPUB_HAVE_INTTYPES_H
#include <inttypes.h>
#elif FNPPUB_HAVE_WINSTDINT_H
#include "windows/winstdint.h"
#endif

#endif /* FNP_STDTYPES_H */

