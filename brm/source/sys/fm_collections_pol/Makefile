#############################################################################
#
#
# Copyright (c) 2001, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation or its
#      licensors and may be used, reproduced, stored or transmitted only in
#      accordance with a valid Oracle license or sublicense agreement.
#
#############################################################################

OS=linux
VERSION=15.0

##########
CC_linux = gcc
CC = $(CC_$(OS))

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFOR_CM -DFM_COLLECTIONS_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM -DFM_COLLECTIONS_POL_DLL

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
	$(INCDIR)/pin_act.h $(INCDIR)/pin_bill.h \
	$(INCDIR)/pin_flds.h $(INCDIR)/pin_currency.h \
	$(INCDIR)/fm_collections.h

#

FILES= fm_collections_pol_config.c \
	fm_collections_pol_exit_scenario.c\
	fm_collections_pol_exec_policy_action.c\
	fm_collections_pol_prep_dunning_data.c\
	fm_collections_pol_select_profile.c\
	fm_collections_pol_apply_late_fees.c \
	fm_collections_pol_assign_agent.c \
	fm_collections_pol_apply_finance_charges.c 

#

OBJECTS= fm_collections_pol_config.o \
	fm_collections_pol_exit_scenario.o\
	fm_collections_pol_exec_policy_action.o\
	fm_collections_pol_prep_dunning_data.o\
	fm_collections_pol_select_profile.o\
	fm_collections_pol_apply_late_fees.o \
	fm_collections_pol_assign_agent.o \
	fm_collections_pol_apply_finance_charges.o 

LIBCOLLECTIONS=fm_collections_pol_custom.$(SL_EXT)

###########

all: $(LIBCOLLECTIONS)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBCOLLECTIONS)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBCOLLECTIONS): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBCOLLECTIONS) $(LDFLAGS) $(OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:

