###########
#
# Copyright (c) 2007, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation or its
#      licensors and may be used, reproduced, stored or transmitted only in
#      accordance with a valid Oracle license or sublicense agreement.
#
###########

OS=linux
VERSION=15.0

####

PINDIR=../../..
INCDIR=$(PINDIR)/include
LIBDIR=$(PINDIR)/lib

##########
CC_linux = gcc
CC = $(CC_$(OS))

##########

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -D_REENTRANT -DFOR_CM -DFM_GL_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM -DFM_GL_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS)) -L$(LIBDIR)

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_act.h $(INCDIR)/pin_bill.h \
	$(INCDIR)/pin_flds.h $(INCDIR)/pin_currency.h \
	$(INCDIR)/pin_gl.h 

#

FILES= fm_gl_pol_config.c

#

OBJECTS= fm_gl_pol_config.o

LIBGL=fm_gl_pol_custom.$(SL_EXT)

###########

all: $(LIBGL)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBGL)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBGL): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBGL) $(LDFLAGS) $(OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile $(FILES)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:

