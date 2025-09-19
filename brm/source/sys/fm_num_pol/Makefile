#
#
# Copyright (c) 2003, 2024, Oracle and/or its affiliates.
#
# This material is the confidential property of Oracle Corporation or its 
# licensors and may be used, reproduced, stored or transmitted only in 
# accordance with a valid Oracle license or sublicense agreement.
#

OS=linux
PIN_VERSION=15.0

##########

PINDIR=../../..
INCDIR=$(PINDIR)/include

##########
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

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h

#

FILES= fm_num_pol_canonicalize.c \
	fm_num_pol_config.c \
	fm_num_pol_device_associate.c \
	fm_num_pol_device_create.c \
	fm_num_pol_device_set_attr.c \
	fm_num_pol_device_set_brand.c \
	fm_num_pol_util.c

OBJECTS= fm_num_pol_canonicalize.o \
	fm_num_pol_config.o \
	fm_num_pol_device_associate.o \
	fm_num_pol_device_create.o \
	fm_num_pol_device_set_attr.o \
	fm_num_pol_device_set_brand.o \
	fm_num_pol_util.o

#

LIBNUMPOL=fm_num_pol_custom.$(SL_EXT)

DEP_linux = $(FILES)
DEP = $(DEP_$(OS))

###########

all: $(LIBNUMPOL)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBNUMPOL)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBNUMPOL): $(OBJECTS) $(INCFILES) Makefile $(DEP)
	$(LD) -o $(LIBNUMPOL) $(LDFLAGS) $(OBJECTS) $(OS_LIBS)

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) 

#

FRC:
