/* Line A entry routine */

#include "linea.h"

	.global	LAEntry

	.text

	.comm	laptr,4

LAEntry:moveal	%sp@(4),%a0
	movel	%a0@(-RESV1),laptr
	movel	%sp@(8),%d0
	andiw	#0x000F,%d0
	lslw	#2,%d0
	leal	lafntab,%a0
	movel	%a0@(%d0),%a0
	jsr	%a0@
	rts

