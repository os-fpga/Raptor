/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Function: lmfeats
 *
 *	Description: Creates a FEATSET line for a license file
 *
 *	Parameters:	lmfeats daemon [-c license_file]
 *
 *	M. Christiano
 *	10/30/89
 *
 *	Last changed:  2/2/96
 *
 */

#include "lmclient.h"
#include "lm_code.h"
#include "lm_attr.h"
#include <stdio.h>

#ifdef WINNT
#include <process.h>
#endif

LM_DATA;

LM_CODE(code, ENCRYPTION_SEED1, ENCRYPTION_SEED2, VENDOR_KEY1,
			VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4, VENDOR_KEY5);
char *pszDaemon = "";
int quiet = 0;
static void process_args(), usage();

int
main(argc, argv)
int argc;
char *argv[];
{
  char *p;

	(void) lm_init(VENDOR_NAME, &code, &lm_job);
	process_args(argc, argv);
	if (!quiet)
		(void) printf("lmfeats - "COPYRIGHT_STRING(1989)"\n");
	p = lc_feat_set(lm_job, pszDaemon, &code, 0);
	if (p)
	  (void) printf("%s %s %s\n", LM_RESERVED_FEATURESET, pszDaemon,
					lc_feat_set(lm_job, pszDaemon, &code, 0));
	else
	  lm_perror("lm_feat_set");

	exit(_lm_errno);
	return 0;
}

static void
process_args(argc, argv)
int argc;
char *argv[];
{
	while (argc > 1)
	{
	  char *p = argv[1]+1;

                if (*argv[1] == '-') switch (*p)
		{
			case 'c':
				if (argc > 2 && *argv[2] != '-')
				{

					argv++; argc--;
					(void) lm_set_attr(LM_A_DISABLE_ENV,
						(LM_A_VAL_TYPE)1);
					(void) lm_set_attr(LM_A_LICENSE_FILE,
						(LM_A_VAL_TYPE) argv[1]);
				}
				break;

			case 'n':
				quiet++;
				break;

			default:
				break;
		}
		else
		{
			pszDaemon = argv[1];
		}

		argc--; argv++;
	}
	if (*pszDaemon == '\0')
	{
		usage();
		exit(0);
	}
}

static
void
usage()
{
	(void) printf("usage: lmfeats daemon [-c license_file] [-n]\n");
}
