/* Interleaved 8 bits per pixel */

#include <sys/types.h>

#include "ofbis.h"
#include "common.h"
#include "i2_common.h"

void
i2_8_putpixel( FB *f, u_int16_t x, u_int16_t y, u_int32_t col )
{
  register u_int16_t *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*8;
  register u_int16_t sc = 15 - (x & 15);

	writetoi2plane(0)
	writetoi2plane(1)
	writetoi2plane(2)
	writetoi2plane(3)
	writetoi2plane(4)
	writetoi2plane(5)
	writetoi2plane(6)
	writetoi2plane(7)
}

u_int32_t
i2_8_getpixel( FB *f, u_int16_t x, u_int16_t y )
{
  register u_int16_t *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*8;
  register u_int16_t sc = 15 - (x & 15);
  register u_int32_t col=0x00000000;

	readfromi2plane(0);
	readfromi2plane(1);
	readfromi2plane(2);
	readfromi2plane(3);
	readfromi2plane(4);
	readfromi2plane(5);
	readfromi2plane(6);
	readfromi2plane(7);

	return col;
}

void
i2_8_hline( FB *f, u_int16_t x1, u_int16_t x2, u_int16_t y, u_int32_t col )
{
	register u_int16_t	x = MIN(x1,x2);
	register u_int16_t	j = MAX(x1,x2);
	register u_int16_t *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*8;

	while ( x <= j )
	{
		register u_int16_t sc = 15 - (x & 15);

		writetoi2plane(0)
		writetoi2plane(1)
		writetoi2plane(2)
		writetoi2plane(3)
		writetoi2plane(4)
		writetoi2plane(5)
		writetoi2plane(6)
		writetoi2plane(7)
		base+=( ++x & 15 ) ? 0 : 8;
	}
}

void
i2_8_putchar( FB *f, u_int16_t x, u_int16_t y, u_int32_t fgcol, u_int32_t bgcol, u_int8_t ch )
{
	int skew = f->font->width-(x%16);
	int row;
	int linewords = (f->vinf.xres_virtual>>4)*8;
	register u_int16_t *base = f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*8;
	register u_int8_t *chardata = &f->font->data[f->font->height*ch];

	if (skew >= 0) { /* only one short to put */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			setonei2(*chardata, 0, fgcol);
			setonei2(*chardata, 1, fgcol);
			setonei2(*chardata, 2, fgcol);
			setonei2(*chardata, 3, fgcol);
			setonei2(*chardata, 4, fgcol);
			setonei2(*chardata, 5, fgcol);
			setonei2(*chardata, 6, fgcol);
			setonei2(*chardata, 7, fgcol);
		}
	} else { /* two shorts are needed */
		for (row = 0 ; row < f->font->height ; row++,chardata++,base+=linewords) {
			settwoi2(*chardata, 0, 8, fgcol);
			settwoi2(*chardata, 1, 8, fgcol);
			settwoi2(*chardata, 2, 8, fgcol);
			settwoi2(*chardata, 3, 8, fgcol);
			settwoi2(*chardata, 4, 8, fgcol);
			settwoi2(*chardata, 5, 8, fgcol);
			settwoi2(*chardata, 6, 8, fgcol);
			settwoi2(*chardata, 7, 8, fgcol);
		}
	}
}

/* The converting routines has to be made in a different way later, if 
 * they should even be in oFBis at all.
 */

/* Optimized m68k assembler c2p routine
 *
 * Copyright 1998 Johan Klockars <rand@cd.chalmers.se>
 *
 * Slightly modified by Tomas to fit oFBis.
 * MUST be compiled with -fomit-frame-pointer!!
 *
 */
#ifdef mc68020
static void debitplane(char *chunky, u_int32_t chunky_width, 
		       u_int32_t width, u_int32_t height, 
		       short *planar, u_int32_t planar_width)
{
asm("	moveml	%%d2-%%d7/%%a2-%%a5,%%sp@-\n"
    "	movel	%%sp@(44),%%a0\n"
    "	movel	%%sp@(48),%%d0\n"
    "	movel	%%sp@(52),%%d1\n"
    "	movel	%%sp@(56),%%d2\n"
    "	movel	%%sp@(60),%%a1\n"
    "	exg	%%d0,%%d1\n"
    "	movew	%%d2,%%sp@-\n"
    "	subw	%%d0,%%d1\n"
    "	movew	%%d1,%%sp@-\n"
    "	movew	%%d0,%%d3\n"
    "	lsrw	#4,%%d0\n"
    "	movew	%%d0,%%a4\n"
    "	movew	%%a4,%%a5\n"
    "	andw	#0x000f,%%d3\n"
    "	movew	%%d3,%%sp@-\n"
    "line:\n"
    "	movew	%%a5,%%a4\n"
    "	movel	%%a1,%%sp@-\n"
    "	cmpw	#0,%%a4\n"
    "	beq	loop_end\n"
    "loop:\n"
    "	movel	%%a0@+,%%d0\n"
    "	movel	#0x00ff00ff,%%d4\n"
    "	movel	%%a0@+,%%d1\n"
    "	moveq	#8,%%d5\n"
    "	movel	%%a0@+,%%d2\n"
    "#	movel	%%d0,%%d6\n"
    "	movel	%%a0@+,%%d3\n"
    "	bra	loop_start\n"
    "pass_2:\n"
    "	movel	#0x33333333,%%d4\n"
    "	swap	%%d1\n"
    "	movel	%%d1,%%d7\n"
    "	movew	%%d0,%%d1\n"
    "	swap	%%d1\n"
    "	movew	%%d7,%%d0\n"
    "	swap	%%d3\n"
    "	movel	%%d3,%%d7\n"
    "	movew	%%d2,%%d3\n"
    "	swap	%%d3\n"
    "	movew	%%d7,%%d2\n"
    "	exg	%%d1,%%d2\n"
    "loop_start:\n"
    "	movel	%%d2,%%d7\n"
    "	lsrl	%%d5,%%d7\n"
    "	eorl	%%d0,%%d7\n"
    "	andl	%%d4,%%d7\n"
    "	eorl	%%d7,%%d0\n"
    "	lsll	%%d5,%%d7\n"
    "	eorl	%%d7,%%d2\n"
    "	movel	%%d3,%%d7\n"
    "	lsrl	%%d5,%%d7\n"
    "	eorl	%%d1,%%d7\n"
    "	andl	%%d4,%%d7\n"
    "	eorl	%%d7,%%d1\n"
    "	lsll	%%d5,%%d7\n"
    "	eorl	%%d7,%%d3\n"
    "	lsrw	#1,%%d5\n"
    "	movel	%%d4,%%d7\n"
    "	lsll	%%d5,%%d4\n"
    "	eorl	%%d7,%%d4\n"
    "	movel	%%d1,%%d7\n"
    "	lsrl	%%d5,%%d7\n"
    "	eorl	%%d0,%%d7\n"
    "	andl	%%d4,%%d7\n"
    "	eorl	%%d7,%%d0\n"
    "	lsll	%%d5,%%d7\n"
    "	eorl	%%d7,%%d1\n"
    "	movel	%%d3,%%d7\n"
    "	lsrl	%%d5,%%d7\n"
    "	eorl	%%d2,%%d7\n"
    "	andl	%%d4,%%d7\n"
    "	eorl	%%d7,%%d2\n"
    "	lsll	%%d5,%%d7\n"
    "	eorl	%%d7,%%d3\n"
    "	lsrw	#1,%%d5\n"
    "	bne	pass_2\n"
    "	movew	%%d3,%%a1@+\n"
    "	exg	%%d1,%%d2\n"
    "	movew	%%d1,%%a1@+\n"
    "	swap	%%d3\n"
    "	movew	%%d2,%%a1@+\n"
    "	swap	%%d1\n"
    "	movew	%%d0,%%a1@+\n"
    "	swap	%%d2\n"
    "	movew	%%d3,%%a1@+\n"
    "	swap	%%d0\n"
    "	movew	%%d1,%%a1@+\n"
    "	subqw	#1,%%a4\n"
    "	movew	%%d2,%%a1@+\n"
    "	movew	%%d0,%%a1@+\n"
    "	cmpw	#0,%%a4\n"
    "	bne	loop\n"
    "loop_end:\n"
    "	movew	%%sp@(4),%%d6\n"
    "	beq	next\n"
    "	moveq	#16,%%d5\n"
    "	subw	%%d6,%%d5\n"
    "	subqw	#1,%%d6\n"
    "inner2:	\n"
    "	moveb	%%a0@+,%%d7\n"
    "	lslb	#5,%%d7\n"
    "	addxw	%%d0,%%d0\n"
    "	lslb	#1,%%d7\n"
    "	addxw	%%d1,%%d1\n"
    "	lslb	#1,%%d7\n"
    "	addxw	%%d2,%%d2\n"
    "	lslb	#1,%%d7\n"
    "	addxw	%%d3,%%d3\n"
    "	dbra	%%d6,inner2\n"
    "	lslw	%%d5,%%d0\n"
    "	lslw	%%d5,%%d1\n"
    "	lslw	%%d5,%%d2\n"
    "	lslw	%%d5,%%d3\n"
    "	swap	%%d3\n"
    "	movew	%%d2,%%d3\n"
    "	swap	%%d1\n"
    "	movew	%%d0,%%d1\n"
    "	movel	%%d3,%%a2\n"
    "	movel	%%d1,%%a3\n"
    "	movew	%%sp@(4),%%d6\n"
    "	subw	%%d6,%%a0\n"
    "	subqw	#1,%%d6\n"
    "inner1:\n"
    "	moveb	%%a0@+,%%d7\n"
    "	lslb	#1,%%d7\n"
    "	addxw	%%d0,%%d0\n"
    "	lslb	#1,%%d7\n"
    "	addxw	%%d1,%%d1\n"
    "	lslb	#1,%%d7\n"
    "	addxw	%%d2,%%d2\n"
    "	lslb	#1,%%d7\n"
    "	addxw	%%d3,%%d3\n"
    "	dbra	%%d6,inner1\n"
    "	movel	%%a2,%%a1@+\n"
    "	lslw	%%d5,%%d0\n"
    "	lslw	%%d5,%%d1\n"
    "	movel	%%a3,%%a1@+\n"
    "	lslw	%%d5,%%d2\n"
    "	lslw	%%d5,%%d3\n"
    "	movew	%%d3,%%a1@+\n"
    "	movew	%%d2,%%a1@+\n"
    "	movew	%%d1,%%a1@+\n"
    "	movew	%%d0,%%a1@+\n"
    "next:\n"
    "	movel	%%sp@+,%%a1\n"
    "	addl	%%sp@(64+6),%%a1\n"
    "	addw	%%sp@(2),%%a0\n"
    "	subqw	#1,%%sp@(4)\n"
    "	bne	line\n"
    "	addql	#6,%%sp\n"
    "	moveml	%%sp@+,%%d2-%%d7/%%a2-%%a5"
    : "=m" (planar)
    : "m" (chunky), "m" (chunky_width), "m" (width), "m" (height), "m" (planar_width));
}
#endif /* mc68020 */

/* 1998-10-27 Tomas Berndtsson
 *
 * Convert 8-bit static pseudocolour to 
 * 8 interleaved bitplanes. Pad with zeros.
 * Sets width and height for destination block.
 *
 * Use optimized m68k assembler if possible.
 */
void i2_8_sp8_convert( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
#ifdef mc68020 /* Use optimized m68k assembler */
  unsigned char *srcptr = (unsigned char *)srcblock->addr;
  unsigned short *dstptr = (unsigned short *)dstblock->addr;
  int padded_width = (srcblock->width+15) & 0xfffffff0;

  dstblock->width = srcblock->width;
  dstblock->height = srcblock->height;

  debitplane(srcptr, srcblock->width,
	     srcblock->width, srcblock->height,
	     dstptr, padded_width);

#else /* General _slow_ C function */

  unsigned char *srcptr = (unsigned char *)srcblock->addr;
  unsigned short *dstptr = (unsigned short *)dstblock->addr;
  int words = (srcblock->width+15) >> 4;
  int word, x, y, bpl;
  register unsigned char chunk;

  dstblock->width = srcblock->width;
  dstblock->height = srcblock->height;

  for(y = 0 ; y < srcblock->height ; y++) {
    for(word = 0 ; word < words ; word++) {
	  for(x = 0 ; x < 16 && (word*16+x) < srcblock->width ; x++) {
	    chunk = *srcptr++;
			
	    for(bpl = 0 ; bpl < 8 ; bpl++) {
	      dstptr[bpl] = (dstptr[bpl] << 1) | (chunk & 1);
	      chunk >>= 1;
	    }
	  }
      for(bpl = 0 ; bpl < 8 ; bpl++)
	*dstptr++ <<= 16-x;
    }
  }
#endif /* mc68020 */
}

void
i2_8_register_functions(FB *f)
{
  f->putpixel       = &i2_8_putpixel;
  f->getpixel       = &i2_8_getpixel;
  f->hline          = &i2_8_hline;
  f->putchar        = &i2_8_putchar;
  f->bitblt         = &i2_generic_bitblt;
  f->c24_to_cnative = &i2_generic_c24_to_cnative;
  f->cnative_to_c24 = &i2_generic_cnative_to_c24;
  f->sp8_convert    = &i2_8_sp8_convert;
}
