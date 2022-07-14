/***************************************************************************************************
 *
 * $Archive: /Eng/Common/Library/Security/Sdt/source/SdtType.cpp $ 	
 *
 * $Revision: #1 $
 *
 * Description:	 	
 *		
 **************************************************************************************************/
/**************************************************************************************************
* Copyright (c) 1997-2016, 2018-2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/***************************************************************************************************
 *
 * $NoKeywords: $
 *
 **************************************************************************************************/

#include "fnpstdtypes.h"
#include "sdt/Include/sdtconfig.h"
#include "sdt/Include/sdtmarker.h"
#include "sdt/Include/sdttype.h"
#include "sdt/Include/sdtfeature.h"

/******************************************************************************
 * Metrowerks CodeWarrior:	prevent linker dead-strip of SDT feature markers.
 ******************************************************************************/

#if defined(__MWERKS__)

#pragma force_active on

void FeatureReference( )
{
	extern struct _FeatureData	SDT_FeatureData [];

	if ( SDT_FeatureData[0].Signature[0] == 0xAA )	// Will never happen
		SDT_FeatureData[0].Signature[0]++;
}

#endif

// DO NOT EDIT THIS CODE
//
#define SDT_TYPE_DECODER_SETUP( Seed )  \
{ \
    SDT_BASE_ENCODER_START, \
    sizeof( SDT_BASE_ENCODER ), \
    SDT_BASE_VERSION,\
    Sdt::CKeyData< Seed >::SEED, \
    Sdt::CKeyData< Seed >::ALGORITHM, \
    Sdt::CKeyData< Seed >::SELECT, \
    {   (SDT_CORE_TYPE)Sdt::CKeyData< Seed >::KEY_0, (SDT_CORE_TYPE)Sdt::CKeyData< Seed >::KEY_1, \
        (SDT_CORE_TYPE)Sdt::CKeyData< Seed >::KEY_2, (SDT_CORE_TYPE)Sdt::CKeyData< Seed >::KEY_3, \
        (SDT_CORE_TYPE)Sdt::CKeyData< Seed >::KEY_4, (SDT_CORE_TYPE)Sdt::CKeyData< Seed >::KEY_5, \
        (SDT_CORE_TYPE)Sdt::CKeyData< Seed >::KEY_6, (SDT_CORE_TYPE)Sdt::CKeyData< Seed >::KEY_7  }, \
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
    0,\
    0,\
    SDT_BASE_ENCODER_END \
}

#define SDT_TYPE_DECODER_SETUP_BLOCK( Base )\
SDT_TYPE_DECODER_SETUP( (Base) + 0 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 1 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 2 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 3 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 4 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 5 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 6 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 7 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 8 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 9 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 10 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 11 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 12 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 13 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 14 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 15 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 16 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 17 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 18 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 19 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 20 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 21 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 22 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 23 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 24 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 25 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 26 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 27 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 28 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 29 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 30 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 31 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 32 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 33 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 34 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 35 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 36 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 37 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 38 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 39 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 40 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 41 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 42 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 43 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 44 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 45 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 46 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 47 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 48 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 49 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 50 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 51 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 52 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 53 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 54 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 55 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 56 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 57 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 58 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 59 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 60 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 61 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 62 ), \
SDT_TYPE_DECODER_SETUP( (Base) + 63 )

namespace
{
    inline void DummyFunc()
    {
        SDT_STATIC_ASSERT( SDT_ENCODER_USER_LIMIT <= 1024 );
    }
}

// Default set of encoders used for key based encoding of data values
// These must have global scope to be referenced throughout program okay
//
Sdt::SDT_TYPE_ENCODER    Sdt::CEncoderSet::EncoderSet[ Sdt::CEncoderSet::SDT_ENCODER_LIMIT ]  = {
    SDT_TYPE_DECODER_SETUP_BLOCK( 0 ),

#if SDT_ENCODER_USER_LIMIT > SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 2 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 2 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 3 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 3 * SDT_ENCODER_BLOCK_SIZE ),
#endif

#if SDT_ENCODER_USER_LIMIT > 4 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 4 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 5 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 5 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 6 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 6 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 7 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 7 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 8 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 8 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 9 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 9 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 10 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 10 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 11 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 11 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 12 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 12 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 13 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 13 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 14 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 14 * SDT_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 15 * SDT_ENCODER_BLOCK_SIZE
    SDT_TYPE_DECODER_SETUP_BLOCK( 15 * SDT_ENCODER_BLOCK_SIZE ), 
#endif
};
