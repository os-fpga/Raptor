/**************************************************************************************************
* Copyright (c) 2014-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *	Vendor daemon overdraft callback example
 */

#ifndef FLXLSCOOD_H_INCLUDED
#define FLXLSCOOD_H_INCLUDED

/* 
 * Vendor daemon init callback:
 * e.g. void (*ls_user_init1)() = ls_user_init_callback_example;
 */
void ls_user_init_callback_example();

/*
 * Overdraft callback (checkout): 
 * void (*ls_outod_callback)() = ls_outod_callback_example; 
 */
void ls_outod_callback_example();

/*
 * Overdraft callback (checkin): 
 * void (*ls_inod_callback)() = ls_inod_callback_example; 
 */
void ls_inod_callback_example();

/*
 * Vendor daemon shutdown callback:
 * e.g. void (*ls_vd_shutdown)() = ls_vd_shutdown_callback_example;	
 */
void ls_vd_shutdown_callback_example();

#endif /* FLXLSCOOD_H_INCLUDED */
