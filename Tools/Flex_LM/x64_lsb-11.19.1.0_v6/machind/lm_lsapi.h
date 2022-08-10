/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Description: 	LSAPI v1.1 prototypes, definitions and constants.
 *
 */

#ifndef _LM_CLIENT_H
#include "lmclient.h"
#endif

/*
 *	LSAPI data types
 */

#if !defined(PC) && !defined(VXWORKS)
typedef unsigned long UINT32;
#endif
typedef unsigned long LS_ULONG;
typedef unsigned long LS_HANDLE;
typedef unsigned long LS_STATUS_CODE;
typedef char LS_STR;
typedef char FAR *LS_STR_PTR;
typedef long LS_LONG;
typedef void FAR *LS_VOID_PTR;
typedef LS_ULONG FAR *LS_ULONG_PTR;
typedef LS_HANDLE FAR *LS_HANDLE_PTR;

#ifndef LM_MSG_LEN
#define LM_MSG_LEN 147
#endif

#define LS_VOID void

#define LS_CHALLENGE_RESPONSE_LEN 16	/* By definition */
#define LS_MSG_DIGEST_SIZE 16	/* 16-byte message digest (md4) */

#define LS_CHALLENGEMSG_SECRET MSG_DATA
#define LS_CHALLENGEMSG_RANDOM (LS_CHALLENGEMSG_SECRET+MAX_LONG_LEN+1)
#define LS_CHALLENGEMSG_MSGDIGEST (LS_CHALLENGEMSG_RANDOM+MAX_LONG_LEN+1)
#define LS_MAX_CHALLENGE (LS_CHALLENGEMSG_MSGDIGEST+LS_MSG_DIGEST_SIZE)
#if (LS_MAX_CHALLENGE > LM_MSG_LEN)
Error - Challenge does not fit into a FLEXlm message.
#endif

typedef struct ls_msg_digest {
			      LS_STR MessageDigest[LS_MSG_DIGEST_SIZE];
			     } LS_MSG_DIGEST;

typedef struct ls_challdata {
			      LS_ULONG SecretIndex;
			      LS_ULONG Random;
			      LS_MSG_DIGEST MsgDigest;
			    } LS_CHALLDATA;

typedef struct ls_challdata_flexlm {
			      char VendorName[MAX_VENDOR_NAME];
			      VENDORCODE VendorCode;
			    } LS_CHALLDATA_FLEXLM;

typedef struct ls_challenge {
			      LS_ULONG Protocol;
			      LS_ULONG Size;
			      LS_CHALLDATA ChallengeData;
			    } LS_CHALLENGE, FAR *LS_CHALLENGE_PTR;

typedef struct ls_challenge_flexlm {
			      LS_ULONG Protocol;
			      LS_ULONG Size;
			      LS_CHALLDATA_FLEXLM ChallengeData;
			    } LS_CHALLENGE_FLEXLM;

#define LS_DEFAULT_UNITS (LS_LONG) 0xffffffff
#define LS_ANY		 (LS_STR_PTR *) 0
#define LS_USE_LAST	 (LS_ULONG) 0x0000ffff
#define LS_BASIC_PROTOCOL (LS_ULONG) 0x00000001
#define LS_FLEXLM_PROTOCOL (LS_ULONG)((LS_ULONG)LS_USE_LAST+(LS_ULONG)0x1234)
#define LS_OUT_OF_BAND_PROTOCOL (LS_ULONG) 0xffffffff
#define LS_NULL		 (LS_VOID FAR *) 0

/*
 *	arguments (ulInformation) for LSQueryLicense() call
 */

#define LS_INFO_NONE	(unsigned long) 0	/* Reserved */
#define LS_INFO_SYSTEM	(unsigned long) 1	/* Return the unique id of */
						/* the license system (char *)*/
#define LS_INFO_DATA	(unsigned long) 2	/* Return block of misc */
						/* application data -
						   ULONG (# bytes) + # bytes  */
#define LS_UPDATE_PERIOD (unsigned long) 3	/* Returns the recommended */
						/*  update interval  (ULONG) */
#define LS_LICENSE_CONTEXT (unsigned long) 4	/* Returns license context */
						/* ULONG (# bytes) + # bytes  */

/*
 *	Status codes
 */

#define LS_SUCCESS		0
#define LS_BAD_HANDLE		0xc0001001
#define LS_INSUFFICIENT_UNITS	0xc0001002
#define LS_SYSTEM_UNAVAILABLE	0xc0001003
#define LS_LICENSE_TERMINATED	0xc0001004
#define LS_AUTHORIZATION_UNAVAILABLE	0xc0001005
#define LS_LICENSE_UNAVAILABLE	0xc0001006
#define LS_RESOURCES_UNAVAILABLE 0xc0001007
#define LS_NETWORK_UNAVAILABLE	0xc0001008
#define LS_TEXT_UNAVAILABLE	0xc0001009
#define LS_UNKNOWN_STATUS	0xc000100a
#define LS_BAD_INDEX		0xc000100b
#define LS_LICENSE_EXPIRED	0xc000100c
#define LS_BUFFER_TOO_SMALL	0xc000100d
#define LS_BAD_ARG		0xc000100e
#define LS_FIRST_ERROR		LS_BAD_HANDLE
#define LS_LAST_ERROR		LS_BAD_ARG
#define LS_OTHER_FLEX_ERROR	0xc0001100

/*
 *	The LSAPI functions
 */

lm_extern LS_STATUS_CODE API_ENTRY LSEnumProviders lm_args((LS_ULONG Index,
						  LS_STR_PTR Buffer));

lm_extern void API_ENTRY LSFreeHandle lm_args((LS_HANDLE LicenseHandle));

lm_extern LS_STATUS_CODE API_ENTRY LSGetMessage lm_args((
					       LS_HANDLE LicenseHandle,
					       LS_STATUS_CODE Value,
					       LS_STR_PTR Buffer,
					       LS_ULONG BufferSize));

lm_extern LS_STATUS_CODE API_ENTRY LSQuery    lm_args((LS_HANDLE LicenseHandle,
					          LS_ULONG Information,
					          LS_VOID_PTR InfoBuffer,
					          LS_ULONG BufferSize,
						  LS_ULONG_PTR ActualBufferSize));

lm_extern LS_STATUS_CODE API_ENTRY LSRelease  lm_args((LS_HANDLE LicenseHandle,
					    LS_ULONG TotUnitsConsumed,
					     LS_STR_PTR LogComment));

lm_extern LS_STATUS_CODE API_ENTRY LSRequest lm_args((LS_STR_PTR LicenseSystem,
				 	    LS_STR_PTR PublisherName,
				 	    LS_STR_PTR ProductName,
				 	    LS_STR_PTR Version,
				 	    LS_ULONG TotUnitsReserved,
				 	    LS_STR_PTR LogComment,
				 	    LS_CHALLENGE_PTR Challenge,
					    LS_ULONG_PTR TotUnitsGranted,
					    LS_HANDLE_PTR Handle));

lm_extern LS_STATUS_CODE API_ENTRY LSUpdate   lm_args((LS_HANDLE LicenseHandle,
					     LS_ULONG TotUnitsConsumed,
					     LS_ULONG NewUnitsRequired,
					     LS_STR_PTR LogComment,
					     LS_CHALLENGE_PTR Challenge,
					     LS_ULONG_PTR TotUnitsGranted));

