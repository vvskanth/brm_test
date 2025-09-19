###########
#
#
# Copyright (c) 1999, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation or
#      its licensors and may be used, reproduced, stored or transmitted only 
#      in accordance with a valid Oracle license or sublicense agreement.
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

FILES= fm_repl_pol_config.c \
	fm_repl_pol_push.c \
	fm_repl_pol_translate.c \
	fm_repl_pol_utils.c

OBJECTS= fm_repl_pol_config.o \
	fm_repl_pol_push.o \
	fm_repl_pol_translate.o \
	fm_repl_pol_utils.o

#

DEP_linux=$(FILES)
DEP=$(DEP_$(OS))
LIBRATE=fm_repl_pol.$(SL_EXT)

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

$(OBJECTS): $(INCFILES) Makefile $(DEP)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $*.c

#

FRC:






