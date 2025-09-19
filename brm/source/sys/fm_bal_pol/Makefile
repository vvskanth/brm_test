###########
#
# Copyright (c) 2005, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation or its
#      licensors and may be used, reproduced, stored or transmitted only in
#      accordance with a valid Oracle license or sublicense agreement.
#
###########

OS=linux
VERSION=15.0

####

PINDIR=../../..
INCDIR=$(PINDIR)/include
LIBDIR=$(PINDIR)/lib

##########
CC_linux = gcc
CC = $(CC_$(OS))

##########

CFLAGS_linux=  -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFOR_CM -DFM_BAL_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM -DFM_BAL_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS)) -L$(LIBDIR)

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_act.h $(INCDIR)/pin_bill.h \
	$(INCDIR)/pin_flds.h $(INCDIR)/pin_currency.h

#

FILES= fm_bal_pol_config.c \
	fm_bal_pol_get_bal_grp_and_svc.c


#

OBJECTS= fm_bal_pol_config.o \
	fm_bal_pol_get_bal_grp_and_svc.o

LIBBAL=fm_bal_pol_custom.$(SL_EXT)

###########

all: $(LIBBAL)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBBAL)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC


###########

$(LIBBAL): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBBAL) $(LDFLAGS) $(OBJECTS) -lm -lpsiu_for_cm

$(OBJECTS): $(INCFILES) Makefile $(FILES)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:

