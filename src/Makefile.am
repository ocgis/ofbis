SUBDIRS = display_interleaved display_planes display_packed \
	display_vga generic

# Include files goes into a special directory
ofbisdir = @OSISINCLUDE@
ofbis_HEADERS = ofbis.h ofbis_keyboard.h

# Make a shared library using libtool
lib_LTLIBRARIES = libofbis.la
libofbis_la_SOURCES = \
	alloc.c char.c cmap.c debug.c error.c \
	event.c funcs.c kbd.c mouse.c open.c bitblt.c \
	set.c vt.c mmap.c font_6x11.c font_8x8.c font_8x14.c font_8x16.c \
	common.h alloc.h error.h mmap.h vt.h bitblt.h \
	event.h kbd.h mouse.h char.h funcs.h \
	keys.h list.h

libofbis_la_LIBADD = \
	display_interleaved/libinterleaved.la \
	display_planes/libplanes.la \
	display_packed/libpacked.la \
	display_vga/libvga.la \
	generic/libgeneric.la

# This is a bad versioning system, since much has changed
# in the interface during its early development.
libofbis_la_LDFLAGS = @LT_VERSION_INFO@
