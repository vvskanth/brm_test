############
#
#
# Copyright (c) 2001, 2024, Oracle and/or its affiliates.
#
# This material is the confidential property of Oracle Corporation or its 
# licensors and may be used, reproduced, stored or transmitted only in 
# accordance with a valid Oracle license or sublicense agreement.
#
###########

OS=linux
PIN_VERSION=15.0

##########

PINDIR=../../..
INCDIR=$(PINDIR)/include

###########
CC_linux = gcc
CC = $(CC_$(OS))

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFLIST_HEAP -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT
CPPFLAGS = -I$(INCDIR)

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

OS_LIBS_linux= -lm
OS_LIBS= $(OS_LIBS_$(OS))

####

INCFILES= $(INCDIR)/ops/prov.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_flds.h

FILES= fm_prov_pol_config.c \
	fm_prov_pol_update.c

OBJECTS= fm_prov_pol_config.o \
	fm_prov_pol_update.o 

#

LIBPROV=fm_prov_pol_custom.$(SL_EXT)

DEP_linux = $(FILES)
DEP = $(DEP_$(OS))

###########

all: $(LIBPROV)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBPROV)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBPROV): $(OBJECTS) $(INCFILES) Makefile $(DEP)
	$(LD) -o $(LIBPROV) $(LDFLAGS) $(OBJECTS) $(OS_LIBS)

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:






