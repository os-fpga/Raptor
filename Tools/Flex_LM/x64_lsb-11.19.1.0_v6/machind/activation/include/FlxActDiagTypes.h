/**************************************************************************************************
* Copyright (c) 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
#if !defined( FLXACTDIAGTYPES_H_INCLUDED )
#define FLXACTDIAGTYPES_H_INCLUDED 

/*
 *	Declarations for diagnostic functions.
 */

/**************************************************************************************************
 *	Trusted Storage Health Check - check item ids
 *************************************************************************************************/
typedef enum flxTsHcItems
{
	flxTsHcEventLog			= 0,	/* Able to write to the event log	*/
	flxTsHcLicensingService	= 1,	/* Able to obtain the licensing service version - i.e. it is correctly installed and running	*/
	flxTsHcAnchoring		= 2,	/* Able to write and read anchors of each type supported on this platform						*/
	flxTsHcBinding			= 3,	/* Able to read each binding entity supported on this platform									*/
	flxTsHcFile				= 4,	/* Able to create (or, if it exists, open) the trusted storage file defined by this prep		*/
	flxTsHcSection			= 5,	/* Able to process prepped trusted configuration and confirm trusted section exists; 			*/
	
	flxTsHcCount			= 6		/* Number of checks */
} FlxTsHcItems;

#endif	// Include guard
