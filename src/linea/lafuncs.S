#include "linea.h"

	.global lafntab

/* Line A function table */

lafntab:
	.long	lainit 			/* 0xA000 Initialization */
lainit:	leal	laptr,%a0		/* Line A Init */
	movel	%a0,%d0 		/* Line A variable struct */
	lea	sysfonts,%a1		/* Pointers to system font headers */
	lea	lafntab(%PC),%a2	/* Pointer to null terminated array */
	rts				/* of function pointers */

	.comm	sysfonts,12

