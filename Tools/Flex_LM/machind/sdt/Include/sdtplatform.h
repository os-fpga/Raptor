/**************************************************************************************************
* Copyright (c) 1997-2016, 2020-2021 Flexera. All Rights Reserved.
**************************************************************************************************/

#if !defined( sdtplatform_H )
#define sdtplatform_H

/***************************************************************************************************
* Macro helpers
**************************************************************************************************/
#if !defined(FNP_STRINGIZE)
#	define  FNP_STRINGIZE(N)   		FNP_STRINGIZE_X(N) 
#	define  FNP_STRINGIZE_X(N)   	#N
#endif 

/***************************************************************************************************
 * Determine the compiler/instruction set combination. Supported combinations that don't use 
 * code markers map to a generic setting.
 **************************************************************************************************/

#if defined(_MSC_VER)
#	define SDT_COMPILER_MICROSOFT
#	if defined(_M_IX86)
		/* Visual C++ : x86 */
#		define SDT_COMPILER_MICROSOFT_X86
#		define SDT_PROCESSOR_X86
#	elif defined(_M_AMD64)
		/* Visual C++ : x86_64 */
#		define SDT_COMPILER_MICROSOFT_X86_64
#		define SDT_PROCESSOR_X86_64
#	elif defined(_M_IA64)
		/* Visual C++ : Itanium. SDT_COMPILER_MICROSOFT_ITANIUM ==> SDT_COMPILER_GENERIC. */
#		define SDT_COMPILER_MICROSOFT_ITANIUM
#		define SDT_PROCESSOR_ITANIUM
#	else
		/* Visual C++ : unknown, untested architecture. */
#		error Unsupported compiler
#	endif
#elif defined(__GNUC__)
#	define SDT_COMPILER_GCC
#	if defined(__i386__)
		/* gcc : x86. */
#		define SDT_COMPILER_GNU_X86
#		define SDT_PROCESSOR_X86
#	elif defined(__x86_64__) || defined(__amd64__) || defined(__arm64__)
		/* gcc : x86_64. SDT_COMPILER_GNU_X86_64 ==> SDT_COMPILER_GENERIC. */
#		define SDT_COMPILER_GENERIC
#		define SDT_PROCESSOR_X86_64
#	elif defined(__sparc__)
		/* gcc : Sparc. */
#		define SDT_COMPILER_GNU_SPARC
#		define SDT_PROCESSOR_SPARC
#   elif defined(__ppc64__) || defined(__powerpc64__)
        /* gcc : PowerPC 64. */
#       define SDT_COMPILER_GENERIC
#       define SDT_PROCESSOR_PPC
#	elif defined(__ppc__) || defined(__powerpc__)
		/* gcc : PowerPC. */
#		define SDT_COMPILER_GNU_PPC
#		define SDT_PROCESSOR_PPC
#	elif defined(__ia64__)
		/* gcc : Itanium. SDT_COMPILER_GNU_ITANIUM ===> SDT_COMPILER_GENERIC. */
#		define SDT_COMPILER_GENERIC
#		define SDT_PROCESSOR_ITANIUM
#	else
		/* Other architectures aren't on the Tamper Resistance list. */
#		error Unsupported compiler
#	endif
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#	define SDT_COMPILER_SUNPRO
#	if defined(__sparc)
		/* SunPRO compiler: Sparc. */
#		define SDT_COMPILER_SUN_SPARC
#		define SDT_PROCESSOR_SPARC
#	elif defined(__i386)
		/* SunPRO compiler: X86. SDT_COMPILER_SUN_X86 ==> SDT_COMPILER_GENERIC */
#		define SDT_COMPILER_GENERIC
#		define SDT_PROCESSOR_X86
#	elif defined(__x86_64) || defined(__amd64)
		/* SunPRO compiler: X86. SDT_COMPILER_SUN_X86 ==> SDT_COMPILER_GENERIC */
#		define SDT_COMPILER_GENERIC
#		define SDT_PROCESSOR_X86_64
#	else
		/* Mapped to unsupported until tested. */
#		error Unsupported compiler
#	endif
#elif defined(__MWERKS__)
#	define SDT_COMPILER_METROWERKS
#	if defined(__ppc__) || defined(__powerpc__)
		/* Metrowerks: PowerPC. */
#		define SDT_COMPILER_METROWERKS_PPC
#		define SDT_PROCESSOR_PPC
#	else
#		error Unsupported compiler
#	endif
#endif

/***************************************************************************************************
 * Determine integral types for OSes that require them.
**************************************************************************************************/
#if defined(__linux__) 
#	include <stdint.h>
	typedef uintptr_t		SDT_UINTPTR;
#elif ( defined(__SVR4) && defined(__sun) )
#	include <inttypes.h>
	typedef uintptr_t		SDT_UINTPTR;
#elif defined(__APPLE__)
#	include <stdint.h>
	typedef uintptr_t		SDT_UINTPTR;
#elif defined(_WIN32)
	typedef unsigned		SDT_UINTPTR;
#endif

/***************************************************************************************************
* Declare underlying SDT types
**************************************************************************************************/
typedef int8_t      SDT_BASE_INT8;
typedef uint8_t     SDT_BASE_UINT8;
typedef int16_t     SDT_BASE_INT16;
typedef uint16_t    SDT_BASE_UINT16;
typedef int32_t     SDT_BASE_INT32;
typedef uint32_t    SDT_BASE_UINT32;
typedef int64_t     SDT_BASE_INT64;
typedef uint64_t    SDT_BASE_UINT64;

/***************************************************************************************************
* Declare code markers. For Mac OS X Universal builds, endian-swapped variants may need to be 
* declared. Universal builds are only done with gcc, so testing __BIG_ENDIAN__ is appropriate.
**************************************************************************************************/

/* X86 code markers. */
#if __BIG_ENDIAN__
	/* Swapped. */
#	define SDT_PLATFORM_START_X86		0xcccdcecf
#	define SDT_PLATFORM_END_X86			0xcbcccdce
#	define SDT_PLATFORM_ERASE_X86		0x1000000
#else
#	define SDT_PLATFORM_START_X86		0xcfcecdcc
#	define SDT_PLATFORM_END_X86			0xcecdcccb
#	define SDT_PLATFORM_ERASE_X86		0x1
#endif

/* PowerPC code markers */
#if __BIG_ENDIAN__ || ( defined(__MWERKS__) && defined(__ppc__))
#	define SDT_PLATFORM_START_PPC		0x60a5aabb
#	define SDT_PLATFORM_END_PPC			0x60e5eeff
#	define SDT_PLATFORM_ERASE_PPC		0x7ca53a78
#else
	/* Swapped */
#	define SDT_PLATFORM_START_PPC		0xbbaaa560
#	define SDT_PLATFORM_END_PPC			0xffeee560
#	define SDT_PLATFORM_ERASE_PPC		0x783aa57c
#endif

/* Sparc code markers. */
#define SDT_PLATFORM_START_SPARC		0x0333f3b3
#define SDT_PLATFORM_END_SPARC			0x0333b373
#define SDT_PLATFORM_ERASE_SPARC		0xb0100001

/* Default code markers. */
#define SDT_PLATFORM_START_DEFAULT		0xcfcecdcc
#define SDT_PLATFORM_END_DEFAULT		0xcecdcccb
#define SDT_PLATFORM_ERASE_DEFAULT		0x1

/* Choose appropriate code marker for current architecture. Code markers exist for i386, ppc, sparc.
 * Code handling cross-prep scenarios should avoid these defines.
 */

#if defined(SDT_PROCESSOR_X86)
#	define SDT_PLATFORM_START			SDT_PLATFORM_START_X86
#	define SDT_PLATFORM_END				SDT_PLATFORM_END_X86
#	define SDT_PLATFORM_ERASE			SDT_PLATFORM_ERASE_X86
#elif defined(SDT_PROCESSOR_PPC)
#	define SDT_PLATFORM_START			SDT_PLATFORM_START_PPC
#	define SDT_PLATFORM_END				SDT_PLATFORM_END_PPC
#	define SDT_PLATFORM_ERASE			SDT_PLATFORM_ERASE_PPC
#elif defined(SDT_PROCESSOR_SPARC)
#	define SDT_PLATFORM_START			SDT_PLATFORM_START_SPARC
#	define SDT_PLATFORM_END				SDT_PLATFORM_END_SPARC
#	define SDT_PLATFORM_ERASE			SDT_PLATFORM_ERASE_SPARC
#else
#	define SDT_PLATFORM_START			SDT_PLATFORM_START_DEFAULT
#	define SDT_PLATFORM_END				SDT_PLATFORM_END_DEFAULT
#	define SDT_PLATFORM_ERASE			SDT_PLATFORM_ERASE_DEFAULT
#endif

#define SDT_PLATFORM_NULL     			0x0
#define SDT_PLATFORM_CHECK    			0x2

/***************************************************************************************************
* Compiler-specific helpers.
**************************************************************************************************/

#if defined(SDT_COMPILER_MICROSOFT)
	/* Windows.h provides ReadTimeStampCounter and GetCallersEflags. */
#	include <windows.h>

	/* A compile time assert which MS is happy with. */
#	define SDT_STATIC_ASSERT( expr ) { unsigned char CTAssert[ (expr) ? 1 : 0 ] = { 0 }; }
#endif

#if defined(SDT_COMPILER_GCC)
#	include <stdlib.h>					/* for abort( ) */
#	include <time.h>

	/* Compile time assert which GCC is happy with. */
#	define SDT_STATIC_ASSERT( expr ) { unsigned char CTAssert[ (expr) ? 1 : 0 ] __attribute__((unused)) = { 0 }; }
#endif

#if defined(SDT_COMPILER_SUNPRO)
#	include <time.h>
#	define SDT_STATIC_ASSERT( expr ) { unsigned char CTAssert[ (expr) ? 1 : 0 ] = { 0 }; }
#endif

#if defined(SDT_COMPILER_METROWERKS)
#	define SDT_STATIC_ASSERT( expr ) { unsigned char CTAssert[ (expr) ? 1 : 0 ] __attribute__((unused)) = { 0 }; }
#endif

/***************************************************************************************************
* Handle each compiler/processor case.
**************************************************************************************************/

#if defined(SDT_COMPILER_MICROSOFT_X86)
	/***************************************************************************************************
	 * Microsoft VC++ : X86.
	 **************************************************************************************************/

#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) 				\
		__asm { rdtsc };												\
		__asm { mov  TimeStampLoWord , eax };

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) 					\
		__asm { mov  EspWord , esp };

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord ) 					\
		__asm { mov  EbpWord, ebp };

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) 						\
		__asm { push    eax }; 											\
		__asm { pushfd }; 												\
		__asm { pop     eax }; 											\
		__asm { mov     EFlagsWord, eax }; 								\
		__asm { pop     eax };

#	define SDT_PLATFORM_DEFINE_START( Attributes  ) 					\
		__asm { push       eax };										\
		__asm { mov        eax, SDT_PLATFORM_START_X86 };				\
		__asm { SDT_PLATFORM_LABEL: };									\
		__asm { mov        eax, SDT_PLATFORM_LABEL };					\
		__asm { mov        eax, Attributes };							\
		__asm { mov        eax, 0 };									\
		__asm { mov        eax, SDT_PLATFORM_START_X86 };				\
		__asm { pop        eax };

#	define SDT_PLATFORM_DEFINE_END( ) 									\
		__asm { push       eax };										\
		__asm { mov        eax, SDT_PLATFORM_START_X86 };				\
		__asm { mov        eax, SDT_PLATFORM_LABEL };					\
		__asm { mov        eax, 0 };									\
		__asm { mov        eax, 0 };									\
		__asm { mov        eax, SDT_PLATFORM_END_X86 };					\
		__asm { pop        eax };

#	if	_MSC_VER >= 1300
#		define SDT_PLATFORM_BREAKPOINT() __debugbreak()
#	else
#		define SDT_PLATFORM_BREAKPOINT() __asm { int 3 }
#	endif

#elif defined(SDT_COMPILER_MICROSOFT_X86_64)
	/***************************************************************************************************
	 * Microsoft VC++ : X86 64 bit.
	 **************************************************************************************************/

#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) 				\
		TimeStampLoWord = (SDT_BASE_UINT32)(ReadTimeStampCounter() & 0xFFFFFFFF);

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) 					\
		EspWord = (SDT_BASE_UINT32)(SDT_BASE_UINT64)&EspWord

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord ) 					\
		EbpWord = (SDT_BASE_UINT32)(SDT_BASE_UINT64)&EbpWord

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) 						\
		EFlagsWord = GetCallersEflags();

#	define SDT_PLATFORM_DEFINE_START( Attributes  )

#	define SDT_PLATFORM_DEFINE_END( )

#	define SDT_PLATFORM_BREAKPOINT() __debugbreak()

#	ifndef SDT_DISABLE_UNPREPPED
#		define SDT_DISABLE_UNPREPPED 	1
#	endif

#elif defined(SDT_COMPILER_MICROSOFT_ITANIUM)
	/***************************************************************************************************
	 * Microsoft VC++ : Itanium.
	 **************************************************************************************************/

#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) 				\
		TimeStampLoWord = (SDT_BASE_UINT32) GetTickCount( );

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) 					\
		EspWord = (SDT_BASE_UINT32)(SDT_UINTPTR)&EspWord

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord ) 					\
		EbpWord = (SDT_BASE_UINT32)(SDT_UINTPTR)&EbpWord

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) 						\
		EFlagsWord = (SDT_BASE_UINT32)(SDT_UINTPTR)&EFlagsWord

#	define SDT_PLATFORM_DEFINE_START( Attributes )

#	define SDT_PLATFORM_DEFINE_END( )

#	define SDT_PLATFORM_BREAKPOINT() __debugbreak()

#	ifndef SDT_DISABLE_UNPREPPED
#		define SDT_DISABLE_UNPREPPED	1
#	endif

#elif defined(SDT_COMPILER_GNU_X86)
	/***************************************************************************************************
	 * GCC: X86. Code markers have been deprecated for this compiler due to inlining problems. 
	 **************************************************************************************************/

	static inline long long RDTSC( ) 
	{
		register long long TSC __asm__("eax");
		__asm__ volatile (".byte 15, 49" : : : "eax", "edx");
		return TSC;
	}

#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) TimeStampLoWord = (SDT_BASE_UINT32) RDTSC( )

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) 					\
		__asm__ volatile ("movl %%esp, %0\n" :"=r"(EspWord) );

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord ) 					\
		__asm__ volatile ("movl %%ebp, %0\n" :"=r"(EbpWord) );

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) 						\
		__asm__ volatile ( "push    %eax" ); 							\
		__asm__ volatile ( ".byte 0x9c" ); 								\
		__asm__ volatile ( "pop     %eax" ); 							\
		__asm__ volatile ( "movl    %%eax, %0" : "=r"(EFlagsWord) );	\
		__asm__ volatile ( "pop     %eax" );

/* No code markers -- they don't work well when gcc inlines. */
#	define SDT_PLATFORM_DEFINE_START( Attributes )
#	define SDT_PLATFORM_DEFINE_END( )

#	define SDT_PLATFORM_BREAKPOINT() __asm__ volatile ( ".byte 0xCC " )

#	ifndef SDT_DISABLE_UNPREPPED
#		define SDT_DISABLE_UNPREPPED 	1
#	endif

#elif defined(SDT_COMPILER_GNU_PPC)
	/***************************************************************************************************
	 * GCC: PowerPC.
	 **************************************************************************************************/

#	if defined(__APPLE__)
#		include <ppc_intrinsics.h>
#	endif

#	define SDT_ASM_OPCODE( op_ )			asm volatile(".long " FNP_STRINGIZE(op_))

	static __inline__ SDT_BASE_UINT32 mftb_glue( void ) __attribute__((always_inline));

	static __inline__ SDT_BASE_UINT32 mftb_glue( void )
	{
		register SDT_BASE_UINT32	result = 0;
		__asm__ volatile ( "mfspr %0, 268" : "=r" (result) );
		return result;
	}

#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) TimeStampLoWord = (SDT_BASE_UINT32) mftb_glue( )

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) \
		do { char c; EspWord = reinterpret_cast<uintptr_t>(&c) + 0; } while (0)

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord ) \
		__asm__ volatile ("mflr %0" :"=r"(EbpWord) );

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) \
		EFlagsWord = (SDT_BASE_UINT32)(SDT_UINTPTR)&EFlagsWord

#	define SDT_PLATFORM_DEFINE_START( Attributes )				 			\
		SDT_ASM_OPCODE( 0x4800001C );				/* branch  */			\
		SDT_ASM_OPCODE( SDT_PLATFORM_START_PPC );	/* TypeStart */			\
		SDT_ASM_OPCODE( 0xf000feeb );				/* Unique1 */			\
		SDT_ASM_OPCODE( 0x60000000 );				/* Label */				\
		SDT_ASM_OPCODE( 0xfeebcafe );				/* Unique2 */			\
		SDT_ASM_OPCODE( Attributes );				/* Attributes */		\
		SDT_ASM_OPCODE(SDT_PLATFORM_START_PPC )		/* TypeID */

#	define SDT_PLATFORM_DEFINE_END( )							 			\
		SDT_ASM_OPCODE( 0x4800001C );				/* branch  */			\
		SDT_ASM_OPCODE( SDT_PLATFORM_START_PPC );	/* TypeStart */			\
		SDT_ASM_OPCODE( 0xf000feeb );				/* Unique1 */			\
		SDT_ASM_OPCODE( 0x60000000 );				/* Label */				\
		SDT_ASM_OPCODE( 0xfeebcafe );				/* Unique2 */			\
		SDT_ASM_OPCODE( 0x60000000 );				/* Attributes */		\
		SDT_ASM_OPCODE( SDT_PLATFORM_END_PPC )		/* TypeID */

#	define SDT_PLATFORM_BREAKPOINT() __asm__ volatile ( ".long 0x7fe00008" )

#elif defined(SDT_COMPILER_GNU_SPARC)
	/***************************************************************************************************
	 * GCC: Sparc.
	 **************************************************************************************************/

	static __inline__ volatile long long RDTSC() 
	{
		register long long TSC;
		__asm__ volatile("mov %%l1, %0" : "=r" (TSC));
		__asm__ volatile("and %%sp, %0, %1" : "=r" (TSC) : "r" (TSC));
		return TSC;
	}

#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) TimeStampLoWord = (SDT_BASE_UINT32)RDTSC()

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) 				\
		__asm__ volatile ("mov %%sp, %0" :"=r"(EspWord) );

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord ) 				\
		__asm__ volatile ("mov %%fp, %0" :"=r"(EbpWord) );

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) 						\
		EFlagsWord = (SDT_BASE_UINT32)(SDT_UINTPTR)&EFlagsWord

#	define SDT_PLATFORM_DEFINE_START( Attributes  ) 				\
	__asm__ volatile (".long 0x0333f3b3"); 							\
	__asm__ volatile (".long 0x821061cc"); 							\
	__asm__ volatile (".long 0xa2100001"); 							\
	__asm__ volatile (".long 0xb0100001"); 							\
	__asm__ volatile (".long 0x01000000"); 							\
	__asm__ volatile (".long 0x0333f3b3");

#	define SDT_PLATFORM_DEFINE_END() 								\
	__asm__ volatile (".long 0x0333f3b3"); 							\
	__asm__ volatile (".long 0x821061cc"); 							\
	__asm__ volatile (".long 0xa2100001"); 							\
	__asm__ volatile (".long 0xb0100001"); 							\
	__asm__ volatile (".long 0x01000000"); 							\
	__asm__ volatile (".long 0x0333b373");

#	define SDT_PLATFORM_BREAKPOINT() __asm__ volatile ( ".long 0x000000ff" )

#elif defined(SDT_COMPILER_SUN_SPARC)
	/***************************************************************************************************
	 * Sun C/C++ : Sparc.
	 **************************************************************************************************/

	/* Depending on which version of the compiler is in use, you may have to use asm or __asm. The older
	 * asm is chosen here, with a macro override for when it isn't appropriate. The use of asm here is
	 * reasonably trivial, as .il use isn't really appropriate.
	 */
	
#	if !defined(SDT_ASM_SUNPRO)
#		define SDT_ASM_SUNPRO( str_ )		asm( str_ )
#	endif

#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) TimeStampLoWord = clock( )

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) EspWord = (SDT_BASE_UINT32)((SDT_UINTPTR)&EspWord)

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord )  EbpWord = (SDT_BASE_UINT32)((SDT_UINTPTR)&EbpWord)

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) EFlagsWord = time(0)

#	define SDT_PLATFORM_DEFINE_START( Attributes  ) 				\
	SDT_ASM_SUNPRO (".long 0x0333f3b3"); 							\
	SDT_ASM_SUNPRO (".long 0x821061cc"); 							\
	SDT_ASM_SUNPRO (".long 0xa2100001"); 							\
	SDT_ASM_SUNPRO (".long 0xb0100001"); 							\
	SDT_ASM_SUNPRO (".long 0x01000000"); 							\
	SDT_ASM_SUNPRO (".long 0x0333f3b3");

#	define SDT_PLATFORM_DEFINE_END() 								\
	SDT_ASM_SUNPRO (".long 0x0333f3b3"); 							\
	SDT_ASM_SUNPRO (".long 0x821061cc"); 							\
	SDT_ASM_SUNPRO (".long 0xa2100001"); 							\
	SDT_ASM_SUNPRO (".long 0xb0100001"); 							\
	SDT_ASM_SUNPRO (".long 0x01000000"); 							\
	SDT_ASM_SUNPRO (".long 0x0333b373");

#	define SDT_PLATFORM_BREAKPOINT() abort( )

#elif defined(SDT_COMPILER_METROWERKS_PPC)
	/***************************************************************************************************
	 * Metrowerks CodeWarrior: PowerPC.
	 **************************************************************************************************/

	static inline SDT_BASE_UINT32 __mftb( void )
	{
		register long	result;
		asm volatile { mfspr result, 268; } 
		return result;
	}

#	define SDT_ASM_OPCODE( op_ )		asm volatile { opword op_ }

#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) TimeStampLoWord = (SDT_BASE_UINT32)__mftb( )

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) 				\
		do { char c; EspWord = reinterpret_cast<uintptr_t>(&c) + 0; } while (0)

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord ) 				\
		__asm__ volatile ("mflr %0" :"=r"(EbpWord) );

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) 					\
		EFlagsWord = (SDT_BASE_UINT32)&EFlagsWord

/* No code markers -- they don't work well with CodeWarrior's inlining. */
#	define SDT_PLATFORM_DEFINE_START( Attributes )
#	define SDT_PLATFORM_DEFINE_END( )

#	define SDT_PLATFORM_BREAKPOINT() SDT_ASM_OPCODE ( 0x7fe00008 )

#	ifndef SDT_DISABLE_UNPREPPED
#		define SDT_DISABLE_UNPREPPED 	1
#	endif

#elif defined(SDT_COMPILER_GENERIC)
	/***************************************************************************************************
	 * Generic compiler/instruction set.
	 **************************************************************************************************/
	
#	define SDT_PLATFORM_READ_TIME_STAMP( TimeStampLoWord ) TimeStampLoWord = clock()

#	define SDT_PLATFORM_READ_STACK_REGISTER( EspWord ) EspWord = static_cast<SDT_BASE_UINT32>(reinterpret_cast<uintptr_t>(&EspWord))

#	define SDT_PLATFORM_READ_BASE_REGISTER( EbpWord )  EbpWord = static_cast<SDT_BASE_UINT32>(reinterpret_cast<uintptr_t>(&EbpWord))

#	define SDT_PLATFORM_READ_FLAGS( EFlagsWord ) EFlagsWord = time(0)

#	define SDT_PLATFORM_DEFINE_START( Attributes ) 

#	define SDT_PLATFORM_DEFINE_END( )

#	define SDT_PLATFORM_BREAKPOINT() abort( )

#	ifndef SDT_DISABLE_UNPREPPED
#		define SDT_DISABLE_UNPREPPED 	1
#	endif
#else
#	error Unknown compiler/instruction set combination.
#endif

/***************************************************************************************************
* If SDT_DISABLE_UNPREPPED has not yet been defined, assume that unprepped SDTs are supported.
**************************************************************************************************/

#if !defined( SDT_DISABLE_UNPREPPED )
#	define SDT_DISABLE_UNPREPPED 	0
#endif

/***************************************************************************************************
* Define the appropriate information for supporting 64-bit values
**************************************************************************************************/

typedef SDT_BASE_UINT32 	SDT_CORE_TYPE;

typedef SDT_BASE_INT32 		SDT_ENCODED_TYPE;

#define SDT_DATA_TYPE_ID 	9

#endif /* sdtplatform_H */
