###########
#
#
# Copyright (c) 1999, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation
#      or its licensors and may be used, reproduced, stored
#      or transmitted only in accordance with a valid Oracle license or
#      sublicense agreement.
#
###########

OS=linux
VERSION=15.0

##########

PINDIR=../../..
INCDIR=$(PINDIR)/include
LIBDIR=$(PINDIR)/lib

##########
CC_linux = gcc
CC = $(CC_$(OS))

###########

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFLIST_HEAP -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR)

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS)) -L$(LIBDIR)

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

OS_LIBS_linux= -lm
OS_LIBS= $(OS_LIBS_$(OS))

####

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_flds.h $(INCDIR)/pin_currency.h \
	$(INCDIR)/pin_act.h $(INCDIR)/pin_bill.h

FILES= fm_act_pol_config.c \
	fm_act_pol_event_limit.c \
	fm_act_pol_event_notify.c \
	fm_act_pol_spec_vrfy.c \
	fm_act_pol_spec_rates.c \
	fm_act_pol_spec_glid.c \
	fm_act_pol_validate_schedule.c \
	fm_act_pol_spec_candidate_rums.c \
	fm_act_pol_spec_event_cache.c \
	fm_act_pol_config_billing_cycle.c \
	fm_act_pol_pre_authorize.c \
	fm_act_pol_post_authorize.c \
	fm_act_pol_pre_reauthorize.c \
	fm_act_pol_post_reauthorize.c 
#	fm_act_pol_process_events.cpp

OBJECTS= fm_act_pol_config.o \
	fm_act_pol_event_limit.o \
	fm_act_pol_event_notify.o \
	fm_act_pol_spec_vrfy.o \
	fm_act_pol_spec_rates.o \
	fm_act_pol_spec_glid.o \
	fm_act_pol_validate_schedule.o \
	fm_act_pol_spec_candidate_rums.o \
	fm_act_pol_spec_event_cache.o \
	fm_act_pol_config_billing_cycle.o \
	fm_act_pol_pre_authorize.o \
	fm_act_pol_post_authorize.o \
	fm_act_pol_pre_reauthorize.o \
	fm_act_pol_post_reauthorize.o 
#	fm_act_pol_process_events.o

#

LIBACT=fm_act_pol_custom.$(SL_EXT)

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
	$(LD) -o $(LIBACT) $(LDFLAGS) $(OBJECTS) $(OS_LIBS) -lpsiu_for_cm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:
