/* Interleaved 4 bits per pixel */

#include <sys/types.h>

#include "ofbis.h"
#include "common.h"
#include "i2_common.h"

void
i2_4_putpixel( FB *f, u_int16_t x, u_int16_t y, u_int32_t col )
{
  register u_int16_t *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*4;
  register u_int16_t sc = 15 - (x & 15);

	writetoi2plane(0)
	writetoi2plane(1)
	writetoi2plane(2)
	writetoi2plane(3)
}

u_int32_t
i2_4_getpixel( FB *f, u_int16_t x, u_int16_t y )
{
  register u_int16_t *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*4;
  register u_int16_t sc = 15 - (x & 15);
  register u_int32_t col=0x00000000;

	readfromi2plane(0);
	readfromi2plane(1);
	readfromi2plane(2);
	readfromi2plane(3);

	return col;
}

void
i2_4_hline( FB *f, u_int16_t x1, u_int16_t x2, u_int16_t y, u_int32_t col )
{
	register u_int16_t	x = MIN(x1,x2);
	register u_int16_t	j = MAX(x1,x2);
	register u_int16_t *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*4;

	while ( x <= j )
	{
		register u_int16_t sc = 15 - (x & 15);

		writetoi2plane(0)
		writetoi2plane(1)
		writetoi2plane(2)
		writetoi2plane(3)
		base+=( ++x & 15 ) ? 0 : 4;
	}
}

void
i2_4_putchar( FB *f, u_int16_t x, u_int16_t y, u_int32_t fgcol, u_int32_t bgcol, u_int8_t ch )
{
	int skew = f->font->width-(x%16);
	int row;
	int linewords = (f->vinf.xres_virtual>>4)*4;
	register u_int16_t *base = f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*4;
	register u_int8_t *chardata = &f->font->data[f->font->height*ch];

	if (skew >= 0) { /* only one short to put */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			setonei2(*chardata, 0, fgcol);
			setonei2(*chardata, 1, fgcol);
			setonei2(*chardata, 2, fgcol);
			setonei2(*chardata, 3, fgcol);
		}
	} else { /* two shorts are needed */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			settwoi2(*chardata, 0, 4, fgcol);
			settwoi2(*chardata, 1, 4, fgcol);
			settwoi2(*chardata, 2, 4, fgcol);
			settwoi2(*chardata, 3, 4, fgcol);
		}
	}
}

void
i2_4_register_functions(FB *f)
{
  f->putpixel       = &i2_4_putpixel;
  f->getpixel       = &i2_4_getpixel;
  f->hline          = &i2_4_hline;
  f->putchar        = &i2_4_putchar;
  f->bitblt         = &i2_generic_bitblt;
  f->c24_to_cnative = &i2_generic_c24_to_cnative;
  f->cnative_to_c24 = &i2_generic_cnative_to_c24;
}

