###########
#
#
# Copyright (c) 1999, 2024, Oracle and/or its affiliates.
#
#      This material is the confidential property of Oracle Corporation or its
#      licensors and may be used, reproduced, stored or transmitted only in
#      accordance with a valid Oracle license or sublicense agreement.
#
###########
OS=linux
VERSION=15.0

###########
PINDIR=../../..
INCDIR=$(PINDIR)/include
LIBDIR=$(PINDIR)/lib

##########
CC_linux = gcc
CC = $(CC_$(OS))

##########

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFOR_CM -DFM_INV_POL_DLL -DASSIGN_NULL_AFTER_ELEM_PUT -DASSIGN_NULL_AFTER_SUBSTR_PUT -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS = -I$(INCDIR) -DPCMCPP_CONST_SAFE -DFOR_CM  -DFM_INV_POL_DLL

LDFLAGS_linux= -shared
LDFLAGS = $(LDFLAGS_$(OS)) -L$(LIBDIR)

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h \
	$(INCDIR)/pin_act.h $(INCDIR)/pin_bill.h \
	$(INCDIR)/pin_flds.h $(INCDIR)/pin_currency.h \
	$(INCDIR)/pin_inv.h

#

FILES= fm_inv_pol_config.c \
	fm_inv_pol_init.c\
	fm_inv_pol_common.c\
	fm_inv_pol_format_view.c\
	fm_inv_pol_prep_invoice.c \
	fm_inv_pol_format_invoice.c \
	fm_inv_pol_format_invoice_doc1.c \
	fm_inv_pol_format_invoice_html.c \
	fm_inv_pol_format_invoice_xml.c \
	fm_inv_pol_format_invoice_xslt.c \
	fm_inv_pol_select.c \
	fm_inv_pol_post_make_invoice.c


#

OBJECTS= fm_inv_pol_config.o \
	fm_inv_pol_init.o\
	fm_inv_pol_common.o\
	fm_inv_pol_format_view.o\
	fm_inv_pol_prep_invoice.o \
	fm_inv_pol_format_invoice.o \
	fm_inv_pol_format_invoice_doc1.o \
	fm_inv_pol_format_invoice_html.o \
	fm_inv_pol_format_invoice_xml.o \
	fm_inv_pol_format_invoice_xslt.o \
	fm_inv_pol_select.o \
	fm_inv_pol_post_make_invoice.o

LIBBILL=fm_inv_pol_custom.$(SL_EXT)

###########

all: $(LIBBILL)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBBILL)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

###########

$(LIBBILL): $(OBJECTS) $(INCFILES) Makefile
	$(LD) -o $(LIBBILL) $(LDFLAGS) $(OBJECTS) -lm -lpsiu_for_cm

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

#

FRC:

