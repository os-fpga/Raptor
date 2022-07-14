/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
#ifndef INCLUDE_LMSIGN_H
#define INCLUDE_LMSIGN_H

/****************************************************************************/
/**	@brief	Sign specified license using specified hostid.
 *
 *	@param	pszHostId	Hostid to sign feature/increment lines with
 *	@param	pszInput	FEATURE/INCREMENT line(s) to sign
 *	@param	pszOutput	Pointer to buffer that will receive signed output
 *	@param	pOutSize	Size of output buffer, if 0 will return size needed
 *						in bytes (including null) to hold output.
 *	@param	pszError	Pointer to buffer that will receive error message if an
 *						an error occurs.
 *	@param	pErrorSize	Size of error buffer, if 0 and an error occurs will
 *						return size needed in bytes (including null) to hold
 *						error message.
 *
 *	@return	LM_NOERROR on success else a FLEX error code, same as lc_cryptstr()
 *	@note	If size of output buffer is smaller than generated output, data
 *			will be truncated (and null terminated).
 ****************************************************************************/
int
l_flexSignLicense(
	char *			pszHostId,
	char *			pszInput,
	char *			pszOutput,
	size_t *		pOutSize,
	char *			pszError,
	size_t *		pErrorSize);

#endif /* INCLUDE_SIGN_H */
