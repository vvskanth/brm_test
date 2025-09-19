#
#
# Copyright (c) 2003, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation
#      or its subsidiaries or licensors and may be used, reproduced, stored
#      or transmitted only in accordance with a valid Oracle license or
#      sublicense agreement.
#

OS=linux
VERSION=15.0

##########
CC_linux = gcc
CC = $(CC_$(OS))

C++_linux = gcc
C++ = $(C++_$(OS))
##########

CFLAGS_linux = -fPIC

CFLAGS= $(CFLAGS_$(OS)) -DASSIGN_NULL_AFTER_ELEM_PUT -DASSIGN_NULL_AFTER_SUBSTR_PUT -DASSIGN_NULL_AFTER_FLD_PUT

C++FLAGS_linux = -x c++ -fPIC -DPIN_NOT_USING_OSTREAM

C++FLAGS= $(C++FLAGS_$(OS)) -DPCMCPP_CONST_SAFE -DASSIGN_NULL_AFTER_ELEM_PUT -DASSIGN_NULL_AFTER_SUBSTR_PUT -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DFOR_CM -DFM_SUBSCRIPTION_POL_DLL

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
	$(INCDIR)/pin_subscription.h

#
FILES= fm_subscription_pol_config.c \
	fm_subscription_pol_config_eet.c \
	fm_subscription_pol_count_lines.c \
	fm_subscription_pol_get_sponsors.c \
	fm_subscription_pol_notify_aggregation.c \
	fm_subscription_pol_pre_fold.c \
	fm_subscription_pol_pre_transition_deal.c \
	fm_subscription_pol_pre_transition_plan.c \
	fm_subscription_pol_prep_fold.c \
	fm_subscription_pol_prep_members.c \
	fm_subscription_pol_provisioning.c \
	fm_subscription_pol_provisioning_content.c \
	fm_subscription_pol_provisioning_ssg.c \
	fm_subscription_pol_spec_cancel.c \
	fm_subscription_pol_spec_cancel_discount.c \
	fm_subscription_pol_spec_cycle_fee_interval.c \
	fm_subscription_pol_spec_fold.c \
	fm_subscription_pol_spec_rerate.c \
	fm_subscription_pol_update_cdc.c


OBJECTS= fm_subscription_pol_config.o \
	fm_subscription_pol_config_eet.o \
	fm_subscription_pol_count_lines.o \
	fm_subscription_pol_get_sponsors.o \
	fm_subscription_pol_notify_aggregation.o \
	fm_subscription_pol_pre_fold.o \
	fm_subscription_pol_pre_transition_deal.o \
	fm_subscription_pol_pre_transition_plan.o \
	fm_subscription_pol_prep_fold.o \
	fm_subscription_pol_prep_members.o \
	fm_subscription_pol_provisioning.o \
	fm_subscription_pol_provisioning_content.o \
	fm_subscription_pol_provisioning_ssg.o \
	fm_subscription_pol_spec_cancel.o \
	fm_subscription_pol_spec_cancel_discount.o \
	fm_subscription_pol_spec_cycle_fee_interval.o \
	fm_subscription_pol_spec_fold.o \
	fm_subscription_pol_spec_rerate.o \
	fm_subscription_pol_update_cdc.o

C++_FILES= fm_subscription_pol_init.cpp \
	fm_subscription_pol_snowball_discount.cpp

C++_OBJECTS= fm_subscription_pol_init.o \
	fm_subscription_pol_snowball_discount.o

LIBSUBS=fm_subscription_pol_custom.$(SL_EXT)

###########

all: $(LIBSUBS)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBSUBS)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBSUBS): $(OBJECTS) $(C++_OBJECTS) $(INCFILES) Makefile
	$(C++) -o $(LIBSUBS) $(LDFLAGS) $(OBJECTS) $(C++_OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

$(C++_OBJECTS): $(INCFILES) Makefile $(C++_FILES)
	$(C++) -c $(C++FLAGS) $(CPPFLAGS) $(C++_FILES)

#fm_subscription_pol_init.o: $(INCFILES) Makefile
#	$(C++) -c $(C++FLAGS) $(CPPFLAGS) fm_subscription_pol_init.cpp

#fm_subscription_pol_snowball_discount.o: $(INCFILES) Makefile
#	$(C++) -c $(C++FLAGS) $(CPPFLAGS) fm_subscription_pol_snowball_discount.cpp

#

FRC:

