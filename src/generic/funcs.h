#ifndef _GENERIC_FUNCS_H_
#define _GENERIC_FUNCS_H_

#include "ofbis.h"

extern void generic_line( FB *f, u_int16_t a1, u_int16_t b1, 
		   u_int16_t a2, u_int16_t b2, u_int32_t col);
extern void generic_hline( FB *f, u_int16_t x1, u_int16_t x2, 
		    u_int16_t y, u_int32_t col );
extern void generic_putchar( FB *f, u_int16_t x, u_int16_t y, 
		      u_int32_t fgcol, u_int32_t bgcol, unsigned char ch );
extern void generic_bitblt( FB *f, FBBLTPBLK *fbb );
extern void generic_sp8_convert( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );

#endif
