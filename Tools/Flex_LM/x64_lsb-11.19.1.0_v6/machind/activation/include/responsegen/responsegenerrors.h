/**************************************************************************************************
* Copyright (c) 2005-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
*	Header file for the manual response generator component error codes (responsegen.dll/so)
*
* Header file for the manual response generator component error codes.
*
*/

#ifndef MVSN_FNP_RESPONSEGEN_ERRORS_INCLUDED
#define MVSN_FNP_RESPONSEGEN_ERRORS_INCLUDED

/* Error codes returned by fnpMargeGetLastError - KEEP IN ORDER */
enum fnpMargeErrorCode
{
    ERROR_NOT_SET = 0,                                  /*  0	An error code has not been set. */
    ERROR_INVALID_CONTEXT,                              /*  1	The hContext parameter is invalid. */
    ERROR_UNSUPPORTED_CLIENT_VERSION,                   /*  2	The client version is not supported. */
    ERROR_XML_MISSING_MANDATORY_ELEMENT,                /*  3	An XML mandatory element is missing. */
    ERROR_XML_BADLY_FORMED,                             /*  4	Some XML is badly formed. */
    ERROR_XML_INVALID,                                  /*  5	Some XML is invalid. */
    ERROR_HASH_UNSUPPORTED,                             /*  6	The hash version of the client request is unsupported. */
    ERROR_HASH_MISMATCH,                                /*	7	The hash in the client request is incorrect.  */
    ERROR_UNSUPPORTED_SIGNATURE,                        /*	8	The signature type in the client request is unsupported. */
    ERROR_BAD_PARAMETER,                                /*  9	An invalid parameter was received. */
    ERROR_PARAMETER_MISMATCH,                           /* 10	Two conflicting parameters were received. */
    ERROR_SHORT_CODE_SUPPLY_RESPONSE_VALUE_NOT_FOUND,   /* 11	The short code response expected a value that was not supplied. */
    ERROR_SHORT_CODE_ALIAS_NOT_SUPPORTED,               /* 12	The short code alias in the request code is not supported. */
    ERROR_UNEXPECTED,                                   /* 13	Any other error. */
    ERROR_NOT_AN_FNP_SHORT_CODE,                        /* 14	Not a valid short code. */
    ERROR_INVALID_SHORT_CODE_CHAR,                      /* 15	Invalid short code character. */
    ERROR_INVALID_REQUEST_CODE,							/* 16 */
    ERROR_WRONG_SHORT_CODE_ALIAS,						/* 17 */
    ERROR_INVALID_RESPONSE_CODE,						/* 18 */
    ERROR_INVALID_REQUEST_TYPE,							/* 19 */
    ERROR_UNKNOWN_SCHEME_ID,                            /* 20	Unknown short code scheme ID. */
    ERROR_SHORT_CODE_RESPONSE_TYPE,						/* 21	Response type not appropriate for request. */
    ERROR_EXPECTED_XML_PARAMETER_GOT_DATA,              /* 22	Non-XML data was passed in where XML data was expected. */
    ERROR_EXPECTED_DATA_PARAMETER_GOT_XML,              /* 23	XML data was passed in where non-XML data was expected. */
    ERROR_STD_LIBRARY_EXCEPTION,                        /* 24	A standard library exception was encountered */
	ERROR_INVALID_SHORTCODE_VENDOR_ITEM_VALUE,			/* 25   Invalid data supplied for short code vendor item */
	ERROR_MISSING_KEY_DATA,								/* 26   Key data for one or more Trusted Ids used by response not supplied */
	ERROR_CONFIG_CLIENT_SERVER,							/* 27   Sending server config to client or vice versa. */
	ERROR_INVALID_KEY_DATA,								/* 28   Key data supplied is invalid.  */
	ERROR_UMN_NOT_IN_REQUEST,							/* 29   UMN value needed by response not present in request.  */
	ERROR_REQUEST_ENTRY									/* 30   Request code entered incorrectly.  */
};

#endif
