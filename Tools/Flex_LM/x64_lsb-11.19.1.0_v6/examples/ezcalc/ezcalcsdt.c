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
#endif /* WINDOWS */
#include "lm_attr.h"
#define LICPATH "@localhost:license.dat:."
#include "FlxActApp.h"
#include "sdtdefs.h"

VENDORCODE code;

const char *	pszAppName = "ezcalcsdt";
const char *	pszPublisherName = "demo";

typedef enum ez_operation {
	NOOP, ADD, ADD_NO_CHECKOUT, SUBTRACT, MULTIPLY, DIVIDE, POWER, AVG, SET, CLEAR, QUIT
} EZ_OPERATION;


static
void
sInstallService()
{
#ifdef WINDOWS
	fnpActSvcInstallWin("demo", "ezcalcsdt");
#endif /* WINDOWS */
}

char getOpChar(EZ_OPERATION op)
{
	char c = ' ';

	switch (op)
	{
		case ADD:
		case ADD_NO_CHECKOUT:
						c = '+';
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
		default:        break ;
	}

	return c;
}

void getOpString(EZ_OPERATION op, char *opString)
{
	strcpy(opString, "");

	switch (op)
	{
		case ADD:		
		case ADD_NO_CHECKOUT:
						strcpy(opString, "ADD");
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
		default:        break ;
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
	printf("  + to add\n");
	printf("  - to subtract\n");
	printf("  x to multiply\n");
	printf("  / to divide\n");
	printf("  ^ to raise to power\n");
	printf("  a to average\n");
	printf("  t to add (behavior of SDTs when valid license checkout circumvented) e.g. t 5\n");
	printf("  c to clear\n");
	printf("  q to quit\n");


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
				case 't':   *opPtr = ADD_NO_CHECKOUT;
							break;
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

	printf("\nChecked out: %s\n", feature);
	return 1;
}

void checkinFeature(LM_HANDLE *lm_job, EZ_OPERATION op)
{
	char feature[256];

	if ((op == SET) || (op == CLEAR) || (op == QUIT))
		return;

	getOpString(op, feature);

	lc_checkin(lm_job, feature, 0);

	printf("Checked in : %s\n", feature);
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


int main(int argc, char * argv[])
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


	while (1)
	{
		getOpInput(lastOp, currentValue, &op, &enteredValue);

		if (op == QUIT)
		{
			printf("Thank you for using EZCalc. Bye!\n");
			break;
		}

		if (op != ADD_NO_CHECKOUT)
		{
			if (!checkoutFeature(lm_job, op))
				continue;
		}
		else
		{
			printf("\nCheckout: skipped for ADD\n");
		}

		switch (op)
		{
			case SET:
                {
                    currentValue = enteredValue;
                }
                break;

			case CLEAR:
                {
                    currentValue = 0;
                }
                break;

            case ADD: /* currentValue = currentValue + enteredValue; */
			case ADD_NO_CHECKOUT: 
                {
                    SDT_FEATURE_DECLARE_VAR( FeatureAdd, current );
                    SDT_FEATURE_DECLARE_VAR( FeatureAdd, entered );

					printf("Operation  : %d + %d = ", currentValue, enteredValue);

                    SDT_FEATURE_SET( current, currentValue );
                    SDT_FEATURE_SET( entered, enteredValue );

                    SDT_FEATURE_ADD( current, entered );

                    currentValue = SDT_FEATURE_GET( current );

					printf("%d\n", currentValue);
                }
                break;

			case SUBTRACT: /* currentValue = currentValue - enteredValue; */
                {
                    SDT_FEATURE_DECLARE_VAR( FeatureSubtract, current );
                    SDT_FEATURE_DECLARE_VAR( FeatureSubtract, entered );

					printf("Operation  : %d - %d = ", currentValue, enteredValue);

                    SDT_FEATURE_SET( current, currentValue );
                    SDT_FEATURE_SET( entered, enteredValue );

                    SDT_FEATURE_SUB( current, entered );

                    currentValue = SDT_FEATURE_GET( current );

					printf("%d\n", currentValue);
                }
                break;

			case MULTIPLY: /* currentValue = currentValue * enteredValue; */
                {
                    SDT_FEATURE_DECLARE_VAR( FeatureMultiply, current );
                    SDT_FEATURE_DECLARE_VAR( FeatureMultiply, entered );

					printf("Operation  : %d x %d = ", currentValue, enteredValue);

                    SDT_FEATURE_SET( current, currentValue );
                    SDT_FEATURE_SET( entered, enteredValue );

                    SDT_FEATURE_MUL( current, entered );

                    currentValue = SDT_FEATURE_GET( current );
					printf("%d\n", currentValue);
                }
                break;

			case DIVIDE: /* currentValue = currentValue / enteredValue; */
                {
                    SDT_FEATURE_DECLARE_VAR( FeatureDivide, current );
                    SDT_FEATURE_DECLARE_VAR( FeatureDivide, entered );

					printf("Operation  : %d / %d = ", currentValue, enteredValue);

                    if(0 == enteredValue)
                    {
						printf("Division by zero is invalid\n");
					}
					else
					{

						SDT_FEATURE_SET( current, currentValue );
						SDT_FEATURE_SET( entered, enteredValue );

						SDT_FEATURE_DIV( current, entered );

						currentValue = SDT_FEATURE_GET( current );
					}

					printf("%d\n", currentValue);
                }
                break;

			case POWER:
                {
                    /*
                    if (enteredValue == 0)
                    currentValue = 1;
                    else
                    {
                    int i=0, curValue = currentValue;
                    for (i = 1; i < enteredValue; i++)
                    currentValue = currentValue * curValue;
                    }
                    */
                    SDT_FEATURE_DECLARE_VAR( FeaturePower, current );
                    SDT_FEATURE_DECLARE_VAR( FeaturePower, entered );
                    SDT_FEATURE_DECLARE_VAR( FeaturePower, zero );

					printf("Operation  : %d ^ %d = ", currentValue, enteredValue);

                    SDT_FEATURE_SET_CONST( zero, 0 );

                    SDT_FEATURE_SET( current, currentValue );
                    SDT_FEATURE_SET( entered, enteredValue );

                    /* if (enteredValue == 0) */
                    if( SDT_FEATURE_EQ( entered, zero ) )
                    {
                        /* currentValue = 1; */
                        SDT_FEATURE_SET_CONST( current, 1 );
                    }
                    else
                    {
                        /* int i=0, curValue = currentValue; */
                        SDT_FEATURE_DECLARE_VAR( FeaturePower, increment );
                        SDT_FEATURE_DECLARE_VAR( FeaturePower, i );
                        SDT_FEATURE_DECLARE_VAR( FeaturePower, curValue );

                        curValue = current;
                        SDT_FEATURE_SET_CONST( increment, 1 );

                        /* for( i = 1; i < enteredValue; ++i )
                           {
                                currentValue = currentValue * curValue;
                           }
                        */
                        SDT_FEATURE_SET_CONST( i, 1 );
                        while( SDT_FEATURE_LT( i, entered ) )
                        {
                            SDT_FEATURE_MUL( current, curValue );
                            SDT_FEATURE_ADD( i, increment );
                        }
                    }

                    currentValue = SDT_FEATURE_GET( current );
					printf("%d\n", currentValue);

                }
                break;

			case AVG: /* currentValue = (currentValue + enteredValue)/2; */
                {
                    SDT_FEATURE_DECLARE_VAR( FeatureAvg, current );
                    SDT_FEATURE_DECLARE_VAR( FeatureAvg, entered );
                    SDT_FEATURE_DECLARE_VAR( FeatureAvg, two );

					printf("Operation  : Average of (%d + %d) = ", currentValue, enteredValue);

                    SDT_FEATURE_SET_CONST( two, 2 );
                    SDT_FEATURE_SET( current, currentValue );
                    SDT_FEATURE_SET( entered, enteredValue );

                    SDT_FEATURE_ADD( current, entered );
                    SDT_FEATURE_DIV( current, two );

                    currentValue = SDT_FEATURE_GET( current );
					printf("%d\n", currentValue);
                }
                break;
			default:
				break ;
		}

		if (op != ADD_NO_CHECKOUT)
			checkinFeature(lm_job, op);
	}

	lc_free_job(lm_job);
	cleanup(initHandle);
	return 0;
}
