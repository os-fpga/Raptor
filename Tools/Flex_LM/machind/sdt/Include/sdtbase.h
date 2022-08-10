/***************************************************************************************************
 *
 * $Archive: /Eng/Common/Library/Security/Sdt/include/Sdt/SdtBase.h $     
 *
 * $Revision: #1 $
 *
 * Description:         
 *      The base definitions of the Secure Data Type implementation - definitions in
 *  this file must NOT be altered or backward compatilbity will be broken. New
 *  definitons for algorithms and new operators may be added, but never removed.
 *
 **************************************************************************************************/
/**************************************************************************************************
* Copyright (c) 1997-2016, 2018-2020 Flexera. All Rights Reserved.
**************************************************************************************************/
/**************************************************************************************************
 *
 * $NoKeywords: $
 *
 **************************************************************************************************/
#ifndef __SDT_BASE_H__
#define __SDT_BASE_H__

#include "sdtplatform.h"
#include <stddef.h>

#define SDT_BASE_VERSION 2

#ifdef __cplusplus
extern "C" 
{
#endif

#define SDT_BASE_ROOT_HASH_LO( V, K0, K1, K2, K3 ) ( V ^ (K0 * (V + K1)) ^ (K2 * (V + K3)) )

#define SDT_BASE_ROOT_HASH_HI( V, K0, K1, K2, K3 ) ( V ^ (K1 * (V + K0)) ^ (K3 * (V + K2)) )

#define SDT_BASE_ROOT_HASH_LITE( V, K0, K1 ) ( V ^ ((V + K1) * (V + K0)) )

#define SDT_BASE_HI( V, B ) (V & B)

#define SDT_BASE_LO( V, B ) (V & ~B)

#define SDT_BASE_ROUND_LO( V, B, K0, K1, K2, K3 ) ( (V) ^ SDT_BASE_HI( SDT_BASE_ROOT_HASH_LO( SDT_BASE_LO(V, B), (K0), (K1), (K2), (K3) ), B ) )

#define SDT_BASE_ROUND_HI( V, B, K0, K1, K2, K3 )  ( (V) ^ SDT_BASE_LO( SDT_BASE_ROOT_HASH_HI( SDT_BASE_HI(V, B), (K0), (K1), (K2), (K3) ), B ) )

#define SDT_BASE_ENCODE( S, V, B, K0, K1, K2, K3, K4, K5, K6, K7 ) SDT_BASE_ROUND_HI( SDT_BASE_ROUND_LO( V, B, K0, K1, K2, K3 ), B, K4, K5, K6, K7 ) 

#define SDT_BASE_DECODE( S, V, B, K0, K1, K2, K3, K4, K5, K6, K7 ) SDT_BASE_ROUND_LO( SDT_BASE_ROUND_HI( V, B, K4, K5, K6, K7 ), B, K0, K1, K2, K3 ) 

#define SDT_BASE_ENCODE_LITE( S, V, B, K0, K1 ) ( (V) ^ SDT_BASE_HI( SDT_BASE_ROOT_HASH_LITE( SDT_BASE_LO(V, B), (K0), (K1) ), B ) )

#define SDT_BASE_DECODE_LITE( S, V, B, K0, K1 ) ( (V) ^ SDT_BASE_HI( SDT_BASE_ROOT_HASH_LITE( SDT_BASE_LO(V, B), (K0), (K1) ), B ) )


enum { 
    SDT_BASE_KEY_COUNT = 8,
    SDT_BASE_KEY_LIMIT = 8
};

enum
{
    SDT_BASE_HANDLER_MESSAGE_32    = 0,
    SDT_BASE_HANDLER_MESSAGE_64 = 1,
    SDT_BASE_HANDLER_LIMIT      = 16
};

enum
{
    SDT_BASE_DATA_ENCODER   = 1,
    SDT_BASE_DATA_LIMIT     = 16
};

enum { SDT_BASE_ALIAS_ID_LIMIT = 16 };

enum { SDT_BASE_VALUE_LIMIT = 16 };

#if defined(_MSC_VER)
#pragma pack( push, 4 )
#endif

typedef struct 
{
    SDT_BASE_UINT32     Start;
    SDT_BASE_UINT32     Size;
    SDT_BASE_UINT32     Version;
    SDT_BASE_UINT32     Seed;
    SDT_BASE_UINT32     AlgorithmId;
    SDT_BASE_UINT32     SelectBits;
    SDT_BASE_UINT32     Keys[SDT_BASE_KEY_LIMIT];
    SDT_BASE_UINT64     pHandlers[SDT_BASE_HANDLER_LIMIT];
    SDT_BASE_UINT64     pData[SDT_BASE_DATA_LIMIT];
    SDT_BASE_UINT32     Values[SDT_BASE_VALUE_LIMIT];
    SDT_BASE_UINT64     Rva;
    SDT_BASE_UINT32     Index;
    SDT_BASE_UINT32     End;
}
SDT_BASE_ENCODER;

#if defined(_MSC_VER)
#pragma pack( pop )
#endif

enum { SDT_BASE_ENCODER_START  = 0x57231342 };
enum { SDT_BASE_ENCODER_END    = 0x68822409 };

/* And their endian-swapped equivalents. */
enum { SDT_BASE_ENCODER_START_OTHER_ENDIAN  = 0x42132357 };
enum { SDT_BASE_ENCODER_END_OTHER_ENDIAN    = 0x09248268 };

#define SDT_BASE_DECODER_SETUP( Seed, AlgorithmId, SelectBits, Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7 )  \
{ \
    SDT_BASE_ENCODER_START, \
    sizeof( SDT_BASE_ENCODER ), \
    SDT_BASE_VERSION,\
    Seed, \
    AlgorithmId, \
    SelectBits, \
    { Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7 }, \
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, \
    0,\
    0,\
    SDT_BASE_ENCODER_END \
}

enum { SDT_BASE_OPERATION_NONE = 0 };
enum { SDT_BASE_OPERATION_SET = 1 };
enum { SDT_BASE_OPERATION_ADD = 2 };
enum { SDT_BASE_OPERATION_SUB = 3 };
enum { SDT_BASE_OPERATION_MUL = 4 };
enum { SDT_BASE_OPERATION_DIV = 5 };
enum { SDT_BASE_OPERATION_MOD = 6 };
enum { SDT_BASE_OPERATION_AND = 7 };
enum { SDT_BASE_OPERATION_OR  = 8 };
enum { SDT_BASE_OPERATION_XOR = 9 };
enum { SDT_BASE_OPERATION_SHR = 10 };
enum { SDT_BASE_OPERATION_SHL = 11 };
enum { SDT_BASE_OPERATION_GET = 13 };
enum { SDT_BASE_OPERATION_LT  = 14 };
enum { SDT_BASE_OPERATION_LTE = 15 };
enum { SDT_BASE_OPERATION_EQ  = 16 };
enum { SDT_BASE_OPERATION_GTE = 17 };
enum { SDT_BASE_OPERATION_GT  = 18 };
enum { SDT_BASE_OPERATION_CON = 19 };
enum { SDT_BASE_OPERATION_MOV = 20 };
enum { SDT_BASE_OPERATION_NOA = 21 };
enum { SDT_BASE_OPERATION_NOS = 22 };


enum { SDT_BASE_TYPE_ID_NONE = 0 };
enum { SDT_BASE_TYPE_ID_UINT8 = 2 };
enum { SDT_BASE_TYPE_ID_INT8 = 3 };
enum { SDT_BASE_TYPE_ID_UINT16 = 4 };
enum { SDT_BASE_TYPE_ID_INT16 = 5 };
enum { SDT_BASE_TYPE_ID_UINT32 = 8 };
enum { SDT_BASE_TYPE_ID_INT32 = 9 };
#if defined(SDT_64_BIT_SUPPORTED)
enum { SDT_BASE_TYPE_ID_UINT64 = 16 };
enum { SDT_BASE_TYPE_ID_INT64 = 17 };
#endif

enum { SDT_BASE_FORMAT_NONE  = 0 };
enum { SDT_BASE_FORMAT_1     = 1 };
enum { SDT_BASE_FORMAT_2     = 2 };

enum { SDT_BASE_NOTIFY_NULL        = 0x00000000 };
enum { SDT_BASE_NOTIFY_BREAK       = 0x00000001 };
enum { SDT_BASE_NOTIFY_DLB_INIT    = 0x00000002 };
enum { SDT_BASE_NOTIFY_DLB_TEST    = 0x00000003 };
enum { SDT_BASE_NOTIFY_DLB_DROP    = 0x00000004 };
enum { SDT_BASE_NOTIFY_DLB_DISABLE	= 0x00000005 };
enum { SDT_BASE_NOTIFY_DLB_ENABLE	= 0x00000006 };

enum { SDT_BASE_ITEM_SEED       = 0 };
enum { SDT_BASE_ITEM_FORMAT     = 1 };
enum { SDT_BASE_ITEM_TYPE       = 2 };
enum { SDT_BASE_ITEM_OPERATOR   = 3 };
enum { SDT_BASE_ITEM_VALUE      = 4 };
enum { SDT_BASE_ITEM_PARAMETER  = 5 };
enum { SDT_BASE_ITEM_LIMIT      = 8 };

typedef struct
{
    SDT_BASE_UINT32     Item[ SDT_BASE_ITEM_LIMIT ];
}
SDT_BASE_ITEM_SET_32;


#if defined(SDT_64_BIT_SUPPORTED)

typedef struct
{
    SDT_BASE_UINT64     Item[ SDT_BASE_ITEM_LIMIT ];
}
SDT_BASE_ITEM_SET_64;

#define SDT_BASE_ITEM_SET SDT_BASE_ITEM_SET_64
enum { SDT_BASE_HANDLER_MESSAGE = SDT_BASE_HANDLER_MESSAGE_64 };

#else 

#define SDT_BASE_ITEM_SET SDT_BASE_ITEM_SET_32
enum { SDT_BASE_HANDLER_MESSAGE = SDT_BASE_HANDLER_MESSAGE_32 };

#endif /* defined(SDT_64_BIT_SUPPORTED) */

#define SDT_BASE_ITEM_SET_ENCODE( ItemSet, Key ) \
    ItemSet.Item[1] ^= (Key * Key); \
    ItemSet.Item[2] ^= (Key * ItemSet.Item[1]); \
    ItemSet.Item[3] ^= (Key * ItemSet.Item[2]); \
    ItemSet.Item[4] ^= (Key * ItemSet.Item[3]); \
    ItemSet.Item[5] ^= (Key * ItemSet.Item[4]); \
    ItemSet.Item[6] ^= (Key * ItemSet.Item[5]); \
    ItemSet.Item[7] ^= (Key * ItemSet.Item[6]);

#define SDT_BASE_ITEM_SET_DECODE( ItemSet, Key ) \
    ItemSet.Item[7] ^= (Key * ItemSet.Item[6]); \
    ItemSet.Item[6] ^= (Key * ItemSet.Item[5]); \
    ItemSet.Item[5] ^= (Key * ItemSet.Item[4]); \
    ItemSet.Item[4] ^= (Key * ItemSet.Item[3]); \
    ItemSet.Item[3] ^= (Key * ItemSet.Item[2]); \
    ItemSet.Item[2] ^= (Key * ItemSet.Item[1]); \
    ItemSet.Item[1] ^= (Key * Key); 


typedef SDT_BASE_INT32(*SDT_BASE_MESSAGE_HANDLER32)(
    SDT_BASE_UINT64     pData,
    SDT_BASE_UINT64     pItemSet   
    );

#if defined(SDT_64_BIT_SUPPORTED)

typedef SDT_BASE_INT64(*SDT_BASE_MESSAGE_HANDLER64)(
    SDT_BASE_UINT64     pData,
    SDT_BASE_UINT64     pItemSet   
    );

typedef SDT_BASE_MESSAGE_HANDLER64 SDT_BASE_MESSAGE_HANDLER;

#else

typedef SDT_BASE_MESSAGE_HANDLER32 SDT_BASE_MESSAGE_HANDLER;

#endif

#define SDT_ENCODER_BLOCK_SIZE 64

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __SDT_BASE_BASE_H__
