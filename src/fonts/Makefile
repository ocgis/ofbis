#
# oFBis -- src subdir makefile
#

SRCS = font_6x11.c font_8x8.c font_8x16.c
OBJS = $(SRCS:%.c=%.o)

SHARED_OBJS= $(OBJS:%.o=shared/%.o)


all: createshared fonts.o $(SHARED_OBJS)

fonts.o: $(OBJS)
	$(LD) -nostdlib -Wl,-r -o $@ $^

shared/%.o: %.c
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) -c -o $@ $<

dep depend .depend:
	$(CC) $(CFLAGS) -M $(SRCS) >.depend

createshared:
	mkdir -p shared

clean:
	rm -f core *~ $(OBJS) $(SHARED_OBJS) .depend

realclean: clean
	rm -f fonts.o

ifeq (.depend,$(wildcard .depend))
include .depend
endif
