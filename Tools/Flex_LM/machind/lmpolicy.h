/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Description: 	lmpolicy.h -- for Simple and Trivial API
 *
 */
#ifndef _LM_POLICY_H
#define _LM_POLICY_H
#include "lmclient.h"

#if defined (WINNT) && defined(FLEXLM_DLL)
#ifndef FLEXLM_KITBUILD
#include "lmseeds.h"
#endif
#include "lm_code.h"
#endif

typedef struct _lp_handle {
	char feature[MAX_FEATURE_LEN + 1];
	char lickey[MAX_CRYPT_LEN + 1];
#ifdef LM_INTERNAL
	long last_failed_reconnect;
	long *recent_reconnects;
	int num_minutes;
	LM_HANDLE *job;
	int policy;
#endif /* LM_INTERNAL */
} LP_HANDLE, FAR * LP_HANDLE_PTR;

/*
 *		Main policies first byte -- mask: 0xff
 *		one of the following:
 */
#define LM_RESTRICTIVE		0x1
#define LM_QUEUE		0x2
#define LM_FAILSAFE		0x3
#define LM_LENIENT		0x4

/*
 *		Modifiers -- next 3 bytes
 */

#define LM_MANUAL_HEARTBEAT 		0x100
#define LM_RETRY_RESTRICTIVE 		0x200
#define LM_ALLOW_FLEXLMD		0x400
#define LM_CHECK_BADDATE		0x800
#define LM_USE_LICENSE_KEY 	       0x2000

typedef struct _lpcode_handle {
	VENDORCODE * code;
	char *		vendor_name;
} LPCODE_HANDLE;

#ifndef LMPOLICY_C
#if defined (WINNT) && defined(FLEXLM_DLL)
LM_CODE(lp_code, ENCRYPTION_SEED1, ENCRYPTION_SEED2, VENDOR_KEY1,
	VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4, VENDOR_KEY5);
#else
 static VENDORCODE lp_code;
#endif

static LP_HANDLE_PTR lp;
static LPCODE_HANDLE _lpcode  = { &lp_code,
#if defined (WINNT) && defined(FLEXLM_DLL)
	VENDOR_NAME
#else
	0
#endif
	};
#define LPCODE &_lpcode
#endif /* LMPOLICY_C */

/*
 *	prototypes
 */
lm_extern int API_ENTRY lp_checkout 	lm_args(( LPCODE_HANDLE *, int, char *,
					char *, int, char *, LP_HANDLE **));
lm_extern void API_ENTRY lp_checkin	lm_args(( LP_HANDLE *));
lm_extern int API_ENTRY lp_heartbeat	lm_args(( LP_HANDLE *, int *, int));
lm_extern char * API_ENTRY lp_errstring	lm_args(( LP_HANDLE *));
lm_extern char * API_ENTRY lp_warning	lm_args(( LP_HANDLE *));
lm_extern void API_ENTRY lp_perror	lm_args(( LP_HANDLE *, char *));
lm_extern void API_ENTRY lp_pwarn	lm_args(( LP_HANDLE *, char *));


#define CHECKOUT(policy, feature, version, path) \
	lp_checkout(LPCODE, policy, feature, version, \
						1, path, &lp)
#define CHECKIN() lp_checkin(lp)
#define ERRSTRING() lp_errstring(lp)
#define WARNING() lp_warning(lp)
#define PERROR(str) lp_perror(lp, str)
#define PWARN(str) lp_pwarn(lp, str)
#define HEARTBEAT() lp_heartbeat(lp, 0, 0)

#if defined (WINNT)
void   WINAPI lt_checkin();
int    WINAPI lt_checkout(int, char*, char*, char*);
int    WINAPI lt_heartbeat();
char*  WINAPI lt_errstring();
void   WINAPI lt_perror(char*);
void   WINAPI lt_pwarn(char*);
char*  WINAPI lt_warning();
#endif

#endif /* _LM_POLICY_H */
