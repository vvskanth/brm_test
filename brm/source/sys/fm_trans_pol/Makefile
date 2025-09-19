###########
#
# Copyright (c) 1999, 2024, Oracle and/or its affiliates.
#      This material is the confidential property of Oracle Corporationi.
#      or its subsidiaries or licensors and may be used, reproduced, stored
#      or transmitted only in accordance with a valid Oracle license or
#      sublicense agreement.
#
###########

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
CFLAGS= $(CFLAGS_$(OS)) -DFOR_CM -DFM_TRANS_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM -DFM_TRANS_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))


###########

INCFILES= $(INCDIR)/pcm.h $(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h

#

FILES= fm_trans_pol_config.c \
	fm_trans_pol_open.c \
	fm_trans_pol_prep_commit.c \
	fm_trans_pol_commit.c \
	fm_trans_pol_abort.c

OBJECTS= fm_trans_pol_config.o \
	fm_trans_pol_open.o \
	fm_trans_pol_prep_commit.o \
	fm_trans_pol_commit.o \
	fm_trans_pol_abort.o

#

DEP_linux=$(FILES)
DEP=$(DEP_$(OS))
LIBTRANS=fm_trans_pol_custom.$(SL_EXT)

###########

all: $(LIBTRANS)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBTRANS)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBTRANS): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBTRANS) $(LDFLAGS) $(OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile $(DEP)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:






