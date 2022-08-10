#############################################################################
# Copyright (c) 1997-2018, 2020, 2022 Flexera. All Rights Reserved.
#############################################################################

# Use make PIE=1 for creating Position Independent Executable

# Use make SDT=1 for License File-Based licensing TRA
# Use make SIMPLIFY_LMNEW=1 for generating a fixed value lm_new.c


MAINSRCDIR=..
SRCDIR = $(MAINSRCDIR)/machind

INCFLAGS = -I$(SRCDIR) -I.

THREADLIB = -lpthread

XTRALIB = ${SSLLIB} -lrt -ldl

XTRACFLAG = -DFD_LIMIT_CHECK -DFLEX_INET6 -g -O2 -m64 -D__BITS64 -pipe -D_POSIX_C_SOURCE=199506L -D_GNU_SOURCE -DOS_LINUX -pthread -DECMC_DISABLE_FAKE_TPM=1 -DLM_INTERNAL -DFLEXLM_KITBUILD -DFLEX_STATIC -DVALIDATOR_UTILITY_SUPPORT -DLINUX 

LDSHAREFLAGS = -shared -Wl,--dynamic-linker=/lib64/ld-lsb-x86-64.so.3

MAJORVER = 11

SOEXTENSION = .so

#
#	Use XTRAOBJS to define your object files for vendor daemon
#	initialization routines, checkout filter, checkin callback, etc.
#
XTRAOBJS = 

LDFLAGS = -Wl,--dynamic-linker=/lib64/ld-lsb-x86-64.so.3

LINTFLAGS = -a -b -h

PIC = _pic
CFLAGS_PIC = -fPIC -DIS_PIC
CFLAGS_COMMON = -g $(INCFLAGS) $(XTRACFLAG) 
CFLAGS = $(CFLAGS_COMMON) -DNO_ACTIVATION_SUPPORT

SRCS	= $(SRCDIR)/lsvendor.c

STRIP = strip

OBJS = lmcrypt.o \
	lsvendor.o 

USYMBOLS =

ifdef DONGLE
DONGLELIB = liblmgr_dongle.a
DONGLEPICLIB = liblmgr_dongle_pic.a
else
DONGLELIB = liblmgr_dongle_stub.a
DONGLEPICLIB = liblmgr_dongle_stub_pic.a
endif


#
#For GCC version > 6 PIE is configured as default.
#Checking GCC version based on which Linker flag will be decided when kit is built without PIE.
#
CHECK_ENABLE_DEFAULT_PIE=$(shell expr `gcc -dumpversion | cut -f1-2 -d.` \>= 6| bc)

#
#Enable PIE for Client application and utitlies
#
ifdef PIE
CFLAGS_PIE=-fpie
LDFLAGS_PIE =-pie
PIC_LIB= _pic
DONGLELIB=$(DONGLEPICLIB)
else
CFLAGS_PIE=-fno-pie
ifeq (${CHECK_ENABLE_DEFAULT_PIE},1)
LDFLAGS_PIE=-no-pie
endif
endif

#
#Enable dynamic SSL library linking
#
ifdef DYNSSL
ifdef DYNSSLDIR
BUILD_SSL_LIB=ld -shared -o libfnpssl.so fnp_ssl_wrapper.o -L $(DYNSSLDIR) -lssl -lcrypto
else
BUILD_SSL_LIB=ld -shared -o libfnpssl.so fnp_ssl_wrapper.o -lssl -lcrypto
endif
else
BUILD_SSL_LIB=ln -s libfnpssl_static.so libfnpssl.so
endif

#
#use PIC library while building with PIE option,else use non PIC library
#
FNPLOADLIB= libFNPload${PIC_LIB}.a
CLIENTLIB = liblmgr$(PIC_LIB)_trl.a libcrvs$(PIC_LIB).a libsb$(PIC_LIB).a $(BORROWOBJ) ${DONGLELIB}
LIBS = liblmgr_as$(PIC_LIB).a liblmgr_s$(PIC_LIB).a $(CLIENTLIB)
ACTSTUB =./activation/lib/libnoact$(PIC_LIB).a

ifdef SDT
LMFLEXSDTSRC = ../examples/sdt
SDTINCLUDES = $(SRCDIR)/sdt/Include
PREPTOOL = ./preptool
AAINCDIR = $(SRCDIR)/activation/include
endif

LM_NEW_OBJ = lm_new$(PIC_LIB).o

#
#Disable PIE for lm_new.o
#
CFLAGS_NO_PIE=-fno-pie
ifeq (${CHECK_ENABLE_DEFAULT_PIE},1)
LDFLAGS_NO_PIE=-no-pie
endif

LD=$(CC)

DAEMON = demo

EXECS = lmcrypt lmflex


LINTLIB =	llib-llmgr.a.ln

UTILS = lmhostid lmdown lminstall lmremove \
	lmreread lmswitchr lmstat lmdiag lmver lmpath lmborrow lmborrowl lmswitch lmnewlog lmvminfo lmlicvalidator

ifdef SDT
SDT:lmflexsdt
endif

all: $(EXECS) $(DAEMON) utils 


lmnewgen:	$(MAINSRCDIR)/machind/lsvendor.c $(MAINSRCDIR)/machind/lm_code.h
	./lmrand1 -i $(SRCDIR)/lsvendor.c
	$(CC) -c $(CFLAGS) $(CFLAGS_PIE) lmcode.c
	$(CC) $(CFLAGS) $(LDFLAGS_PIE) $(LDFLAGS) lmnewgen.o lmcode.o -L . -lsimplifier -Wl,-rpath,.  $(CLIENTLIB) $(ACTSTUB) $(XTRALIB) $(THREADLIB) \
						-o lmnewgen

daemon:	$(DAEMON)

$(DAEMON):	$(XTRAOBJS) $(LIBS) $(SRCDIR)/lsserver.h $(SRCDIR)/lm_code.h $(LM_NEW_OBJ) 
	$(MAKE) $(LM_NEW_OBJ)
	$(CC) -c $(CFLAGS_PIE) $(CFLAGS) lsrvend.c 
	mv lsrvend.o lsvendor.o
	rm -f lsrvend.c
	$(CC) $(CFLAGS) $(LDFLAGS_PIE) $(LDFLAGS) -o $(DAEMON) lsvendor.o $(LM_NEW_OBJ) $(XTRAOBJS) \
				$(LIBS) $(ACTSTUB) $(XTRALIB) $(THREADLIB)
	$(STRIP) $(DAEMON)
	rm -f $(LM_NEW_OBJ)

lm_new.o: $(SRCDIR)/lsvendor.c $(SRCDIR)/lm_code.h $(SRCDIR)/lmclient.h \
	$(CLIENTLIB)
	./lmrand1 -i $(SRCDIR)/lsvendor.c
	$(CC) -c $(CFLAGS_NO_PIE) $(CFLAGS) lmcode.c
	$(CC) $(CFLAGS) $(LDFLAGS_NO_PIE) $(LDFLAGS) lmnewgen.o lmcode.o -L . -lsimplifier -Wl,-rpath,.  $(CLIENTLIB) $(XTRALIB) $(THREADLIB) $(ACTSTUB) \
						-o lmnewgen
	rm -f lm_new.c
ifdef SIMPLIFY_LMNEW 
	 ./lmnewgen $(DAEMON) -s 
else 
	 ./lmnewgen $(DAEMON) 
endif
	$(CC) -c $(CFLAGS_NO_PIE) $(CFLAGS) lm_new.c

lm_new$(PIC).o: $(SRCDIR)/lsvendor.c $(SRCDIR)/lm_code.h $(SRCDIR)/lmclient.h \
	$(CLIENTLIB)
	./lmrand1 -i $(SRCDIR)/lsvendor.c
	$(CC) -c $(CFLAGS) $(CFLAGS_PIE) lmcode.c
	$(CC) $(CFLAGS) $(LDFLAGS_PIE) $(LDFLAGS) lmnewgen.o lmcode.o -L . -lsimplifier -Wl,-rpath,.  $(CLIENTLIB) $(XTRALIB) $(THREADLIB) $(ACTSTUB) \
						-o lmnewgen
	rm -f lm_new.c
ifdef SIMPLIFY_LMNEW 
	 ./lmnewgen $(DAEMON) -s 
else 
	 ./lmnewgen $(DAEMON) 
endif
	$(CC) -c $(CFLAGS) $(CFLAGS_PIC) -o lm_new$(PIC).o lm_new.c

lmcrypt: $(SRCDIR)/lmcrypt.c \
		$(SRCDIR)/lmclient.h $(SRCDIR)/lm_code.h lmprikey.h $(CLIENTLIB)
	$(CC) -c $(CFLAGS_PIE) $(CFLAGS) $(SRCDIR)/lmcrypt.c
	$(CC) $(LDFLAGS_PIE) $(CFLAGS) $(LDFLAGS) -o lmcrypt lmcrypt.o \
	$(CLIENTLIB) $(ACTSTUB) $(XTRALIB) $(THREADLIB)
	rm -f $(LM_NEW_OBJ)

lmprikey.h: $(LM_NEW_OBJ)

lmflex:	$(SRCDIR)/lmflex.c $(SRCDIR)/lm_code.h $(CLIENTLIB) $(LM_NEW_OBJ)
	$(MAKE) $(LM_NEW_OBJ)
	$(CC) -c $(CFLAGS_PIE) $(CFLAGS) $(SRCDIR)/lmflex.c
	$(CC) $(CFLAGS) $(LDFLAGS_PIE) $(LDFLAGS) -o lmflex lmflex.o $(LM_NEW_OBJ) $(CLIENTLIB) $(ACTSTUB) $(XTRALIB) \
 $(THREADLIB)
	rm lmflex.o
	$(STRIP) lmflex
	rm -f $(LM_NEW_OBJ)


lmutil:	liblmutil.a liblmgr_trl.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o lmutil liblmutil.a $(CLIENTLIB) $(XTRALIB) $(THREADLIB)
	$(STRIP) lmutil

lmgrd:	liblmgrd.a liblmgr_s.a liblmgr_trl.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o lmgrd liblmgrd.a liblmgr_s.a liblmgr_trl.a $(XTRALIB) $(THREADLIB)
	$(STRIP) lmgrd

shared_object:	liblmgr$(PIC)_trl.a libsb$(PIC).a libcrvs$(PIC).a lm_new$(PIC).o
	$(CC) $(CFLAGS) $(LDSHAREFLAGS) $(USYMBOLS) -o liblmgr$(MAJORVER)$(SOEXTENSION) \
	lm_new$(PIC).o \
	liblmgr$(PIC)_trl.a \
	$(DONGLEPICLIB) \
	libcrvs$(PIC).a \
	libsb$(PIC).a \
	$(ACTSTUB) \
	$(XTRALIB) \
	$(THREADLIB) \
	-lc

lmflexsdt: $(LMFLEXSDTSRC)/lmflexsdt.c $(SRCDIR)/sdt/Source/sdtdata.c $(LMFLEXSDTSRC)/lmflexsdtdefs.c ${LM_NEW_OBJ}
	$(MAKE) ${LM_NEW_OBJ}
	$(CC) -c $(CFLAGS_PIE) $(CFLAGS_COMMON) -I$(AAINCDIR) -I$(SDTINCLUDES) $(SRCDIR)/sdt/Source/sdtdata.c
	$(CC) -c $(CFLAGS_PIE) $(CFLAGS_COMMON) -I$(AAINCDIR) -I$(SDTINCLUDES) $(LMFLEXSDTSRC)/lmflexsdtdefs.c
	$(CC) -c $(CFLAGS_PIE) $(CFLAGS_COMMON) -I$(AAINCDIR) -I$(SDTINCLUDES) $(LMFLEXSDTSRC)/lmflexsdt.c
	$(CC) $(CFLAGS_COMMON) $(LDFLAGS_PIE) $(LDFLAGS) -o lmflexsdt lmflexsdt.o sdtdata.o lmflexsdtdefs.o ${LM_NEW_OBJ} $(CLIENTLIB) $(FNPLOADLIB) $(XTRALIB) $(THREADLIB)
	$(STRIP) lmflexsdt
	$(PREPTOOL) --sdtonly -v $(LMFLEXSDTSRC)/lmflexsdt_unix.xml
	rm -f ${LM_NEW_OBJ} sdtdata.o lmflexsdtdefs.o lmflexsdt.o

lint:;	lint $(INCFLAGS) $(LINTFLAGS) $(SRCS) $(LINTLIB)

clean:;	rm -f $(OBJS) $(EXECS) $(DAEMON) $(UTILS) lmcode.c lmcode.o \
	lmflexsdt* \
	lmnewgen lsrvend.c lmflex lm_new* *.h




lmcrypt.o:	$(SRCDIR)/lmcrypt.c $(SRCDIR)/lm_code.h $(SRCDIR)/lmclient.h
	$(CC) -c $(CFLAGS_PIE) $(CFLAGS) $(SRCDIR)/lmcrypt.c

ssl_library:
	@rm -f libfnpssl.so
	$(BUILD_SSL_LIB)

utils:	
	-rm -f lmhostid lmver lmdown lmremove lmreread lmswitchr \
		lmstat lmdiag lminstall lmpath lmborrow lmborrowl lmswitch lmnewlog lmvminfo lmlicvalidator
	ln -s lmutil lmhostid
	ln -s lmutil lmver
	ln -s lmutil lmdown
	ln -s lmutil lmremove 
	ln -s lmutil lmreread 
	ln -s lmutil lmswitchr 
	ln -s lmutil lmstat 
	ln -s lmutil lmdiag
	ln -s lmutil lminstall
	ln -s lmutil lmpath
	ln -s lmutil lmnewlog
	ln -s lmutil lmborrow
	ln -s lmutil lmborrowl
	ln -s lmutil lmswitch
	ln -s lmutil lmvminfo
	ln -s lmutil lmlicvalidator

oldnames_comment:
	@echo Many filenames were shortened in v5.
	@echo If you want links to the old pre-v5 names, please type
	@echo 	\'make oldnames\'

oldnames:
	rm -f lmcrypter; ln lmcrypt lmcrypter	
	-(\
		cd ../machind; \
		rm -f ls_vendor.c; ln lsvendor.c ls_vendor.c; \
		rm -f lm_client.h; ln lmclient.h lm_client.h; \
		rm -f lm_errors.h; ln lmerrors.h lm_errors.h; \
		rm -f lm_hosttype.h; ln	lmhtype.h lm_hosttype.h; \
		rm -f lmcrypter.c; ln lmcrypt.c lmcrypter.c; \
		rm -f ls_feature.h; ln lsfeatur.h ls_feature.h; \
		rm -f ls_master.h; ln lsmaster.h ls_master.h; \
		rm -f ls_server.h; ln lsserver.h ls_server.h; \
		rm -f ls_vendor.c; ln lsvendor.c ls_vendor.c; \
	)

eval:
	rm -f lmprikey.h $(UTILS) lmcrypt.o lmpubkey.h lsvendor.o lm_new.c \
			lmcode.c lmcode.o
