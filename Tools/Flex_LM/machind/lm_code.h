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
 

#define VENDOR_KEY1 0x7db9688a
#define VENDOR_KEY2 0xc0da9916
#define VENDOR_KEY3 0xa1309a57
#define VENDOR_KEY4 0xc7ab1963
#define VENDOR_KEY5 0x7961299c
/*
 * 	Vendor name.  		Leave "demo" if evaluating.  Otherwise,
 *			 	set to your vendor daemon name.
 */
#define VENDOR_NAME "rapidsil"
/*
 * 	Private SEEDs: 		Make up 3, 8-hex-char numbers, replace and
 *				guard securely.  You can also use the command
 *				'lmrand1 -seed' to make these numbers up.
 */
#define LM_SEED1 0x9364d378
#define LM_SEED2 0x0b7bea73
#define LM_SEED3 0x1bf89622
/*
 *	Pick an LM_STRENGTH:
 */
#define LM_STRENGTH LM_STRENGTH_239BIT
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
#define TRL_KEY1    0xbe7876ac
#define TRL_KEY2    0x6cd713b5


#include "lm_code2.h"
#endif /* LM_CODE_H */
