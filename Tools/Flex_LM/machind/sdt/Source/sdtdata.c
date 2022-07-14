/***************************************************************************************************
 *
 * $Archive: /Eng/Common/Library/Security/Sdt/source/SdtData.c $ 	
 *
 * $Revision: #1 $
 *
 * Description:	 	
 *		
 *************************************************************************************************/
/**************************************************************************************************
* Copyright (c) 1997-2016, 2018-2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/***************************************************************************************************
 *
 * $NoKeywords: $
 *
 **************************************************************************************************/
#include "fnpstdtypes.h"
#include "sdt/Include/sdtmarker.h"
#include "sdt/Include/sdtdata.h"

#ifdef __cplusplus
#error This file should be compiled as a "C" file
#endif

void SDT_STATIC_ASSERT_LIMITS()
{
     /* Max Encoder Limit Compile-time Assertion. */
     SDT_STATIC_ASSERT( SDT_ENCODER_USER_LIMIT <= 1024 );
 
     /* Max Encoder Limit is a multiplier of SDT_DATA_ENCODER_BLOCK_SIZE. */
     SDT_STATIC_ASSERT( (SDT_DATA_ENCODER_LIMIT % SDT_DATA_ENCODER_BLOCK_SIZE) == 0 );
}


#define SDT_DATA_DEFINE_ENCODER( Seed )  SDT_BASE_DECODER_SETUP( \
    Seed, \
    SDT_DATA_USER_ALGORITHM, \
    SDT_DATA_SELECT_BITS, \
    SDT_DATA_KEY_0(Seed), SDT_DATA_KEY_1(Seed), \
    SDT_DATA_KEY_2(Seed), SDT_DATA_KEY_3(Seed), \
    SDT_DATA_KEY_4(Seed), SDT_DATA_KEY_5(Seed), \
    SDT_DATA_KEY_6(Seed), SDT_DATA_KEY_7(Seed) \
)

#define SDT_DATA_DEFINE_ENCODER_BLOCK( Base )\
    SDT_DATA_DEFINE_ENCODER( ((Base) + 0 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 1 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 2 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 3 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 4 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 5 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 6 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 7 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 8 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 9 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 10 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 11 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 12 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 13 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 14 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 15 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 16 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 17 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 18 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 19 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 20 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 21 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 22 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 23 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 24 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 25 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 26 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 27 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 28 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 29 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 30 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 31 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 32 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 33 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 34 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 35 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 36 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 37 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 38 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 39 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 40 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 41 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 42 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 43 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 44 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 45 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 46 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 47 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 48 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 49 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 50 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 51 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 52 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 53 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 54 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 55 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 56 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 57 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 58 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 59 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 60 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 61 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 62 )), \
    SDT_DATA_DEFINE_ENCODER( ((Base) + 63 ))

/* Default set of encoders used for key based encoding of data values */
/* These must have global scope to be referenced throughout program   */

SDT_DATA_ENCODER   SDT_DATA_EncoderArray[SDT_DATA_ENCODER_LIMIT] = {
    SDT_DATA_DEFINE_ENCODER_BLOCK( 0 ),

#if SDT_ENCODER_USER_LIMIT > SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 2 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 2 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 3 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 3 * SDT_DATA_ENCODER_BLOCK_SIZE ),
#endif

#if SDT_ENCODER_USER_LIMIT > 4 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 4 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 5 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 5 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 6 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 6 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 7 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 7 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 8 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 8 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 9 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 9 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 10 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 10 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 11 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 11 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 12 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 12 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 13 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 13 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 14 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 14 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif

#if SDT_ENCODER_USER_LIMIT > 15 * SDT_DATA_ENCODER_BLOCK_SIZE
    SDT_DATA_DEFINE_ENCODER_BLOCK( 15 * SDT_DATA_ENCODER_BLOCK_SIZE ), 
#endif
};

SDT_DATA_LONG   SDT_DATA_Encode(
    SDT_DATA_ENCODER*   pEncoder,
    SDT_DATA_LONG       Value
)
{
    SDT_DATA_LONG   Result = 0;

#if SDT_DISABLE_UNPREPPED
    SDT_PLATFORM_BREAKPOINT();
#else
    SDT_MARKER_DEFINE_START( SDT_MARKER_ERASE );

    Result = SDT_DATA_ENCODE_RUNTIME( pEncoder, Value );

    SDT_MARKER_DEFINE_END();
#endif

    return Result;
}


SDT_DATA_LONG   SDT_DATA_Decode(
    SDT_DATA_ENCODER*   pEncoder,
    SDT_DATA_LONG       Value
)
{
    SDT_DATA_LONG   Result = 0;

#if SDT_DISABLE_UNPREPPED
    SDT_PLATFORM_BREAKPOINT();
#else
    SDT_MARKER_DEFINE_START( SDT_MARKER_ERASE );

    Result = SDT_DATA_DECODE_RUNTIME( pEncoder, Value );

    SDT_MARKER_DEFINE_END();
#endif

    return Result;
}


SDT_DATA_LONG   SDT_DATA_DecodeLite(
    SDT_DATA_ENCODER*   pEncoder,
    SDT_DATA_LONG       Value
)
{
    SDT_DATA_LONG   Result = 0;

#if SDT_DISABLE_UNPREPPED
    SDT_PLATFORM_BREAKPOINT();
#else
    SDT_MARKER_DEFINE_START( SDT_MARKER_ERASE );

    Result = SDT_DATA_DECODE_LITE_RUNTIME( pEncoder, Value );

    SDT_MARKER_DEFINE_END();
#endif

    return Result;
}




SDT_DATA_LONG   SDT_DATA_Operation(
    SDT_DATA_VALUE*   pData,
    SDT_DATA_LONG     Format1,
    SDT_DATA_LONG     Type,
    SDT_DATA_LONG     Operation,
    SDT_DATA_LONG     Value,
    SDT_DATA_LONG     Parameter,
    SDT_DATA_LONG     EncodedZero
)
{
    SDT_DATA_LONG       Result = 0;

    if ( 0 != pData->pEncoder->pHandlers[SDT_BASE_HANDLER_MESSAGE] )
    {
        SDT_BASE_MESSAGE_HANDLER  pOperation = (SDT_BASE_MESSAGE_HANDLER)(intptr_t)( pData->pEncoder->pHandlers[SDT_BASE_HANDLER_MESSAGE] );

        SDT_BASE_ITEM_SET   ParameterSet;

        ParameterSet.Item[SDT_BASE_ITEM_SEED] = 0;
        ParameterSet.Item[SDT_BASE_ITEM_FORMAT] = Format1;
        ParameterSet.Item[SDT_BASE_ITEM_TYPE] = Type;
        ParameterSet.Item[SDT_BASE_ITEM_OPERATOR] = Operation;
        ParameterSet.Item[SDT_BASE_ITEM_VALUE] = Value;
        ParameterSet.Item[SDT_BASE_ITEM_PARAMETER] = Parameter;

        SDT_BASE_ITEM_SET_ENCODE( ParameterSet, EncodedZero );

        Result = (SDT_ENCODED_TYPE)pOperation(
            pData->pEncoder->pData[SDT_BASE_DATA_ENCODER],
            (SDT_BASE_UINT64)(intptr_t)(&ParameterSet)
            );
    }
    else
    {
#if SDT_DISABLE_UNPREPPED
        SDT_PLATFORM_BREAKPOINT();
#else
        SDT_MARKER_DEFINE_START( SDT_MARKER_ERASE );

        {
            SDT_DATA_LONG       PlainOperation = SDT_DATA_DecodeLite( pData->pEncoder, Operation );
            SDT_DATA_LONG       PlainValue = SDT_DATA_Decode( pData->pEncoder, Value  );
            SDT_DATA_LONG       PlainParameter = SDT_DATA_Decode( pData->pEncoder, Parameter );
            SDT_DATA_LONG       PlainResult = 0; 

            switch ( PlainOperation )
            {
                case SDT_BASE_OPERATION_SET : PlainResult = Parameter; break;
                case SDT_BASE_OPERATION_GET : PlainResult = PlainValue; break;
                case SDT_BASE_OPERATION_ADD : PlainResult = PlainValue + PlainParameter; break;
                case SDT_BASE_OPERATION_SUB : PlainResult = PlainValue - PlainParameter; break;
                case SDT_BASE_OPERATION_MUL : PlainResult = PlainValue * PlainParameter; break;
                case SDT_BASE_OPERATION_DIV : PlainResult = PlainValue / PlainParameter; break;
                case SDT_BASE_OPERATION_MOD : PlainResult = PlainValue % PlainParameter; break;
                case SDT_BASE_OPERATION_AND : PlainResult = PlainValue & PlainParameter; break;
                case SDT_BASE_OPERATION_OR  : PlainResult = PlainValue | PlainParameter; break;
                case SDT_BASE_OPERATION_XOR : PlainResult = PlainValue ^ PlainParameter; break;
                case SDT_BASE_OPERATION_SHR : PlainResult = PlainValue >> PlainParameter; break;
                case SDT_BASE_OPERATION_SHL : PlainResult = PlainValue << PlainParameter; break;
                case SDT_BASE_OPERATION_LT  : PlainResult = PlainValue < PlainParameter; break;
                case SDT_BASE_OPERATION_LTE : PlainResult = PlainValue <= PlainParameter; break;
                case SDT_BASE_OPERATION_EQ  : PlainResult = PlainValue == PlainParameter; break;
                case SDT_BASE_OPERATION_GTE : PlainResult = PlainValue >= PlainParameter; break;
                case SDT_BASE_OPERATION_GT  : PlainResult = PlainValue > PlainParameter; break;
                default : break;
            }
            if ( PlainOperation == SDT_BASE_OPERATION_GET )
            {
                Result = PlainResult;
            }
            else
            {
                Result = SDT_DATA_Encode( pData->pEncoder, PlainResult );
            }
        }
        SDT_MARKER_DEFINE_END();
#endif
    }

    return Result;
}


/* #pragma optimize( "", off ) */


