#
# oFBLIBis toplevel makefile
#

# select whether to support debugging ('yes' or 'no')
DEBUG = no

# compilers and flags:
CC      = gcc
CFLAGS  = -Wall -I$(shell pwd)/include -pipe
LD      = gcc
LDFLAGS = 
AR      = ar
LIBS    =
PREFIX	= /usr/local

# different CFLAGS depending on DEBUG
ifeq ($(DEBUG),yes)
CFLAGS += -g -DDEBUG
else
CFLAGS += -O2 -fno-strength-reduce -fomit-frame-pointer
endif

# test for cross compiling
COMPILE_ARCH = $(shell uname -m)
ifneq ($(COMPILE_ARCH),m68k)
	CROSSPREFIX=m68k-linux-
	CC := $(CROSSPREFIX)$(CC)
	LD := $(CROSSPREFIX)$(LD)
	AR := $(CROSSPREFIX)$(AR)
endif

DO_MAKE = $(MAKE) 'CC=$(CC)' 'CFLAGS=$(CFLAGS)' 'LD=$(LD)' \
			    'LDFLAGS=$(LDFLAGS)' 'AR=$(AR)'

# determine output binary format, and modify linker options for ELF
#BINFMT=$(shell $(CC) -E -P test-elf.c | grep -v ^$$)
#ifeq ($(BINFMT),ELF)
#LDFLAGS += -Wl,-Telf-ldscript.x
#endif

.PHONY: force

# overall target:
EXEC = fbtst

# sub-directories:
SUBDIRS = src
SUBDIROBJS = src/libfb.a

# objects in main directory
OBJS = fbtst.o

# C sources in main directory
CSRC = fbtst.c

# determine prerequisites for 'make all' to be done before making
# target $(EXEC). These could be .depend and optiondef.[ch]
PREREQ :=
ifneq (optiondef.c,$(wildcard optiondef.c))
PREREQ += optiondef.c
endif
ifneq (.depend,$(wildcard .depend))
PREREQ += dep
endif


# general target:
all: $(EXEC)

$(EXEC): $(OBJS) subdirs		# include/compile.h
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SUBDIROBJS) $(LIBS)

subdirs: force
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir; \
	done

include/option.h: include/optiondef.h

# special rule to build optiondef.[ch] from options.def
optiondef.c include/optiondef.h: options.def options.awk
	awk -f options.awk <$<

include/compile.h: force
	@echo \#define OFBIS_COMPILE_TIME \"`date '+%D %T'`\" > include/compile.h
	@echo \#define OFBIS_COMPILE_BY \"`whoami`\" >> include/compile.h
	@echo \#define OFBIS_COMPILE_HOST \"`hostname`\" >> include/compile.h
	@echo \#define OFBIS_COMPILER \"`$(CC) -v 2>&1 | tail -1`\" >> include/compile.h

install:
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) 'PREFIX=$(PREFIX)' -C $$dir install; \
	done

clean:
	rm -f *.o *~ optiondef.c include/optiondef.h .depend
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir clean; \
	done

realclean:
	rm -f $(EXEC)
	rm -f *.o *~ optiondef.c include/optiondef.h .depend
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir realclean; \
	done

dep depend .depend:
	$(CC) $(CFLAGS) -M $(CSRC) >.depend
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir dep; \
	done

tar:
	cd ..; tar cvzf $(notdir $(shell pwd)).tar.gz \
		`find $(notdir $(shell pwd)) -type f -a ! -name '*.[oa]' \
		-a ! -name .depend -a ! -name fbtst -a ! -name 'optiondef.[ch]' \
		-a ! -name '*~' -a ! -name '*.orig' -a ! -name 'libfb.so*'`

force:

ifeq (.depend,$(wildcard .depend))
include .depend
endif
