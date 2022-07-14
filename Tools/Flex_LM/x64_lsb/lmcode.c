#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef PC
#include <shlobj.h>
#endif
#include "lm_code.h"
#include "lmclient.h"
#include "lm_attr.h"
#ifdef PC
#include <io.h>
#define PREV_FLEX_DIR "Macrovision\\FLEXlm"
#endif
#define CURR_FLEX_DIR "FNP\\FLEXlm"
VENDORCODE vendorkeys[] = {		/* Possible keys for vendor daemons */
		{ VENDORCODE_7,
		ENCRYPTION_SEED1^VENDOR_KEY5, ENCRYPTION_SEED2^VENDOR_KEY5,
		  VENDOR_KEY1, VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4,
		  FLEXLM_VERSION, FLEXLM_REVISION, FLEXLM_PATCH,
		  LM_BEHAVIOR_CURRENT, {TRL_KEY1, TRL_KEY2}, 0,
		  LM_STRENGTH, LM_SIGN_LEVEL
		},
		    	   };	/* End of vendor codes*/
int keysize = sizeof(vendorkeys)/sizeof(vendorkeys[0]);
#define TWELVE_HOURS (12 * 60 * 60)
FLEX_VM_TYPE ls_allow_vm = VM_ALL;
#ifdef PC 
#endif
char *vendor_name = VENDOR_NAME;
unsigned long seed1 = ENCRYPTION_SEED1;
unsigned long seed2 = ENCRYPTION_SEED2;
unsigned long seed3 = ENCRYPTION_SEED3;
unsigned long seed4 = ENCRYPTION_SEED4;
unsigned long lmseed1 = LM_SEED1;
unsigned long lmseed2 = LM_SEED2;
unsigned long lmseed3 = LM_SEED3;
int lm_sign_level = LM_SIGN_LEVEL;
int pubkey_strength = LM_STRENGTH;
int l_borrow_ok = LM_BORROW_OK;
