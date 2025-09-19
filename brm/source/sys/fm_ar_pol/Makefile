###########
#
#
# Copyright (c) 2004, 2024, Oracle and/or its affiliates.
#
#	This material is the confidential property of Oracle Corporation or its
#	licensors and may be used, reproduced, stored or transmitted only in
#	accordance with a valid Oracle license or sublicense agreement.
#
###########

OS=linux
VERSION=15.0

##########
CC_linux = gcc
CC = $(CC_$(OS))

##########

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFOR_CM -DFM_AR_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM -DFM_AR_POL_DLL

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

####

PINDIR=../../..
INCDIR=$(PINDIR)/include

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_act.h $(INCDIR)/pin_ar.h \
	$(INCDIR)/pin_bill.h $(INCDIR)/pin_flds.h \
	$(INCDIR)/pin_currency.h

#

FILES= fm_ar_pol_config.c \
	fm_ar_pol_reverse_writeoff.c \
	fm_ar_pol_get_event_rum_charge_details.c 
#	fm_ar_pol_pre_event_adjustment_custom.c

#

OBJECTS= fm_ar_pol_config.o \
	fm_ar_pol_reverse_writeoff.o \
	fm_ar_pol_get_event_rum_charge_details.o 
#	fm_ar_pol_pre_event_adjustment_custom.o


LIBAR=fm_ar_pol_custom.$(SL_EXT)

###########

all: $(LIBAR)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBAR)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBAR): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBAR) $(LDFLAGS) $(OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile 
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

FRC:

