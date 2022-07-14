/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Description: 	header file required by lm_code.h
 *			Used for TRL (Tamper Resistant Licenses)
 */
#ifndef LM_TRL_H
#define LM_TRL_H

#define LM_STRENGTH_LICENSE_KEY	0
#define LM_STRENGTH_DEFAULT	1
#define LM_STRENGTH_113BIT	2
#define LM_STRENGTH_163BIT	3
#define LM_STRENGTH_239BIT	4
#define LM_STRENGTH_PUBKEY	LM_STRENGTH_113BIT
#define LM_STRENGTH_VERYHIGH	LM_STRENGTH_239BIT
				/* >= PUBKEY uses Public-Key */

/*
 *	Values for LM_SIGN_LEVEL
 */
#define LM_SIGN2 	2  /* SIGN2= */
#define LM_SIGN	 	1  /* SIGN= the default */
#define LM_NO_SIGN	 0  /* license key */
#endif /* LM_TRL_H */
