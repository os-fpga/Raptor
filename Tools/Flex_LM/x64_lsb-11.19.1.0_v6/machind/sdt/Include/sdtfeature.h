/**************************************************************************************************/
/*!@file
* @brief Sdt Feature header
*
* @version \$Revision: #1 $
*/
/**************************************************************************************************
* Copyright (c) 1997-2016, 2018-2020 Flexera. All Rights Reserved.
**************************************************************************************************/
#if !defined(SDT_FEATURE_INCLUDED)
#define SDT_FEATURE_INCLUDED

#include "sdtconfig.h"
#include "sdtdata.h"

#if defined(__GNUC__)
#	if __GNUC__ >= 4
#		define	SDT_FEATURE_GCC_NODEADSTRIP			__attribute__((used))
#	elif __GNUC__ == 3
#		if __GNUC_MINOR__ > 2
#			define	SDT_FEATURE_GCC_NODEADSTRIP		__attribute__((used))
#		else
#			define	SDT_FEATURE_GCC_NODEADSTRIP		__attribute__((unused))
#		endif
#	else
#		error GCC 2 is not supported.
#	endif
#else
#	define	SDT_FEATURE_GCC_NODEADSTRIP
#endif

#if defined( K_AND_R_TOKEN_PASTE )
#	define SDT_FEATURE_TOKENPASTE(x_,y_)			x_/**/y_
#	define SDT_FEATURE_TOKENPASTE_3(x_,y_,z_)		x_/**/y_/**/z_
#else
#	define SDT_FEATURE_TOKENPASTE(x_,y_)			x_##y_
#	define SDT_FEATURE_TOKENPASTE_3(x_,y_,z_)		x_##y_##z_
#endif

/* Used for setting SDT_CONFIG_ENCODING value in SdtConfig.h */
/* Enables the secure data type encryption to be completely compiled out */
#define SDT_API_ENCODING_EXCLUDE_NONE    0x0
#define SDT_API_ENCODING_EXCLUDE_TYPE    0x1 
#define SDT_API_ENCODING_EXCLUDE_DATA    0x2 
#define SDT_API_ENCODING_EXCLUDE_ALL     0xf 


/* C++ versions of macros to allow the definiton of secure data types */
#ifdef __cplusplus

#if  ( 0 == ( SDT_CONFIG_ENCODING & SDT_API_ENCODING_EXCLUDE_TYPE ) )

#include "sdttype.h"

#define SDT_FEATURE_DEFINE_TYPE( Type, FeatureId, TypeName ) 	SDT_TYPE_DEFINE_TYPE( Type, FeatureId, TypeName )
#define SDT_FEATURE_DEFINE_CONST( TypeName, Value )				SDT_TYPE_DEFINE_CONST( TypeName, Value ) 
#define SDT_FEATURE_TEST_FAILURE( Variable )            		SDT_TYPE_TEST_FAILURE( Variable )

#else /* ( 0 == ( SDT_CONFIG_ENCODING & SDT_API_ENCODING_EXCLUDE_TYPE ) ) */

#define SDT_FEATURE_DEFINE_TYPE( Type, FeatureId, TypeName ) typedef Type TypeName
#define SDT_FEATURE_DEFINE_CONST( TypeName, Value )			    (Value)
#define SDT_FEATURE_TEST_FAILURE( Variable )

#endif /* ( 0 == ( SDT_CONFIG_ENCODING & SDT_API_ENCODING_EXCLUDE_TYPE ) ) */

/* The C++ API */
#define SDT_FEATURE_DEFINE_INT8( FeatureAlias, TypeName)	SDT_FEATURE_DEFINE_TYPE( SDT_BASE_INT8, SDT_EncoderId_##FeatureAlias, TypeName) 
#define SDT_FEATURE_DEFINE_UINT8( FeatureAlias, TypeName)	SDT_FEATURE_DEFINE_TYPE( SDT_BASE_UINT8, SDT_EncoderId_##FeatureAlias, TypeName) 
#define SDT_FEATURE_DEFINE_INT16( FeatureAlias, TypeName)	SDT_FEATURE_DEFINE_TYPE( SDT_BASE_INT16, SDT_EncoderId_##FeatureAlias, TypeName) 
#define SDT_FEATURE_DEFINE_UINT16( FeatureAlias, TypeName)	SDT_FEATURE_DEFINE_TYPE( SDT_BASE_UINT16, SDT_EncoderId_##FeatureAlias, TypeName) 
#define SDT_FEATURE_DEFINE_INT32( FeatureAlias, TypeName)	SDT_FEATURE_DEFINE_TYPE( SDT_BASE_INT32, SDT_EncoderId_##FeatureAlias, TypeName) 
#define SDT_FEATURE_DEFINE_UINT32( FeatureAlias, TypeName)	SDT_FEATURE_DEFINE_TYPE( SDT_BASE_UINT32, SDT_EncoderId_##FeatureAlias, TypeName) 

#define SDT_FEATURE_DEFINE( Name, FeatureIdentity, FeatureAlias )\
	enum SDT_Enum_##FeatureAlias {\
	SDT_FeatureId_##FeatureAlias = FeatureIdentity,\
	SDT_EncoderId_##FeatureAlias = FeatureIdentity%SDT_ENCODER_USER_LIMIT\
	};\
	static const char SDT_FeatureName_##FeatureAlias[] SDT_FEATURE_GCC_NODEADSTRIP = Name;\
    SDT_FEATURE_DEFINE_INT8( FeatureAlias, FeatureAlias##_int8);\
    SDT_FEATURE_DEFINE_UINT8( FeatureAlias, FeatureAlias##_uint8);\
    SDT_FEATURE_DEFINE_INT16( FeatureAlias, FeatureAlias##_int16);\
    SDT_FEATURE_DEFINE_UINT16( FeatureAlias, FeatureAlias##_uint16);\
    SDT_FEATURE_DEFINE_INT32( FeatureAlias, FeatureAlias##_int32);\
    SDT_FEATURE_DEFINE_UINT32( FeatureAlias, FeatureAlias##_uint32)

#else /* C API */

/* C versions of the secure data types to allow definition of secure integer values */

typedef SDT_DATA_LONG                                   SDT_FEATURE_LONG;   

#if  ( 0 == ( SDT_CONFIG_ENCODING & SDT_API_ENCODING_EXCLUDE_DATA ) )

#define SDT_FEATURE_DECLARE_VAR( FeatureAlias, VariableName ) \
    enum { SDT_FEATURE_TOKENPASTE_3(SDT_DATA_,VariableName,_SEED) = SDT_FEATURE_TOKENPASTE(SDT_EncoderId_,FeatureAlias)  }; \
    SDT_DATA_VALUE   VariableName = { &SDT_DATA_EncoderArray[ SDT_FEATURE_TOKENPASTE(SDT_EncoderId_,FeatureAlias) ], 0 }

#define SDT_FEATURE_TEST_DATA( Data, TestSettings )         SDT_DATA_TEST_DATA( Data, TestSettings )

/* Macros to allow manipulation of secure integer values in C programs */
#define SDT_FEATURE_GET(        Data1 )        SDT_DATA_GET(         Data1 )       
#define SDT_FEATURE_SET(        Data1, Value ) SDT_DATA_SET(         Data1, Value )
#define SDT_FEATURE_SET_CONST(  Data1, Const ) SDT_DATA_SET_CONST(   Data1, Const )       
#define SDT_FEATURE_ADD(        Data1, Data2 ) SDT_DATA_ADD(         Data1, Data2 )
#define SDT_FEATURE_SUB(        Data1, Data2 ) SDT_DATA_SUB(         Data1, Data2 )
#define SDT_FEATURE_MUL(        Data1, Data2 ) SDT_DATA_MUL(         Data1, Data2 )
#define SDT_FEATURE_DIV(        Data1, Data2 ) SDT_DATA_DIV(         Data1, Data2 )
#define SDT_FEATURE_MOD(        Data1, Data2 ) SDT_DATA_MOD(         Data1, Data2 )
#define SDT_FEATURE_AND(        Data1, Data2 ) SDT_DATA_AND(         Data1, Data2 )
#define SDT_FEATURE_OR(         Data1, Data2 ) SDT_DATA_OR(          Data1, Data2 )
#define SDT_FEATURE_XOR(        Data1, Data2 ) SDT_DATA_XOR(         Data1, Data2 )
#define SDT_FEATURE_SHR(        Data1, Data2 ) SDT_DATA_SHR(         Data1, Data2 )
#define SDT_FEATURE_SHL(        Data1, Data2 ) SDT_DATA_SHL(         Data1, Data2 )
#define SDT_FEATURE_LT(         Data1, Data2 ) SDT_DATA_LT(          Data1, Data2 )
#define SDT_FEATURE_LTE(        Data1, Data2 ) SDT_DATA_LTE(         Data1, Data2 )
#define SDT_FEATURE_EQ(         Data1, Data2 ) SDT_DATA_EQ(          Data1, Data2 )
#define SDT_FEATURE_GTE(        Data1, Data2 ) SDT_DATA_GTE(         Data1, Data2 )
#define SDT_FEATURE_GT(         Data1, Data2 ) SDT_DATA_GT(          Data1, Data2 )

#else /* #if ( 0 == ( SDT_CONFIG_ENCODING & SDT_API_ENCODING_EXCLUDE_DATA ) ) */

#define SDT_FEATURE_DECLARE_VAR( FeatureAlias, VariableName ) \
    SDT_FEATURE_LONG  VariableName

#define SDT_FEATURE_TEST_DATA( Data, TestSettings )         

#define SDT_FEATURE_GET(        Data1 )        ( Data1 )       
#define SDT_FEATURE_SET(        Data1, Value ) ( Data1 =  Value )
#define SDT_FEATURE_SET_CONST(  Data1, Const ) ( Data1 =  Const )
#define SDT_FEATURE_MOV(        Data1, Data2 ) ( Data1 =  Data2 )
#define SDT_FEATURE_ADD(        Data1, Data2 ) ( Data1 += Data2 )
#define SDT_FEATURE_SUB(        Data1, Data2 ) ( Data1 -= Data2 )
#define SDT_FEATURE_MUL(        Data1, Data2 ) ( Data1 *= Data2 )
#define SDT_FEATURE_DIV(        Data1, Data2 ) ( Data1 /= Data2 )
#define SDT_FEATURE_MOD(        Data1, Data2 ) ( Data1 %= Data2 )
#define SDT_FEATURE_AND(        Data1, Data2 ) ( Data1 &= Data2 )
#define SDT_FEATURE_OR(         Data1, Data2 ) ( Data1 |= Data2 )
#define SDT_FEATURE_XOR(        Data1, Data2 ) ( Data1 ^= Data2 )
#define SDT_FEATURE_SHR(        Data1, Data2 ) ( Data1 >>=Data2 )
#define SDT_FEATURE_SHL(        Data1, Data2 ) ( Data1 <<=Data2 )
#define SDT_FEATURE_LT(         Data1, Data2 ) ( Data1 <  Data2 )
#define SDT_FEATURE_LTE(        Data1, Data2 ) ( Data1 <= Data2 )
#define SDT_FEATURE_EQ(         Data1, Data2 ) ( Data1 == Data2 )
#define SDT_FEATURE_GTE(        Data1, Data2 ) ( Data1 >= Data2 )
#define SDT_FEATURE_GT(         Data1, Data2 ) ( Data1 >  Data2 )

#endif /* #if ( 0 == ( SDT_CONFIG_ENCODING & SDT_API_ENCODING_EXCLUDE_DATA ) ) */


#define SDT_FEATURE_DEFINE( Name, FeatureIdentity, FeatureAlias )	\
    enum SDT_FEATURE_TOKENPASTE(SDT_Enum_,FeatureAlias) {	\
	SDT_FEATURE_TOKENPASTE(SDT_FeatureId_,FeatureAlias) = FeatureIdentity,\
	SDT_FEATURE_TOKENPASTE(SDT_EncoderId_,FeatureAlias) = FeatureIdentity%SDT_ENCODER_USER_LIMIT\
	};\
	static const char SDT_FEATURE_TOKENPASTE(SDT_FeatureName_,FeatureAlias) [] SDT_FEATURE_GCC_NODEADSTRIP = Name

#endif /* C API */


/* Feature implementation API. */

/* There are cases (thanks to over-enthusiastic linkers) where preptool can't calculate the address of the string referenced by FeatureNamePtr
 * (the relevant relocation entry just isn't there with Solaris -zcombreloc option, for instance). In these cases, we need to use 
 * the SDT_FEATURE_IMPLEMENT_EX macro. It's also the only alternative on some platforms : x86_64 on Mac OS X, for instance.
 * The previous style is maintained for backwards compatibility.
 */

#if defined(__APPLE__) && ( defined(__x86_64__) || defined(__ppc64__) )
#	define	REQUIRE_FEATURE_EXTENDED		1
#else
#	define	REQUIRE_FEATURE_EXTENDED		0
#endif

#define SDT_FEATURE_NAME_MAX_SZ						32

struct _FeatureData
	{
	uint8_t		Signature[8];
	uint32_t	EncoderId;
	uint32_t	FeatureId;
	uintptr_t	FeatureNamePtr;
	char		FeatureName[ SDT_FEATURE_NAME_MAX_SZ ];
	};

#define SDT_FEATURE_IMPLEMENT_BEGIN() \
volatile struct _FeatureData SDT_FeatureData [] SDT_FEATURE_GCC_NODEADSTRIP = 		\
	{ 																				\
		{ { 0xB7, 0xB0, 0xB3, 0xB3, 0xB0, 0xA8, 0xBE, 0xA6 }, 0, 0, 0 }, 

#if ! REQUIRE_FEATURE_EXTENDED
#	define SDT_FEATURE_IMPLEMENT( FeatureAlias ) \
		{ { 0xB7, 0xB0, 0xB3, 0xB3, 0xB0, 0xA8, 0xBE, 0xA8 }, SDT_FEATURE_TOKENPASTE(SDT_EncoderId_,FeatureAlias),SDT_FEATURE_TOKENPASTE(SDT_FeatureId_,FeatureAlias),(uintptr_t)SDT_FEATURE_TOKENPASTE(SDT_FeatureName_,FeatureAlias)},
#else
#	define SDT_FEATURE_IMPLEMENT( FeatureAlias )	Please use SDT_FEATURE_IMPLEMENT_EX instead.
#endif

#define SDT_FEATURE_IMPLEMENT_EX( FeatureAlias, FeatureName ) 															\
	{ { 0xB7, 0xB0, 0xB3, 0xB3, 0xB0, 0xA8, 0xBE, 0xAA },SDT_FEATURE_TOKENPASTE(SDT_EncoderId_,FeatureAlias),SDT_FEATURE_TOKENPASTE(SDT_FeatureId_,FeatureAlias),  	\
		0xc0ffc0ff, FeatureName },

#define SDT_FEATURE_IMPLEMENT_C( FeatureAlias, Name ) SDT_FEATURE_IMPLEMENT_EX( FeatureAlias, Name )

#define SDT_FEATURE_IMPLEMENT_END() { { 0xB7, 0xB0, 0xB3, 0xB3, 0xB0, 0xA8, 0xBE, 0xA7 }, 0, 0, 0 } };

#define SDT_FEATURE_GET_NAME( FeatureAlias ) (SDT_FEATURE_TOKENPASTE(SDT_FeatureName_,FeatureAlias))

#endif /* !defined(SDT_FEATURE_INCLUDED) */
