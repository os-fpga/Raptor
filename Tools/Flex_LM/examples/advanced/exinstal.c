/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*
 *
 *	Description: 	Example of a license file installer
 *
 *			install(char *what, char *where)
 *
 *			what:	path to input license file
 *			where:	path to directory where license
 *				resulting license should go.
 *
 *	       	This example takes a license file.
 *			It checks format and license-keys in the file.
 *
 *
 *	Suggested Enhancements
 *
 *			Output:	"where" should be a directory
 *				in the "expected location" for your
 *				company.  Usually, this means the
 *				user is required to have an environment
 *				var or registry set that says where
 *				the product is installed.  This tool
 *				should then place the file in this
 *				directory, with the name vendorYYYYMMDD.lic
 *				where vendor is the vendor-daemon name.
 *
 *			Input:	Particularly on windows, it would be
 *				nice to provide a GUI form for typing
 *				a license, when needed.
 *
 *			FlexNet Licensing version:
 *				If you want the output file to be compatible
 *				with a previous FlexNet Licensing version, then
 *				use
 *			lc_set_attr(job, LM_A_LICENSE_FMT_VER, LM_BEHAVIOR_Vx);
 *				where 'x' is 2, 3, 4, 5, or 5_1.
 *
 *	D. Birns
 *	9/30/97
 *
 *	Last changed:  10/7/97
 *
 */

#include "lmclient.h"
#include "lm_code.h"
#include "lm_attr.h"
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>

#ifdef PC
#define PATHSEP '\\'
#else
#define PATHSEP '/'
#endif

static void usage 		lm_args((lm_noargs));
static void get_file_size	lm_args((char *, struct stat *, char **));
static void check_file		lm_args((LM_HANDLE *));
static void output_file		lm_args((char *, char *, char *));
static void install 		lm_args((char *, char *));

LM_CODE(code, ENCRYPTION_SEED1, ENCRYPTION_SEED2, VENDOR_KEY1,
	VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4, VENDOR_KEY5);


main(argc, argv)
int argc;
char **argv;
{
	if (argc < 2) usage();
	install(argv[1], ".");
}

static
void
install(what, where)
char *what;
char *where;
{

  LM_HANDLE *lm_job;
  char *buf;
  char *readable_buf;
  struct stat st_buff;
  FILE *fp;
  char *cp;
  char *err;
  char *ofilename;

/*
 *	Get started:    1) get the size of the input file
 *			2) malloc a buffer big enough for the file
 *			3) read the file into the buffer
 *			4) lc_init()
 */
/*
 *	get file size
 */
	get_file_size(what, &st_buff, &buf);


/*
 *	open input file
 */

	if (!(fp = fopen(what, "r")))
	{
		perror("Can't open input file, exiting");
		usage();
	}
	if (!fread(buf, st_buff.st_size, 1, fp))
	{
		perror("Can't read input file, exiting");
		usage();
	}

	if (lc_init((LM_HANDLE *)0, VENDOR_NAME, &code, &lm_job))
	{
		lc_perror(lm_job, "FlexNet Licensing init failed");
		exit(lc_get_errno(lm_job));
	}
/*
 *	Checks license file format and license key
 */
	if (lc_convert(lm_job, buf, &readable_buf, &err,
						LM_CONVERT_TO_READABLE))
	{
		printf("Error: license file information is incorrect\n");
		if (lc_get_errno(lm_job))
			printf("%s\n", lc_errstring(lm_job));
	}
	if (err)
	{
		printf("Errors found in license:\n");
		printf("%s\n", err);
		lc_free_mem(lm_job, err);
	}
	lc_free_mem(lm_job, buf);
/*
 *	output converted file
 */
	if (!(ofilename = (char *)malloc(strlen(where) + MAX_VENDOR_NAME + 15)))
	{
		perror("exiting");
		return;
	}
	output_file(readable_buf, ofilename, where);

	lc_free_mem(lm_job, readable_buf);

/*
 *	Set the license file to the new file
 *		These 2 attributes should never be used in application
 *		that's doing a checkout, only for a utility like this.
 *		They guarantee that the utility only looks at just
 *		this one license file, ignoring $LM_LICENSE_FILE, etc.
 */
	lc_set_attr(lm_job, LM_A_DISABLE_ENV, (LM_A_VAL_TYPE)1);
	lc_set_attr(lm_job, LM_A_LICENSE_FILE, (LM_A_VAL_TYPE)ofilename);

/*
 *	Check the converted file
 */
	check_file(lm_job);
	lc_free_job(lm_job);
	exit(0);
}
static
void
usage()
{
	printf("Usage: exinstal file\n");
	exit(1);
}

static
void
get_file_size(what, st_buff, buf)
char *what;
struct stat *st_buff;
char **buf;
{

	if (stat(what, st_buff))
	{
		perror("Can't stat input file, exiting");
		usage();
	}
	if (!(*buf = (char *)malloc(st_buff->st_size + 1)))
	{
		printf("Out of memory, exiting\n");
		exit(1);
	}
}
static
void
check_file(lm_job)
LM_HANDLE *lm_job;
{
  char **featlist;
  CONFIG *conf;
  char *err;

/*
 *	Walk through the license file.
 *	We need to get everything into a CONFIG struct
 *	First use lc_feat_list to get the list of featnames
 */
	for (featlist = lc_feat_list(lm_job, 0, 0); featlist && *featlist;
								featlist++)
	{
	   CONFIG *pos;

/*
 *		Now use lc_next_conf to get each CONFIG for this feature
 */
		for (pos = 0, conf = lc_next_conf(lm_job, *featlist, &pos);
				conf;
				conf = lc_next_conf(lm_job, *featlist, &pos))
		{
/*
 *			Now check the config with lc_check_key() and
 *			lc_chk_conf()
 *			Note that
 */
			if (lc_check_key(lm_job, conf, &code))
			{
				printf("%s\n", lc_errstring(lm_job));
			}
			if (err = lc_chk_conf(lm_job, conf, 0))
			{
				printf("Error on %s/%s: %s\n",
					conf->feature, conf->code, err);
				lc_free_mem(lm_job, err);
			}
		}
	}
}
static void
output_file(buf, ofilename, where)
char *buf;
char *ofilename;
char *where;
{
  struct stat st;
  int cnt = 1;
  FILE *ofp;
  time_t now;
#ifdef THREAD_SAFE_TIME
  struct tm tst;
#endif
  struct tm *t;


	now = time(0);
#ifdef THREAD_SAFE_TIME
	localtime_r(&now, &tst);
	t = &tst;
#else /* !THREAD_SAFE_TIME */
	t = localtime(&now);
#endif

	sprintf(ofilename, "%s%c%s%d%02d%02d.lic", where, PATHSEP,
		VENDOR_NAME, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
	while (!stat(ofilename, &st))
	{
		printf("(%s already exists)\n", ofilename);
		sprintf(ofilename, "%s%c%s%d%02d%02d.lic", where, PATHSEP,
			VENDOR_NAME, t->tm_year + 1900, t->tm_mon + 1,
			t->tm_mday + cnt++);
	}
	printf("Creating output file %s\n", ofilename);
	if (!(ofp = fopen(ofilename, "w")))
	{
		perror("Can't open output file, exiting\n");
		exit(1);
	}
	if (!fwrite(buf, strlen(buf), 1, ofp))
	{
		perror("Can't write to output file, exiting\n");
		exit(1);
	}
	fclose(ofp);
}
