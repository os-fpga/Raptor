/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Description: Definitions for the various license manager platforms.
 *
 *
 */

#ifndef _LM_HOSTTYPE_H
#define _LM_HOSTTYPE_H

/*
 *	Computer manufacturer
 */
#define LM_MANUFAC	0xff0000	/* Manufacturer mask */

#define LM_UNKNOWN	0		/* Unknown manufacturer/machine */
#define LM_SUN		0x10000
#define LM_DEC		0x20000
#define LM_HP		0x30000
#define LM_APOLLO	0x40000
#define LM_SGI		0x50000
#define LM_MIPS		0x60000
#define LM_DG		0x70000
#define LM_NEXT		0x80000
#define LM_IBM		0x90000
#define LM_CONVEX	0xa0000
#define LM_MOTOROLA	0xb0000
#define LM_SCO_UNIX	0xc0000
#define LM_CRAY		0xd0000
#define LM_SNI          0xe0000         /* Siemens Nixdorf Info Systems */
#define LM_NCR          0xf0000
#define LM_NEC         0x100000
#define LM_HAL         0x200000
#define LM_LINUX       0x400000
#define LM_ALPHA_LINUX 0x800000
#define LM_MAC		0x1000000


/*
 *	Product line, within a manufacturer
 */
#define LM_PRODUCT_LINE 0xff00		/* Product line mask */
/*
 *	CPU, within a product line
 */
#define LM_MACHINE	0xff		/* Machine mask */


/*
 *	Host types for "code" member of lm_hosttype
 */

/*	S U N		*/


#define LM_SUN3		0x100		/* Generic SUN3 */
#define LM_SUN4		0x200		/* Generic SUN4 */
#define LM_SUN386	0x300		/* Generic SUN386 */
#define LM_SOLBOURNE	0x400		/* Generic SOLBOURNE */
#define LM_SUNX86	(LM_SUN | 0x500	) /* Intel 86 */

#define LM_SUN3_75	( LM_SUN | LM_SUN3 | 0x1)
#define LM_SUN3_50	( LM_SUN | LM_SUN3 | 0x2)
#define LM_SUN3_260	( LM_SUN | LM_SUN3 | 0x3)
#define LM_SUN3_470	( LM_SUN | LM_SUN3 | 0x4)
#define LM_SUN3_80	( LM_SUN | LM_SUN3 | 0x5)
#define LM_SUN3_60	( LM_SUN | LM_SUN3 | 0x6)
#define LM_SUN3_110	( LM_SUN | LM_SUN3 | 0x7)
#define LM_SUN3_E	( LM_SUN | LM_SUN3 | 0x8)
#define LM_SUN3_460	( LM_SUN | LM_SUN3 | 0x9)

#define LM_SUN4_3xx	( LM_SUN | LM_SUN4 | 0x1)
#define LM_SUN4_260	( LM_SUN | LM_SUN4 | 0x2)
#define LM_SUN4_60	( LM_SUN | LM_SUN4 | 0x3)
#define LM_SUN4_65	( LM_SUN | LM_SUN4 | 0x4)
#define LM_SUN4_470	( LM_SUN | LM_SUN4 | 0x5)
#define LM_SUN4_40	( LM_SUN | LM_SUN4 | 0x6)
#define LM_SUN4_20	( LM_SUN | LM_SUN4 | 0x7)
#define LM_SUN4_E	( LM_SUN | LM_SUN4 | 0x8)
#define LM_SUN4_110	( LM_SUN | LM_SUN4 | 0x9)
#define LM_SUN4C	( LM_SUN | LM_SUN4 | 0xa)

#define LM_SUN386I	( LM_SUN | LM_SUN386 | 0x1)

/*	D E C		*/

#define LM_VAX		0x100		/* GENERIC Vax */
#define LM_DECSTATION	0x200		/* Generic DECstation */
#define LM_ALPHA	0x300		/* Generic Alpha */
#define LM_ALPHA_OSF	(LM_ALPHA | 0x1) /* Generic Alpha */
#define LM_ALPHA_WINNT	(LM_ALPHA | 0x1) /* Generic Alpha */

/*	H P		*/


/*	A P O L L O	*/

#define LM_DN3000	(LM_APOLLO | 0x1)
#define LM_DN4000	(LM_APOLLO | 0x2)
#define LM_DN3500	(LM_APOLLO | 0x3)
#define LM_DN4500	(LM_APOLLO | 0x4)
#define LM_DN10000	(LM_APOLLO | 0x5)

/*	S G I	 	*/


/*	M I P S	 	*/


/*	D G	 	*/

#define LM_AVIION	0x100
#define LM_AV3200	(LM_DG | LM_AVIION | 0x1)
#define LM_AV4000	(LM_DG | LM_AVIION | 0x2)
#define LM_AV4100	(LM_DG | LM_AVIION | 0x3)
#define LM_AV4020	(LM_DG | LM_AVIION | 0x4)
#define LM_AV4120	(LM_DG | LM_AVIION | 0x5)
#define LM_AV5010	(LM_DG | LM_AVIION | 0x6)
#define LM_AV5200	(LM_DG | LM_AVIION | 0x7)
#define LM_AV5220	(LM_DG | LM_AVIION | 0x8)
#define LM_AV6200	(LM_DG | LM_AVIION | 0x9)
#define LM_AV6220	(LM_DG | LM_AVIION | 0xa)
#define LM_AV6200_20	(LM_DG | LM_AVIION | 0xb)
#define LM_AV6220_20	(LM_DG | LM_AVIION | 0xc)
#define LM_AV200	(LM_DG | LM_AVIION | 0xd)
#define LM_AV300	(LM_DG | LM_AVIION | 0xe)
#define LM_AV310	(LM_DG | LM_AVIION | 0xf)
#define LM_AV400	(LM_DG | LM_AVIION | 0x10)
#define LM_AV402	(LM_DG | LM_AVIION | 0x11)
#define LM_AV410	(LM_DG | LM_AVIION | 0x12)
#define LM_AV412	(LM_DG | LM_AVIION | 0x13)

/*	N E X T	 	*/


/*	I B M	 	*/

#define LM_IBMRISC	0x100
#define LM_IBMPC	0x200
#define LM_PCRT		(LM_IBM | LM_IBMRISC | 0x1)
#define LM_RS6000	(LM_IBM | LM_IBMRISC | 0x2)
#define LM_MSDOS	(LM_IBM | LM_IBMPC | 0x1)

/* Misc I86 OS host types */
#define LM_I86_OBSD	(LM_UNKNOWN | LM_IBMPC | 0x1)

/*	C O N V E X	*/

/*	M O T O R O L A  */

#define LM_MOT88K	0x100
#define LM_MOTSVR4	0x200
#define LM_MOTO88K	(LM_MOTOROLA | LM_MOT88K)
#define LM_MOTOSVR4	(LM_MOTOROLA | LM_MOTSVR4)

/*      SNI     */

#define LM_RM           0x100           /* SNI RM series */
#define LM_MX           0x200           /* SNI MX series */

#define LM_RM400        ( LM_SNI | LM_RM | 0x1)
#define LM_MX300I       ( LM_SNI | LM_MX | 0x1)

#endif /* _LM_HOSTTYPE_H */
