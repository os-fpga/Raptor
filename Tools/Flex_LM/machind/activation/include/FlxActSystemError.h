/**************************************************************************************************
* Copyright (c) 2014-2018 Flexera. All Rights Reserved.
**************************************************************************************************/
#ifndef FLX_ACT_SYSTEM_ERROR_H
#define FLX_ACT_SYSTEM_ERROR_H

/*
	This header defines System Error Codes.
	These are returned by API calls in the sysErrorNo member of the FlxActError structure.

	Legacy codes in the range 1 - 199 may be returned in some cases, these are undocumented. If the 
	majorErrorNo does not identify the error in sufficent detail the function flxAct<tba>
	may provide more information.
*/

#define LM_TSSE_NO_ERROR				    0	/* No system error code was set by the last API function called. */

/*
	Licensing service errors
	========================
	The function flxActCommonInit() does basic checks for the presence of the licensing service
	so unless it is uninstalled or otherwise changed whilst the application is running, the errors 
	LM_TSSE_SERVICE_NOT_PRESENT and LM_TSSE_SERVICE_NOT_ENOUGH_RIGHTS will not be returned by calls
	to other functions.

	Other licensing service errors are normally reported by flxActCommonHandleOpen().  	These are 
	usually caused by installation issues.

	Action:
	1. Install the licensing service from the  same kit that was used to build the application.
	2. If that does not work, completely uninstall the service, re-boot and re-install.
*/
#define LM_TSSE_SERVICE_NOT_PRESENT			10101	/* The licensing service is not present on the system.
													   (may require a restart or a re-install) */
#define LM_TSSE_SERVICE_NOT_ENOUGH_RIGHTS	10102	/* Insufficient rights to connect to the licensing service. */
#define LM_TSSE_SERVICE_TOO_OLD				10103	/* Licensing service too old; its version must be the same
													   or greater than the kit used to build this application. */	
#define LM_TSSE_SERVICE_DISABLED			10104	/* The licensing service has been disabled. */
#define LM_TSSE_SERVICE_MARKED_FOR_DELETE	10105	/* The licensing service is marked for deletion. */
#define LM_TSSE_SERVICE_DATABASE_LOCKED		10106	/* The Windows service control manager database is locked. */
#define LM_TSSE_SERVICE_CONFIGURATION		10107	/* The Windows licensing service or its dependencies are configured 
                                                       incorrectly. On some Windows platforms this can also indicate
                                                       insufficient rights of the service log-on user.  
                                                       See TS Event Log for Windows error code.  */
#define LM_TSSE_SERVICE_DID_NOT_START		10108	/* The licensing service did not start. 
													   See TS Event Log for Windows service status. */

#define LM_TSSE_SERVICE_READ_ONLY_PORTAL	10109	/* The RO portal interface call failed - see event log 
													   for portal error code (service status). */
#define LM_TSSE_SERVICE_NONE				10110	/* This platform does not have a licensing service. */
#define LM_TSSE_SERVICE_INFO_NOT_AVAILABLE	10111	/* The information requested is not available from the
													   licensing service on this platform. */

/*
	MAC licensing service errors
	============================
*/
#define LM_TSSE_SERVICE_NOT_PRESENT_MAC		10121	/* The MAC licensing service is not present on the system. */
#define LM_TSSE_SERVICE_FAILED_MAC			10122	/* The MAC licensing service failed. */

/*
	Local activation (ASR loading)
	==============================
	Functions	flxActCommonLicSpcCheckASR	 flxActCommonLicSpcCheckASRFromBuffer
				flxActCommonLicSpcAddASRs	 flxActCommonLicSpcAddASRFromBuffer
				flxActCommonLicSpcResetASR	 flxActCommonLicSpcResetASRFromBuffer
*/
#define LM_TSSE_IS_ASR_ERROR(err) ((err) > 10200 && (err) < 10240)
#define LM_TSSE_ASR_PATH					10201	/* The ASR path supplied cannot be opened as a file or directory. */
#define LM_TSSE_ASR_NOT_XML					10202	/* The ASR supplied is not well-formed XML. */
#define LM_TSSE_ASR_NOT_ASR					10203	/* The ASR supplied is not an ASR (XML root element not 'ActivationSpecificationRecord'). */
#define LM_TSSE_ASR_XML_ELEMENT_MISSING		10204	/* The ASR supplied has a missing XML element. */
#define LM_TSSE_ASR_SIGNATURE				10205	/* The ASR signature does not verify. */

#define LM_TSSE_ASR_LOADED_ALREADY			10206	/* The ASR has been loaded already; its fulfillment exists in Trusted Storage. */
#define LM_TSSE_ASR_LOADED_PREVIOUSLY		10207	/* The ASR has been loaded previously; its fulfillment (or Trusted Storage) 
													   has been deleted at some time. */
#define LM_TSSE_ASR_NONE_IN_DIRECTORY		10208	/* The ASR path supplied was a directory (as opposed to a file) and no ASRs were found in it. */
#define LM_TSSE_ASR_IS_SHORTCODE			10209	/* The ASR cannot be loaded because it is for use only in shortcode transactions. */
#define LM_TSSE_ASR_READ					10210	/* The ASR can be opened but not read. */
#define LM_TSSE_ASR_HAS_NO_CONFIG			10211	/* The ASR has no ConfigData (Validate the ASR against Schema B). */
#define LM_TSSE_ASR_WRONG_SERVER_MODE		10212	/* The ASR is for server TS and is being loaded to application TS, or vice versa. */
#define LM_TSSE_ASR_NO_TRUSTED_SECTION		10213	/* The ASR has a TrustedId for a section that does not exist (trusted config not
													   included in ASR and section not previously created). */
#define LM_TSSE_ASR_NO_RESET_ATTRIBUTE		10214	/* The ASR can't be used to reset a trial because it does not have the reset attribute. */
#define LM_TSSE_ASR_ALL_IN_DIRECTORY		10215	/* All the ASRs in the directory failed to load. */
#define LM_TSSE_ASR_CHECK_IS_DIR			10216	/* Path for flxActCommonLicSpcCheckASR must be a file not a directory. */

/*
	Single action transaction response processing errors
	====================================================
	Functions flxActAppActivationRespProcess, flxActAppActivationSend, flxActAppReturnSend, flxActAppRepairSend
	
*/
#define LM_TSSE_RESP_NOT_XML_OR_SHORTCODE	10241	/* Response supplied is not an expected format (not well-formed XML or shortcode). */
#define LM_TSSE_RESP_NOT_XML_RESPONSE		10242	/* Response supplied is XML but not a response (does not contain a ResponseHeader). */
#define LM_TSSE_RESP_UNKNOWN_TYPE			10243	/* XML response is of an unknown type (not Activate, Return, Repair or Error). */
#define LM_TSSE_RESP_XML_ELEMENT_MISSING	10244	/* XML response has a missing element. */
#define LM_TSSE_RESP_SIGNATURE				10245	/* XML response signature does not verify. */
#define LM_TSSE_RESP_NO_REQUEST				10246	/* No valid outstanding request for this XML response (response already processed,  
													   response is for a different machine, request has been cancelled
													   or trust has been lost since the request was created). */
#define LM_TSSE_RESP_NOT_XML				10247	/* Response supplied is not XML or XML is not well-formed.  */
#define LM_TSSE_RESP_SERVER_DENY			10248	/* XML response is that the request was denied by the server.  */
#define LM_TSSE_RESP_SERVER_ERROR			10249	/* XML response is that the request was errored by the server.  */
#define LM_TSSE_RESP_NO_KEY					10250	/* XML response targets a trusted section that does not exist 
													   or is untrusted or does not contain the verification key required. */

/*
	Short code transaction response processing errors
	=================================================
	Function flxActAppActivationRespProcess.
	Note that if a response code is entered incorrectly (e.g. a digit is randomly mis-typed) then then about 97% of the time the error reported
	will be LM_TSSE_SHORTCODE_TYPE but there is a 3% chance that it could be LM_TSSE_SHORTCODE_NO_REQUEST and a 0.02% chance it could be
	LM_TSSE_SHORTCODE_SIGNATURE.
	This is because of the way the codes are constructed (to keep them short).
*/
#define LM_TSSE_SHORTCODE_NO_REQUEST		10281	/* No valid outstanding request for this shortcode response (response already processed,  
													   response entered incorrectly, response is for a different machine, 
													   request has been cancelled or trust has been lost since the request was created). */
#define LM_TSSE_SHORTCODE_TYPE				10282	/* Shortcode response type unknown (response code entered incorrectly). */
#define LM_TSSE_SHORTCODE_SIGNATURE			10283	/* Shortcode response signature did not verify (response entered incorrectly, 
													   response is not for the pending request on this machine, key management issue). */
#define LM_TSSE_SHORTCODE_LOCAL_ASR			10284	/* The ASR cannot be loaded because it is for local activations only. */
#define LM_TSSE_SHORTCODE_WARN_OVERWRITE	10285	/* (Warning) activation response replaced an existing fulfillment with the same id. */

/*
	Short code errors
	=================
*/
#define LM_TSSE_SHORTCODE_FID_NOT_FOR_ASR	10291	/* The FID specified for return or repair was not created using the specified ASR. */
#define LM_TSSE_SHORTCODE_FID_NOT_FOUND		10292	/* No fulfillment record with the FID specified exists. */
#define LM_TSSE_SHORTCODE_NO_FR_FOR_ASR		10293	/* Cannot return or repair because no FR activated with the specified ASR exits. */
#define LM_TSSE_SHORTCODE_FR_DISABLED		10294	/* Cannot return because there is a pending return request for the FR */
#define LM_TSSE_SHORTCODE_PENDING			10295	/* Cannot create request because there is one pending for the ASR */
#define LM_TSSE_SHORTCODE_INVALID_CHAR		10296	/* Invalid character(s) in shortcode.  */
#define LM_TSSE_SHORTCODE_INVALID_CHECK		10297	/* Short code check digit verification failed (code entered incorrectly).  */
#define LM_TSSE_SHORTCODE_BAD_ASR_CDT		10298	/* Short code ASR 'CheckDigiting' property not 'none', 'all' or 'group'.  */
#define LM_TSSE_SHORTCODE_TOO_BIG			10299	/* Short code exceeds 1024 bits (check ASR overrides).  */
#define LM_TSSE_SHORTCODE_OVERRIDE_SIZE		10300	/* Short code ASR override size is invalid (non-numeric or greater than UINT32_T_MAX). */
#define LM_TSSE_SHORTCODE_UNDERFLOW			10301	/* Short code does not contain enough bits (request entered as response?). */

/*
	API function parameter errors
	=============================
*/
#define LM_TSSE_PARAM_POINTER_IS_NULL		11001	/* Pointer parameter is NULL. */
#define LM_TSSE_PARAM_BUFFER_TOO_SMALL		11002	/* The buffer supplied is too small. */

/*
	Health cherck errors
	====================
*/
#define LM_TSSE_CHECK_EVENT_LOG				11101	/* Health Check: can't log events			*/
#define LM_TSSE_CHECK_PLATFORM				11102	/* Health Check: check not implemented on this platform	*/
#define LM_TSSE_CHECK_NO_INVALID			11103	/* Health Check: check number invalid		*/
#define LM_TSSE_CHECK_NOT_RUN				11104	/* Health Check: check not run				*/
#define LM_TSSE_CHECK_ANCHOR_WRITE			11105	/* Health Check: anchor write error			*/
#define LM_TSSE_CHECK_ANCHOR_READ			11106	/* Health Check: anchor read error			*/
#define LM_TSSE_CHECK_ANCHOR_DATA			11107	/* Health Check: anchor data incorrect		*/
#define LM_TSSE_CHECK_ANCHOR_REMOVE			11108	/* Health Check: anchor remove error		*/
#define LM_TSSE_CHECK_BINDING_NO_VALUE		11109	/* Health Check: can't get a value for any binding entity	*/
#define LM_TSSE_CHECK_EXCEPTION				11110	/* Health Check: exception - see event log	*/
#define LM_TSSE_CHECK_TS_CREATE				11111	/* Health Check: can't create TS file		*/
#define LM_TSSE_CHECK_GET_COMMON			11112	/* Health Check: can't get common section	*/
#define LM_TSSE_CHECK_CREATE_SECTION		11113	/* Health Check: can't create Health Check section	*/
#define LM_TSSE_CHECK_SET_VALUE				11114	/* Health Check: can't get set value in HC section	*/
#define LM_TSSE_CHECK_GET_VALUE				11115	/* Health Check: can't get get value from HC section	*/
#define LM_TSSE_CHECK_WRONG_VALUE			11116	/* Health Check: value read is not value written	*/
#define LM_TSSE_CHECK_TS_SAVE				11117	/* Health Check: can't save	TS						*/
#define LM_TSSE_CHECK_DELETE_SECTION		11118	/* Health Check: can't delete Health Check section	*/
#define LM_TSSE_CHECK_PREPPED_CONFIG		11119	/* Health Check: processing prepped trusted config failed */
#define LM_TSSE_CHECK_OPEN_UNTRUSTED		11120	/* Health Check: can't open trusted section even when ignoring trust issues */
#define LM_TSSE_CHECK_OPEN_TRUSTED			11121  	/* Health Check: section is not trusted		*/
#define LM_TSSE_CHECK_OPEN_PTC_SECTION		11122  	/* Health Check: the prepped trusted config section was not created	*/
#define LM_TSSE_CHECK_DELETE_PTC_SECTION	11123  	/* Health Check: could not delete the prepped trusted config section*/
#define LM_TSSE_CHECK_DELETE_PTC_ANCHORS	11124  	/* Health Check: could not delete the prepped trusted config section anchors */
#define LM_TSSE_CHECK_SERVICE				11125	/* Health Check: service errror catch-all 
													   Service errors are nomally reported by LM_TSSE_SERVICE_XXX, this is a catch-all if not */
/*
	Resource errors
	===============
*/
#define LM_TSSE_REQ_V_XML					16901	/* Unexpected error creating internal request XML; could be low memory. */

/*
	Development Errors
	==================
	These errors should only occur during software development as a result of incorrect API use
	or invalid local or server document content (e.g. ASR, response XML).
*/
#define LM_TSSE_SHORTCODE_SCHEME_UNKNOWN	17001	/* The shortcode SchemeId in the ASR is not valid. */
#define LM_TSSE_SHORTCODE_VENDOR_DATA		17002	/* The vendor data item being added to the shortcode is not valid. */
#define LM_TSSE_SHORTCODE_KEY_MISSING		17003	/* The shortcode key required is not present in the trusted configuration. */
#define LM_TSSE_RESP_FAILURE_REASON			17004	/* The ResponseReason in a failure response is not DENIED or ERROR.  Validate against XML schema 'T'. */
#define LM_TSSE_SHORTCODE_NO_TS_SECTION		17005	/* The trusted section needed by the short code ASR does not exist; this is usually because the
													   prepped trusted config of a server activation application does not have the same trusted id as
													   the ASR */

/*
	Unexpected errors
	=================
	At the time this software was finalised there were no known conditions that would cause these errors to be returned. 

	Action:
	1. Check release notices, post-release support documentation and knowledge bases.
*/
#define LM_TSSE_LAST_ERROR_NOT_SET			19001	/* fnpActManGetLastError() failed and no System Error Code was set. */
#define LM_TSSE_LAST_ERROR_STD_EXCEPTION	19002	/* fnpActManGetLastError() threw a std::exception. */
#define LM_TSSE_NOT_XML						19203	/* Generic, not XML when XML expected.  */
#define LM_TSSE_NOT_SHORTCODE				19204	/* Pre 11.14.0 not a shortcode; replaced by LM_TSSE_SHORTCODE_INVALID.  */
#define LM_TSSE_SHORTCODE_ASR_LOAD			19205	/* Failed to load stored shortcode ASR when processing response.  */
#define LM_TSSE_SHORTCODE_ASR_LOAD_GET		19206	/* Failed to get stored shortcode request after loading.  */
#define LM_TSSE_SHORTCODE_NO_FR				19207	/* FR targeted by shortcode response does not exist.  */
#define LM_TSSE_SHORTCODE_REQ_TYPE			19208	/* Stored shortcode request type unknown.  */
#define LM_TSSE_SHORTCODE_ALIAS_MISMATCH	19209	/* Shortcode alias in response does not match that in stored ASR. */
#define LM_TSSE_SERVICE_REGISTER			19210	/* Could not register the licensing service. */
#define LM_TSSE_ASR_NOT_DIRECTORY			19211	/* Converted to LM_TSSE_ASR_PATH. */
#define LM_TSSE_SERVICE_CONNECT_EXCEPTION	19212	/* Service connect threw unexpected exception. */
#define LM_TSSE_ASR_HAS_MULTIPLE_IDS		19213	/* Multiple trusted ids in an ASR. */
#define LM_TSSE_ASR_SECTION_NOT_TRUSTED		19214	/* The target section for an ASR is not fully trusted. */
#define LM_TSSE_ASR_HAS_SERVER_CONFIG		19215	/* The ASR is for server TS and it contains a Trusted Configuration. */
#define LM_TSSE_CTXN_ELS_UNEXPECTED			19216	/* Error when performing composiet transaction with the Enterprise
													   Licensing Server.  See event log for code location. */
#define LM_TSSE_STD_EXCEPTION				19217	/* Unexpected std::exception. See event log. */
#define LM_TSSE_INTEGER_EXCEPTION			19218	/* Unexpected integer exception. See event log. */
#define LM_TSSE_STRING_EXCEPTION			19219	/* Unexpected const char* exception. See event log. */
#define LM_TSSE_DEFAULT_EXCEPTION			19220	/* Unexpected exception of unknown type. */

#define LM_TSSE_EXCEPTION_ID_BASE			19800	/* System error codes 19801-19998 are mapped to exception ids 1 - 198. */
#define LM_TSSE_EXCEPTION_ID_FUTURE			19999	/* Reserved for (unlikely) exception ids greater than 198. */
#endif
