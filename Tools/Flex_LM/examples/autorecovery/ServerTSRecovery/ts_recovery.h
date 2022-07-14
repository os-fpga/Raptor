/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description:  This is a sample application program, to illustrate
 *			      how to achieve automatic recovery of a broken ServerTS using V2 transaction.
 *
 *  Points to note:
 *               - To achive automatic serverTS recovery, this header file needs to be included into the lsvendor.c and
 *                 the function pointer "void (*ls_ts_recovery)()" declared in lsvendor.c needs to be assigned with the
 *                 address of tsRecovery().
 *
 *                 Example:
 *						Take a reference of updated lsvender.c file shipped along with this file.
 *
 *               - Make sure the makefile.act is updated to build ServerTSRecovery.c file.
 *
 *                 Example: 
 *                      Take a reference of updated makefile.act file shipped along with this file.
 *					
*/
 
#ifndef TS_RECOVERY_H 
#define TS_RECOVERY_H

void tsRecovery(void);

#endif /* TS_RECOVERY_H */

  	


