/**************************************************************************************************
* Copyright (c) 1997-2016, 2018 Flexera. All Rights Reserved.
**************************************************************************************************/
/*

 *
 *	Description:	This is a demo application program, to illustrate
 *			the use of the Flexible License Manager.
 *			This is not intended to be complete, functional calculator application.
 *			This only exists to illustrate the use of the license APIs.
 */

#include "lmclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "lm_redir_std.h"
#ifdef WINDOWS
#include "wininstaller.h"
#endif
#include "lm_attr.h"
#ifdef PC
#define LICPATH "@localhost;license.dat;."
#else
#define LICPATH "@localhost:license.dat:."
#endif
#include "FlxActApp.h"

VENDORCODE code;

const char *	pszAppName = "ezcalc";
const char *	pszPublisherName = "demo";

typedef enum ez_operation {
	NOOP, ADD, SUBTRACT, MULTIPLY, DIVIDE, POWER, AVG, SET, CLEAR, QUIT
} EZ_OPERATION;

static
void
sInstallService()
{
#ifdef WINDOWS
	fnpActSvcInstallWin("demo", "ezcalc");
#endif /* WINDOWS */
}

char getOpChar(EZ_OPERATION op)
{
	char c = ' ';

	switch (op)
	{
		case ADD:		c = '+';
						break;
		case SUBTRACT:	c = '-';
						break;
		case MULTIPLY:	c = 'x';
						break;
		case DIVIDE:	c = '/';
						break;
		case POWER:		c = '^';
						break;
		case AVG:		c = 'a';
						break;
		default:		break;
	}

	return c;
}

void getOpString(EZ_OPERATION op, char *opString)
{
	strcpy(opString, "");

	switch (op)
	{
		case ADD:		strcpy(opString, "ADD");
						break;
		case SUBTRACT:	strcpy(opString, "SUBTRACT");
						break;
		case MULTIPLY:	strcpy(opString, "MULTIPLY");
						break;
		case DIVIDE:	strcpy(opString, "DIVIDE");
						break;
		case POWER:		strcpy(opString, "POWER");
						break;
		case AVG:		strcpy(opString, "AVG");
						break;
		default:		break;
	}
}

void
printScreen(EZ_OPERATION curOp, int curValue)
{
	char curOpChar = getOpChar(curOp);

	printf("EZ Calculator\n");
	printf("Current value:      %d\n", curValue);

	if (curOpChar != ' ')
		printf("Current operation:  %c\n", curOpChar);
	else
		printf("\n");

	printf("Enter operator and operand (right hand number) e.g. + 5\n");
	printf("        + to add\n");
	printf("        - to subtract\n");
	printf("        x to multiply\n");
	printf("        / to divide\n");
	printf("        ^ to raise to power\n");
	printf("        a to average\n\n");
	printf("        c to clear\n");
	printf("        q to quit\n");

	printf("\nOr, just enter a number  >:");
}

void
getOpInput(EZ_OPERATION curOp, int curValue, EZ_OPERATION *opPtr, int *enteredValuePtr)
{
	char inputStr[256];
	char c, *tmp;

	*enteredValuePtr=0;

	while (1)
	{
		printScreen(curOp, curValue);
		strcpy(inputStr, "");

		fgets(inputStr, 254, lm_flex_stdin());	/* add 2 for newline and \0 */
		inputStr[strlen(inputStr) - 1] = '\0';

		if (strlen(inputStr) == 0)
			continue;

		tmp = inputStr;
		/* trim initial spaces */
		while ((c = *tmp++) == ' ');

		if (isdigit(c))
		{
			tmp--;
			*enteredValuePtr = atoi(tmp);
			*opPtr = SET;
			return;
		}
		else {
			switch (c)
			{
				case '+':	*opPtr = ADD;
							break;
				case '-':	*opPtr = SUBTRACT;
							break;
				case 'x':	*opPtr = MULTIPLY;
							break;
				case '/':	*opPtr = DIVIDE;
							break;
				case '^':	*opPtr = POWER;
							break;
				case 'a':	*opPtr = AVG;
							break;
				case 'c':	*opPtr = CLEAR;
							break;
				case 'q':	*opPtr = QUIT;
							break;

				default:	printf("Invalid input, please choose from the choices\n");
							continue;
			}

			if (strlen(tmp) > 0)
				*enteredValuePtr = atoi(tmp);

			return;
		}
	}
}

int checkoutFeature(LM_HANDLE *lm_job, EZ_OPERATION op)
{
	char feature[256];

	if ((op == SET) || (op == CLEAR) || (op == QUIT))
		return 1;

	getOpString(op, feature);

	if(lc_checkout(lm_job, feature, "1.0", 1, LM_CO_NOWAIT, &code, LM_DUP_NONE))
	{
		lc_perror(lm_job, "checkout failed");
		/* exit (lc_get_errno(lm_job)); */

		printf("Checkout failed for %s with error code=%d\n",
						feature, lc_get_errno(lm_job));
		return 0;
	}

	printf("%s checked out\n", feature);
	return 1;
}

void checkinFeature(LM_HANDLE *lm_job, EZ_OPERATION op)
{
	char feature[256];

	if ((op == SET) || (op == CLEAR) || (op == QUIT))
		return;

	getOpString(op, feature);

	lc_checkin(lm_job, feature, 0);

}

static void
init(struct flexinit_property_handle **handle)
{
	struct flexinit_property_handle *ourHandle;
	int stat;

	if ((stat = lc_flexinit_property_handle_create(&ourHandle)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_create() failed: %d\n", stat);
		exit(1);
	}
	if ((stat = lc_flexinit_property_handle_set(ourHandle,
			(FLEXINIT_PROPERTY_TYPE)FLEXINIT_PROPERTY_USE_TRUSTED_STORAGE,
			(FLEXINIT_VALUE_TYPE)1)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_set failed: %d\n", stat);
	    exit(1);
	}
	if ((stat = lc_flexinit(ourHandle)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit failed: %d\n", stat);
	    exit(1);
	}
	*handle = ourHandle;
}

static void
cleanup(struct flexinit_property_handle *initHandle)
{
	int stat;

	if ((stat = lc_flexinit_cleanup(initHandle)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_cleanup failed: %d\n", stat);
	}
	if ((stat = lc_flexinit_property_handle_free(initHandle)))
	{
		fprintf(lm_flex_stderr(), "lc_flexinit_property_handle_free failed: %d\n", stat);
	}
}

int
main(int argc, char * argv[])
{
	LM_HANDLE *lm_job = NULL;
	struct flexinit_property_handle *initHandle;
	EZ_OPERATION op = NOOP, lastOp = NOOP;
	int currentValue = 0, enteredValue=0;

	/* install service if it's not already installed */
	sInstallService();

	/* initialize TS */
	init(&initHandle);

	/* initialize the job handle for license checking */
	if (lc_new_job(0, lc_new_job_arg2, &code, &lm_job))
	{
		int errorInfoNum = lc_get_errno(lm_job);

		lc_perror(lm_job, "Failed in license handle creation");
		cleanup(initHandle);
		exit(errorInfoNum);
	}

	(void)lc_set_attr(lm_job, LM_A_LICENSE_DEFAULT, (LM_A_VAL_TYPE)LICPATH);
	(void)lc_set_attr(lm_job, LM_A_CHECK_BADDATE, (LM_A_VAL_TYPE)1);
	(void)lc_set_attr(lm_job, LM_A_TS_CHECK_BADDATE, (LM_A_VAL_TYPE)1);


	while (1)
	{
		getOpInput(lastOp, currentValue, &op, &enteredValue);

		if (op == QUIT)
		{
			printf("Thank you for using EZCalc. Bye!\n");
			break;
		}

		if (!checkoutFeature(lm_job, op))
			continue;

		switch (op)
		{
			case SET:		currentValue = enteredValue;
							break;
			case CLEAR:		currentValue = 0;
							break;

			case ADD:		currentValue = currentValue + enteredValue;
							break;
			case SUBTRACT:	currentValue = currentValue - enteredValue;
							break;
			case MULTIPLY:	currentValue = currentValue * enteredValue;
							break;
			case DIVIDE:	if (enteredValue == 0)
								printf("Division by zero is invalid\n");
							else
								currentValue = currentValue / enteredValue;
							break;
			case POWER:		if (enteredValue == 0)
								currentValue = 1;
							else
							{
								int i=0, curValue = currentValue;
								for (i = 1; i < enteredValue; i++)
									currentValue = currentValue * curValue;
							}
							break;
			case AVG:		currentValue = (currentValue + enteredValue)/2;
							break;
			default:		break;
		}

		checkinFeature(lm_job, op);
	}

	lc_free_job(lm_job);
	cleanup(initHandle);
	return 0;
}
