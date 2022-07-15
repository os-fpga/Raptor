/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
 */

#ifndef LM_CODE_H
#define LM_CODE_H
#include "lm_trl.h"
/*
 * 	Vendor keys:   		Enter keys received from Flexera.
 *				Changing keys has NO impact on license files
 *				(unlike LM_SEEDs).
 */
#define VENDOR_KEY1 0x0
#define VENDOR_KEY2 0x0
#define VENDOR_KEY3 0x0
#define VENDOR_KEY4 0x0
#define VENDOR_KEY5 0x0
/*
 * 	Vendor name.  		Leave "demo" if evaluating.  Otherwise,
 *			 	set to your vendor daemon name.
 */
#define VENDOR_NAME "demo"
/*
 * 	Private SEEDs: 		Make up 3, 8-hex-char numbers, replace and
 *				guard securely.  You can also use the command
 *				'lmrand1 -seed' to make these numbers up.
 */
#define LM_SEED1 0x12345678
#define LM_SEED2 0x87654321
#define LM_SEED3 0xabcdef01
/*
 *	Pick an LM_STRENGTH:
 */
#define LM_STRENGTH LM_STRENGTH_113BIT
/*
 *			       	LM_STRENGTH Options are
 *			       	LM_STRENGTH_DEFAULT:
 *			      	 Public key protection unused. Use SIGN=
 *			      	 attribute. sign length = 12
 *			       	TRL:
 *			       	LM_STRENGTH_113BIT, LOW:   sign length= 60 chars
 *			       	LM_STRENGTH_163BIT, MEDIUM:sign length= 84 chars
 *			       	LM_STRENGTH_239BIT, HIGH:  sign length=120 chars
 *				Pre-v7.1:
 *			       	LM_STRENGTH_LICENSE_KEY:   Use old license-key.
 *			       	If you're upgrading from
 *			       	pre-v7.1, and want no changes, set this to
 *			       	LM_STRENGTH_LICENSE_KEY.
*/
/*
 *	TRL Keys:		Provided by Flexera, if TRL is used.
 * 				Be sure to set LM_STRENGTH to
 * 				LM_STRENGTH_DEFAULT, above, if TRL_KEYs are zero.
 */
#define TRL_KEY1    0x0
#define TRL_KEY2    0x0


#include "lm_code2.h"
#endif /* LM_CODE_H */
