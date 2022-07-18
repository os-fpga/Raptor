/**************************************************************************************************
* Copyright (c) 1997-2018 Flexera. All Rights Reserved.
**************************************************************************************************/

/*

 *
 *	Description:	Reads an existing license file and fills in the
 *			encryption codes in the FEATURE and FEATURESET lines.
 *
 *	Usage:		lmcrypt [iofiles] [-i inputfile] [-o outputfile]
 *
 *
 */

#include "lmprikey.h"

#include "lmclient.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "lm_redir_std.h"
#ifdef LINUX
#include <stdint.h>
#endif

#ifdef PC
#include <malloc.h>
#include <io.h>
#ifdef OS2
#define mkstemp(x) tmpnam(NULL)
#endif /* OS2 */
#else
int mkstemp();
char *mktemp();
char *getenv();
#endif /* PC */

#include "lm_code.h"
#include "lmseeds.h"
#include "lm_attr.h"

LM_HANDLE *lm_job = (LM_HANDLE *)NULL;

/* forward ref */
static char * safe_malloc lm_args((int));
void usage();
char *errfilename;
FILE *efp;
int forceit = LM_CRYPT_FORCE; /* new default in v5.1 */
int dofilecrypt lm_args((char *infilename, char *outfilename,
			VENDORCODE *code));
int dofile1crypt lm_args((char *filename, VENDORCODE *code));
int dofpcrypt lm_args((FILE *ifp, FILE *ofp, char *ifname,
		char *ofname, VENDORCODE *code));
static int flag = 0;

LM_CODE_NEW(site_code, ENCRYPTION_SEED1, ENCRYPTION_SEED2,
            VENDOR_KEY1, VENDOR_KEY2, VENDOR_KEY3,
            VENDOR_KEY4, VENDOR_KEY5,
            FLEXLM_VERSION, FLEXLM_REVISION, FLEXLM_PATCH, LM_VER_BEHAVIOR,
            TRL_KEY1, TRL_KEY2, LM_STRENGTH);
int
main(int argc, char * argv[])
{
	int i;
	int estat=0;
	int iofilecount=0;
	int cpyrtflag=0;
	char **iofiles;
	char *infilename = NULL;
	char *outfilename = NULL;
    VENDORCODE *code;
    char primVendName[MAX_LONGNAME_SIZE + 1];

	errfilename = 0;


    LM_CODE_GEN_INIT_NEW(&site_code, ENCRYPTION_SEED1, ENCRYPTION_SEED2,
        l_priseedcnt, lm_prikey, lm_prisize);

    code = &site_code;
    strcpy(primVendName, VENDOR_NAME);
    if (lc_init((LM_HANDLE *)0, primVendName, code, &lm_job))
    {
        lc_perror(lm_job, "lc_init failed");
        exit(-1);
    }

	iofiles = (char **)safe_malloc(argc*sizeof(char *));
	for (i=1; i<argc; i++)
	{
		if (strcmp(argv[i],"-i")==0)
		{
			infilename = argv[++i];
			if (!infilename)
			{
				usage();
				exit(1);
			}
			/* swap file to iofiles list if in and out have the same path */
			if (outfilename && !strcmp(infilename, outfilename))
			{
				iofiles[iofilecount++] = infilename;
				infilename = outfilename = NULL;
			}
		}
		else if (strcmp(argv[i],"-o")==0)
		{
			outfilename = argv[++i];
			if (!outfilename)
			{
				usage();
				exit(1);
			}
			/* swap file to iofiles list if in and out have the same path */
			if (infilename && !strcmp(infilename, outfilename))
			{
				iofiles[iofilecount++] = outfilename;
				infilename = outfilename = NULL;
			}
		}
		else if (strcmp(argv[i],"-e")==0)
		{
			errfilename = argv[++i];
			if (!errfilename)
			{
				usage();
				exit(1);
			}
		}
/*
 *		NOTE: LM_CRYPT_FORCE is the new default
 *		so -f does nothing.
 */
		else if (!strcmp(argv[i],"-f"))
			forceit |= LM_CRYPT_FORCE;
		else if (!strcmp(argv[i],"-longkey"))
			lc_set_attr(lm_job, LM_A_LKEY_LONG, (LM_A_VAL_TYPE)1);
		else if (!strcmp(argv[i],"-shortkey"))
			lc_set_attr(lm_job, LM_A_LKEY_LONG, (LM_A_VAL_TYPE)0);
		else if (!strcmp(argv[i], "-java"))
			lc_set_attr(lm_job, LM_A_JAVA_LIC_FMT,(LM_A_VAL_TYPE)1);
		else if (!strcmp(argv[i],"-decimal"))
			forceit |= LM_CRYPT_DECIMAL_FMT;
		else if (!strcmp(argv[i],"-maxlen"))
		{
			int max = 0;
			i++;

			if (i >= argc)
				break;
			sscanf(argv[i], "%d", &max);
			if (!max) fprintf(lm_flex_stderr(),
				"Error: -maxlen %s Invalid line length\n", argv[i]);
			else
				lc_set_attr(lm_job, LM_A_MAX_LICENSE_LEN, (LM_A_VAL_TYPE) (intptr_t) max);
		}
		else if (!strcmp(argv[i],"-verfmt"))
		{
			i++;
			if (i >= argc)
				break;
			switch(*argv[i])
			{
			case '2':
				lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER, (LM_A_VAL_TYPE)LM_BEHAVIOR_V2);
				break;
			case '3':
				lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER, (LM_A_VAL_TYPE)LM_BEHAVIOR_V3);
				break;
			case '4':
				lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER, (LM_A_VAL_TYPE)LM_BEHAVIOR_V4);
				break;
			case '5':
				if (!strncmp(argv[i],"5.1", 3))
				{
					lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER,
						(LM_A_VAL_TYPE)LM_BEHAVIOR_V5_1);
				}
				else
				{
					lc_set_attr(lm_job,	LM_A_LICENSE_FMT_VER,
						(LM_A_VAL_TYPE)LM_BEHAVIOR_V5);
				}
				break;
			case '6':
				lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER, (LM_A_VAL_TYPE)LM_BEHAVIOR_V6);
				break;
			case '7':
				if (!strncmp(argv[i],"7.1", 3))
				{
					lc_set_attr(lm_job,	LM_A_LICENSE_FMT_VER,
						(LM_A_VAL_TYPE)LM_BEHAVIOR_V7_1);
				}
				else
				{
					lc_set_attr(lm_job,	LM_A_LICENSE_FMT_VER,
						(LM_A_VAL_TYPE)LM_BEHAVIOR_V7);
				}
				break;
			default :
				usage();
			}
		}
		else if (!strcmp(argv[i],"-r"))
		{
			printf("lmcrypt(%s) - "COPYRIGHT_STRING(1991)"\n",	primVendName);
			cpyrtflag = 1;
		}
		else if (argv[i][0]=='-')
		{	/* unknown switch */
			usage();
			exit(1);
		}
		else
			iofiles[iofilecount++] = argv[i];
	}
	if (!errfilename || strcmp(errfilename,"-")==0)
	{
		errfilename = "stderr";
	}
	if (iofilecount)
	{
		for (i=0; i<iofilecount; i++)
		{
			estat |= dofile1crypt(iofiles[i], code);
            if (flag == 1)
            {
                estat = 1;
                flag = 0;
            }
		}
		if (!infilename && !outfilename)
			exit(estat);
	}
	if (cpyrtflag && !infilename && !outfilename)
		exit(0);
	if (!infilename || strcmp(infilename,"-")==0)
	{
		infilename = "stdin";
	}
	if (!outfilename || strcmp(outfilename,"-")==0)
	{
		outfilename = "stdout";
	}
	estat |= dofilecrypt(infilename,outfilename, code);
    if (flag == 1)
    {
        estat = 1;
        flag = 0;
    }
	exit(estat);
}

void
usage()
{
fprintf(lm_flex_stderr(),"usage: lmcrypt [-i infile] [-o outfile] [-e errfile]\n\t[licenses ...] [-r] [-verfmt ver] [-decimal] [-maxlen n] [-help]\n");
fprintf(lm_flex_stderr(),"   [-r]     Print copyright notice.\n");
fprintf(lm_flex_stderr(),"   [-help]  Display usage information.\n");
fprintf(lm_flex_stderr(),
	"   If no input file, or if specified as - or stdin, stdin is used.\n");
fprintf(lm_flex_stderr(),
	"   If no output file, or if specified as - or stdout, stdout is used.\n");
fprintf(lm_flex_stderr(),
    "   If no error file, or if specified as - or stderr, stderr is used.\n");
fprintf(lm_flex_stderr(),"   licenses are read and written back in place.\n");
fprintf(lm_flex_stderr(),"   If no arguments, reads stdin and writes stdout.\n");
fprintf(lm_flex_stderr(),"   -verfmt prints in old format and prints errors when \n");
fprintf(lm_flex_stderr(),"   is newer than specified. Version formats are 2-5, 5.1, 6, 7 and 7.1\n");

}

static
char *
safe_malloc(int n)
{
	char * p = NULL;

	p = (char *)malloc(n);
	if (p == NULL)
	{
		fprintf(lm_flex_stderr(), "lmcrypt: no more memory\n");
		exit(2);
	}
	return p;
}

int
dofile1crypt(char * filename, VENDORCODE *code)
{
	char obuf[LM_MAXPATHLEN+10] = {'\0'};
	int estat = 0;
	int t = 0;
#ifndef PC
	int tmp_fd = -1;
#endif

#ifdef PC
	char * cp = NULL;

	strncpy( obuf, filename, sizeof(obuf)-1);
	if ( cp = strrchr(obuf, '.') )
		*cp = 0;
	strcat( obuf, ".XXXXXX" );
#else
	(void)sprintf(obuf,"%s.XXXXXX",filename);
#endif /* PC */
#ifdef PC
	(void)mktemp(obuf);
#else
	tmp_fd = mkstemp(obuf);
	if (tmp_fd != -1)
		close(tmp_fd);
#endif
	estat = dofilecrypt(filename,obuf, code);
	if (estat==0)
	{
		/* succeeded, move new file onto old */
#ifdef PC
		unlink( filename );
#endif
		t = rename(obuf,filename);
		if (t)
		{
			perror(filename);
			estat = 1;
		}
	}
	return estat;
}

int
dofilecrypt(
	char *			infilename,
	char *			outfilename,
	VENDORCODE *	code)
{
	FILE * ifp = NULL;
	FILE * ofp = NULL;
	int ret = 0;

	if (strcmp(infilename,"stdin")==0)
		ifp = lm_flex_stdin();
	else
		ifp = fopen(infilename,"r");
	if (!ifp)
	{
		perror(infilename);
		return 1;
	}
	if (strcmp(errfilename,"stderr")==0)
		efp = lm_flex_stderr();
	else
		efp = fopen(errfilename,"w");
	if (!efp)
	{
		fclose(ifp);
		perror(errfilename);
		return 1;
	}
	if (strcmp(outfilename,"stdout")==0)
		ofp = lm_flex_stdout();
	else
		ofp = fopen(outfilename,"w");
	if (!ofp)
	{
		fclose(ifp);
		perror(outfilename);
		return 1;
	}

	ret = dofpcrypt(ifp,ofp,infilename,outfilename, code);
	fclose(ifp);
	fclose(ofp);
	return ret;
}



int	/* 0 if OK, 1 if error */
dofpcrypt(
	FILE *			ifp,
	FILE *			ofp,
	char *			ifname,	/* for error messages */
	char *			ofname,	/* for error messages */
	VENDORCODE *	code)
{
  struct stat st;
  char * str = NULL, * cur = NULL, * ofile_str = NULL, * err = NULL;
  FILE *tmpf = (FILE *)0;
  int c;
  int std_in = 0;
  char *tempname = 0;
#ifdef PC
  int ret = 0;
  size_t size =0;
#endif
#ifndef PC
  int tmp_fd = -1;
  char *temp_prefix;
  char tmp_buf[500] = {'\0'};
#endif /* PC */
  /*VENDORCODE v;*/

	if (!strcmp(ifname, "stdin"))
	{
		std_in = 1;
/*
 *		read stdin into temp file, and use that as input file
 */
#ifdef PC
		size = strlen("lmXXXXXX") + 1;
		tempname = (char *) malloc(size * sizeof(char));
		if(tempname != NULL)
		{
			strcpy(tempname,"lmXXXXXX");
			ret = _mktemp_s(tempname, size);
			if(ret == 0)
				ifname = tempname;
			else
			{
				perror("Can't create temporary file");
				free(tempname);
				return 1;
				
			}
		}
		else
		{
				perror("malloc failure occured");
				return 1;
		}
#else
		if (!(temp_prefix = getenv("TMPDIR")))
#ifdef FREEBSD
			temp_prefix = "/var/tmp";
#else
			temp_prefix = "/usr/tmp";
#endif
		sprintf(tmp_buf, "%s/lmcryptXXXXXX", temp_prefix);	/* OVERRUN */
		if ((tmp_fd = mkstemp(tmp_buf)) != -1)
		{
			close(tmp_fd);
			tempname = ifname = tmp_buf;
		}
		else
		{
			perror("Can't create temporary file");
			return 1;
		}
#endif /* PC */
		if (!*ifname)
		{
			perror("Can't create temporary file");
			if (tempname)
			{
#ifdef PC
				free(tempname);
#endif /* PC */
				tempname = NULL;
			}
			return 1;
		}
		if (!(tmpf = fopen(ifname, "w")))
		{
			perror("Can't open temporary file");
			if (tempname)
			{
#ifdef PC
				free(tempname);
#endif /* PC */
				tempname = NULL;
			}
			return 1;
		}
		while ((c = getchar()) != EOF)
			fputc(c, tmpf);
		fclose(tmpf);
		tmpf = NULL;

        ifp = fopen(ifname, "r");
	}
	if (stat(ifname, &st))
	{
		perror("Can't stat input file");
		if (tempname)
		{
#ifdef PC
			free(tempname);
#endif
			tempname = NULL;
		}
        
        if (std_in)
        {
            (void)fclose(ifp);
        }
		return 1;
	}
	cur = str = (char *)safe_malloc(st.st_size + 1);

   	while ((ifp) && (fgets(cur, (int)st.st_size, ifp)))

    {

        if (st.st_size > 1)

                cur += strlen(cur);

        else

                break;

    }
	if (std_in)
		ifname = "stdin";
	if (lc_cryptstr(lm_job, str, &ofile_str, /*&v*/code,
			forceit, ifname, &err))
	{
		if (efp == lm_flex_stderr())
		{
			fprintf(ofp, "%s\n", err);
			if (ofp != lm_flex_stdout())
				fprintf(lm_flex_stderr(), "%s\n", err);
		}
		else
			fprintf(efp, "%s\n", err);
        if (err)
            flag = 1;
	}
	if (ofile_str)
	{
		fputs(ofile_str, ofp);
		lc_free_mem(lm_job, ofile_str);
	}
	free(str);
	if (tempname)
	{
		remove(tempname);
#ifdef PC
		free(tempname);
#endif
	}
    
    if (std_in)
    {
        (void)fclose(ifp);
    }
    
	return 0;
}
