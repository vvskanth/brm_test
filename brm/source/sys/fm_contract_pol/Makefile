###########
#
#
# Copyright (c) 2020, 2024 Oracle and/or its affiliates.
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

C++_linux = gcc
C++ = $(C++_$(OS))
##########

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -D_REENTRANT -DFOR_CM -DFM_BILL_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

C++FLAGS_linux = -x c++ -DPIN_NOT_USING_OSTREAM
C++FLAGS= $(C++FLAGS_$(OS)) -DPCMCPP_CONST_SAFE

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM -DFM_BILL_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS)) -L$(LIBDIR)

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_act.h $(INCDIR)/pin_bill.h \
	$(INCDIR)/pin_flds.h $(INCDIR)/pin_currency.h

#
 
FILES= fm_contract_pol_config.c 

#

OBJECTS= fm_contract_pol_config.o 

#

LIBCONTRACTPOL=fm_contract_pol_custom.$(SL_EXT)

###########

all: $(LIBCONTRACTPOL)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBCONTRACTPOL)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC


###########

$(LIBCONTRACTPOL): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBCONTRACTPOL) $(LDFLAGS) $(OBJECTS) -lm -lpsiu_for_cm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:

