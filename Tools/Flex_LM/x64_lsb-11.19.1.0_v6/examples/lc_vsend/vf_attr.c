/**************************************************************************************************
* Copyright (c) 2021 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
 * Vendor Function example code-snippet: Server Attributes Accessor
 * ================================================================
 *
 * The purpose of this function is to demonstrate the operation of a Vendor Function
 *
 * Specifically the function below returns attrbiutes from the ls_get_attr() call.
 * The result is returned as a string.
 *
 * To use this function it must be inserted into ls_vendor.c immediately after the "* vendor message *"
 * comment. The variable 'ls_vendor_msg' should be assigned the value s_vf_func instead of 0:-
 *
 * char *(*ls_vendor_msg)() = s_vf_func;
 *
 * On platforms that support it, a worker thread can be enabled to process Vendor Functions asynchronously
 * and hence not disrupt normal licensing-related operations whilst they are running.  To enable the worker
 * thread, change the initialization of ls_vendor_msg_async from 0 to 1.
 *
 * int ls_vendor_msg_async = 1;
 *
 * There is one final variable associated the the asynchronous processing of Vendor Functions: ls_vendor_msg_list_size
 * This variable limits the number of queued asynchronous Vendor Function request to the chosen value.
 * In most use-cases it is likely that the default value in the lsvendor.c should not need changing.
 *
 */

#ifdef OS_LINUX
#include <sys/socket.h>
#include <netdb.h>
#endif

#include "ls_attr.h"
#include "lsserver.h"

struct ls_attr_xlat_entry
{
	const char* name;
	int			val;
};
	
static char* s_vf_func(char* buffer)
{
	static const struct ls_attr_xlat_entry xlat[] = {

		{ "LS_ATTR_FEATURE",					LS_ATTR_FEATURE }, /* (char *) */
		{ "LS_ATTR_NLIC",						LS_ATTR_NLIC }, /* (int)    */
		{ "LS_ATTR_FLAG",						LS_ATTR_FLAG }, /* (int)    */
		{ "LS_ATTR_VERSION",					LS_ATTR_VERSION }, /* (char *) */
		{ "LS_ATTR_CLIENT",						LS_ATTR_CLIENT }, /* (CLIENT_DATA *) */
		{ "LS_ATTR_DUP_SEL",					LS_ATTR_DUP_SEL }, /* (int)    */
		{ "LS_ATTR_LINGER",						LS_ATTR_LINGER }, /* (int)    */
		{ "LS_ATTR_CODE",						LS_ATTR_CODE }, /* (char *) */
		{ "LS_ATTR_SERVER",						LS_ATTR_SERVER }, /* (int)    */
		{ "LS_ATTR_VENDOR_NAME",				LS_ATTR_VENDOR_NAME }, /* (char *) */
		{ "LS_ATTR_LAST_REPORT_LOG_ROTATION",	LS_ATTR_LAST_REPORT_LOG_ROTATION }, /* (time_t) */
		{ "LS_ATTR_REPORT_LOG_FILENAME",		LS_ATTR_REPORT_LOG_FILENAME }, /* (const char *) */
		{ "LS_ATTR_CHECKOUT_DATA",				LS_ATTR_CHECKOUT_DATA }, /* (char *) */
		{ "LS_ATTR_NLIC_OD",					LS_ATTR_NLIC_OD }, /* (int)    */
		{ "LS_ATTR_TOTAL_NLIC_OD_INUSE",		LS_ATTR_TOTAL_NLIC_OD_INUSE }, /* (int)    */
		{ "LS_ATTR_OD_REQUESTS",				LS_ATTR_OD_REQUESTS }, /* (int)    */
		{ "LS_ATTR_CLIENT_TIMEZONE",			LS_ATTR_CLIENT_TIMEZONE }, /* (int)    */
		{ "LS_ATTR_CLIENT_TIME",				LS_ATTR_CLIENT_TIME }, /* (time_t) */
		{ "LS_ATTR_CLIENT_HOSTID_ETHER",		LS_ATTR_CLIENT_HOSTID_ETHER }, /* (char *) - release memory after its use */
		{ "LS_ATTR_CLIENT_HOSTID_VENDOR",		LS_ATTR_CLIENT_HOSTID_VENDOR }  /* (char *) - release memory after its use */
	};

	/* A useful tip to avoid the need for heap management is to use the input buffer
     * to store the reply.  
     * Note: The buffer size for message (incoming and outgoing) is limited to LM_VSEND_MAX_LEN bytes (defined in lmclient.h)
     */

	/* Find the request attribute */
	const int count = sizeof(xlat)/sizeof(struct ls_attr_xlat_entry);
	int i, attrNum = -1;
	const char* attrNam;
	int res = LM_NOSUCHATTR;

	for(i=0; i<count; i++)
	{
		if( !strcmp(buffer, xlat[i].name) )
		{
			/* Found it */
			attrNum=xlat[i].val;
			attrNam=xlat[i].name;
			break;
		}
	}

	switch(attrNum)
	{
		/* int attributes */
		case LS_ATTR_NLIC:                        
		case LS_ATTR_FLAG:                        
		case LS_ATTR_DUP_SEL:                     
		case LS_ATTR_LINGER:                      
		case LS_ATTR_SERVER:                      
		case LS_ATTR_NLIC_OD:                     
		case LS_ATTR_TOTAL_NLIC_OD_INUSE:         
		case LS_ATTR_OD_REQUESTS:                 
		case LS_ATTR_CLIENT_TIMEZONE:             
		{
			intptr_t attrVal;
			res = ls_get_attr(attrNum, (char**)&attrVal);

			if( res == LM_NOERROR )
			{
				snprintf(buffer,LM_VSEND_MAX_LEN,"%s=%d",attrNam,(int)attrVal);
			}
		}
		break;

		/* time_t attributes */
		case LS_ATTR_LAST_REPORT_LOG_ROTATION:
		case LS_ATTR_CLIENT_TIME:
		{
			intptr_t attrVal;
			res = ls_get_attr(attrNum, (char**)&attrVal);

			if( res == LM_NOERROR )
			{
				snprintf(buffer,LM_VSEND_MAX_LEN,"%s=%ld",attrNam,(long)attrVal);
			}
		}
		break;

		/* Non-allocated strings */
		case LS_ATTR_CHECKOUT_DATA:
		case LS_ATTR_REPORT_LOG_FILENAME:
		case LS_ATTR_VENDOR_NAME:
		case LS_ATTR_CODE:
		case LS_ATTR_VERSION:
		case LS_ATTR_FEATURE:
		{
			char* attrVal;
			res = ls_get_attr(attrNum, (char**)&attrVal);

			if( res == LM_NOERROR )
			{
				if( attrVal == NULL )
					attrVal = "<null>";
				snprintf(buffer,LM_VSEND_MAX_LEN,"%s=%s",attrNam,attrVal);
			}
		}
		break;

		/* Allocated strings */
		case LS_ATTR_CLIENT_HOSTID_ETHER:
		case LS_ATTR_CLIENT_HOSTID_VENDOR:
		{
			char* attrVal;
			res = ls_get_attr(attrNum, (char**)&attrVal);

			if( res == LM_NOERROR )
			{
				if( attrVal == NULL )
					attrVal = "<null>";
				snprintf(buffer,LM_VSEND_MAX_LEN,"%s=%s",attrNam,attrVal);
				free(attrVal);
			}
		}
		break;

		/* CLIENT_DATA structure */
		case LS_ATTR_CLIENT:
		do 
		{
			CLIENT_DATA* attrVal;

			res = ls_get_attr(attrNum, (char**)&attrVal);

			if( res != LM_NOERROR )
				break;

#ifdef OS_LINUX
			{
				int err;
				char host[256] = { '\0' };
				struct sockaddr_in sa;
				socklen_t sa_len = sizeof(sa);

				if( attrVal->addr.addr.fd < 0 )
				{
					res = LM_NOSUCHATTR;
					break;
				}

				if( (err=getsockname(attrVal->addr.addr.fd,(struct sockaddr *)&sa,&sa_len)) != 0 )
				{
					res = LM_NOSUCHATTR;
					break;
				}

				if( (err=getnameinfo((const struct sockaddr *)&sa, sa_len, host, sizeof(host), NULL, 0, 0)) != 0 )
				{
					res = LM_NOSUCHATTR;
					break;
				}

				snprintf(buffer,LM_VSEND_MAX_LEN,"%s=addr(%s:%d)",attrNam,host,(int)htons(sa.sin_port));
			}
#else
			snprintf(buffer,LM_VSEND_MAX_LEN,"%s=%p",attrNam,(void*)attrVal);
#endif
		

		} while(0);
		break;

		default:
			break;

	}

	if( res != LM_NOERROR )
	{
		snprintf(buffer,LM_VSEND_MAX_LEN,"Error:%d",res);
	}

	return buffer;
}
