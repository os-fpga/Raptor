/**************************************************************************************************
* Copyright (c) 1997-2016, 2018, 2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *	Module: $Id: //FNPBRANCH/FNP-11.19.1/flexnet/machind/ls_vendor.tpl#1 $
 *
 *	Function:  None
 *
 *	Description: Vendor customization data for server.
 *
 *	M. Christiano
 *	2/15/88
 *
 *	Last changed:  9/24/98
 *
 */

#include <stdio.h>
#include "lmclient.h"
#include "lm_code.h"


/* Vendor encryption routine */

char *(*ls_user_crypt)() = 0;

/* Vendor initialization routines */

void (*ls_user_init1)() = 0;
void (*ls_user_init2)() = 0;
void (*ls_user_init3)() = 0;

/* Client hostid callback */
void (*ls_client_hostid_callback)() = ls_client_hostid_callback_example;

/* Checkout, checkin filters, checkin callback */

int (*ls_outfilter)() = 0;
int (*ls_infilter)() = 0;
int (*ls_incallback)() = 0;

/* vendor message, challenge */

char *(*ls_vendor_msg)() = 0;
char *(*ls_vendor_challenge)() = 0;


VENDORCODE vendorkeys[] = {		/* Possible keys for vendor daemons */
		{ VENDORCODE_5, 
		ENCRYPTION_SEED1^VENDOR_KEY5, ENCRYPTION_SEED2^VENDOR_KEY5,
		  VENDOR_KEY1, VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4,
		  {0}, {0}, LM_STRENGTH,  0,
		  FLEXLM_VERSION, FLEXLM_REVISION, FLEXLM_PATCH, 
		  LM_BEHAVIOR_CURRENT, {CRO_KEY1, CRO_KEY2}},
		    	   };	/* End of vendor codes*/

int keysize = sizeof(vendorkeys)/sizeof(vendorkeys[0]);

int ls_crypt_case_sensitive = 0; /* Is license key case-sensitive? -
					Only used for vendor-supplied 
					encryption routines. */
/* 
 *      ls_user_lockfile:  We no longer recommend changing this
 *          here, since the text string for the lockfile will
 *          be visible in the binary, and could be therefore
 *          changed.  
 */

char *ls_user_lockfile = (char *)NULL;

#ifdef VMS
char *default_license_file = (char *) NULL;	
				/* Default location for license file */
#endif
#ifdef OS2
extern void main();
void (*drag_in_main_from_lib)()=main;
#endif


int ls_read_wait = 10;		/* How long to wait for solicited reads */
int ls_dump_send_data = 0;	/* Set to non-zero value for debug output */
int ls_normal_hostid = 1;	/* <> 0 -> normal hostid call, 0 -> extended */
int ls_conn_timeout = MASTER_WAIT;  /* How long to wait for a connection */
int ls_enforce_startdate = 1;	/* Enforce start date in features */
int ls_tell_startdate = 1;	/* Tell the user if it is earlier than start
								date */
int ls_minimum_user_timeout = 900; /* Minimum user inactivity timeout (seconds) 
					<= 0 -> activity timeout disabled */
int ls_min_lmremove = 120;	/* Minimum amount of time (seconds) that a
				   client must be connected to the daemon before
				   an "lmremove" command will work. */
int ls_use_featset = 0;		/* Use the FEATURESET line from the license
									file */
int ls_use_all_feature_lines = 0; /* Use ALL copies of feature lines that are
				     unique if non-zero (ADDITIVE licenses) */
int ls_do_checkroot = 0;	/* Perform check that we are running on the
				   real root of filesystem.  NOTE: this check
				   will fail on diskless systems, but they
				   shouldn't be used as server nodes, anyway */
int ls_show_vendor_def = 0;	/* If non-zero, the vendor daemon will send
				   the vendor-defined checkout data back in
				   lm_userlist() calls */
int ls_allow_borrow = 0;	/* Allow "Borrowing" of licenses by another
				   server */
				

/* Hostid redirection verification routine */

int (*ls_hostid_redirect_verify)() = 0;  
				/* Hostid Redirection verification */

/* Vendor-defined periodic call */

void (*ls_daemon_periodic)() = 0;  
				/* Vendor-defined periodic call in daemon */
void (*ls_child_exited)() = 0;  
				/* Vendor-defined callback -- called with
				   CLIENT_DATA * argument */

int ls_compare_vendor_on_increment = 0;	/* Compare vendor-defined fields to
					 declare matches for INCREMENT lines? */
int ls_compare_vendor_on_upgrade = 0;	/* Compare vendor-defined fields to
					   declare matches for UPGRADE lines? */
/*
 *	ls_a_behavior_ver can also be set in lm_code.h.
 *	lm_code.h takes priority.  If set there, the value here
 *	will not be used.
 */
char *ls_a_behavior_ver = 0;	/* like LM_A_BEHAVIOR_VER */

int ls_a_check_baddate = 0;		/* like LM_A_CHECK_BADDATE */
int ls_a_lkey_start_date = 0;		/* like LM_A_KEY_START_DATE */
int ls_a_lkey_long = 0;			/* like LM_A_KEY_LONG */
int ls_a_license_case_sensitive = 0;	/* like LM_A_LICENSE_CASE_SENSITIVE */
void (*ls_a_user_crypt_filter)() = 0;	
void (*ls_a_phase2_app)() = 0;

