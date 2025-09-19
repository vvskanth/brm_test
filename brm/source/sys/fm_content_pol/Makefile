###########
#
# Copyright (c) 2001, 2024, Oracle and/or its affiliates.
#
# This material is the confidential property of Oracle Corporation or
# its licensors and may be used, reproduced, stored or transmitted only
# in accordance with a valid Oracle license or sublicense agreement.
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

CFLAGS_linux= -fPIC
CFLAGS= $(CFLAGS_$(OS)) -DFLIST_HEAP -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT
CPPFLAGS = -I$(INCDIR)

LDFLAGS_linux= -m elf_x86_64 -shared
LDFLAGS = $(LDFLAGS_$(OS))

SL_EXT_linux= so
SL_EXT= $(SL_EXT_$(OS))

OS_LIBS_linux= -lm
OS_LIBS= $(OS_LIBS_$(OS))

###########

INCFILES= $(INCDIR)/pin_cust.h $(INCDIR)/pcm.h \
	$(INCDIR)/pin_errs.h $(INCDIR)/pinlog.h

#
FILES= fm_content_pol_config.c \
	fm_content_pol_authorize.c \
	fm_content_pol_accounting.c \
	fm_content_pol_resolve_event_extensions.c \
	fm_content_pol_resolve_user.c \
	fm_content_pol_post_authorize.c \
	fm_content_pol_post_accounting.c 

OBJECTS= fm_content_pol_config.o \
	fm_content_pol_authorize.o \
	fm_content_pol_accounting.o \
	fm_content_pol_resolve_event_extensions.o \
	fm_content_pol_resolve_user.o \
	fm_content_pol_post_authorize.o \
	fm_content_pol_post_accounting.o 

#

LIBCONTENTPOL=fm_content_pol_custom.$(SL_EXT)

DEP_linux = $(FILES)
DEP = $(DEP_$(OS))

###########

all: $(LIBCONTENTPOL)

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBCONTENTPOL)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC


###########

$(LIBCONTENTPOL): $(OBJECTS) $(INCFILES) Makefile $(DEP)
	$(LD) -o $(LIBCONTENTPOL) $(LDFLAGS) $(OBJECTS) $(OS_LIBS)

$(OBJECTS): $(INCFILES) Makefile
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES) 


#

FRC:




