#
#
# Copyright (c) 1999, 2024, Oracle and/or its affiliates.
#
#       This material is the confidential property of Oracle Corporation
#       or its licensors and may be used, reproduced, stored or transmitted
#       only in accordance with a valid Oracle license or sublicense agreement.
#


OS=linux
VERSION=15.0

##########

PINDIR=../../..
INCDIR=$(PINDIR)/include

##########
CC_linux = gcc
CC = $(CC_$(OS))

###########

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFOR_CM -DFM_REMIT_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM -DFM_REMIT_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

OS_LIBS_linux= -lm
OS_LIBS= $(OS_LIBS_$(OS))

####

INCFILES= $(INCDIR)/pin_rate.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_flds.h $(INCDIR)/pin_currency.h

FILES= fm_remit_pol_config.c \
	fm_remit_pol_spec_qty.c

OBJECTS= fm_remit_pol_config.o \
	fm_remit_pol_spec_qty.o

#

LIBACT=fm_remit_pol_custom.$(SL_EXT)

###########

all: $(LIBACT)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBACT)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBACT): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBACT) $(LDFLAGS) $(OBJECTS) $(OS_LIBS)

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:
