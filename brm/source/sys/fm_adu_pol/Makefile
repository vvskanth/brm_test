##############################################################################
#
# Copyright (c) 2008, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation
#      or its licensors and may be used, reproduced, stored or transmitted
#      only in accordance with a valid Oracle license or sublicense agreement.
#
###############################################################################

OS=linux
VERSION=15.0

###########

PINDIR=../../..
INCDIR=$(PINDIR)/include
LIBDIR=$(PINDIR)/lib

###########

CC_linux = gcc
CC = $(CC_$(OS))

###########

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFOR_CM -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS)) -L$(LIBDIR)

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

###########

INCFILES= $(INCDIR)/pcm.h

###########

FILES=	fm_adu_pol_validate.c \
	fm_adu_pol_dump.c \
	fm_adu_pol_config.c

OBJECTS= fm_adu_pol_validate.o \
	 fm_adu_pol_dump.o \
	 fm_adu_pol_config.o

###########

LIBADU=fm_adu_pol_custom.$(SL_EXT)

###########

all: $(LIBADU)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBADU)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBADU): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBADU) $(LDFLAGS) $(OBJECTS) -lpin_cc -lm -lpsiu_for_cm -lregex

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) 

###########

FRC:
