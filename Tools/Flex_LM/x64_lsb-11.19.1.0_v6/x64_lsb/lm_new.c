/******************************************************************************************

	 NOTICE OF COPYRIGHT AND OWNERSHIP OF SOFTWARE: 
	Copyright (c) 1988-2022 Flexera. All Rights Reserved.
	 This computer program is the property of Flexera of 
	 Schaumburg, Illinois, U.S.A.  Any use, copy, publication, distribution, display, modification, or
	 transmission of this computer program in whole or in part in any form or by any means without 
	 the prior express written permission of Flexera is 
	 strictly prohibited.
	 Except when expressly provided by Flexera in writing,
	 possession of this computer program shall not be construed to confer any license or rights under 
	 any of Flexera's intellectual property rights, whether by
	 estoppel, implication, or otherwise.
	 ALL COPIES OF THIS PROGRAM MUST DISPLAY THIS NOTICE OF COPYRIGHT AND OWNERSHIP IN FULL.
************************************************************************************************/
#define LM_PUBKEYS 	3
#define LM_MAXPUBKEYSIZ 40

typedef struct _pubkeyinfo {
		    int pubkeysize[LM_PUBKEYS];
		    unsigned char pubkey[LM_PUBKEYS][LM_MAXPUBKEYSIZ];
		    int (*pubkey_fptr)();
		    int strength;
		    int sign_level;
} LM_VENDORCODE_PUBKEYINFO;
#define LM_MAXSIGNS 	4   /* SIGN=, SIGN2=, SIGN3=, SIGN4= */


typedef struct _vendorcode {
		    short type;	   
		    unsigned long data[2]; 
		    unsigned long keys[4]; 
		    short flexlm_version;
		    short flexlm_revision;
		    char flexlm_patch[2];
#define LM_MAX_BEH_VER 4 
		    char behavior_ver[LM_MAX_BEH_VER + 1];
		    unsigned long trlkeys[2]; 
		    int signs; 
		    int strength; 
		    int sign_level;
		    LM_VENDORCODE_PUBKEYINFO pubkeyinfo[LM_MAXSIGNS];

			  } VENDORCODE;
extern char *(*l_borrow_dptr)(void *, char *, int, int);

/* Forces the JNI wrapper function into the shared library */
extern void* Java_com_macrovision_flexlm_HostId_lGetNativeHostId;
void** dummy = &Java_com_macrovision_flexlm_HostId_lGetNativeHostId;
/* Forces the lc_new_job function into the shared library */
extern void* lc_new_job;
void** dummy2 = &lc_new_job;
/* Forces the lc_flexinit_property_handle_create function into the shared library */
extern void* lc_flexinit_property_handle_create;
void** dummy3 = &lc_flexinit_property_handle_create;
/* Forces the lc_flexinit_property_handle_set function into the shared library */
extern void* lc_flexinit_property_handle_set;
void** dummy4 = &lc_flexinit_property_handle_set;
/* Forces the lc_flexinit function into the shared library */
extern void* lc_flexinit;
void** dummy5 = &lc_flexinit;
/* Forces the lc_flexinit_cleanup function into the shared library */
extern void* lc_flexinit_cleanup;
void** dummy6 = &lc_flexinit_cleanup;

extern int l_pubkey_verify();
#include <string.h>
#include <time.h>

int vc(char *buf, VENDORCODE *v, int x, char *y, int z, int *a, char *buf2)
{
    static int c;

    if (x) return 0;

    if (!buf)
    {
        c = 0;
        return 0;
    }
    if (c > 1) return 0;
    memset(v, 0, sizeof(VENDORCODE));
    strcpy(buf, "demo");
	if (a) *a = 1;
	if (c == 0) v->keys[0] = 0x9ddcd080;
	if (c == 0) v->keys[1] = 0x99aa830f;
	if (c == 0) v->keys[2] = 0x33995896;
	if (c == 0) v->keys[3] = 0x3491136d;
	if (c == 0) v->trlkeys[0] = 0x5f1dcea6;
	if (c == 0) v->trlkeys[1] = 0x24af8961;
	if (c == 0) v->sign_level = 0x1;
	if (c == 0) v->strength = 0x2;
	if (c == 0) v->data[0] = 0xd65a6665;
	if (c == 0) v->data[1] = 0xbe36d012;
	if (c == 0)
	{
		v->pubkeyinfo[0].pubkeysize[0] = 0x10;
		v->pubkeyinfo[0].pubkeysize[1] = 0x16;
		v->pubkeyinfo[0].pubkeysize[2] = 0x1f;
		v->pubkeyinfo[0].strength = 0x2;
		v->pubkeyinfo[0].sign_level = 0x1;
		v->pubkeyinfo[0].pubkey_fptr = l_pubkey_verify;
		v->pubkeyinfo[0].pubkey[0][0] = 0x67;
		v->pubkeyinfo[0].pubkey[0][1] = 0x9c;
		v->pubkeyinfo[0].pubkey[0][2] = 0xb2;
		v->pubkeyinfo[0].pubkey[0][3] = 0x96;
		v->pubkeyinfo[0].pubkey[0][4] = 0x9d;
		v->pubkeyinfo[0].pubkey[0][5] = 0xe9;
		v->pubkeyinfo[0].pubkey[0][6] = 0x74;
		v->pubkeyinfo[0].pubkey[0][7] = 0x24;
		v->pubkeyinfo[0].pubkey[0][8] = 0x59;
		v->pubkeyinfo[0].pubkey[0][9] = 0xa0;
		v->pubkeyinfo[0].pubkey[0][10] = 0x8c;
		v->pubkeyinfo[0].pubkey[0][11] = 0x0;
		v->pubkeyinfo[0].pubkey[0][12] = 0x81;
		v->pubkeyinfo[0].pubkey[0][13] = 0x65;
		v->pubkeyinfo[0].pubkey[0][14] = 0x67;
		v->pubkeyinfo[0].pubkey[0][15] = 0x1e;
		v->pubkeyinfo[0].pubkey[0][16] = 0x0;
		v->pubkeyinfo[0].pubkey[0][17] = 0x0;
		v->pubkeyinfo[0].pubkey[0][18] = 0x0;
		v->pubkeyinfo[0].pubkey[0][19] = 0x0;
		v->pubkeyinfo[0].pubkey[0][20] = 0x0;
		v->pubkeyinfo[0].pubkey[0][21] = 0x0;
		v->pubkeyinfo[0].pubkey[0][22] = 0x0;
		v->pubkeyinfo[0].pubkey[0][23] = 0x0;
		v->pubkeyinfo[0].pubkey[0][24] = 0x0;
		v->pubkeyinfo[0].pubkey[0][25] = 0x0;
		v->pubkeyinfo[0].pubkey[0][26] = 0x0;
		v->pubkeyinfo[0].pubkey[0][27] = 0x0;
		v->pubkeyinfo[0].pubkey[0][28] = 0x0;
		v->pubkeyinfo[0].pubkey[0][29] = 0x0;
		v->pubkeyinfo[0].pubkey[0][30] = 0x0;
		v->pubkeyinfo[0].pubkey[0][31] = 0x0;
		v->pubkeyinfo[0].pubkey[0][32] = 0x0;
		v->pubkeyinfo[0].pubkey[0][33] = 0x0;
		v->pubkeyinfo[0].pubkey[0][34] = 0x0;
		v->pubkeyinfo[0].pubkey[0][35] = 0x0;
		v->pubkeyinfo[0].pubkey[0][36] = 0x0;
		v->pubkeyinfo[0].pubkey[0][37] = 0x0;
		v->pubkeyinfo[0].pubkey[0][38] = 0x0;
		v->pubkeyinfo[0].pubkey[0][39] = 0x0;
		v->pubkeyinfo[0].pubkey[1][0] = 0x67;
		v->pubkeyinfo[0].pubkey[1][1] = 0xa2;
		v->pubkeyinfo[0].pubkey[1][2] = 0x39;
		v->pubkeyinfo[0].pubkey[1][3] = 0x4a;
		v->pubkeyinfo[0].pubkey[1][4] = 0x31;
		v->pubkeyinfo[0].pubkey[1][5] = 0xb3;
		v->pubkeyinfo[0].pubkey[1][6] = 0x35;
		v->pubkeyinfo[0].pubkey[1][7] = 0xfa;
		v->pubkeyinfo[0].pubkey[1][8] = 0x7c;
		v->pubkeyinfo[0].pubkey[1][9] = 0xb4;
		v->pubkeyinfo[0].pubkey[1][10] = 0xf2;
		v->pubkeyinfo[0].pubkey[1][11] = 0x51;
		v->pubkeyinfo[0].pubkey[1][12] = 0xf6;
		v->pubkeyinfo[0].pubkey[1][13] = 0xdf;
		v->pubkeyinfo[0].pubkey[1][14] = 0x6c;
		v->pubkeyinfo[0].pubkey[1][15] = 0x94;
		v->pubkeyinfo[0].pubkey[1][16] = 0x12;
		v->pubkeyinfo[0].pubkey[1][17] = 0x87;
		v->pubkeyinfo[0].pubkey[1][18] = 0x2a;
		v->pubkeyinfo[0].pubkey[1][19] = 0xc;
		v->pubkeyinfo[0].pubkey[1][20] = 0xbd;
		v->pubkeyinfo[0].pubkey[1][21] = 0xee;
		v->pubkeyinfo[0].pubkey[1][22] = 0x0;
		v->pubkeyinfo[0].pubkey[1][23] = 0x0;
		v->pubkeyinfo[0].pubkey[1][24] = 0x0;
		v->pubkeyinfo[0].pubkey[1][25] = 0x0;
		v->pubkeyinfo[0].pubkey[1][26] = 0x0;
		v->pubkeyinfo[0].pubkey[1][27] = 0x0;
		v->pubkeyinfo[0].pubkey[1][28] = 0x0;
		v->pubkeyinfo[0].pubkey[1][29] = 0x0;
		v->pubkeyinfo[0].pubkey[1][30] = 0x0;
		v->pubkeyinfo[0].pubkey[1][31] = 0x0;
		v->pubkeyinfo[0].pubkey[1][32] = 0x0;
		v->pubkeyinfo[0].pubkey[1][33] = 0x0;
		v->pubkeyinfo[0].pubkey[1][34] = 0x0;
		v->pubkeyinfo[0].pubkey[1][35] = 0x0;
		v->pubkeyinfo[0].pubkey[1][36] = 0x0;
		v->pubkeyinfo[0].pubkey[1][37] = 0x0;
		v->pubkeyinfo[0].pubkey[1][38] = 0x0;
		v->pubkeyinfo[0].pubkey[1][39] = 0x0;
		v->pubkeyinfo[0].pubkey[2][0] = 0x66;
		v->pubkeyinfo[0].pubkey[2][1] = 0xcb;
		v->pubkeyinfo[0].pubkey[2][2] = 0x85;
		v->pubkeyinfo[0].pubkey[2][3] = 0xd5;
		v->pubkeyinfo[0].pubkey[2][4] = 0xf8;
		v->pubkeyinfo[0].pubkey[2][5] = 0xce;
		v->pubkeyinfo[0].pubkey[2][6] = 0xf5;
		v->pubkeyinfo[0].pubkey[2][7] = 0xae;
		v->pubkeyinfo[0].pubkey[2][8] = 0x75;
		v->pubkeyinfo[0].pubkey[2][9] = 0xfe;
		v->pubkeyinfo[0].pubkey[2][10] = 0x64;
		v->pubkeyinfo[0].pubkey[2][11] = 0x6e;
		v->pubkeyinfo[0].pubkey[2][12] = 0x2f;
		v->pubkeyinfo[0].pubkey[2][13] = 0x7d;
		v->pubkeyinfo[0].pubkey[2][14] = 0x5d;
		v->pubkeyinfo[0].pubkey[2][15] = 0x1b;
		v->pubkeyinfo[0].pubkey[2][16] = 0x53;
		v->pubkeyinfo[0].pubkey[2][17] = 0x5d;
		v->pubkeyinfo[0].pubkey[2][18] = 0x7d;
		v->pubkeyinfo[0].pubkey[2][19] = 0xc2;
		v->pubkeyinfo[0].pubkey[2][20] = 0x22;
		v->pubkeyinfo[0].pubkey[2][21] = 0x6;
		v->pubkeyinfo[0].pubkey[2][22] = 0xd5;
		v->pubkeyinfo[0].pubkey[2][23] = 0xc2;
		v->pubkeyinfo[0].pubkey[2][24] = 0x44;
		v->pubkeyinfo[0].pubkey[2][25] = 0xe7;
		v->pubkeyinfo[0].pubkey[2][26] = 0xe6;
		v->pubkeyinfo[0].pubkey[2][27] = 0x69;
		v->pubkeyinfo[0].pubkey[2][28] = 0xbb;
		v->pubkeyinfo[0].pubkey[2][29] = 0x6a;
		v->pubkeyinfo[0].pubkey[2][30] = 0x75;
		v->pubkeyinfo[0].pubkey[2][31] = 0x0;
		v->pubkeyinfo[0].pubkey[2][32] = 0x0;
		v->pubkeyinfo[0].pubkey[2][33] = 0x0;
		v->pubkeyinfo[0].pubkey[2][34] = 0x0;
		v->pubkeyinfo[0].pubkey[2][35] = 0x0;
		v->pubkeyinfo[0].pubkey[2][36] = 0x0;
		v->pubkeyinfo[0].pubkey[2][37] = 0x0;
		v->pubkeyinfo[0].pubkey[2][38] = 0x0;
		v->pubkeyinfo[0].pubkey[2][39] = 0x0;
	}

{
	  char *l_borrow_decrypt(void *, char *, int, int);
		l_borrow_dptr = l_borrow_decrypt;
	}
	v->type = 4;
	strcpy(v->behavior_ver, "11.0");
	v->flexlm_version = 11;
	v->flexlm_revision = 19;
	v->flexlm_patch[0] = 0;
	c++;
	return 1;
}
int (*l_n36_buf)() = vc;
