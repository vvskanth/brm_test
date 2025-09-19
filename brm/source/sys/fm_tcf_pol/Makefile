###########
#
#
# Copyright (c) 2003, 2024, Oracle and/or its affiliates.
#
# This material is the confidential property of Oracle Corporation or its 
# licensors and may be used, reproduced, stored or transmitted only in 
# accordance with a valid Oracle license or sublicense agreement.
#
###########

OS =			linux
PIN_VERSION =		15.0

##########

PINDIR =		../../..
INCDIR =		$(PINDIR)/include

##########
CC_linux = 		gcc

CC = 			$(CC_$(OS))

CFLAGS_linux= 		-fPIC
CFLAGS =		$(CFLAGS_$(OS)) -DFLIST_HEAP -DASSIGN_NULL_AFTER_ELEM_PUT  -DASSIGN_NULL_AFTER_SUBSTR_PUT   -DASSIGN_NULL_AFTER_FLD_PUT

CPPFLAGS =		-I$(INCDIR)

LDFLAGS_linux= 		-m elf_x86_64 -shared
LDFLAGS =		$(LDFLAGS_$(OS))

SL_EXT_linux =		so
SL_EXT =		$(SL_EXT_$(OS))


OS_LIBS_linux= 		-lm

OS_LIBS= 		$(OS_LIBS_$(OS))


###########

DEP_linux = 		$(FILES)
DEP =			$(DEP_$(OS))

LIBTCFPOL =		fm_tcf_pol_custom.$(SL_EXT)

INCFILES =		$(INCDIR)/pin_cust.h\
			$(INCDIR)/pcm.h\
			$(INCDIR)/pin_errs.h\
			$(INCDIR)/pinlog.h

FILES =			fm_tcf_pol_config.c\
			fm_tcf_pol_apply_parameter.c

OBJECTS =		fm_tcf_pol_config.o\
			fm_tcf_pol_apply_parameter.o

all: $(LIBTCFPOL)



$(LIBTCFPOL): $(OBJECTS) $(INCFILES) Makefile  $(DEP)
	$(LD) -o $@ $(LDFLAGS) $(OBJECTS) $(OS_LIBS) 

$(OBJECTS): $(INCFILES) Makefile 
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FILES)

###########

clean:
	rm -f $(OBJECTS) core

clobber: clean
	rm -f $(LIBTCFPOL)

lint:
	lint $(CPPFLAGS) $(FILES)

tags: FRC

FRC:
