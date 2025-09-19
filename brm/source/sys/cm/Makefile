###########
#
#
# Copyright (c) 1999, 2023, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation or its
#      licensors and may be used, reproduced, stored or transmitted only in
#      accordance with a valid Oracle license or sublicense agreement.
#
###########

###############################################################################
###############################################################################
###############################################################################
#
# NOTE FOR PURIFY/PURECOVERAGE/QUANTIY USE:
#   Please set PURECOV_OPTIONS,QUANTIY and/or PURIFY_OPTIONS prior to running make.
#   To ensure data consistancy, use the same machine for compiling and testing.
#
###############################################################################
###############################################################################
###############################################################################


OS=linux
PIN_VERSION=15.0


PINDIR=$(PIN_HOME)
INCDIR=$(PINDIR)/include

##########
CC_linux = gcc
CC = $(CC_$(OS))

###########

CFLAGS_linux= -g 
CFLAGS= $(CFLAGS_$(OS)) -DFLIST_HEAP

CPPFLAGS = -I$(INCDIR)

LDFLAGS = $(LDFLAGS_$(OS))

FLDFLAGS_linux=
FLDFLAGS = $(FLDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

OS_LIBS_linux=  -L $(PIN_HOME)/lib -lcmpin -lpinsys -lcm_main -lBAS -lSCR  -ldl -ldbt23112d -lfunctor23112d -lfunctor_list23112d -lfunctor_map23112d -litc23112d -lmime23112d -lmny23112d -lnetwork23112d -lpointer23112d -lstdc++ -lstreams23112d -lsync23112d -lthread23112d -lthrexcept23112d -ltls23112d -ltrace23112d -lcrypt -lpthread -lrt -lm -lc -Wl,--as-needed /lib64/ld-linux-x86-64.so.2

OS_LIBS= $(OS_LIBS_$(OS))

DBG_OS_LIBS_linux= -L .  -L /vobs/publish_linux_vob/publish_linux/lib -lcmpin_dbg -lpinsys_dbg -lcm_main_dbg -lBAS_dbg -lSCR_dbg -ldl -ldbt7215d -lfunctor2615d -lfunctor_list2615d -lfunctor_map2615d -litc2615d -lmime1415d -lmny21015d -lnetwork3115d -lpointer2815d -lstdc++ -lstreams1615d -lsync4115d -lthread3215d -lthrexcept2615d -ltls9115d -ltrace3115d -lcrypt -lpthread -lrt -lm -lc  -lsupc++  -Wl,--as-needed /lib64/ld-linux-x86-64.so.2

DBG_OS_LIBS= $(DBG_OS_LIBS_$(OS))


####

INCFILES=

FILES= cm.cpp

OBJECTS= cm.o

#

PROG= cm

###########

all: $(PROG)

dbg: $(PROG)_dbg

clean:
	rm -f $(PROG) $(OBJECTS) core

clobber: clean
	rm -f $(PROG) $(OBJECTS) core

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

purifypurecov: $(FILES) $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) $(LDFLAGS)
	purify purecov $(CC) -o $(PROG) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) $(OS_LIBS) $(FLDFLAGS)

purifypurecovdbg: $(FILES) $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) $(LDFLAGS)
	purify purecov $(CC) -o $(PROG) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) $(DBG_OS_LIBS) $(FLDFLAGS)

purecovdbg: $(FILES) $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) $(LDFLAGS)
	purecov $(CC) -o $(PROG) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) $(DBG_OS_LIBS) $(FLDFLAGS)

purecov: $(FILES) $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) $(LDFLAGS)
	purecov $(CC) -o $(PROG) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) $(OS_LIBS) $(FLDFLAGS)

purifydbg: $(FILES) $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) $(LDFLAGS) 
	purify $(CC) -o $(PROG) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) $(DBG_OS_LIBS) $(FLDFLAGS)

purify: $(FILES) $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) $(LDFLAGS)
	purify $(CC) -o $(PROG) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) $(OS_LIBS) $(FLDFLAGS)

$(PROG)_dbg: $(FILES) $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) $(LDFLAGS)
	$(CC) -o $(PROG) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) $(DBG_OS_LIBS) $(FLDFLAGS)

$(PROG): $(FILES) $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) $(LDFLAGS) 
	$(CC) -o $(PROG) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) $(OS_LIBS) $(FLDFLAGS)

#

FRC:


