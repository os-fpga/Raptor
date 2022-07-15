/**************************************************************************************************
* Copyright (c) 2014-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *	Vendor daemon checkout filter example header file
 */

#ifndef FLXLSCO_H_INCLUDED
#define FLXLSCO_H_INCLUDED

/*
 * Checkout filter callback example: 
 *		int (*ls_outfilter)() = ls_co_filter_example; 
 */
int ls_co_filter_example();

/* 
 * Extracting client HostID from vendor daemon callbacks:
 * 
 * An example to demonstrate, how to get MAC address (aka ETHER) and vendor defined hostids 
 * of licensing client. 
 *
 * In order to get hostID values of licensing client, its mandatory to enable 
 * ls_client_hostid_callback and pass required hostid types via ls_attr_init_client_hostids API. 
 *
 * Client hostid callback example:
 *       void (*ls_client_hostid_callback)() = ls_client_hostid_callback_example;
 * 
 * MAC addresses (aka ETHER) hostids are retrieved by ls_get_attr() with LS_ATTR_CLIENT_HOSTID_ETHER attribute.
 *
 * Vendor defined hostids are retrieved by ls_get_attr() with LS_ATTR_CLIENT_HOSTID_VENDOR attribute.
 */
void ls_client_hostid_callback_example();

#endif /* FLXLSCO_H_INCLUDED */

