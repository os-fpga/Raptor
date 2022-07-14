/**************************************************************************************************
* Copyright (c) 2005-2013, 2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
* Header file for the manual response generator component (responsegen.dll/so) for "C"
*
* Header file for the manual response generator component. 
* On Win32 this is responsegen.dll (formerly manualresponsegeneratordll.dll)
* On non-Win32 this responsegen.so.
*
*/

#ifndef MANUALRESPONSEGENERATORDLLC_INCLUDED
#define MANUALRESPONSEGENERATORDLLC_INCLUDED

/* ========== API Overview =========================================
	Context handle
		Get one from fnpMargeOpenHandleC.
		Use it for one or more transactions (XML or shorcode or both).
		Close it with fnpMargeCloseHandleC.

	Error reporting
		If a function fails get error information from
			fnpMargeGetLastErrorC - error code (see responsegenerrors.h).
			fnpMargeGetErrorDescriptionC - text description of error code.
			fnpMargeGetLastErrorDetailC - error detail text.

	XML Transaction
	  *	Call fnpMargeValidateRequestC to alidate the request XML.
	  *	If valid, identify the request type from the root element an  use the information 
		in the request and the outcome required to construct a response parameter XML  
		document confroming to the "W1" (activate), "W2" (return)XML schemas), "W3" (repair),
		"W4S/C" (cofig) or "W5" (failure) schema.
	  *	Identify the key data needed, one of
		  *	NULL for config and failure responses.
		  *	If the response targets just one trusted section, the "Long_1" <KeyData> value 
			from a "P2" schema <SecureKeyInformation> XML document.
		  *	If the response targets multiple trusted sections, a "P3" schema <MultipleKeyData> 
			XML document containing the keys for all the trusted sctions. 
			This must be manually constructed from the "P2" documents for each section.
	  *	Call fnpMargeGenerateResponseC.

	Shortcode Transaction
	  *	Call fnpMargeValidateRequestCodeAndExtractAliasC to validate the request and 
		get its short code alias.
	  *	Use the short code alias to identify the ASR used to create the request.
	  *	Call fnpMargeAnalyzeRequestCodeC to get the request code content.
	  * Use the request code content and the outcome required to construct a
	    response code parameter XML document conforming to the "W6" schema.
	  *	Get the key data, which is The "Short_?" <KeyData> value from the "P2" schema 
		<SecureKeyInformation> XML document for the trusted sction specified by the ASR
	  *	Call fnpMargeGenerateResponseCodeC
*/

#if OS_WINDOWS

	#ifdef MANUALRESPONSEGENERATORDLL_EXPORTS
		#define MANUALRESPONSEGENERATORDLL_API __declspec(dllexport)
	#else
		#define MANUALRESPONSEGENERATORDLL_API __declspec(dllimport)
	#endif

#include "winstdint.h"

#else
	#if defined(__GNUC__) && __GNUC__ >= 4
	    #define MANUALRESPONSEGENERATORDLL_API __attribute__ ((visibility("default")))
	#else
	    #define MANUALRESPONSEGENERATORDLL_API
	#endif

#if OS_SOLARIS
#include "inttypes.h"
#else
#include "stdint.h"
#endif

#endif

// Common error codes.
#include "responsegenerrors.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef void* HMARGE_CONTEXTC;
typedef int32_t MARGE_BOOL;
#define MARGE_TRUE	1
#define MARGE_FALSE	0

/* ========== Generic Marge functions =========================================

//		fnpMargeGetVersion
//		==================
/// Return the API version.
///
/// @param[out]	pMajor			For return of the major version number.
/// @param[out]	pMinor			For return of the minor version number.
/// @param[out]	pMaint			For return of the maintenance version number.
/// @param[out]	pHotfix			For return of the hotfix version number.
///
*/
MANUALRESPONSEGENERATORDLL_API void fnpMargeGetVersionC(uint32_t* pMajor, 
													    uint32_t* pMinor,
													    uint32_t* pMaint,
													    uint32_t* pHotfix);

/*		fnpMargeOpenHandleC
//		===================
/// Open a handle.
///
/// @return	A valid handle on success, zero otherwise.
*/
MANUALRESPONSEGENERATORDLL_API HMARGE_CONTEXTC fnpMargeOpenHandleC();

/*		fnpMargeCloseHandleC
//		============================
/// Close a Marge handle.
///
/// @param[in]	hContextC		            A handle to be closed.
///
/// @return	void
*/ 
MANUALRESPONSEGENERATORDLL_API void fnpMargeCloseHandleC(HMARGE_CONTEXTC hContextC);

/* ========== XML based Marge functions =======================================

//		fnpMargeValidateRequestC
//		========================
/// Validate an activation request from a client.
///
/// @param[in]	hContextC		            An open handle.
/// @param[in]	pszXmlRequest		        The client request to validate.
/// @param[out]	rErrorCode		            An error code on failure.
///
/// @return	MARGE_BOOL -	true if the request is valid, false otherwise - 
///					rErrorCode will have been set.
*/ 
MANUALRESPONSEGENERATORDLL_API MARGE_BOOL fnpMargeValidateRequestC(HMARGE_CONTEXTC hContextC,
                                                            const char *pszXmlRequest);

/*		fnpMargeGenerateResponseC
//		=========================
/// Generate a response to the client.
///
/// @param[in]	hContextC		            An open handle.
/// @param[in]	pszXmlRequest		        The client request.
/// @param[in]	pszXmlResponseParam		    The parameter to 'drive' the response generations. See schemas W1-W5.
/// @param[in]	pszKeyData					The key data.  One of
///											  *	NULL for config and failure responses.
///											  *	If the response targets just one trusted section, the "Long_1" <KeyData> value 
///												from a "P2" schema <SecureKeyInformation> XML document.
///											  *	If the response targets multiple trusted sections, a "P3" schema <MultipleKeyData>
///                                         	XML document containing the keys for all the trusted sctions. 
///												This must be manually constructed from the "P2" documents for each section.
/// @param[out]	pszXmlResponse				The signed XML response.
///
/// @return	MARGE_BOOL -	true if the response is generated, false otherwise.
*/
MANUALRESPONSEGENERATORDLL_API MARGE_BOOL fnpMargeGenerateResponseC(HMARGE_CONTEXTC hContextC, 
                                                            const char *pszXmlRequest,
                                                            const char *pszXmlResponseParam,
                                                            const char *pszKeyData,
                                                            const char **ppszXmlResponse);

/* ========== Short Code based Marge functions ================================

//		fnpMargeValidateRequestCodeAndExtractAliasC
//		===========================================
/// Validate a short request code, and extract its Short Code Alias.
///
/// @param[in]	hContextC		            An open handle.
/// @param[in]	pszRequestCode		        The client request *code* to validate (NOTE: NOT XML).
/// @param[out]	rShortCodeAlias		        The short code alias from within the request code.
///
/// @return	MARGE_BOOL -	true if the request is valid, false otherwise.
*/ 
MANUALRESPONSEGENERATORDLL_API MARGE_BOOL fnpMargeValidateRequestCodeAndExtractAliasC(HMARGE_CONTEXTC hContextC,
																               const char *pszRequestCode,
																               const char **ppszShortCodeAlias);

/*		fnpMargeAnalyzeShortRequestCodeC
//		================================
/// Analyze a short request code and extract its key/values.
///
/// @param[in]	hContextC		            An open handle.
/// @param[in]	pszRequestCode		        The client request *code* to validate (NOTE: NOT XML).
/// @param[in]	pszKeyData					The "Short_?" <KeyData> value from a "P2" schema <SecureKeyInformation> XML document
///											for the trusted section specified in the ASR.
/// @param[in]	pszActSpecRecXml		    The identical Activation Specification Record XML used by the client 
///											when generating the short request code.
/// @param[out]	ppszAnalysisXml		        The analysis of what is in the request code. See schema TBD.
///
/// @return	MARGE_BOOL -	true if the request is valid, false otherwise.
*/ 
MANUALRESPONSEGENERATORDLL_API MARGE_BOOL fnpMargeAnalyzeRequestCodeC(HMARGE_CONTEXTC hContextC,
																const char *pszRequestCode,
																const char *pszKeyData,
																const char *pszXmlActSpecRec,
																const char **ppszXmlAnalysis);

/*		fnpMargeGenerateResponseCodeC
//		=============================
/// Generate a short response code
///
/// @param[in]	hContextC		            An open handle.
/// @param[in]	pszRequestCode		        The client request *code* to validate (NOTE: NOT XML).
/// @param[in]	pszKeyData					The "Short_?" <KeyData> value from a "P2" schema <SecureKeyInformation> XML document
///											for the trusted section specified in the ASR.
/// @param[in]	pszActSpecRecXml		    The identical Activation Specification Record XML used by the client 
///											when generating the short request code.
/// @param[in]	pszW6ParameterXml		    All the XML needed to drive the response generation.
/// @param[out]	ppszResponseCode		    The response code.
///
/// @return	MARGE_BOOL -	true if the request is valid, false otherwise.
*/ 
MANUALRESPONSEGENERATORDLL_API MARGE_BOOL fnpMargeGenerateResponseCodeC(HMARGE_CONTEXTC hContextC,
																 const char *pszRequestCode,
																 const char *pszKeyData,
                                                                 const char *pszXmlActSpecRec,
																 const char *pszXmlW6Parameter,
																 const char **ppszResponseCode);
/*		fnpMargeGetLastErrorC
//		=====================
/// Get the error code for the last error.
///
/// @param[in]	hContextC		            An open handle.
///
/// @return	fnpMargeErrorCode -	an appropriate error enum value.
*/ 
MANUALRESPONSEGENERATORDLL_API enum fnpMargeErrorCode fnpMargeGetLastErrorC(HMARGE_CONTEXTC hContextC);

/*		fnpMargeGetErrorDescriptionC
//		============================
/// Get the error description for an error code.
///
/// @param[in]	err	    An error to look up.
///
/// @return	const char* -	an appropriate error description.
*/ 
MANUALRESPONSEGENERATORDLL_API const char* fnpMargeGetErrorDescriptionC(enum fnpMargeErrorCode err);

/*		fnpMargeGetLastErrorDetailC
//		===========================
/// Get a text string with more details of the last error (for diagnostics and support).
/// The details are not defined but provide additional information on the error for
/// use by Flexera when support for unexpected errors is needed.
/// If no details are available an empty string is returned.
///
/// @param[in]	hContext		            An open handle.
///
/// @return	const char* -	the error detail.
*/ 
MANUALRESPONSEGENERATORDLL_API const char* fnpMargeGetLastErrorDetailC(HMARGE_CONTEXTC hContextC);
#ifdef __cplusplus
}
#endif

#endif	// #include guard
