/**************************************************************************************************/
/*!@file
 * @brief A brief description goes here
 *
 * Detailed description goes here
 *
 * @version \$Revision: #1 $
 */
/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
#if !defined( FNP_PLATFORMTYPES_H )
#define FNP_PLATFORMTYPES_H

#if defined(__linux__) 
#	define FNPPUB_HAVE_STDINT_H 1

#elif ( defined(__SVR4) && defined(__sun) )
#	define FNPPUB_HAVE_INTTYPES_H 			1

#elif defined(__APPLE__)
#	define FNPPUB_HAVE_STDINT_H 1
#	define FNPPUB_HAVE_APPLEINSTALLER_H    	1

#elif defined(_WIN32)
#	define FNPPUB_HAVE_WINSTDINT_H         	1
#	define FNPPUB_HAVE_WININSTALLER_H      	1
    
#elif defined(__CYGWIN__)
#	define FNPPUB_HAVE_STDINT_H            	1
#	define FNPPUB_HAVE_WININSTALLER_H      	1

#elif defined(_AIX)
#	define FNPPUB_HAVE_STDINT_H            	1
#elif defined(__sgi__)
#	define FNPPUB_HAVE_INTTYPES_H 			1
#elif defined(__hpux__)
#	define FNPPUB_HAVE_INTTYPES_H 			1
#else
#	error Unknown platform
#endif

#ifndef FNPPUB_HAVE_STDINT_H
#	define FNPPUB_HAVE_STDINT_H 0
#endif

#ifndef FNPPUB_HAVE_INTTYPES_H
#	define FNPPUB_HAVE_INTTYPES_H 0
#endif

#ifndef FNPPUB_HAVE_WINSTDINT_H
#	define FNPPUB_HAVE_WINSTDINT_H 0
#endif

#ifndef FNPPUB_HAVE_APPLEINSTALLER_H
#	define FNPPUB_HAVE_APPLEINSTALLER_H 0
#endif

#ifndef FNPPUB_WININSTALLER_H
#	define FNPPUB_WININSTALLER_H 0
#endif


#endif /* FNP_PLATFORMTYPES_H */
