#
# oFBis -- src subdir makefile
#

VERSION_MAJOR=0
VERSION_MINOR=0
VERSION_SUBMINOR=5

SRCS = fbopen.c fberror.c fbpixel.c fbdebug.c fbset.c fbcmap.c fbvt.c fballoc.c\
       fbbitblt.c fbfuncs.c fbhline.c fbline.c fbkbd.c fbmouse.c fbevent.c fbchar.c

OBJS= $(SRCS:%.c=%.o) 

MAINOBJS = libfb.o

SUBDIRS = fonts
SUBDIROBJS = fonts/fonts.o


# shared library stuff

SHARED_OBJS= $(OBJS:%.o=shared/%.o) \
             fonts/shared/font_6x11.o fonts/shared/font_8x8.o fonts/shared/font_8x16.o

SHARED_LIB=libfb.so.$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_SUBMINOR)
SHARED_MAJORNAME=libfb.so.$(VERSION_MAJOR)
SHARED_NAME=libfb.so

SHARED_CFLAGS = -fPIC -D_REENTRANT


DO_MAKE = $(MAKE) 'CC=$(CC)' 'CFLAGS=$(CFLAGS)' 'SHARED_CFLAGS=$(SHARED_CFLAGS)' \
			'LD=$(LD)' 'LDFLAGS=$(LDFLAGS)' 'AR=$(AR)'

all: libfb.a $(SHARED_LIB)

libfb.o: $(OBJS)
	$(LD) -nostdlib -Wl,-r -o $@ $^

libfb.a: $(MAINOBJS) subdirs
	rm -f $@
	$(AR) rcs $@ $(MAINOBJS) $(SUBDIROBJS)

$(SHARED_LIB): createshared subdirs $(SHARED_OBJS)
	gcc -shared -Wl,-soname,$(SHARED_MAJORNAME) -o $(SHARED_LIB) $(SHARED_OBJS)
	chmod a+x $(SHARED_LIB)

shared/%.o: %.c
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) -c -o $@ $<

subdirs: force
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir; \
	done

install:
	mkdir -p $(PREFIX)/include/fb
	cp libfb.a $(PREFIX)/lib
	cp libfb.h $(PREFIX)/include/fb
	cp $(SHARED_LIB) $(PREFIX)/lib
	cd $(PREFIX)/lib
	rm -f $(SHARED_MAJORNAME)
	ln -s $(SHARED_LIB) $(SHARED_MAJORNAME)
	rm -f $(SHARED_NAME)
	ln -s $(SHARED_MAJORNAME) $(SHARED_NAME)

dep depend .depend:
	$(CC) $(CFLAGS) -M $(SRCS) >.depend

createshared:
	mkdir -p shared

clean:
	rm -f core *~ $(OBJS) $(SHARED_OBJS) .depend
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir clean; \
	done

realclean:
	rm -f core *~ $(OBJS) $(SHARED_OBJS) .depend
	rm -f libfb.a $(SHARED_LIB)
	set -e; for dir in $(SUBDIRS); do \
		$(DO_MAKE) -C $$dir realclean; \
	done

force:

ifeq (.depend,$(wildcard .depend))
include .depend
endif
