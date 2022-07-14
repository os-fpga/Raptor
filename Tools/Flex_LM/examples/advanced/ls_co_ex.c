/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Function: 	ls_co_filter_example
 *
 *	Description:
 * 			example of how to use ls_checkout() and
 *			ls_get_attr() in a vendor daemon checkout filter.
 *			The tricky part of this is that ls_get_attr()
 *			often returns a type unacceptable to
 *			ls_checkout, and the value
 *			has to be converted, as shown in the example.
 *
 * Return:
 * 	  0 - This function has processed the checkout request.
 *	      If the checkout fails then the errno should be set in the
 *	      global lm_job->lm_errno
 *	  1 - The request was not processed so normal server checkout
 *	      should occur.
 *
 */
#include "lmclient.h"
#include <stdio.h>
#include "ls_attr.h"
#include "lsserver.h"
#include "lm_code.h"
#include "lm_redir_std.h"
#include "ls_co_ex.h"

static void s_get_client_hostids();

/**********************************************************************
 * NOTE:
 * 	the function prototype for ls_checkout() that is
 *	used in this example is not in the include file so it
 *	is included here.
 *********************************************************************/

/**********************************************************************
 * Checkout a feature
 *
 * Return:
 * 	  0 - checkout not available
 *	  >0 - checkout done
 *	  <0 - checkout queued
 *
 *********************************************************************/
int ls_checkout(char *	feature,	/* The feature desired */
		char *	ndesired,	/* Number of licenses desired */
		char *	waitflag,	/* "Wait until available".
		     			 * if *wait == '1' the request will be
					 * queued if the license is not available */
		CLIENT_DATA *who,	/* Who is asking for it */
		char *	version,	/* The feature's version number */
		char *	dup_sel,	/* Duplicate selection criteria */
		char *	linger,		/* How long license lingers after checkin */
		char *	code,		/* Encryption code from license file line */
		int	always_zero, 	/* historical. Set to zero */
		int	always_one);	/* historical. Set to one */

/**********************************************************************
 * Example of a vendor daemon checkout filter using ls_checkout()
 * and ls_get_attr()
 *
 * To use this example set int (*ls_outfilter)() = ls_co_filter_example
 * in lsvendor.c
 *
 * Return:
 * 	  0 - This function has processed the checkout request.
 *	      If the checkout fails then the errno should be set in the
 *	      global lm_job->lm_errno
 *	  1 - The request was not processed so normal server checkout
 *	      should occur.
 *********************************************************************/
int ls_co_filter_example()
{
	char *str_feature;
	char str_num_lic_requested[MAX_LONG_LEN+1];
	char str_wait_flag[MAX_LONG_LEN+1];
	char *str_version;
	char str_dup_sel[MAX_LONG_LEN+1];
	char str_linger[MAX_LONG_LEN+1];
	char *str_code;
	CLIENT_DATA *client_data;
	int num_lic_requested;
	int wait_flag;
	int dup_sel;
	int linger;
	int did_checkout;
	int timezone;
    time_t  ttime;

	/* which feature does the client want to checkout.
	 * See ls_attr.h for valid attribute values. */
	ls_get_attr(LS_ATTR_FEATURE, &str_feature);

	/* get the number of licenses requested */
	ls_get_attr(LS_ATTR_NLIC, &num_lic_requested);
	sprintf(str_num_lic_requested, "%d", num_lic_requested);  /* convert to string for ls_checkout() */

	/* get the "wait until available" flag */
	ls_get_attr(LS_ATTR_FLAG, &wait_flag);
	sprintf(str_wait_flag, "%d", wait_flag); /* convert to string for ls_checkout() */

	/* get the version of the requested feature */
	ls_get_attr(LS_ATTR_VERSION, &str_version);

	/* all the info about the client.
	 * There is a lot of stuff that can be filtered on in here. Be careful.
	 * See lsserver.h  */
	ls_get_attr(LS_ATTR_CLIENT, &client_data);
	ls_get_attr(LS_ATTR_CLIENT_TIMEZONE, &timezone);
    ls_get_attr(LS_ATTR_CLIENT_TIME, &ttime);

    if((timezone / 60) < 0)
    {
        fprintf(lm_flex_stdout(), "Client timezone:%s%s%02d%s%02d\n", "UTC", "-", timezone / (-60), ":",(-1)*(timezone % 60));
    }
    else
    {
        fprintf(lm_flex_stdout(), "Client timezone:%s%s%02d%s%02d\n", "UTC", "+", timezone / 60, ":", timezone % 60);
    }
    fprintf(lm_flex_stdout(), "Client time:%02lld%s%02lld\n", (long long int)(ttime / 60), ":", (long long int)(ttime % 60));

	/* duplicate selection criteria */
	ls_get_attr(LS_ATTR_DUP_SEL, &dup_sel);
	sprintf(str_dup_sel, "%d", dup_sel);  /* convert to string for ls_checkout() */

	/* how long will the license linger after checkin */
	ls_get_attr(LS_ATTR_LINGER, &linger);
	sprintf(str_linger, "%d", linger); /* convert to string for ls_checkout() */

	/* encryption code from license file */
	ls_get_attr(LS_ATTR_CODE, &str_code);

	/* get client hostids of hostid types given in client hostid callback */
	s_get_client_hostids();

	/* do the checkout */
	did_checkout = ls_checkout(str_feature,
					str_num_lic_requested,
					str_wait_flag,
					client_data,
					str_version,
					str_dup_sel,
					str_linger,
					str_code,
					0,
					1);
	if ( did_checkout ==0 )
	{
	     	/* the checkout failed.
		 * may want to do something here   */
		;
	}

	return 0; /* this means we've done the ls_checkout call */
}


/******************************************************************************
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
 *
 ******************************************************************************/
void ls_client_hostid_callback_example()
{
	int cl_hostid_list[] = {HOSTID_ETHER, HOSTID_VENDOR+1};
	int cl_hostid_cnt = 2;

	ls_attr_init_client_hostids(cl_hostid_list, cl_hostid_cnt);
	return;
}

/******************************************************************************
 * An example to retrieve client ether and vendor defined hostid via ls_get_attr()
 * 
 * ETHER hostids are retrieved by ls_get_attr() with LS_ATTR_CLIENT_HOSTID_ETHER attribute.
 * Vendor defined hostids are retrieved by ls_get_attr() with LS_ATTR_CLIENT_HOSTID_VENDOR attribute.
 *
 * In order to get hostid values of licensing client, its mandatory to enable 
 * ls_client_hostid_callback and pass required hostid types via ls_attr_init_client_hostids API. 
 * Refer, ls_client_hostid_callback_example.
 * 
 ******************************************************************************/
static void s_get_client_hostids()
{
	CLIENT_DATA *client_data = NULL;
	char *str_ether_hostid = NULL, *str_vdh = NULL;
	
	ls_get_attr(LS_ATTR_CLIENT, &client_data);

	if (!client_data)
	{
		fprintf(lm_flex_stdout(), "Client data not available!\n");
		return;
	}

	/* Get client ether hostids */
	ls_get_attr(LS_ATTR_CLIENT_HOSTID_ETHER, (char **)&str_ether_hostid);

	if (!str_ether_hostid)
	{
		fprintf(lm_flex_stdout(), "Client (Host:%s, User:%s):: ether hostid not available.\n", client_data->node, client_data->name);
	}
	else
	{
		fprintf(lm_flex_stdout(), "Client (Host:%s, User:%s):: ether hostids = %s \n", client_data->node, client_data->name, str_ether_hostid);
	}

	/* Get client vendor hostids */
	ls_get_attr(LS_ATTR_CLIENT_HOSTID_VENDOR, (char **)&str_vdh);

	if (!str_vdh)
	{
		fprintf(lm_flex_stdout(), "Client (Host:%s, User:%s):: vendor hostid not available.\n", client_data->node, client_data->name);
	}
	else
	{
		fprintf(lm_flex_stdout(), "Client (Host:%s, User:%s):: vendor hostids = %s \n", client_data->node, client_data->name, str_vdh);
	}

	/* Release memory after its use - ls_get_attr(LS_ATTR_CLIENT_HOSTID_ETHER) */
	if (str_ether_hostid)
	{
		free (str_ether_hostid);
		str_ether_hostid = NULL;
	}

	/* Release memory after its use - ls_get_attr(LS_ATTR_CLIENT_HOSTID_VENDOR) */
	if (str_vdh)
	{
		free (str_vdh);
		str_vdh = NULL;
	}

	return;
}
