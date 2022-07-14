/***************************************************************************************************
 *
 * $Archive: /Eng/Common/Library/Security/Sdt/include/Sdt/SdtType.h $     
 *
 * $Revision: #1 $
 *
 * Description:     Class to define a secure data type
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
#ifndef __SDT_TYPE_H__
#define __SDT_TYPE_H__
#include "sdtmarker.h"
#include "sdtbase.h"
#include "sdtdefault.h"

namespace Sdt
{

typedef enum 
{
    FORMAT_PLAIN    = 0x1,
    FORMAT_ENCODED  = 0x2
}
VALUE_FORMAT_TYPE;



template <
    class           PlainType,
	EncodedType     Seed,
    class           Plugins
>
struct CDebugValue
{
#ifdef _DEBUG

    typedef     COperation< PlainType, Seed, Plugins, COperatorGet< PlainType, Seed > >  CGet;

    PlainType   m_Value;

    void   Update( const EncodedType&  rEncodedValue ) 
    {      
        m_Value = (PlainType)CGet::Invoke( rEncodedValue, rEncodedValue );
    }

#else

    void   Update( const EncodedType&  )  {}

#endif
};



#define SDT_ZERO_ENCODED( SEED ) (EncodedType)SDT_ENCODE_VALUE( SDT_ENCODED_TYPE, (SDT_ENCODED_TYPE)SEED, (SDT_ENCODED_TYPE)0 )


template <
    class           PlainType,
	EncodedType     Seed,
    class           Plugins
>
struct CEncodedStore
{
    typedef     COperation< PlainType, Seed, Plugins, COperatorSet< PlainType, Seed > >  CSet;
    typedef     COperation< PlainType, Seed, Plugins, COperatorGet< PlainType, Seed > >  CGet;
    typedef     COperation< PlainType, Seed, Plugins, COperatorAdd< PlainType, Seed > >  CAdd;
    typedef     COperation< PlainType, Seed, Plugins, COperatorSub< PlainType, Seed > >  CSub;
    typedef     COperation< PlainType, Seed, Plugins, COperatorMul< PlainType, Seed > >  CMul;
    typedef     COperation< PlainType, Seed, Plugins, COperatorDiv< PlainType, Seed > >  CDiv;
    typedef     COperation< PlainType, Seed, Plugins, COperatorMod< PlainType, Seed > >  CMod;
    typedef     COperation< PlainType, Seed, Plugins, COperatorAnd< PlainType, Seed > >  CAnd;
    typedef     COperation< PlainType, Seed, Plugins, COperatorOr < PlainType, Seed > >  COr ;
    typedef     COperation< PlainType, Seed, Plugins, COperatorXor< PlainType, Seed > >  CXor;
    typedef     COperation< PlainType, Seed, Plugins, COperatorShl< PlainType, Seed > >  CShl;
    typedef     COperation< PlainType, Seed, Plugins, COperatorShr< PlainType, Seed > >  CShr;
    typedef     COperation< PlainType, Seed, Plugins, COperatorLt < PlainType, Seed > >  CLt ;
    typedef     COperation< PlainType, Seed, Plugins, COperatorLte< PlainType, Seed > >  CLte;
    typedef     COperation< PlainType, Seed, Plugins, COperatorEq < PlainType, Seed > >  CEq ;
    typedef     COperation< PlainType, Seed, Plugins, COperatorGte< PlainType, Seed > >  CGte;
    typedef     COperation< PlainType, Seed, Plugins, COperatorGt < PlainType, Seed > >  CGt ;
    typedef     COperation< PlainType, Seed, Plugins, COperatorNoa< PlainType, Seed > >  CNoa;
    typedef     COperation< PlainType, Seed, Plugins, COperatorNos< PlainType, Seed > >  CNos;

    //enum { ZERO_ENCODED = SDT_ENCODE_VALUE( PlainType, Seed, 0 ) };


    CDebugValue< PlainType, Seed, Plugins >  m_PlainValue;
    EncodedType                     m_EncodedValue;
    

    CEncodedStore()
    {
        SetEncodedValue( SDT_ZERO_ENCODED( Seed ) );
    }

    CEncodedStore( 
        const VALUE_FORMAT_TYPE  ValueFormat,
        const PlainType          PlainValue,
        const EncodedType        EncodedValue 
    ) 
    {
        if ( FORMAT_ENCODED == ValueFormat )
        {
            SetEncodedValue( EncodedValue );
        }
        if ( FORMAT_PLAIN == ValueFormat )
        {
            SetEncodedValueFromPlain( PlainValue );
        }
    }

    CEncodedStore( 
        const CEncodedStore& rEncodedStore 
    )  
    {
        SetEncodedValue( rEncodedStore.GetEncodedValue() );

    }
    CEncodedStore& operator=( 
        const CEncodedStore& rEncodedStore 
    ) 
    {  
        SetEncodedValue( rEncodedStore.GetEncodedValue() );

        return *this; 
    }

    PlainType    GetPlainValue() const 
    {  
        return Decode( m_EncodedValue ); 
    };

    void SetEncodedValueFromPlain(
        const PlainType   PlainValue 
    ) 
    { 
        m_EncodedValue = Encode( PlainValue ); 

        m_PlainValue.Update( m_EncodedValue );
    }
    void SetEncodedValue(
        const EncodedType   EncodedValue 
    ) 
    { 
        m_EncodedValue = EncodedValue; 

        m_PlainValue.Update( m_EncodedValue );
    }
    EncodedType GetEncodedValue() const 
    {  
        return m_EncodedValue; 
    };

    CEncodedStore  operator+( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CAdd::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator-( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CSub::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator*( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CMul::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator/( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CDiv::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator%( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CMod::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator&( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CAnd::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator|( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( COr::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator^( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CXor::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator>>( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CShr::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    CEncodedStore  operator<<( const CEncodedStore&    rEncodedStore ) const
    {
        return CEncodedStore( CShl::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() ) );
    }
    bool  operator<( const CEncodedStore&    rEncodedStore ) const
    {
        return SDT_ZERO_ENCODED( Seed ) != CLt::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() );
    }
    bool  operator<=( const CEncodedStore&    rEncodedStore ) const
    {
        return SDT_ZERO_ENCODED( Seed ) != CLte::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() );
    }
    bool  operator==( const CEncodedStore&    rEncodedStore ) const
    {
        return SDT_ZERO_ENCODED( Seed ) != CEq::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() );
    }
    bool  operator>=( const CEncodedStore&    rEncodedStore ) const
    {
        return SDT_ZERO_ENCODED( Seed ) != CGte::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() );
    }
    bool  operator>( const CEncodedStore&    rEncodedStore ) const
    {
        return SDT_ZERO_ENCODED( Seed ) != CGt::Invoke( GetEncodedValue(), rEncodedStore.GetEncodedValue() );
    }      
    EncodedType  NotifyAuthenticator( 
        const EncodedType&   rValueId,
        const EncodedType&   rValue
    ) const
    {
        return CEncodedStore( CNoa::Invoke( rValueId, rValue ) ).GetEncodedValue();
    }
    EncodedType  NotifySecurity( 
        const EncodedType&   rValueId,
        const EncodedType&   rValue
    ) const
    {
        return CEncodedStore( CNos::Invoke( rValueId, rValue ) ).GetEncodedValue();
    }
private:
    EncodedType   Encode(
        const PlainType  PlainValue
    ) const
    {
        return (EncodedType)CSet::Invoke( (EncodedType)PlainValue, (EncodedType)PlainValue );
    }
    PlainType   Decode(
        const EncodedType  EncodedValue
    ) const
    {      
        return (PlainType)CGet::Invoke( EncodedValue, EncodedValue );
    }
    explicit CEncodedStore( 
        const EncodedType        EncodedValue 
    ) 
    {
        SetEncodedValue( EncodedValue );
    }
};

#define SDT_MUNGE_ENCODED( SEED, PLAIN_TYPE ) (PLAIN_TYPE)SDT_ENCODE_VALUE( PLAIN_TYPE, (PLAIN_TYPE)SEED, (PLAIN_TYPE)0x4124356 )

template<
    class               PlainType,
	EncodedType         Seed,
    class               Plugins = SDT_DEFAULT_PLUGIN
>
struct CSdtType
{
    // Compile time checks for invalid feature identifier
    enum { IsValidFeatureId = SDT_STATIC_ASSERT_ENUM( Seed < CEncoderSet::SDT_ENCODER_LIMIT ) };

    // Compile time check for invalid (unsupported) type
    enum { IsValidType = (int)SupportedType< PlainType >::IS_SUPPORTED };

    enum { SEED = Seed };
    typedef PlainType    TYPE;
 
	typedef CEncodedStore< PlainType, Seed, Plugins >    CEncodedValue;
    typedef CEncodedStore< WordType,  Seed, Plugins >    CEncodedWord;

    CSdtType()
    {
    }
    CSdtType( const PlainType&  rPlainValue )      
    :   
        m_EncodedValue(  FORMAT_PLAIN, rPlainValue, 0 )
    {        
    }
    CSdtType( const CEncodedValue&  rEncodedValue )
    :
        m_EncodedValue( rEncodedValue  )
    {
    }

    template<
        class               DifferentPlainType,
        EncodedType         DifferentSeed,
        class               DifferentPlugins
    >
    CSdtType( const CSdtType< DifferentPlainType, DifferentSeed, DifferentPlugins >& rSdtType )
        : m_EncodedValue( FORMAT_PLAIN, SDT_MUNGE_ENCODED( Seed, PlainType), 0 )
    {
        *this ^= (rSdtType ^ CSdtType< DifferentPlainType, DifferentSeed, DifferentPlugins >( SDT_MUNGE_ENCODED( DifferentSeed, DifferentPlainType ) ) ).GetPlainValue();
    }

    operator PlainType() const
    {
        return GetPlainValue();
    }

    CSdtType& operator=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator+=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue + rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator-=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue - rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator*=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue * rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator/=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue / rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator%=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue % rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator&=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue & rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator|=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue | rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator^=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue ^ rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator>>=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue >> rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType& operator<<=( const CSdtType&    rSdtType )
    {
        m_EncodedValue = m_EncodedValue << rSdtType.m_EncodedValue;

        return *this;
    }
    CSdtType  operator+( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue + rSdtType.m_EncodedValue;
    }
    CSdtType  operator-( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue - rSdtType.m_EncodedValue;
    }
    CSdtType  operator*( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue * rSdtType.m_EncodedValue;
    }
    CSdtType  operator/( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue / rSdtType.m_EncodedValue;
    }
    CSdtType  operator%( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue % rSdtType.m_EncodedValue;
    }
    CSdtType  operator&( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue & rSdtType.m_EncodedValue;
    }
    CSdtType  operator|( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue | rSdtType.m_EncodedValue;
    }
    CSdtType  operator^( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue ^ rSdtType.m_EncodedValue;
    }
    CSdtType  operator>>( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue >> rSdtType.m_EncodedValue;
    }
    CSdtType  operator<<( const CSdtType&    rSdtType ) const
    {
        return m_EncodedValue << rSdtType.m_EncodedValue;
    }
    bool operator<( const CSdtType&    rSdtType ) const
    {
        return  ( m_EncodedValue < rSdtType.m_EncodedValue);
    }
    bool operator<=( const CSdtType&    rSdtType ) const
    {
        return  ( m_EncodedValue <= rSdtType.m_EncodedValue);
    }
    bool operator==( const CSdtType&    rSdtType ) const
    {
        return  ( m_EncodedValue == rSdtType.m_EncodedValue);
    }
    bool operator>=( const CSdtType&    rSdtType ) const
    {
        return  ( m_EncodedValue >= rSdtType.m_EncodedValue);
    }
    bool operator>( const CSdtType&    rSdtType ) const
    {
        return  ( m_EncodedValue > rSdtType.m_EncodedValue);
    }
    bool operator!=( const CSdtType&    rSdtType ) const
    {
        return !operator==( rSdtType );
    }
    CSdtType&  operator++()
    {
        CEncodedValue   Parameter( FORMAT_ENCODED, 0, (EncodedType)SDT_ENCODE_VALUE( PlainType, Seed, 1 ) );
        operator+=( Parameter );
        return *this;
    }
    CSdtType&  operator--()
    {
        CEncodedValue   Parameter( FORMAT_ENCODED, 0, (EncodedType)SDT_ENCODE_VALUE( PlainType, Seed, 1 ) );
        operator-=( Parameter );
        return *this;
    }
    CSdtType  operator++(int)
    {
        CSdtType    OriginalValue( *this );
        operator++();
        return OriginalValue;
    }
    CSdtType  operator--(int)
    {
        CSdtType    OriginalValue( *this );
        operator--();
        return OriginalValue;
    }
    PlainType   GetPlainValue() const
    {
        return m_EncodedValue.GetPlainValue();
    }
    EncodedType   GetEncodedValue() const
    {
        return m_EncodedValue.GetEncodedValue();
    }
    EncodedType  NotifyAuthenticator( 
        const EncodedType&  rValueId, 
        const EncodedType&  rValue 
    ) const
    {
        return m_EncodedValue.NotifyAuthenticator( rValueId, rValue );
    }

    EncodedType  NotifySecurity( 
        const EncodedType&  rValueId, 
        const EncodedType&  rValue 
    ) const
    {
        return m_EncodedValue.NotifySecurity( rValueId, rValue );
    }

    EncodedType  NotifyBreak( 
        const EncodedType&  rValue 
    ) const
    {
        CEncodedWord   BreakId( FORMAT_ENCODED, 0, (EncodedType)SDT_ENCODE_VALUE( 
            WordType, Seed, SDT_BASE_NOTIFY_BREAK ) );

        CEncodedWord   BreakValue( FORMAT_PLAIN, rValue, 0 );

        EncodedType Result( NotifySecurity( BreakId.GetEncodedValue(), BreakId.GetEncodedValue() ) );

        m_EncodedValue = m_EncodedValue; // This is needed for security check reasons

        return Result;
    }

private:    

    mutable CEncodedValue           m_EncodedValue;
};



template<
    class			SdtType,
	WordType	ValueId = 0,
	WordType	Value = 0
>
struct CNotifySecurity
{
    static EncodedType  SendValue( 
		const SdtType&		rSdt
    ) 
    {
		typename SdtType::CEncodedWord   EncodedValueId( FORMAT_ENCODED, 0, 
			(EncodedType)SDT_ENCODE_VALUE( WordType, SdtType::SEED, ValueId ) );

		typename SdtType::CEncodedWord   EncodedValue( FORMAT_ENCODED, 0, 
			(EncodedType)SDT_ENCODE_VALUE( WordType, SdtType::SEED, Value ) );

        return rSdt.NotifySecurity( EncodedValueId.GetEncodedValue(), EncodedValue.GetEncodedValue() );
    }

    static EncodedType  SendValue( 
		const SdtType&		rSdt,
        const EncodedType&  rValue 
    ) 
    {
		typename SdtType::CEncodedWord   EncodedValueId( FORMAT_ENCODED, 0, 
			(EncodedType)SDT_ENCODE_VALUE( WordType, SdtType::SEED, ValueId ) );

        typename SdtType::CEncodedWord   EncodedValue( FORMAT_PLAIN, rValue, 0 );

        return rSdt.NotifySecurity( EncodedValueId.GetEncodedValue(), EncodedValue.GetEncodedValue() );
    }
    static EncodedType  SendValue( 
		const SdtType&		rSdt,
        const EncodedType&  rValueId, 
        const EncodedType&  rValue 
    ) 
    {
        typename SdtType::CEncodedWord   EncodedValueId( FORMAT_PLAIN, rValueId, 0 );

        typename SdtType::CEncodedWord   EncodedValue( FORMAT_PLAIN, rValue, 0 );

        return rSdt.NotifySecurity( EncodedValueId.GetEncodedValue(), EncodedValue.GetEncodedValue() );
    }
};

#define SDT_TYPE_DEFINE_TYPE( Type, Seed, TypeName ) typedef Sdt::CSdtType< Type, Seed >  TypeName
#define SDT_TYPE_DEFINE_CONST( TypeName, Value ) ( TypeName::CEncodedValue( Sdt::FORMAT_ENCODED, 0, (Sdt::EncodedType)SDT_ENCODE_VALUE( TypeName::TYPE, TypeName::SEED,  Value ) ) )
#define SDT_TYPE_NOTIFY_SECURITY( TypeName, Variable, ValueId, Value )	Sdt::CNotifySecurity< TypeName, ValueId >::SendValue( Variable, Value );
#define SDT_TYPE_TEST_FAILURE( Variable ) { Variable.NotifyBreak( 0 ); }

} // namespace 


#endif // __SDT_TYPE_H__

