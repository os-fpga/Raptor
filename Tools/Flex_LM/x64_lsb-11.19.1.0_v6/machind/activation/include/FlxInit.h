/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
#if !defined( FLXINIT_H_INCLUDED )
#define FLXINIT_H_INCLUDED

/*
 *	Defines the error codes returned by flxActCommonLibraryInit().
 *
 */

/**************************************************************************************************
 *	Error codes returned by flxActCommonLibraryInit().
 *************************************************************************************************/

#define flxInitSuccess 					0 	/* Load was successful. */
#define flxInitUnableToLocate			1	/* Unable to find security runtime. */
#define flxInitUnableToLoad				2	/* Unable to load security runtime. */
#define flxInitUnsupportedPlatform		3	/* OS version too old. */
#define flxInitInitializationError		4	/* Initialization of security runtime failed. */
#define flxInitAllocationError			5	/* Unable to allocate resources. */
#define flxInitNotProtected				6	/* Preptool has not been run since build. */
#define flxInitCannotReload				7   /* Call to flxInitLoad after an flxInitUnload. */

/* Mac OS X specific error values */
#define flxInitEncodingError			8	/* Path string not in UTF-8. */
#define flxInitUrlError					9	/* Error creating URL. */
#define flxInitUrlNoPathError			10	/* Error creating path from URL. */
#define flxInitFrameworkNotLoadedError	11	/* Framework specified by bundle ID not loaded. */
#define flxInitBundleIDError			12	/* Not a valid bundle ID. */
#define flxInitPathOverflow				13	/* Computed path is too long. */

/* Windows licensing service error values */
#define flxInitServiceNotInstalled		20	/* Licensing ervice not installed. */
#define flxInitServiceNotEnoughRights	21	/* Not enough rights to communicate with service.
											   Set service to start automatically to resolve this. */
#define flxInitServiceConfigurationError 22	/* Service configuration or dependency issue.
											   See event log for Windows error code. */
#define flxInitTSPrepTypeMismatch        23 /* Attempt to access server TS from 
											client app, or vice-versa. */

/* Privilege error */
#define flxInitPrivilegeError			24	/* Insufficient Privilege to complete initialization */
#endif /* FLXINIT_H_INCLUDED */
