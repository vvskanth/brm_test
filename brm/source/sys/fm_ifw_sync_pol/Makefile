##########
#
# Copyright (c) 2002, 2024, Oracle and/or its affiliates.
#
#       This material is the confidential property of Oracle Corporation or its
#       licensors and may be used, reproduced, stored or transmitted only in
#       accordance with a valid Oracle license or sublicense agreement.
#
###########

OS=linux
PIN_VERSION=15.0

##########

PINDIR=../../..
INCDIR=$(PINDIR)/include

##########
CC_linux = gcc
CC = $(CC_$(OS))

##########
CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFLIST_HEAP -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR)

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h

#
FILES= fm_ifw_sync_pol_config.c \
	fm_ifw_sync_pol_publish_event.c 

OBJECTS= fm_ifw_sync_pol_config.o \
	fm_ifw_sync_pol_publish_event.o 

#

LIBIFWSYNCPOL=fm_ifw_sync_pol_custom.$(SL_EXT)

###########

all: $(LIBIFWSYNCPOL)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBIFWSYNCPOL)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBIFWSYNCPOL): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBIFWSYNCPOL) $(LDFLAGS) $(OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:
