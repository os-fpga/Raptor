/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *	Description: 	Example Vendor-defined hostid implementation for
 *                  FlexNet Licensing
 *
 *	Parameters:	(misc)
 *
 *	Return:		(misc)
 *
 *	M. Christiano
 *	11/14/2000 wh/tw
 *
 *	x_flexlm_newid -- call this function just after lc_init()
 *			  from your application, and also in your
 *		 	  license generator (e.g., lmcrypt.c), and
 *			  your vendor daemon.  To do this in the vendor
 *			  daemon, add the following to lsvendor.c:
 *
 *			  void x_flexlm_newid();
 *			  void (*ls_user_init1)() = x_flexlm_newid;
 *
 *
 *			  All programs that call this function MUST
 *			  have  a variable (LM_HANDLE *) lm_job that points
 *			  to the current job.  If your job handle is not
 *			  called lm_job, make one that points to it.
 */


#include "lmclient.h"
#include "lm_attr.h"
#include "string.h"

extern LM_HANDLE *lm_job; 	/* This must be the current job! */

/* This example returns only 1 hostid */
#define VENDEF_ID_TYPE HOSTID_VENDOR+1
#define VENDEF_ID_LABEL "VDH"
#define VENDEF_ID "12345678"


/*
 *      x_flexlm_gethostid() - Callback to get the vendor-defined hostid.
 *		(Sorry about all the windows types for this function...)
 */

HOSTID *
#ifdef PC
LM_CALLBACK_TYPE
#endif /* PC */
/*
 *	IMPORTANT NOTE:  This function MUST call l_new_hostid() for
 *			a hostid struct on each call.
 *			If more than one hostid of a type is
 *			found, then call l_new_hostid for each
 *			and make into a list using the `next' field.
 */

x_flexlm_gethostid(idtype)
short idtype;
{
	HOSTID *h = l_new_hostid();

	memset(h, 0, sizeof(HOSTID));
        if (idtype == VENDEF_ID_TYPE)
        {
                h->type = VENDEF_ID_TYPE;

                strncpy(h->id.vendor, VENDEF_ID, MAX_HOSTID_LEN);	/* LONGNAMES */
		h->id.vendor[MAX_HOSTID_LEN] = 0;
                return(h);
        }
        return((HOSTID *) NULL);
}



void
x_flexlm_newid(id)

HOSTID *id;

{
  LM_VENDOR_HOSTID h;

	 memset(&h, 0, sizeof (h));
	 h.label = VENDEF_ID_LABEL;
	 h.hostid_num = VENDEF_ID_TYPE;
	 h.case_sensitive = 0;
	 h.get_vendor_id = x_flexlm_gethostid;
	 if (lc_set_attr(lm_job, LM_A_VENDOR_ID_DECLARE, (LM_A_VAL_TYPE) &h))
		 lc_perror(lm_job, "LM_A_VENDOR_ID_DECLARE FAILED");

}
