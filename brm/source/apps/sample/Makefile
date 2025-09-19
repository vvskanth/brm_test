#
#
# Copyright (c) 1996, 2024, Oracle and/or its affiliates.
#      
#       This material is the confidential property of Oracle Corporation or its
#       licensors and may be used, reproduced, stored or transmitted only in
#       accordance with a valid Oracle license or sublicense agreement.
#
#

OS=linux
PIN_VERSION=15.0

##########

PINDIR=$(PIN_HOME)
INCDIR=$(PINDIR)/include
LIBDIR=$(PINDIR)/lib

##########
CC_comp_linux = gcc
CC_link_linux = gcc
CC_comp = $(CC_comp_$(OS))
CC_link = $(CC_link_$(OS))

##########
CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS))

CPPFLAGS = -I$(INCDIR)

LDFLAGS = -L$(LIBDIR) $(LDFLAGS_$(OS))


LDLIBS= -lportal -lpinsys $(OS_LIBS_$(OS))

##########

INCFILES= $(INCDIR)/pcm.h \
	$(INCDIR)/pin_bill.h $(INCDIR)/pin_cust.h $(INCDIR)/pin_act.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h

###########

APPS= sample_act sample_app sample_del sample_search sample_who

OBJECTS= sample_act.o sample_app.o sample_del.o sample_search.o sample_who.o

###########

all: $(APPS)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(APPS)

###########

$(APPS): $(OBJECTS) $(INCFILES) Makefile
	$(CC_link) -o $@ $(CPPFLAGS) $(LDFLAGS) $@.o $(LDLIBS)

$(OBJECTS): $(INCFILES) Makefile
	$(CC_comp) -c $(CFLAGS) $(CPPFLAGS) $*.c

###########

FRC:

