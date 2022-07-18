/**************************************************************************************************
* Copyright (c) 2020 Flexera. All Rights Reserved.
**************************************************************************************************/

/*
 * Vendor Function example code-snippet: Prime number generator
 * ============================================================
 *
 * The purpose of this function is to demonstrate the operation of a Vendor Function
 *
 * Specifically the function below finds the next prime number from the specified starting value
 * The result is returned as a string.
 * Additionally the supplied number may be appended with an 'l' character, this will cause the
 * Vendor Function to allow write the answer to the Vendor Daemon log-file.
 *
 * To use this function it must be inserted into ls_vendor.c immediately after the "* vendor message *"
 * comment. The variable 'ls_vendor_msg' should be assigned the value s_vf_func instead of 0:-
 *
 * char *(*ls_vendor_msg)() = s_vf_func;
 *
 * On platforms that support it, a worker thread can be enabled to process Vendor Functions asynchronously
 * and hence not disrupt normal licensing-related operations whilst they are running.  To enable the worker
 * thread, change the initialization of ls_vendor_msg_async from 0 to 1.
 *
 * int ls_vendor_msg_async = 1;
 *
 * There is one final variable associated the the asynchronous processing of Vendor Functions: ls_vendor_msg_list_size
 * This variable limits the number of queued asynchronous Vendor Function request to the chosen value.
 * In most use-cases it is likely that the default value in the lsvendor.c should not need changing.
 *
 */

static char* s_vf_func(char* startStr)
{
	/* A useful tip to avoid the need for heap management is to use the input buffer
     * to store the reply.  
     * Note: The buffer size for message (incoming and outgoing) is limited to LM_VSEND_MAX_LEN bytes (defined in lmclient.h)
     */
	char* res = startStr;

	char* eptr;											/* Will point to first non-digit after calling strtoll */

	long long start = strtoll(startStr, &eptr, 10);		/* Extract the start value from the input string */
	long long candidate = start;						/* Tracks the candidate values for a prime */
	long long prime = 0;								/* Holds the resulting prime number (or 0 on failure) */

	int doLog = 0;										/* Logging enabled flag */

	/* If the supplied number had an 'l' character appended, then enable logging */
	if( *eptr == 'l' )
	{
		doLog = 1;
		eptr++;				/* Skip over the 'l' character */
	}

	/* If the strtoll call was successful, then we can start searching for a prime */
	if( *eptr == 0 )
	{
		int i=1;

		do	
		{
			++candidate;

			/* This loop is deliberately inefficient in order to demonstrate the performance 
             * benefits of using a worker thread
			*/
			for( i=candidate-1; i>1; i-- )
			{
				if( (candidate/i)*i == candidate )
					break;
			}

		} while( i != 1 );

		prime = candidate;
	}

	/* 
     * We've finished processing, so now we convert the value in prime to a string
 	*/
#ifndef PC
	snprintf(res, LM_VSEND_MAX_LEN, "%lld", prime);
#else
	sprintf(res, "%lld", prime);
#endif

	/* If logging was enabled, we also write the result into the Vendor Daemon log */
	if (doLog)
    {
        char buffer[LM_VSEND_MAX_LEN];

        memset(buffer,0,sizeof(buffer));
#ifndef PC
        snprintf(buffer, sizeof(buffer)-1, "INFO: Vendor Function completed. Request=%lld, Response=%lld\n", start, prime);
#else
        sprintf(buffer, "INFO: Vendor Function completed. Request=%lld, Response=%lld\n", start, prime);
#endif

		/* As we are using the ls_log_message() function from the server-side API, we need to ensure we don't clash
		 * with the main thread using the same data structures.  The ls_api_mutex_lock()/ls_api_mutex_unlock() provide
		 * that safeguard.
		 * Note that whilst we hold the lock the Vendor Daemon will not be able to service any incoming messages,
		 * consequently the locked time should be kept to a minimum.
		 */
        ls_api_mutex_lock(1000);

        ls_log_message((buffer));												/* Use the log message ls_ api All directly.. */ 

        ls_api_mutex_unlock();
    }

	
	return res;
}
