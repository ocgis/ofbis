#
# oFBis -- src subdir makefile
#

SRCS = lainit.c
OBJS= laentry.o lafuncs.o lainit.o

SHARED_OBJS= $(OBJS:%.o=shared/%.o)


all: createshared linea.o $(SHARED_OBJS)

linea.o: $(OBJS)
	$(LD) -nostdlib -Wl,-r -o $@ $^

shared/%.o: %.S
	$(CC) -D__ASSEMBLY__ $(SHARED_CFLAGS) -c -o $@ $<

shared/%.o: %.c
	$(CC) $(CFLAGS) $(SHARED_CFLAGS) -c -o $@ $<

.S.o:
	$(CC) -D__ASSEMBLY__ -c -o $@ $<

dep depend .depend:
	$(CC) $(CFLAGS) -M $(SRCS) >.depend

createshared:
	mkdir -p shared

clean:
	rm -f core *~ $(OBJS) $(SHARED_OBJS) .depend

realclean: clean
	rm -f linea.o

ifeq (.depend,$(wildcard .depend))
include .depend
endif
