# Include files goes into a special directory
fbdir = @includedir@/fb
fb_HEADERS = libfb.h

# Make a shared library using libtool
lib_LTLIBRARIES = libfb.la
libfb_la_SOURCES = fbopen.c fberror.c fbpixel.c fbdebug.c fbset.c fbcmap.c \
	fbvt.c fballoc.c fbbitblt.c fbfuncs.c fbhline.c fbline.c fbkbd.c \
	fbmouse.c fbevent.c fbchar.c font_6x11.c font_8x8.c font_8x16.c \
	fballoc.h fberror.h fbhline.h fbkeytbl.h fbmmap.h fbvt.h \
	fbbitblt.h fbevent.h fbkbd.h fbline.h fbmouse.h fbchar.h fbfuncs.h \
	fbkeys.h fblist.h fbpixel.h

# This is a bad versioning system, since much has changed
# in the interface during its early development.
libfb_la_LDFLAGS = -version-info 0:6:0
