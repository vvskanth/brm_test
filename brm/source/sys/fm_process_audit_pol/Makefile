########### 
#
# Copyright (c) 2003, 2024, Oracle and/or its affiliates.
#
#     This material is the confidential property of Oracle Corporation
#     or licensors and may be used, reproduced, stored or transmitted
#     only in accordance with a valid Oracle license or sublicense agreement.
#
#
###########

# this is the Make.user for All_Server/fm_policy/fm_process_audit_pol

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

OS_LIBS_linux= -lm
OS_LIBS= $(OS_LIBS_$(OS))

INCFILES= $(INCDIR)/pcm.h $(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_flds.h

############

FILES= fm_process_audit_pol_config.c \
	fm_process_audit_pol_create.c \
	fm_process_audit_pol_create_and_link.c \
	fm_process_audit_pol_alert.c \
	fm_process_audit_pol_create_writeoff_summary.c

OBJECTS= fm_process_audit_pol_config.o \
	fm_process_audit_pol_create.o \
	fm_process_audit_pol_create_and_link.o \
	fm_process_audit_pol_alert.o \
	fm_process_audit_pol_create_writeoff_summary.o

############

LIBPUBLISH=fm_process_audit_pol_create_custom.$(SL_EXT)

###########

all: $(LIBPUBLISH) 

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBPUBLISH) 

lint:
	$(LINT) $(CPPFLAGS) $(FILES)

###########

$(LIBPUBLISH): $(OBJECTS)
	$(LD) -o $(LIBPUBLISH) $(LDFLAGS) $(OS_LIBFLGS) $(OBJECTS) $(OS_LIBS)

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) 

###########

FRC:


