/* Planar 1 bit per pixel */

#include <sys/types.h>

#include "ofbis.h"
#include "common.h"
#include "pl_common.h"

void
pl_1_putpixel( FB *f, u_int16_t x, u_int16_t y, u_int32_t col )
{
	register u_int16_t	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register u_int16_t sc = 15 - (x & 15);

	writetoplplane(0)
}

u_int32_t
pl_1_getpixel( FB *f, u_int16_t x, u_int16_t y )
{
	register u_int16_t	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register u_int16_t sc = 15 - (x & 15);
	register u_int32_t col=0x00000000;

	readfromplplane(0)

	return col;
}

void
pl_1_hline( FB *f, u_int16_t x1, u_int16_t x2, u_int16_t y, u_int32_t col )
{
	register u_int16_t	x = MIN(x1,x2);
	register u_int16_t	j = MAX(x1,x2);
	register u_int16_t	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);

	while ( x <= j )
	{
		register u_int16_t sc = 15 - (x & 15);

		writetoplplane(0);
		base+=( ++x & 15 ) ? 0 : 1;
	}
}

void
pl_1_register_functions(FB *f)
{
  f->putpixel       = &pl_1_putpixel;
  f->getpixel       = &pl_1_getpixel;
  f->hline          = &pl_1_hline;
  f->bitblt         = &pl_generic_bitblt;
  f->c24_to_cnative = &pl_generic_c24_to_cnative;
  f->cnative_to_c24 = &pl_generic_cnative_to_c24;
}
