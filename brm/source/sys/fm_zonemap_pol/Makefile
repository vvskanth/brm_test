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

CFLAGS_linux= -fPIC -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT
CFLAGS= $(CFLAGS_$(OS)) -DFLIST_HEAP

CPPFLAGS = -I$(INCDIR)

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h

#

FILES= fm_zonemap_pol_config.c \
	fm_zonemap_pol_prep_zonemap.c \
	fm_zonemap_pol_storage.c \
	fm_zonemap_pol_serialize.c \
	fm_zonemap_pol_network.c \
	fm_zonemap_pol_get_lineage.c \
	fm_zonemap_pol_util.c 

OBJECTS= fm_zonemap_pol_config.o \
	fm_zonemap_pol_prep_zonemap.o \
	fm_zonemap_pol_storage.o \
	fm_zonemap_pol_serialize.o \
	fm_zonemap_pol_network.o \
	fm_zonemap_pol_get_lineage.o \
	fm_zonemap_pol_util.o 

#

LIBZONEMAP=fm_zonemap_pol_custom.$(SL_EXT)

###########

all: $(LIBZONEMAP)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBZONEMAP)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBZONEMAP): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBZONEMAP) $(LDFLAGS) $(OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:
