FlexNet Publisher Composite Transaction Utility
===============================================

The files in this folder are used to build demo activation utilities that 
use composite transactions to transfer rights from a server to end-users 
and enterprise license servers.  Composite transactions allow multiple actions
to be performed in one round trip to the server, for example return some rights
and acquire others.


It is intended as a resource for developers, validators and documenters of 
FNP and FNP enabled products.  It is not intended for release to end-users, 
rather as a model for the development of activation utilites for FNP enabled 
products. 

IN PARTICULAR the functionality it provides for cancelling stored requests 
could be used by fraudulent users to obtain additional licences using a manual
(offline) return transaction - by cancelling the request instead of processing
the response.  This ability may need to be provided so that bona fide users 
who start a return transaction but cannot complete can continue to check out 
from the fulfillment.  The business rules must define the criteria for 
allowing return requests to be cancelled.

The utility is built by the kit makefile.act to binaries appcomptranutil and 
servercomptranutil.

To obtain usage information, launch the binary with no arguments or with -help

    appcomptranutil -help

Developers - see comment block at the start of compTranUtil.c
 