/**************************************************************************************************
* Copyright (c) 1997-2018, 2020 Flexera. All Rights Reserved.
**************************************************************************************************/

/**   @file lmhostid.h
 *    @brief host id definitions
 *
  ************************************************************/

/* Currently used HOSTIDs  */
#define HOSTID_DEFAULT -1		    /* for lc_hostid() */
#define HOSTID_LONG 1			    /* Longword hostid, eg, SUN */
#define HOSTID_ETHER 2			    /* Ethernet address, eg, VAX */
#define HOSTID_ANY 3			    /* Any hostid; used for ISV testing and development */
#define HOSTID_USER 4			    /* Username HOSTID=USER=xxx */
#define HOSTID_DISPLAY 5		    /* Display */
#define HOSTID_HOSTNAME 6		    /* Node name HOSTID=HOSTNAME=xxx */
#define HOSTID_ID_STRING 9          /* HOSTID=ID_STRING=xxx, equivalent to HOSTID_STRING */
#define HOSTID_DISK_SERIAL_NUM 11	/* Windows, and NT disk serial number */
#define HOSTID_INTERNET 12		    /* Default v4:= ###.###.###.### /v6:= X:X:X:X:X:X:X:X*/
#define HOSTID_DEMO 13              /* HOSTID=DEMO for ISVs to use in their application */
#define HOSTID_ID 18                /* HOSTID=ID=#, equivalent to HOSTID_SERNUM_ID */
#define HOSTID_FLEXID6 23           /* FLEXID6 */
#define HOSTID_FLEXID7 10           /* FLEXID7 */
#define HOSTID_FLEXID8 14           /* FLEXID8 */
#define HOSTID_FLEXID9 15           /* FLEXID9 */
#define HOSTID_FLEXID10 51          /* FLEXID10 */
#define HOSTID_FLEXID11 52          /* FLEXID11 */
#define HOSTID_FLEXID9_DTYPE 53     /* FLEXID9 Dongle Type */

/* Not currently used, but reserved for future use */
#define HOSTID_FLEXID4_KEY 16
#define HOSTID_FLEXID5_KEY 17
#define HOSTID_DOMAIN 19

/* Backward Compatible HOSTIDs */
#define HOSTID_FLEXID6_KEY 23		/* HOSTID_FLEXID6 */
#define HOSTID_FLEXID1_KEY 10		/* HOSTID_FLEXID7 */
#define HOSTID_FLEXID2_KEY 14		/* HOSTID_FLEXID8 */
#define HOSTID_FLEXID3_KEY 15		/* HOSTID_FLEXID9 */
#define HOSTID_STRING 9			    /* string ID  MAX HOSTID_LEN */
#define HOSTID_SERNUM_ID 18

/* Obsolete HOSTIDs */
#define HOSTID_INTEL32 24		/* 32-bit */
#define HOSTID_INTEL64 25		/* 64-bit */
#define HOSTID_INTEL96 26		/* 96-bit */
#define HOSTID_PC_CPU 27
#define HOSTID_ID_MODULE 8		/* hp300 id-module */
/* #define	NOHOSTID 0 -- removed v5 -- now NULL pointer */

#define HOSTID_CPU 28                   /* not used */
#define HOSTID_DISK_GEOMETRY 29         /* not used */
#define HOSTID_BIOS 30                  /* not used */
#define HOSTID_COMPOSITE 31             /* Composite HostID */
#define HOSTID_HWETHERNET		32			/* Ethernet address in hardware. */

/*
 *	HOSTID's 50-100 reserved for future FLEXIDs
 */
#define HOSTID_FLEXID_FILE_KEY	50		/* for testing purposes */
#define HOSTID_FLEXID10_KEY	HOSTID_FLEXID10	/* FLEXID10 */
#define HOSTID_FLEXID11_KEY	HOSTID_FLEXID11	/* FLEXID11 */
#define PHY_HOSTID_START	81
#define PHY_FLEXID_START	81
#define PHY_FLEXID6		81
#define PHY_FLEXID7		82
#define PHY_FLEXID8		83
#define PHY_FLEXID9		84
#define PHY_FLEXID10	85			/* Physical FLEXID10 */
#define PHY_FLEXID11	86			/* Physical FLEXID11*/
#define PHY_FLEXID_END 100

/*
 *	HOSTID's 81-120 reserved for PHY_* hostids
 */
#define PHY_DISK_SERIAL_NUM	101
#define PHY_DISPLAY			102
#define PHY_ETHER			103
#define PHY_HOSTNAME		104
#define PHY_INTERNET		105
#define PHY_USER			106
#define PHY_HOSTID_END		120

/* 
 *	HOSTID's 121-140 reserved for VMW_* hostids
 */
#define VMW_HOSTID_START	121
#define VMW_ETHER			121
#define VMW_HOSTNAME		122
#define VMW_INTERNET		123
#define VMW_UUID			124           /* UUID of the VM when in virtual environment*/	
#define VMW_HOSTID_END		140

/* 
 *	HOSTID's 141-160 reserved for HPV_* hostids
 */
#define HPV_HOSTID_START	141
#define HPV_ETHER			141
#define HPV_HOSTNAME		142
#define HPV_INTERNET		143
#define HPV_UUID			144
#define HPV_DISK_SERIAL_NUM	145
#define HPV_HOSTID_END		160

/* 
 *	HOSTID's 181-200 reserved for Generic VM hostids for future use
 */
#define VM_HOSTID_START		181
#define VM_UUID				181
#define VM_GENID			182
#define VM_FAMILY			183
#define VM_NAME				184
#define VM_VMID				185
#define VM_HOSTID_END	    200


/*
 *	HOSTID in the range 201 - 250 for LMB hostids
 */
#define LMB_HOSTID_START	201
#define LMB_ETHER			201
#define LMB_INTERNET		202
#define LMB_HOSTNAME		203
#define LMB_FLEXID9			204
#define LMB_FLEXID10		205
#define LMB_HOSTID_END		250


/*
 *	HOSTIDs in the range 301 - 350 reserved for future TPM hostids
 */
#define TPM_HOSTID_START    301
#define HOSTID_TPM_ID1      301		/* TPM 2.0 */
#define TPM_HOSTID_END		350

/*
 *	HOSTIDs in the range 351 - 400 reserved for future DOCKER hostids
 */
#define DOCKER_HOSTID_START    351
#define HOSTID_CONTAINER_ID    351
#define DOCKER_HOSTID_END	   400

/*
 *	HOSTID in the range 501 - 520 for AMZN hostids  
 */
#define AMZN_HOSTID_START	501
#define AMZN_EIP			501
#define AMZN_AMI			502
#define AMZN_IID			503
#define AMZN_HOSTID_END		520

/* 
 *	HOSTID's 1000 reserved for Vendor-defined hostid
 */
#define HOSTID_VENDOR 		1000

/* 
 *	VM supported hostid range
 */
#define FNP_VMSUPPORT_HOSTID_START	81
#define FNP_VMSUPPORT_HOSTID_END 	999

/*
 * INTERNET V4 hostid in IPv6 kit on dual stack environment
 */
#define FNP_HOSTID_INTERNET_V4 49		/* ###.###.###.### */
#define FNP_HOSTID_INTERNET_V6 33

/* 
 *	All hostid related macros define here
 */
#define LM_IS_FLEXID(x) (\
			(x == HOSTID_FLEXID1_KEY) || \
		((x >= HOSTID_FLEXID2_KEY) && (x <= HOSTID_FLEXID5_KEY)) ||\
			(x == HOSTID_FLEXID6_KEY) || \
		((x >= 50) && (x <= 100)) \
		)

#define FNP_IS_VMSUPPORT_HOSTID_TYPE(x) ( ((x >= FNP_VMSUPPORT_HOSTID_START) && (x <= FNP_VMSUPPORT_HOSTID_END)) && \
                                        !((x >= TPM_HOSTID_START) && (x <= TPM_HOSTID_END)) && \
										!((x >= DOCKER_HOSTID_START) && (x <= DOCKER_HOSTID_END)))

#define FNP_IS_VMW_HOSTID_TYPE(x) ((x >= VMW_HOSTID_START) && (x <= VMW_HOSTID_END))

#define FNP_IS_PHY_HOSTID_TYPE(x) ((x >= PHY_HOSTID_START) && (x <= PHY_HOSTID_END))

#define FNP_IS_HPV_HOSTID_TYPE(x) ((x >= HPV_HOSTID_START) && (x <= HPV_HOSTID_END)) 

#define FNP_IS_VM_HOSTID_TYPE(x)  ((x >= VM_HOSTID_START) && (x <= VM_HOSTID_END))

#define FNP_IS_LMB_HOSTID_TYPE(x) ((x >= LMB_HOSTID_START) && (x <= LMB_HOSTID_END))

#define FNP_IS_AMZN_HOSTID_TYPE(x) ((x >= AMZN_HOSTID_START) && (x <= AMZN_HOSTID_END))

#define FNP_IS_AMZN_SERVER_HOSTID_TYPE(x) (x == AMZN_EIP)

#define FNP_IS_AMZN_CLIENT_HOSTID_TYPE(x) ((x >= AMZN_AMI) && (x <= AMZN_HOSTID_END))

#define FNP_IS_TPM_HOSTID_TYPE(x)  ((x >= TPM_HOSTID_START) && (x <= TPM_HOSTID_END))

#define FNP_IS_DOCKER_HOSTID_TYPE(x)  ((x >= DOCKER_HOSTID_START) && (x <= DOCKER_HOSTID_END))
