/**************************************************************************************************
 * Copyright (c) 1997-2022 Flexera. All Rights Reserved.
 **************************************************************************************************/

/*
 *
 *	Description: FLEXlm long error strings
 *
 */

/* 0:NOERROR */
" Usually this error message should be ignored.\n\
 It occurs when the FlexNet Licensing error message function was called\n\
 though no error was detected.",

/* 1: NOCONFFILE */

" The license files (or license server system network addresses) attempted are \n\
listed below.  Use LM_LICENSE_FILE to use a different license file,\n\
 or contact your software provider for a license file.",

/* BADFILE */		0,
/* NOSERVER */
"There is a license server manager/vendor daemon running, but it's for\n\
other vendors.",
/* MAXUSERS */		0,
/* NOFEATURE */		0,
/* NOSERVICE */		0,
/* NOSOCKET */		0,
/* BADCODE */
" The license key and data for the feature do not match.\n\
 This usually happens when a license file has been altered.",
/* NOTTHISHOST */
" The hostid of this system does not match the hostid\n\
 specified in the license file.",
/* LONGGONE */		0,
/* BADDATE */		0,
/* BADCOMM */		0,
/* NO_SERVER_IN_FILE */	0,
/* BADHOST */
" The lookup for the hostname on the SERVER line in the\n\
 license file failed.  This often happens when NIS or DNS\n\
 or the hosts file is incorrect.  Workaround: Use IP-Address\n\
 (e.g., 123.456.789.123) instead of hostname.",
/* CANTCONNECT */
" The license server manager (lmgrd) has not been started yet,\n\
 the wrong port@host or license file is being used, or the\n\
 port or hostname in the license file has been changed.",
/* CANTREAD */
" The license server system appears to be running, but is not\n\
responding.  If this persists, notify the System Administrator.\n\
(The license server manager (lmgrd) and vendor daemon processes\n\
should be terminated and restarted.)",
/* CANTWRITE */		0,
/* NOSERVSUPP */	0,
/* SELECTERR */		0,
/* SERVBUSY */		0,
/* OLDVER */		0,
/* CHECKINBAD */	0,
/* BUSYNEWSERV */	0,
/* USERSQUEUED */	0,
/* SERVLONGGONE */	0,
/* TOOMANY */		0,
/* CANTREADKMEM */	0,
/* CANTREADVMUNIX */	0,
/* CANTFINDETHER */	0,
/* NOREADLIC */		0,
/* TOOEARLY */		0,
/* NOSUCHATTR */	0,
/* BADHANDSHAKE */	0,
/* CLOCKBAD */		0,
/* FEATQUEUE */		0,
/* FEATCORRUPT */	0,
/* BADFEATPARAM */	0,
/* FEATEXCLUDE */	0,
/* FEATNOTINCLUDE */	0,
/* CANTMALLOC */	0,
/* NEVERCHECKOUT */	0,
/* BADPARAM */		0,
/* NOKEYDATA */		0,
/* BADKEYDATA */	0,
/* FUNCNOTAVAIL */	0,
/* DEMOKIT */		0,
/* NOCLOCKCHECK */	0,
/* BADPLATFORM */	0,
/* DATE_TOOBIG */	0,
/* EXPIREDKEYS */	0,
/* NOFLEXLMINIT */	0,
/* NOSERVRESP */	0,
/* CHECKOUTFILTERED */	0,
/* NOFEATSET */		0,
/* BADFEATSET */	0,
/* CANTCOMPUTEFEATSET */ 0,
/* SOCKETFAIL */ 	0,
/* SETSOCKFAIL */	0,
/* BADCHECKSUM */	0,
/* SERVBADCHECKSUM */	0,
/* SERVNOREADLIC */	0,
/* NONETWORK */		0,
/* NOTLICADMIN */	0,
/* REMOVETOOSOON */	0,
/* BADVENDORDATA */	0,
/* LIBRARYMISMATCH */	0,
/* NONETOBORROW */	0,
/* NOBORROWSUPP */	0,
/* LM_NOTONSERVER */
" This error can occur because the license server system isn't running, or\n\
 the FLEXenabled application needs to add @localhost with the\n\
 lmpath command.",
/* LM_BORROWLOCKED */
" lmutil lmborrow -startupdate was issued but not updated yet.\n\
To override this, stop and restart the license server system.\n\
WARNING: overriding may cause loss of licenses.",
/* BAD_TZ */		0,
/* OLDVENDORDATA */	0,
/* LOCALFILTER */	0,
/* LM_ENDPATH */	0,
/* LM_VMS_SETIMR_FAILED */ 0,
/* LM_INTERNAL_ERROR */	0,
/* LM_BAD_VERSION */	0,
/* LM_NOADMINAPI */	0,
/* LM_NOFILEOPS */	0,
/* LM_NODATAFILE */	0,
/* LM_NOFILEVSEND */	0,
/* LM_BADPKG */	 	0,
/* LM_SERVOLDVER */	0,
/* LM_USERBASED	*/ 	0,
/* LM_NOSERVCAP	*/ 	0,
/* LM_OBJECTUSED */	0,
/* LM_MAXLIMIT */	0,
/* LM_BADSYSDATE */	0,
/* LM_PLATNOTLIC */	0,
/* LM_FUTURE_FILE */
" The file was issued for a later version of FlexNet Licensing than this\n\
 program understands.",
/* LM_DEFAULT_SEEDS */ 	0,
/* LM_SERVER_REMOVED */ 	0,
/* LM_POOL */
" This is a warning condition.  The license server system has pooled one\n\
 or more INCREMENT lines into a single pool, and the request was made on\n\
 an INCREMENT line that has been pooled.  If this is reported as an\n\
 error, it's an internal error.",
 /* LM_LGEN_VER */	0,
/* LM_NEED_SERVER_HOSTNAME */
" The license file indicates THIS_HOST, and the server is not\n\
 running on this host.  If it's running on a different host, \n\
 THIS_HOST should be changed to the correct host.",
/* LM_HOSTDOWN */
" See the system administrator about starting the license server system, or\n\
 make sure you're referring to the right host (see LM_LICENSE_FILE).",
/* LM_VENDOR_DOWN */
" Check the lmgrd log file, or try lmreread.",
/* LM_CANT_DECIMAL */ 	0,
/* LM_BADDECFILE */ 	0,
/* LM_REMOVE_LINGER */ 	0,
/* LM_RESVFOROTHERS */
" The system administrator has reserved all the licenses for others.\n\
 Reservations are made in the options file.",
/* LM_BORROW_ERROR */ 	0,
/* LM_TSOK_ERR */  0,
/* LM_BORROW_TOOLONG */
" Retry the checkout again for a shorter period.",
/* LM_UNBORROWED_ALREADY */ 0,
/* LM_SERVER_MAXED_OUT */
" The vendor daemon can't handle any more users.\n\
See the license server manager (lmgrd) debug log for further information.",
/* LM_NOBORROWCOMP */ 	0,
/* LM_BORROW_METEREMPTY */ 	0,
/* LM_NOBORROWMETER */ 	0,
/* LM_NODONGLE*/
" Either the hardware dongle is not attached, or the necessary\n\
 software driver for this dongle type is not installed.",
/* LM_NORESLINK */
" When linking Windows binaries, you must link with lmgr.lib as\n\
 well as lmgr.res.",
/* LM_NODONGLEDRIVER */
"In order to read the dongle hostid, the correct driver must be\n\
 installed.  These drivers are available from your software vendor.",
/* LM_FLEXLOCK2CKOUT */ 0,
/* LM_SIGN_REQ */
" This is probably because the license is older than the application\n\
 You need to obtain a SIGN= version of this license from your vendor.",
/* LM_PUBKEY_ERR */ 0,
/* LM_NOTRLKEYSUPP */ 0,
/* LM_BORROW_LINGER_ERR */ 0,
/* LM_BORROW_EXPIRED */ 0,
/* LM_MUST_BE_LOCAL */ 0,
/* LM_BORROW_DOWN */
" Use lmstat to find the users that have the licenses borrowed.",
/* LM_FLOATOK_ONEHOSTID */
"Use one line per dongle.",
/* LM_BORROW_DELETE_ERR */ 0,
/* LM_BORROW_RETURN_EARLY_ERR */ 0,
/* LM_BORROW_RETURN_SERVER_ERR */ 0,
/* LM_CANT_CHECKOUT_JUST_PACKAGE */ 0,
/* LM_COMPOSITEID_INIT_ERR */ 0,
/* LM_COMPOSITEID_ITEM_ERR */ 0,
/* LM_BORROW_MATCH_ERR */ 0,
/* LM_NULLPOINTER */	0,
/* LM_BADHANDLE */ 0,
/* LM_EMPTYSTRING */ 0,
/* LM_BADMEMORYACCESS */ 0,
/* LM_NOTSUPPORTED */ 0,
/* LM_NULLJOBHANDLE */ 0,
/* LM_EVENTLOG_INIT_ERR	*/ 0,
/* LM_EVENTLOG_DISABLED	*/ 0,
/* LM_EVENTLOG_WRITE_ERR */ 0,
/* LM_BADINDEX */	0,
/* LM_TIMEOUT */ 0,
/* LM_BADCOMMAND */			0,
/* LM_SOCKET_BROKEN_PIPE */ 0,
/* LM_INVALID_SIGNATURE	*/ 0,
/* LM_UNCOUNTED_NOT_SUPPORTED */ 0,
/* LM_REDUNDANT_SIGNATURES */ 0,
/* LM_BADCODE_V71_LK */	0,
/* LM_BADCODE_V71_SIGN */ 0,
/* LM_BADCODE_V80_LK */ 0,
/* LM_BADCODE_V80_SIGN */ 0,
/* LM_BADCODE_V81_LK */ 0,
/* LM_BADCODE_V81_SIGN */ 0,
/* LM_BADCODE_V81_SIGN2	*/ 0,
/* LM_BADCODE_V84_LK */ 0,
/* LM_BADCODE_V84_SIGN */ 0,
/* LM_BADCODE_V84_SIGN2 */ 0,
/* LM_LK_REQ */ 0,
/* LM_BADAUTH */ 0,
/* LM_REPAIR_NEEDED */ "Trusted Storage is invalid, and needs to be repaired.",
/* LM_TS_OPEN */ 0,
/* LM_BAD_FULFILLMENT */ "Trusted Storage contains a fulfillment record which is invalid.",
/* LM_BAD_ACTREQ */ 0,
/* LM_TS_NO_FULFILL_MATCH */ 0,
/* LM_BAD_ACT_RESP */ 0,
/* LM_CANTRETURN */ 0,
/* LM_RETURNEXCEEDSMAX */
"The return is not allowed because one or more fulfillment record counts\n\
would exceed the allowed maximum.",
/* LM_NO_REPAIRS_LEFT */ "There are no repairs left to service this repair request.",
/* LM_NOT_ALLOWED */ 0,
/* ENTLEXCLUDE */	0,
/* ENTLNOTINCLUDE */	0,
/* ACTIVATION */	0,
/* LM_TS_BADDATE */ 0,
/* LM_ENCRYPTION_FAILED */ 0,
/* LM_DECRYPTION_FAILED */ 0,
/* LM_BADCONTEXT */ 0,
/* LM_SUPERSEDE_CONFLICT */ "SUPERSEDE and SUPERSEDE_SIGN can not be used at the same time.",
/* LM_INVALID_SUPERSEDE_SIGN */ "Invalid syntax for SUPERSEDE_SIGN={\"feature1:signature\" \"feature2:signature\" ....}",
/* LM_SUPERSEDE_SIGN_EMPTYSTRING */ "Feature name and signature are required for SUPERSEDE_SIGN",
/* LM_ONE_TSOK_PLATFORM_ERR */ 0,
/* LM_ONE_TSOK_MTX_ERR */ 0, 
/* LM_ONE_TSOK_ERR */ 0,
/* LM_SSIDNULL */ 0,
/* LM_SMTXNOTREL */ 0,
/* LM_MTXNOPERM   */ 0,
/* LM_COMPOSITEID_ETHER_ERR   */ 0,
/* LM_LIC_FILE_CHAR_EXCEED   */ 0,
/* LM_TZ_INVALID_SYNTAX */ 0,
/* LM_TZ_INVALID_TZONE_SPEC */ 0,
/* LM_TZ_INVALID_TZONE_INFO   */ 0,
/* LM_TZ_UNAUTHORIZED */ 0,
/* LM_INVALID_VM_PLATFORMS */ 0,
/* LM_VM_PHYSICAL_ONLY */ 0,
/* LM_VM_VIRTUAL_ONLY */ 0,
/* LM_VM_NOT_SUPPORT */ 0,
/* LM_VM_BAD_KEY */ 0,
/* LM_MAXLIMIT_EXCEED  */ 0,
/* LM_BA_API_INTERNAL_ERROR */ 0,
/* LM_BA_COMM_ERROR */ 0,
/* LM_INVALID_BA_VERSION */ 0,
/* LM_UNSUPPORTED_FEATURE_HOSTID */ 0,
/* LM_SERVERQUERY_LOAD_REQUEST_FAILED */ 0,
/* LM_SERVERQUERY_RESPONSE_FAILED */ 0,
/* LM_INVALID_IPADDRESS */ 0,
/* LM_UNRESOLVED_FEATURE */ 0,
/* LM_TS_GET_TOTAL_FEAT_COUNT_FAILED */ 0,
/* LM_TS_ACT_RECLAIM_NOT_ALLOWED */ 0,
/* LM_TS_ACT_RECLAIM_FAILED */ 0,
/* LM_TS_ACT_RECLAIM_NO_DR_FOUND */ 0,
/* LM_TS_ACT_RECLAIM_LS_TS_NOT_SUPPORTED */ 0,
/* LM_TS_SAVE_ERROR */ 0,
/* LM_MAXSERVERS */ 0,
/* LM_LICSRVC_FAILED */ 0,
/* LM_NO_VMATTR_PHY_SYS */ 0,
/* LM_LIC_SVC_DISABLED */ 0,
/* LM_LIC_SVC_NOTINSTALLED */ 0,
/* LM_LIC_SVC_VER_ERR */ 0,
/* LM_VM_HOSTID_NOT_AVAILABLE */ 0, 
/* LM_COAVAIL_RECONN_NOT_ALL_CNT_AVAIL */ 0, 
/* LM_COAVAIL_FLAG_NOT_SUPPORTED */ 0, 
/* LM_COAVAIL_PACKAGE_NOT_SUPPORTED */ 0, 
/* LM_COAVAIL_UNSUPPORTED */ 0,
/* LM_ONLY_TS_ALLOWED */ 0,
/* LM_TRANSFER_TO_SELF_NOT_ALLOWED */ 0, 
/* LM_HOSTID_UNSUPPORTED */ 0,
/* LM_LIC_SVC_REINSTALL_SVC */ 0, 
/* LM_LIC_SVC_NOT_AVAILABLE */ 0,
/* LM_LIC_SVC_COMMS_ERROR */ 0,
/* LM_TPM_LICSRVC_FAILED	*/ 0, 
/* LM_TPM_VERSION_UNSUPPORTED	*/ 0, 
/* LM_TPM_DISABLED				*/ 0, 
/* LM_TPM_PROPS_NOT_AVAILABLE  */ 0, 
/* LM_TPM_HOSTID_NOT_AVAILABLE   */ 0,
/* LM_TPM_PLATFORM_UNSUPPORTED  */ 0, 
/* LM_TPM_UNKNOWN_ERROR */          0,
/* LM_AMZ_INIT_ERR */ 0,
/* LM_AMZ_NOT_EC2 */ 0, 
/* LM_3STS_ACTIVATION_NOT_ALLOWED */ 0,
/* LM_LMBORROWL_FOR_HYPHEN_FEATURE */ 0,
/* LM_VD_MAX_CLIENTS_REACHED */ "Maximum connections limit for this vendor daemon has been reached",
/* LM_CHECKOUTDATA_TOO_LONG */ "Checkout Data String is too Long",
/* LM_CHECKOUTDATA_TOO_MANY */ "Checkout Data String exceeds the limit",
/* LM_VSEND_ASYNC_NOT_AVAILABLE */       "The server does not support asynchronous lc_vsend processing",
/* LM_VSEND_ASYNC_BUSY   */              "There is an asynchronous message currently either queued or executing",
/* LM_VSEND_INVALID_MESSSAGE  */         "The lc_vsend message is invalid",
/* LM_VSEND_NO_MESSSAGE  */              "There is no message response either pending or available on the server",
/* LM_VSEND_TOO_MANY */					 "The lc_vsend message queue on the server is full",
/* LM_INVALID_FILE_EXTN */ "Invalid license file extension",
/* LM_UNSUPPORTED_LICENSE_FILE_ENCODING */ "Unsupported license file encoding",
/* LM_INVALID_VENDOR_NAME */ "Invalid Vendor/ Daemon name in the license file",
/* LM_NO_VENDOR_IN_FILE */ "No Vendor/Daemon line in license file",
/* LM_INVALID_VENDOR_PORT */ "Vendor ports are not in range in license file",
/* LM_VENDOR_NOT_SERVED */ "Vendor name mentioned is not served by the lmgrd",
/* LM_FEATURE_INVALID_LICENSE_KEY */ "Feature Line license key is invalid",
/* LM_FEATURE_NOT_FOR_VENDOR */ "Feature line is not for the current vendor",
/* LM_FEATURE_FORMAT_INVALID */ "Feature line format is invalid",
/* LM_INVALID_KEYWORD_IN_LICENSE_FILE */ "Few keywords in the license file are invalid.",
/* LM_MAX_LICFILE_SIZE */ "The license file size has exceeded the maximum allowed limit of 1 MB for validation.",
/* LM_FILE_EXISTS */ "File names passed should be different.",
/* LM_FILE_CANNOT_OPEN */ "Unable to open the file.",
/* LM_FIELDS_RSV_FAILED */ 0,
/* LM_UNSUP_RSV_KEY */ 0,
/* LM_INVALID_LIC_COUNT */	0,
/* LM_UNCOUNTED_LIC */ 0,
/* LM_RSV_ID_NOT_EXIST */ 0,
/* LM_RSV_INVALID_DURATION */ 0,
/* LM_SUITE_RESERVED_NOT_SUPP */ 0,
/* LM_NO_SUCH_FILE */ 0,
/* LM_FILE_EMPTY */ 0,
/* LM_NO_READ_PERMISSION */ 0,
/* LM_RESERVE_LEN_EXCEED */ 0,
/* LM_MAX_RESERVE_LINES */ 0,
/* LM_MISSING_MARKERS */ 0,
/* LM_DOCKER_UNSUPPORTED_PLATFORM */ "Not running under docker platform",
/* LM_DOCKER_ID_NOT_AVAILABLE */ "Docker Container ID not available.",
/* LM_TXN_NOEXISTS */ "Transaction not found",
/* LM_TXN_ATTR_NOTSET */ "Attribute is not set in the transaction entry",
/* LM_LOCAL_CHECK_FAILED */ "Local checks for checkout failed",
/* LM_NOT_DUP_LOCALTEST */ 0,
/* LM_TXN_TOO_MANY_ENTRY */ "Too many entry for batch checkout",
/* LM_NO_ECOMMS_SUPP */ "Encrypted communications is not enabled",
/* LM_ECOMMS_ERROR */ "Encrypted communications have failed"


