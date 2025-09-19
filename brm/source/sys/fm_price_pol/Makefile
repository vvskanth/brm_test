###########
#
#
# Copyright (c) 1999, 2024, Oracle and/or its affiliates.
#
#       This material is the confidential property of Oracle Corporation
#       or its subsidiaries or licensors and may be used, reproduced, stored
#       or transmitted only in accordance with a valid Oracle license or
#       sublicense agreement.
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

CPPFLAGS = -I$(INCDIR) -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h

#

FILES= fm_price_pol_config.c \
	fm_price_pol_delete_deal.c \
	fm_price_pol_delete_product.c \
	fm_price_pol_delete_dependency.c \
	fm_price_pol_delete_transition.c \
	fm_price_pol_delete_discount.c \
	fm_price_pol_prep_deal.c \
	fm_price_pol_prep_product.c \
	fm_price_pol_prep_dependency.c \
	fm_price_pol_prep_transition.c \
	fm_price_pol_prep_discount.c \
	fm_price_pol_valid_deal.c \
	fm_price_pol_valid_product.c \
	fm_price_pol_valid_dependency.c \
	fm_price_pol_valid_transition.c \
	fm_price_pol_valid_discount.c 

OBJECTS= fm_price_pol_config.o \
	fm_price_pol_delete_deal.o \
	fm_price_pol_delete_product.o \
	fm_price_pol_delete_discount.o \
	fm_price_pol_delete_dependency.o \
	fm_price_pol_delete_transition.o \
	fm_price_pol_prep_deal.o \
	fm_price_pol_prep_product.o \
	fm_price_pol_prep_dependency.o \
	fm_price_pol_prep_transition.o \
	fm_price_pol_prep_discount.o \
	fm_price_pol_valid_deal.o \
	fm_price_pol_valid_product.o \
	fm_price_pol_valid_dependency.o \
	fm_price_pol_valid_transition.o \
	fm_price_pol_valid_discount.o 

#

LIBPRICE=fm_price_pol_custom.$(SL_EXT)

###########

all: $(LIBPRICE)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBPRICE)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBPRICE): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBPRICE) $(LDFLAGS) $(OBJECTS) -lm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:
