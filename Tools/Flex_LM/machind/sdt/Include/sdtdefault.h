/***************************************************************************************************
 *
 * $Archive: /Eng/Common/Library/Security/Sdt/include/Sdt/SdtDefault.h $     
 *
 * $Revision: #1 $
 *
 * Description:     Set of classes involved with default (unprepped) behaviour of SDT
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
#ifndef __SDT_DEFAULT_H__
#define __SDT_DEFAULT_H__

#include "sdtmarker.h"
#include "sdtbase.h"
#include <limits>

// Todo: Change the itemset key-pair to be the same size as the encoded type

namespace Sdt
{

typedef SDT_CORE_TYPE CoreType;
typedef SDT_ENCODED_TYPE WordType;
typedef SDT_ENCODED_TYPE EncodedType;

struct CDefaultPlugins 
{
    static WordType    InvokeOperatorPlugin(
        const WordType     TypeId,
        const WordType     OperationId,
        const WordType     Value,
        const WordType     Parameter,
        const WordType     ResultIn
    )
    {
        WordType    Result( TypeId ^ OperationId ^ Value ^ Parameter );

        Result = ResultIn;

        return Result;
    }
};

#define SDT_DEFAULT_SELECT_FIXED_X 0xac5ca5a5 
#define SDT_HASH0( Value ) ( ( ( Value ) ^ SDT_DEFAULT_SELECT_FIXED_X) >> 1)
#define SDT_HASH1( Value ) ( SDT_HASH0( Value ) + ( ( ( Value ) ^ SDT_DEFAULT_SELECT_FIXED_X) << 2) )  
#define SDT_HASH2( Value ) ( SDT_HASH1( Value ) * ( ( ( Value ) ^ SDT_DEFAULT_SELECT_FIXED_X) >> 3) )
#define SDT_HASH3( Value ) ( SDT_HASH2( Value ) ^ ( ( ( Value ) ^ SDT_DEFAULT_SELECT_FIXED_X) << 4) )
#define SDT_HASH4( Value ) ( SDT_HASH3( Value ) + ( ( ( Value ) ^ SDT_DEFAULT_SELECT_FIXED_X) >> 5) )
#define SDT_HASH5( Value ) ( SDT_HASH4( Value ) * ( ( ( Value ) ^ SDT_DEFAULT_SELECT_FIXED_X) << 6) )
#define SDT_HASH6( Value ) ( SDT_HASH5( Value ) ^ ( ( ( Value ) ^ SDT_DEFAULT_SELECT_FIXED_X) >> 7) )
#define SDT_HASH( Value )  ( SDT_HASH6( Value ) + ( ( ( Value ) ^ SDT_DEFAULT_SELECT_FIXED_X) << 8) )

#define SDT_KEYED_HASH( Key, Value ) SDT_HASH( ( Value ) ^ SDT_HASH( Key ) )

typedef SDT_BASE_ENCODER    SDT_TYPE_ENCODER;

// SDT_STATIC_ASSERT_ENUM
template< int res >
struct CTAssertEnum
{
    enum { IsValid };
};

template<>
struct CTAssertEnum< 0 >
{
};

#define SDT_STATIC_ASSERT_ENUM( expr ) CTAssertEnum< expr >::IsValid

struct CEncoderSet
{
    enum { SDT_ENCODER_LIMIT = SDT_ENCODER_USER_LIMIT };
    enum { CHECK_ENCODER_LIMIT = SDT_STATIC_ASSERT_ENUM( SDT_ENCODER_LIMIT % SDT_ENCODER_BLOCK_SIZE == 0 ) };
    static SDT_BASE_ENCODER   EncoderSet[ CEncoderSet::SDT_ENCODER_LIMIT ];
};


template <
    CoreType    Seed
>
struct CKeyData
{
    enum { SEED = Seed % CEncoderSet::SDT_ENCODER_LIMIT };

    enum { SEED_0 = SDT_KEYED_HASH( SEED + SDT_CONFIG_SEED_3, SDT_CONFIG_SEED_0  ) };
    enum { SEED_1 = SDT_KEYED_HASH( SEED + SDT_CONFIG_SEED_4, SDT_CONFIG_SEED_1  ) };
    enum { SEED_2 = SDT_KEYED_HASH( SEED + SDT_CONFIG_SEED_5, SDT_CONFIG_SEED_2  ) };
    enum { SEED_3 = SDT_KEYED_HASH( SEED + SDT_CONFIG_SEED_6, SDT_CONFIG_SEED_3  ) };
    enum { SEED_4 = SDT_KEYED_HASH( SEED + SDT_CONFIG_SEED_7, SDT_CONFIG_SEED_4  ) };
    enum { SEED_5 = SDT_KEYED_HASH( SEED + SDT_CONFIG_SEED_0, SDT_CONFIG_SEED_5  ) };
    enum { SEED_6 = SDT_KEYED_HASH( SEED + SDT_CONFIG_SEED_1, SDT_CONFIG_SEED_6  ) };
    enum { SEED_7 = SDT_KEYED_HASH( SEED + SDT_CONFIG_SEED_2, SDT_CONFIG_SEED_7  ) };

    enum { SELECT = 0x5cac5ac5  };
    enum { ALGORITHM = SEED & 0x3   };
    enum { KEY_0 = SDT_KEYED_HASH( SEED, SEED_0 ) };
    enum { KEY_1 = SDT_KEYED_HASH( SEED, SEED_1 ) };
    enum { KEY_2 = SDT_KEYED_HASH( SEED, SEED_2 ) };
    enum { KEY_3 = SDT_KEYED_HASH( SEED, SEED_3 ) };
    enum { KEY_4 = SDT_KEYED_HASH( SEED, SEED_4 ) };
    enum { KEY_5 = SDT_KEYED_HASH( SEED, SEED_5 ) };
    enum { KEY_6 = SDT_KEYED_HASH( SEED, SEED_6 ) };
    enum { KEY_7 = SDT_KEYED_HASH( SEED, SEED_7 ) };
};

template <
    CoreType    Seed
>
struct CEncoderSelect
{
    enum { SEED = CKeyData< Seed >::SEED };
    enum { CHECK_ENCODER_LIMIT = SDT_STATIC_ASSERT_ENUM( Seed < CEncoderSet::SDT_ENCODER_LIMIT) };
    static SDT_BASE_ENCODER&  GetEncoder()
    {
        return CEncoderSet::EncoderSet[ SEED ];
    }
};


template< typename T >
struct TypeIsSigned
{
    enum { IS_SIGNED = (std::numeric_limits< T >::is_signed ? 1 : 0) };
};

// VC6 is a pain here - regarding fixed length types as distinct from
// basic types, and so doesn't supply numeric limits
#if defined(_MSC_VER) && _MSC_VER < 1300

template<> struct TypeIsSigned< __int8 >            { enum { IS_SIGNED = 1 }; };
template<> struct TypeIsSigned< unsigned __int8 >   { enum { IS_SIGNED = 0 }; };
template<> struct TypeIsSigned< __int16 >           { enum { IS_SIGNED = 1 }; };
template<> struct TypeIsSigned< unsigned __int16 >  { enum { IS_SIGNED = 0 }; };
template<> struct TypeIsSigned< __int32 >           { enum { IS_SIGNED = 1 }; };
template<> struct TypeIsSigned< unsigned __int32 >  { enum { IS_SIGNED = 0 }; };
template<> struct TypeIsSigned< __int64 >           { enum { IS_SIGNED = 1 }; };
template<> struct TypeIsSigned< unsigned __int64 >  { enum { IS_SIGNED = 0 }; };

#endif

template< typename T >
struct TypeToIndex
{
    enum { WIDTH = sizeof(T) };
    enum { IS_SIGNED = (int)TypeIsSigned< T >::IS_SIGNED };
    enum { INDEX = ( WIDTH * 2 ) + IS_SIGNED };
};

// All types listed below are supported basic and fixed sdt types
template < class CUnknown > struct SupportedType{  }; 
template <> struct SupportedType< char >            { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< signed char >     { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned char >   { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< short >           { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned short >  { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< int >             { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned int >    { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< long >            { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned long >   { enum { IS_SUPPORTED = 1 }; }; 

#if defined(_MSC_VER) && _MSC_VER < 1300

// In VC6, fixed types are distinct from the base types
template <> struct SupportedType< __int8 >          { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned __int8 > { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< __int16 >         { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned __int16 >{ enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< __int32 >         { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned __int32 >{ enum { IS_SUPPORTED = 1 }; }; 

#if defined(SDT_64_BIT_SUPPORTED)
template <> struct SupportedType< __int64 >         { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned __int64 >{ enum { IS_SUPPORTED = 1 }; }; 
#endif // defined(SDT_64_BIT_SUPPORTED)

#else //!defined(_MSC_VER) && _MSC_VER < 1300

#if defined(SDT_64_BIT_SUPPORTED)
template <> struct SupportedType< long long > { enum { IS_SUPPORTED = 1 }; }; 
template <> struct SupportedType< unsigned long long > { enum { IS_SUPPORTED = 1 }; }; 
#endif // defined(SDT_64_BIT_SUPPORTED)

#endif //defined(_MSC_VER) && _MSC_VER < 1300

template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorAdd
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_ADD };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value + Parameter) );

        return Result;
    }
};

template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorSub
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_SUB };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value - Parameter) );

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorMul
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_MUL };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value * Parameter) );

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorDiv
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_DIV };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value / Parameter) );

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorMod
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_MOD };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value % Parameter) );

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorAnd
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_AND };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value & Parameter) );

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorOr
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_OR };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value | Parameter) );

        return Result;
    }
};



template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorXor
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_XOR };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value ^ Parameter) );

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorShl
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_SHL };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value << Parameter) );

        return Result;
    }
};
template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorShr
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_SHR };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value >> Parameter) );

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorLt
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_LT };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value < Parameter) );

        return Result;
    }
};

template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorLte
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_LTE };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value <= Parameter) );

        return Result;
    }
};



template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorEq
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_EQ };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value == Parameter) );

        return Result;
    }
};

template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorGte
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_GTE };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value >= Parameter) );

        return Result;
    }
};

template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorGt
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_GT };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value > Parameter) );

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorGet
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_GET };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( Parameter );

        Result = Value;

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorSet
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_SET };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( Value );
        
        Result = Parameter;

        return Result;
    }
};


template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorNoa
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_NOA };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value ^ Parameter) );
        
        return Result;
    }
};

template <
    class               PlainType,
    CoreType         Seed
>
struct  COperatorNos
{
    enum { OPERATOR_ID = SDT_BASE_OPERATION_NOS };

    PlainType operator()(
        const PlainType    Value,
        const PlainType    Parameter
    )
    {
        PlainType Result( (PlainType)(Value ^ Parameter) );
        
        return Result;
    }
};

#define SDT_KEYDATA_SELECT( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::SELECT )
#define SDT_KEYDATA_KEY_0( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::KEY_0 )
#define SDT_KEYDATA_KEY_1( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::KEY_1 )
#define SDT_KEYDATA_KEY_2( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::KEY_2 )
#define SDT_KEYDATA_KEY_3( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::KEY_3 )
#define SDT_KEYDATA_KEY_4( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::KEY_4 )
#define SDT_KEYDATA_KEY_5( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::KEY_5 )
#define SDT_KEYDATA_KEY_6( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::KEY_6 )
#define SDT_KEYDATA_KEY_7( SEED ) (SDT_CORE_TYPE)( Sdt::CKeyData< SEED >::KEY_7 )

#define SDT_ENCODE_VALUE_LITE( Type, SEED, PlainValue ) SDT_BASE_ENCODE_LITE( SEED, (Type)PlainValue, SDT_KEYDATA_SELECT( SEED ), SDT_KEYDATA_KEY_0( SEED ), SDT_KEYDATA_KEY_1( SEED ) )
#define SDT_ENCODE_VALUE( Type, SEED, PlainValue ) SDT_BASE_ENCODE( (Type)SEED, (Type)PlainValue, (Type)SDT_KEYDATA_SELECT( SEED ), SDT_KEYDATA_KEY_0( SEED ), SDT_KEYDATA_KEY_1( SEED ), SDT_KEYDATA_KEY_2( SEED ), SDT_KEYDATA_KEY_3( SEED ), SDT_KEYDATA_KEY_4( SEED ), SDT_KEYDATA_KEY_5( SEED ), SDT_KEYDATA_KEY_6( SEED ), SDT_KEYDATA_KEY_7( SEED ) )

template 
<
    class       SourceType,
    SourceType  Value,
    class       TargetType
>
struct CTypeCrowbar
{
#ifdef _MSC_VER
#pragma warning( push, 2 )
#endif // _MSC_VER

	enum { VALUE = (TargetType)Value };

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER
};


template
<
    CoreType     Seed
>
struct CRandomValue
{
    static CoreType    GetRts()
    {
        CoreType     RandomValue(0);

        SDT_PLATFORM_READ_TIME_STAMP( RandomValue );

        return RandomValue;
    }
    static CoreType    GetStack()
    {
        CoreType     RandomValue(0);

        SDT_PLATFORM_READ_STACK_REGISTER( RandomValue );

        return RandomValue;
    }
    static CoreType    GetBase()
    {
        CoreType     RandomValue(0);

        SDT_PLATFORM_READ_BASE_REGISTER( RandomValue );

        return RandomValue;
    }
    static CoreType    GetFlags()
    {
        CoreType     RandomValue(0);

        SDT_PLATFORM_READ_FLAGS( RandomValue );

        return RandomValue;
    }
    static CoreType    Get()
    {
        CoreType     RandomValue( 0 );

        switch ( Seed % 4 )
        {
            case 1 : RandomValue = GetRts(); break;
            case 2 : RandomValue = GetStack(); break;
            case 3 : RandomValue = GetBase(); break;
            case 4 : RandomValue = GetFlags(); break;
            default: RandomValue = GetRts(); break;
        }

        return RandomValue;
    }
};

template
<
    CoreType     Seed,
    CoreType     Variation
>
struct CStreamKey
{
    enum { PLAIN_KEY_0  = SDT_HASH( Seed + Variation + 0 ) };
    enum { PLAIN_KEY_1  = SDT_HASH( Seed + Variation + 1 ) };
    enum { PLAIN_KEY_2  = SDT_HASH( Seed + Variation + 2 ) };
    enum { PLAIN_KEY_3  = SDT_HASH( Seed + Variation + 3 ) };

    enum { ENCODED_KEY_0 = SDT_ENCODE_VALUE_LITE( CoreType, Seed, PLAIN_KEY_0 ) };
    enum { ENCODED_KEY_1 = SDT_ENCODE_VALUE_LITE( CoreType, Seed, PLAIN_KEY_1 ) };
    enum { ENCODED_KEY_2 = SDT_ENCODE_VALUE_LITE( CoreType, Seed, PLAIN_KEY_2 ) };
    enum { ENCODED_KEY_3 = SDT_ENCODE_VALUE_LITE( CoreType, Seed, PLAIN_KEY_3 ) };

    static void SetupKeyPair(
        CoreType&    rPlainKey,
        CoreType&    rEncodedKey
    )
    {
        static const CoreType  PlainKeys[4] = {  (CoreType)PLAIN_KEY_0,  (CoreType)PLAIN_KEY_1,  (CoreType)PLAIN_KEY_2,  (CoreType)PLAIN_KEY_3 };
        static const CoreType  EncodedKeys[4] = { (CoreType)ENCODED_KEY_0, (CoreType)ENCODED_KEY_1, (CoreType)ENCODED_KEY_2, (CoreType)ENCODED_KEY_3 };

        CoreType KeyId( (CoreType)CRandomValue< Seed + Variation >::Get() % 4 );

        rPlainKey = PlainKeys[ KeyId ];
        rEncodedKey = EncodedKeys[ KeyId ];
    }
};


#define SDT_FORMAT1_ID( PlainType, Seed) SDT_ENCODE_VALUE_LITE( PlainType, Seed, SDT_BASE_FORMAT_1 )
#define SDT_TYPE_ID() SDT_ENCODE_VALUE_LITE( PlainType, Seed, TypeToIndex< PlainType >::INDEX )
#define SDT_OPERATOR_ID() SDT_ENCODE_VALUE_LITE( PlainType, Seed, Operator::OPERATOR_ID )

template 
<
    class       PlainType,
    CoreType    Seed,
    class       Operator,
    class       ItemSetType
>
struct CItemSet
{
    ItemSetType   m_ItemSet;

    CItemSet(
        const EncodedType&    rValue,
        const EncodedType&    rParameter
    )
    {
        CoreType    PlainKey;
        CoreType    EncodedKey;

        CStreamKey< Seed, Operator::OPERATOR_ID >::SetupKeyPair( PlainKey, EncodedKey );

        m_ItemSet.Item[SDT_BASE_ITEM_SEED] = PlainKey;
        m_ItemSet.Item[SDT_BASE_ITEM_FORMAT] = SDT_ENCODE_VALUE_LITE( PlainType, Seed, SDT_BASE_FORMAT_1 );
        m_ItemSet.Item[SDT_BASE_ITEM_TYPE] = SDT_ENCODE_VALUE_LITE( PlainType, Seed, TypeToIndex< PlainType >::INDEX );
        m_ItemSet.Item[SDT_BASE_ITEM_OPERATOR] = SDT_ENCODE_VALUE_LITE( PlainType, Seed, Operator::OPERATOR_ID );
        m_ItemSet.Item[SDT_BASE_ITEM_VALUE] = rValue;
        m_ItemSet.Item[SDT_BASE_ITEM_PARAMETER] = rParameter;
        SDT_BASE_ITEM_SET_ENCODE( m_ItemSet, EncodedKey );
    }

    ItemSetType&   rItemSet()
    {
        return m_ItemSet;
    }

};


template <
    class               PlainType,
    CoreType            Seed,
    class               Plugins,
    class               Operator
>
struct CUnpreppedOperation
{
    static EncodedType   Encode(
        const SDT_TYPE_ENCODER*     pEncoder,
        const PlainType             PlainValue
    )
    {
        SDT_MARKER_DEFINE_START( SDT_MARKER_ERASE );

        SDT_CORE_TYPE   SELECT( pEncoder->SelectBits );
        SDT_CORE_TYPE   KEY_0( pEncoder->Keys[0] );
        SDT_CORE_TYPE   KEY_1( pEncoder->Keys[1] );
        SDT_CORE_TYPE   KEY_2( pEncoder->Keys[2] );
        SDT_CORE_TYPE   KEY_3( pEncoder->Keys[3] );
        SDT_CORE_TYPE   KEY_4( pEncoder->Keys[4] );
        SDT_CORE_TYPE   KEY_5( pEncoder->Keys[5] );
        SDT_CORE_TYPE   KEY_6( pEncoder->Keys[6] );
        SDT_CORE_TYPE   KEY_7( pEncoder->Keys[7] );

        EncodedType EncodedValue = SDT_BASE_ENCODE( (EncodedType)Seed, (EncodedType)PlainValue, (EncodedType)SELECT, KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7 );

        SDT_MARKER_DEFINE_END();

        return EncodedValue;

    }
    static PlainType   Decode(
        const SDT_TYPE_ENCODER*   pEncoder,
        const EncodedType         EncodedValue
    )
    {
        SDT_MARKER_DEFINE_START( SDT_MARKER_ERASE );

        SDT_BASE_UINT32  SELECT( pEncoder->SelectBits );
        SDT_CORE_TYPE  KEY_0( pEncoder->Keys[0] );
        SDT_CORE_TYPE  KEY_1( pEncoder->Keys[1] );
        SDT_CORE_TYPE  KEY_2( pEncoder->Keys[2] );
        SDT_CORE_TYPE  KEY_3( pEncoder->Keys[3] );
        SDT_CORE_TYPE  KEY_4( pEncoder->Keys[4] );
        SDT_CORE_TYPE  KEY_5( pEncoder->Keys[5] );
        SDT_CORE_TYPE  KEY_6( pEncoder->Keys[6] );
        SDT_CORE_TYPE  KEY_7( pEncoder->Keys[7] );

        PlainType PlainValue = (PlainType)SDT_BASE_DECODE( (EncodedType)Seed, (EncodedType)EncodedValue, (EncodedType)SELECT, KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7 );

        SDT_MARKER_DEFINE_END( );

        return PlainValue;
    }

    static EncodedType   Invoke(
        const EncodedType   Value,
        const EncodedType   Parameter
    )
    {
        SDT_MARKER_DEFINE_START( SDT_MARKER_ERASE );

        EncodedType         Result( 0 );

        SDT_TYPE_ENCODER*   pEncoder = &CEncoderSelect< Seed >::GetEncoder();

        PlainType           PlainValue( Decode( pEncoder, Value ) );
        PlainType           PlainParameter( Decode( pEncoder, Parameter ) );
        PlainType           PlainResult( (PlainType)Operator()( PlainValue, PlainParameter ) );

        PlainResult = (PlainType)Plugins::InvokeOperatorPlugin( 
            (WordType)TypeToIndex< PlainType >::INDEX, (WordType)Operator::OPERATOR_ID, 
            (WordType)PlainValue, (WordType)PlainParameter, (WordType)PlainResult );

        if ( (int)SDT_BASE_OPERATION_NOS == (int)Operator::OPERATOR_ID )
        {
            if ( PlainValue == SDT_BASE_NOTIFY_BREAK )
            {
                pEncoder->Keys[0] = 0;
            }
        }
        if ( (int)SDT_BASE_OPERATION_SET == (int)Operator::OPERATOR_ID )
        {
            PlainResult = (PlainType)Parameter;
        }
        if ( (int)SDT_BASE_OPERATION_GET == (int)Operator::OPERATOR_ID )
        {
            Result = (EncodedType)PlainValue;
        }
        else
        {
            Result = Encode( pEncoder, PlainResult );           
        }
        SDT_MARKER_DEFINE_END( );

        return Result;
    }      
};



template <
    class               PlainType,
    CoreType            Seed,
    class               Plugins,
    class               Operator
>
struct COperation
{


    static EncodedType   Invoke(
        const EncodedType   Value,
        const EncodedType   Parameter
    )
    {
        SDT_TYPE_ENCODER*   pEncoder = &CEncoderSelect< Seed >::GetEncoder();
        EncodedType         Result( 0 );

        if ( 0 != pEncoder->pHandlers[SDT_BASE_HANDLER_MESSAGE] )
        {
            SDT_BASE_MESSAGE_HANDLER   pOperation = reinterpret_cast<SDT_BASE_MESSAGE_HANDLER>( pEncoder->pHandlers[SDT_BASE_HANDLER_MESSAGE] );
            SDT_BASE_UINT64            pData      = pEncoder->pData[SDT_BASE_DATA_ENCODER];
        
            CItemSet< PlainType, Seed, Operator, SDT_BASE_ITEM_SET >   Parameters( Value, Parameter );

			Result = pOperation( pData, reinterpret_cast<SDT_BASE_UINT64>( &Parameters.rItemSet() ) );
        }
        else
        {
#if SDT_DISABLE_UNPREPPED
            SDT_PLATFORM_BREAKPOINT();
#else
            Result = CUnpreppedOperation< PlainType, Seed, Plugins, Operator>::Invoke( Value, Parameter );
#endif
        }
        return Result;
    }
};


} // namespace


#endif // __SDT_DEFAULT_H__

