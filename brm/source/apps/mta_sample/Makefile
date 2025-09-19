###########
#
#
# Copyright (c) 2003, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation or its
#      licensors and may be used, reproduced, stored or transmitted only in
#      accordance with a valid Oracle license or sublicense agreement.
#
###########

OS=linux
VERSION=15.0

##########

PINDIR=/home/applmgr/app/brm111/BRM/15.0.1/BRM15/BRM
INCDIR=$(PINDIR)/include
LIBDIR=$(PINDIR)/lib

##########
CC_comp_linux = gcc
CC_link_linux= gcc
CC_comp = $(CC_comp_$(OS))
CC_link = $(CC_link_$(OS))

##########
CFLAGS_linux= -fPIC -pthread
CFLAGS= $(CFLAGS_$(OS))

CPPFLAGS = -I$(INCDIR)

LDFLAGS_linux= -Wl,--export-dynamic
LDFLAGS = -L$(LIBDIR) $(LDFLAGS_$(OS))

OS_LIBS_linux= -lpthread -ldl

LDLIBS= -lportal -lmta -lpinsys $(OS_LIBS_$(OS))

##########

INCFILES= $(INCDIR)/pcm.h $(INCDIR)/pin_mta.h \
	$(INCDIR)/pin_bill.h $(INCDIR)/pin_cust.h $(INCDIR)/pin_act.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h

###########

APPS= pin_mta_test

FILES= pin_mta_test.c

OBJECTS= pin_mta_test.o

DEP_linux=$(FILES)
DEP=$(DEP_$(OS))

###########

all: $(APPS)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(APPS)

###########

$(APPS): $(OBJECTS) $(INCFILES) Makefile
	$(CC_link) -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $@.o $(LDLIBS)

$(OBJECTS): $(INCFILES) Makefile $(DEP)
	$(CC_comp) -c $(CFLAGS) $(CPPFLAGS) $*.c

###########

FRC:

