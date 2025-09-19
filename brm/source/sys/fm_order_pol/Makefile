#
#
# Copyright (c) 2002, 2024, Oracle and/or its affiliates.
#
# This material is the confidential property of Oracle Corporation
# or its licensors and may be used, reproduced, stored or transmitted
# only in accordance with a valid Oracle license or sublicense agreement.
#

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

LDFLAGS_linux= -m elf_x86_64 -shared -L/usr/lib64
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

OS_LIBS_linux= -lm
OS_LIBS= $(OS_LIBS_$(OS))

####

INCFILES= $(INCDIR)/pin_order.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_flds.h 

FILES= fm_order_pol_config.c \
	fm_order_pol_associate.c \
	fm_order_pol_process.c \
	fm_order_pol_create.c \
	fm_order_pol_delete.c \
	fm_order_pol_set_state.c \
	fm_order_pol_set_attr.c \
	fm_order_pol_set_brand.c

OBJECTS= fm_order_pol_config.o \
	fm_order_pol_associate.o \
	fm_order_pol_process.o \
	fm_order_pol_create.o \
	fm_order_pol_delete.o \
	fm_order_pol_set_state.o \
	fm_order_pol_set_attr.o \
	fm_order_pol_set_brand.o

#

LIBORDER=fm_order_pol_custom.$(SL_EXT)

DEP_linux = $(FILES)
DEP = $(DEP_$(OS))

###########

all: $(LIBORDER)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBORDER)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBORDER): $(OBJECTS) $(INCFILES) Makefile $(DEP)
	$(LD) -o $(LIBORDER) $(LDFLAGS) $(OBJECTS) $(OS_LIBS)

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) 

#

FRC:






