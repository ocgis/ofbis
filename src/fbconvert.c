#include <string.h>

#include "ofbis.h"
#include "fballoc.h"
#include "fbconvert.h"

/* These need to be done. */
void sp8_to_i1( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
}

void sp8_to_i2( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
}

void sp8_to_i4( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
}

void sp8_to_tc( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
}

void sp8_to_p1( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
}

void sp8_to_p2( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
}

void sp8_to_p4( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
}

void sp8_to_p8( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
}

/* Optimized m68k assembler c2p routine
 *
 * Copyright 1998 Johan Klockars <rand@cd.chalmers.se>
 *
 * Slightly modified by Tomas to fit oFBis.
 * MUST be compiled with -fomit-frame-pointer!!
 *
 */
#ifdef mc68020
static void debitplane(char *chunky, long chunky_width, 
		       long width, long height, 
		       short *planar, long planar_width)
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
void sp8_to_i8( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
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

/* 1998-10-27 Tomas Berndtsson
 *
 * Convert 8-bit static pseudocolour to 
 * 8-bit static pseudocolour. (i.e. do nothing)
 * Sets width and height for destination block.
 */
void sp8_to_sp8( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
  dstblock->width = srcblock->width;  dstblock->height = srcblock->height;

  memcpy(dstblock->addr, srcblock->addr, srcblock->width*srcblock->height);
}

