/* Planar 8 bits per pixel */

#include <sys/types.h>

#include "ofbis.h"
#include "common.h"
#include "pl_common.h"

void
pl_8_putpixel( FB *f, u_int16_t x, u_int16_t y, u_int32_t col )
{
	register u_int16_t	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register u_int16_t sc = 15 - (x & 15);
	register u_int16_t nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;

	writetoplplane(0)
	base += nextpl;
	writetoplplane(1)
	base += nextpl;
	writetoplplane(2)
	base += nextpl;
	writetoplplane(3)
	base += nextpl;
	writetoplplane(4)
	base += nextpl;
	writetoplplane(5)
	base += nextpl;
	writetoplplane(6)
	base += nextpl;
	writetoplplane(7)
}

u_int32_t
pl_8_getpixel( FB *f, u_int16_t x, u_int16_t y )
{
	register u_int16_t	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register u_int16_t sc = 15 - (x & 15);
	register u_int16_t nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;
	register u_int32_t col=0x00000000;

	readfromplplane(0)
	base += nextpl;
	readfromplplane(1)
	base += nextpl;
	readfromplplane(2)
	base += nextpl;
	readfromplplane(3)
	base += nextpl;
	readfromplplane(4)
	base += nextpl;
	readfromplplane(5)
	base += nextpl;
	readfromplplane(6)
	base += nextpl;
	readfromplplane(7)

	return col;
}

void
pl_8_hline( FB *f, u_int16_t x1, u_int16_t x2, u_int16_t y, u_int32_t col )
{
	register u_int16_t	x = MIN(x1,x2);
	register u_int16_t	j = MAX(x1,x2);
	register u_int16_t	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register u_int16_t nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;
	register u_int16_t	*wbase=base;
	register int		pc;

	for ( pc = 0 ; pc < 8 ; pc ++,x = MIN(x1,x2),wbase+=nextpl,base=wbase )
	{
		while ( x <= j )
		{
			register u_int16_t sc = 15 - (x & 15);

			writetoplplane(pc);
			base+=( ++x & 15 ) ? 0 : 1;
		}
	}
}

void
pl_8_register_functions(FB *f)
{
  f->putpixel       = &pl_8_putpixel;
  f->getpixel       = &pl_8_getpixel;
  f->hline          = &pl_8_hline;
  f->bitblt         = &pl_generic_bitblt;
  f->c24_to_cnative = &pl_generic_c24_to_cnative;
  f->cnative_to_c24 = &pl_generic_cnative_to_c24;
}
