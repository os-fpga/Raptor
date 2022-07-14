/***************************************************************************************************
 *
 * $Archive: /Eng/Common/Library/Security/Sdt/include/Sdt/SdtData.h $     
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
#ifndef __SDT_DATA_H__
#define __SDT_DATA_H__
#include "sdtconfig.h"
#include "sdtbase.h"

#ifdef __cplusplus
extern "C" 
{
#endif

typedef SDT_ENCODED_TYPE  SDT_DATA_LONG;
    
#define SDT_DATA_SELECT_FIXED 0xac5ca5a5

#define SDT_DATA_HASH_VALUE( Value, Key ) ( Value \
    ^ (((((Value * Key) ^ SDT_DATA_SELECT_FIXED) >> 1)  \
    * (((Value * Key) ^ SDT_DATA_SELECT_FIXED) << 2)  \
    + (((Value * Key) ^ SDT_DATA_SELECT_FIXED) >> 3))  \
    ^ (((((Value * Key) ^ SDT_DATA_SELECT_FIXED) << 4)  \
    * (((Value * Key) ^ SDT_DATA_SELECT_FIXED) >> 5)  \
    + (((Value * Key) ^ SDT_DATA_SELECT_FIXED) << 6))  \
    ^ ((((Value * Key) ^ SDT_DATA_SELECT_FIXED) >> 7)  \
    * (((Value * Key) ^ SDT_DATA_SELECT_FIXED) << 8)))) \
    )

#define SDT_DATA_KEY_0( Seed ) (SDT_CORE_TYPE)SDT_DATA_HASH_VALUE( (Seed ^ SDT_CONFIG_SEED_1),  SDT_CONFIG_SEED_0 )
#define SDT_DATA_KEY_1( Seed ) (SDT_CORE_TYPE)SDT_DATA_HASH_VALUE( (Seed ^ SDT_CONFIG_SEED_2),  SDT_CONFIG_SEED_1 )
#define SDT_DATA_KEY_2( Seed ) (SDT_CORE_TYPE)SDT_DATA_HASH_VALUE( (Seed ^ SDT_CONFIG_SEED_3),  SDT_CONFIG_SEED_2 )
#define SDT_DATA_KEY_3( Seed ) (SDT_CORE_TYPE)SDT_DATA_HASH_VALUE( (Seed ^ SDT_CONFIG_SEED_4),  SDT_CONFIG_SEED_3 )
#define SDT_DATA_KEY_4( Seed ) (SDT_CORE_TYPE)SDT_DATA_HASH_VALUE( (Seed ^ SDT_CONFIG_SEED_5),  SDT_CONFIG_SEED_4 )
#define SDT_DATA_KEY_5( Seed ) (SDT_CORE_TYPE)SDT_DATA_HASH_VALUE( (Seed ^ SDT_CONFIG_SEED_6),  SDT_CONFIG_SEED_5 )
#define SDT_DATA_KEY_6( Seed ) (SDT_CORE_TYPE)SDT_DATA_HASH_VALUE( (Seed ^ SDT_CONFIG_SEED_7),  SDT_CONFIG_SEED_6 )
#define SDT_DATA_KEY_7( Seed ) (SDT_CORE_TYPE)SDT_DATA_HASH_VALUE( (Seed ^ SDT_CONFIG_SEED_0),  SDT_CONFIG_SEED_7 )

#define SDT_DATA_SELECT_BITS    0x0f0f0f0f


#define SDT_DATA_DECODE( Seed, Value ) SDT_BASE_DECODE( \
    (SDT_ENCODED_TYPE)Seed, (SDT_ENCODED_TYPE)Value, (SDT_ENCODED_TYPE)SDT_DATA_SELECT_BITS, \
    SDT_DATA_KEY_0(Seed), SDT_DATA_KEY_1(Seed), SDT_DATA_KEY_2(Seed), SDT_DATA_KEY_3(Seed), \
    SDT_DATA_KEY_4(Seed), SDT_DATA_KEY_5(Seed), SDT_DATA_KEY_6(Seed), SDT_DATA_KEY_7(Seed)  )

#define SDT_DATA_ENCODE( Seed, Value ) SDT_BASE_ENCODE( \
    (SDT_ENCODED_TYPE)Seed, (SDT_ENCODED_TYPE)Value, (SDT_ENCODED_TYPE)SDT_DATA_SELECT_BITS, \
    SDT_DATA_KEY_0(Seed), SDT_DATA_KEY_1(Seed), SDT_DATA_KEY_2(Seed), SDT_DATA_KEY_3(Seed), \
    SDT_DATA_KEY_4(Seed), SDT_DATA_KEY_5(Seed), SDT_DATA_KEY_6(Seed), SDT_DATA_KEY_7(Seed)  )

#define SDT_DATA_ENCODE_LITE( Seed, Value ) (SDT_ENCODED_TYPE)SDT_BASE_ENCODE_LITE( \
    (SDT_ENCODED_TYPE)Seed, (SDT_ENCODED_TYPE)Value, (SDT_ENCODED_TYPE)SDT_DATA_SELECT_BITS, \
    SDT_DATA_KEY_0(Seed), SDT_DATA_KEY_1(Seed)  )


#define SDT_DATA_DECODE_RUNTIME( pEncoder, Value ) SDT_BASE_DECODE( \
    (SDT_ENCODED_TYPE)pEncoder->Seed, (SDT_ENCODED_TYPE)Value, (SDT_ENCODED_TYPE)pEncoder->SelectBits, \
    pEncoder->Keys[0], pEncoder->Keys[1], \
    pEncoder->Keys[2], pEncoder->Keys[3], \
    pEncoder->Keys[4], pEncoder->Keys[5], \
    pEncoder->Keys[6], pEncoder->Keys[7] )

#define SDT_DATA_ENCODE_RUNTIME( pEncoder, Value ) SDT_BASE_ENCODE( \
    (SDT_ENCODED_TYPE)pEncoder->Seed, (SDT_ENCODED_TYPE)Value, (SDT_ENCODED_TYPE)pEncoder->SelectBits, \
    pEncoder->Keys[0], pEncoder->Keys[1], \
    pEncoder->Keys[2], pEncoder->Keys[3], \
    pEncoder->Keys[4], pEncoder->Keys[5], \
    pEncoder->Keys[6], pEncoder->Keys[7] )

#define SDT_DATA_DECODE_LITE_RUNTIME( pEncoder, Value ) SDT_BASE_DECODE_LITE( \
    (SDT_ENCODED_TYPE)pEncoder->Seed, (SDT_ENCODED_TYPE)Value, (SDT_ENCODED_TYPE)pEncoder->SelectBits, \
    pEncoder->Keys[0], pEncoder->Keys[1] )

typedef SDT_BASE_ENCODER  SDT_DATA_ENCODER;

#define SDT_DATA_ENCODER_LIMIT  SDT_ENCODER_USER_LIMIT
#define SDT_DATA_ENCODER_BLOCK_SIZE 64

extern SDT_DATA_ENCODER   SDT_DATA_EncoderArray[SDT_DATA_ENCODER_LIMIT];

typedef struct 
{
    SDT_DATA_ENCODER*   pEncoder;
    SDT_DATA_LONG       EncodedValue;
}
SDT_DATA_VALUE;


SDT_DATA_LONG   SDT_DATA_Operation(
    SDT_DATA_VALUE*   pData,
    SDT_DATA_LONG     Format,
    SDT_DATA_LONG     Type,
    SDT_DATA_LONG     Operation,
    SDT_DATA_LONG     Value,
    SDT_DATA_LONG     Parameter,
    SDT_DATA_LONG     GroupId
);

#define SDT_DATA_USER_ALGORITHM 0

#define SDT_DATA_DECLARE_DATA( Seed, Data ) \
    enum { SDT_DATA_##Data##_SEED = Seed  }; \
    extern  SDT_DATA_VALUE   Data

#define SDT_DATA_DEFINE_DATA( Seed, Data ) \
    enum { SDT_DATA_##Data##_SEED = Seed  }; \
    SDT_DATA_VALUE   Data = { &SDT_DATA_EncoderArray[Seed], 0 }

#define SDT_DATA_OPERATION( Data, Operation, Value, Parameter )  SDT_DATA_Operation(  \
    &Data, SDT_DATA_ENCODE_LITE( SDT_DATA_##Data##_SEED, SDT_BASE_FORMAT_1 ), \
    SDT_DATA_ENCODE_LITE( SDT_DATA_##Data##_SEED, SDT_DATA_TYPE_ID ), \
    SDT_DATA_ENCODE_LITE( SDT_DATA_##Data##_SEED, Operation ), \
    Value, Parameter, SDT_DATA_ENCODE_LITE( SDT_DATA_##Data##_SEED, 0 ) )

#define SDT_REJECT_NON_CONST( Literal ) { typedef char SDT_TEMP_1[ ( Literal > 0 ) ? 1 : 2 ]; int SDT_TEMP_2 = sizeof( SDT_TEMP_1 ); ++SDT_TEMP_2; } 

#define SDT_DATA_GET( Data )               SDT_DATA_OPERATION( Data, SDT_BASE_OPERATION_GET, Data.EncodedValue, Data.EncodedValue )
#define SDT_DATA_SET( Data, Value )        { Data.EncodedValue = SDT_DATA_OPERATION( Data, SDT_BASE_OPERATION_SET, (Value), (Value) ); }
#define SDT_DATA_SET_CONST( Data, Const )  { SDT_REJECT_NON_CONST( Const ); Data.EncodedValue = 0; Data.EncodedValue = SDT_DATA_ENCODE( SDT_DATA_##Data##_SEED, (Const) ); }

#define SDT_DATA_OPERATE( Operation, Data1, Data2 )  \
{ \
    SDT_DATA_LONG   SDT_PlainData2 = SDT_DATA_GET( Data2 ); \
    SDT_DATA_LONG   SDT_RecodedData2 = SDT_DATA_OPERATION( Data1, SDT_BASE_OPERATION_SET, SDT_PlainData2, SDT_PlainData2 ); \
    Data1.EncodedValue = SDT_DATA_OPERATION( Data1, Operation, Data1.EncodedValue, SDT_RecodedData2 ); \
}


#define SDT_DATA_ADD( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_ADD, Data1, Data2 )
#define SDT_DATA_SUB( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_SUB, Data1, Data2 )
#define SDT_DATA_MUL( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_MUL, Data1, Data2 )
#define SDT_DATA_DIV( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_DIV, Data1, Data2 )
#define SDT_DATA_MOD( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_MOD, Data1, Data2 )
#define SDT_DATA_AND( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_AND, Data1, Data2 )
#define SDT_DATA_OR(  Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_OR,  Data1, Data2 )
#define SDT_DATA_XOR( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_XOR, Data1, Data2 )
#define SDT_DATA_SHR( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_SHR, Data1, Data2 )
#define SDT_DATA_SHL( Data1, Data2 ) SDT_DATA_OPERATE( SDT_BASE_OPERATION_SHL, Data1, Data2 )

#define SDT_DATA_LT(  Data1, Data2 ) ( SDT_DATA_GET( Data1 ) < SDT_DATA_GET( Data2 ) )
#define SDT_DATA_LTE( Data1, Data2 ) ( SDT_DATA_GET( Data1 ) <= SDT_DATA_GET( Data2 ) )
#define SDT_DATA_EQ(  Data1, Data2 ) ( SDT_DATA_GET( Data1 ) == SDT_DATA_GET( Data2 ) )
#define SDT_DATA_GTE( Data1, Data2 ) ( SDT_DATA_GET( Data1 ) >= SDT_DATA_GET( Data2 ) )
#define SDT_DATA_GT(  Data1, Data2 ) ( SDT_DATA_GET( Data1 ) > SDT_DATA_GET( Data2 ) )



#ifdef __cplusplus
} // extern "C"
#endif

#endif // __SDT_DATA_VALUE_H__
