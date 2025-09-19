###########
#
#
# Copyright (c) 1999, 2024, Oracle and/or its affiliates.
#
#       This material is the confidential property of Oracle Corporation
#       or its subsidiaries or licensors and may be used, reproduced,
#       stored or transmitted only in accordance with a valid Oracle
#       license or sublicense agreement.
#
###########

OS=linux
VERSION=15.0

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

FILES= fm_rate_pol_config.c \
	fm_rate_pol_tax_code.c \
	fm_rate_pol_tax_loc.c \
	fm_rate_pol_post_rating.c \
	fm_rate_pol_map_tax_supplier.c \
	fm_rate_pol_tax_supplier.c \
	fm_rate_pol_pre_tax.c \
	fm_rate_pol_post_tax.c  \
	fm_rate_pol_event_zonemap.c \
	fm_rate_pol_pre_rating.c \
	fm_rate_pol_process_eras.c

OBJECTS= fm_rate_pol_config.o \
	fm_rate_pol_tax_code.o \
	fm_rate_pol_tax_loc.o \
	fm_rate_pol_post_rating.o \
	fm_rate_pol_map_tax_supplier.o \
	fm_rate_pol_tax_supplier.o \
	fm_rate_pol_pre_tax.o \
	fm_rate_pol_post_tax.o \
	fm_rate_pol_event_zonemap.o \
	fm_rate_pol_pre_rating.o \
	fm_rate_pol_process_eras.o

#

LIBRATE=fm_rate_pol_custom.$(SL_EXT)

###########

all: $(LIBRATE)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBRATE)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBRATE): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBRATE) $(LDFLAGS) $(OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:
