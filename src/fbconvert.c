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
  unsigned char *dstptr = (unsigned char *)dstblock->addr;
  int padded_width = (srcblock->width+15) & 0xfffffff0;
  int extra = padded_width - srcblock->width;
  int y, width = srcblock->width;
  void *table;

  table = FBalloc(65536L);

  dstblock->width = srcblock->width;
  dstblock->height = srcblock->height;

  /* Copy and pad chunk data to even 16 */
  for(y = 0 ; y < srcblock->height ; y++) {
    memcpy(dstptr, srcptr, width);
    memset(dstptr+width, 0, extra);
    srcptr += width;
    dstptr += width+extra;
  }

#if 0
  m68k_sp8_to_i8_clrtab(table);
  m68k_sp8_to_i8_mktab(table);
  m68k_sp8_to_i8_ctobp(table, srcblock->addr, dstblock->addr, 
		      padded_width*srcblock->height);
#else
  /* Clear table */
  asm ("	clrl   %%d0\n"
       "	movel  %0,%%a0\n"
       "	movew  #4095,%%d1\n"
       "0:	movel  %%d0,%%a0@+\n"
       "	movel  %%d0,%%a0@+\n"
       "	movel  %%d0,%%a0@+\n"
       "	movel  %%d0,%%a0@+\n"
       "	dbra   %%d1,0b"
       : 
       : "m" (table)
       : "d0","d1","a0");

  /* Make table */
  asm ("	movel  %0,%%a0\n"
       "	clrl   %%d0\n"
       "0:	moveb  %%d0,%%d1\n"
       "	moveq  #7,%%d3\n"
       "1:	movel  %%a0,%%a1\n"
       "	lsrb   #1,%%d1\n"
       "	jcc    3f\n"
       "	movew  #0x8000,%%d2\n"
       "2:	movew  %%d2,%%a1@\n"
       "	addaw  #256*16,%%a1\n"
       "	lsrw   #1,%%d2\n"
       "	jne    2b\n"
       "3:	addql  #2,%%a0\n"
       "	dbra   %%d3,1b\n"
       "	addqb  #1,%%d0\n"
       "	jne    0b"
       : 
       : "m" (table)
       : "d0", "d1", "d2", "d3", "a0", "a1");
	

  /* Convert chunk to bitplanes */
  asm ("	movel  %0,%%a4\n"
       "	movel  %1,%%a0\n"
       "	movel  %2,%%a1\n"
       "	movel  %3,%%d0\n"
       "	lsrl   #4,%%d0\n"
       "0:	movel  %%a4,%%a2\n"
       "	clrl   %%d3\n"
       "	clrl   %%d4\n"
       "	clrl   %%d5\n"
       "	clrl   %%d6\n"
       "	moveq  #15,%%d1\n"
       "1:	clrl   %%d2\n"
       "	moveb  %%a0@+,%%d2\n"
       "	addl   %%d2,%%d2\n"
       "	lea    %%a2@(%%d2:l:8),%%a3\n"
       "	orl    %%a3@+,%%d3\n"
       "	orl    %%a3@+,%%d4\n"
       "	orl    %%a3@+,%%d5\n"
       "	orl    %%a3@+,%%d6\n"
       "	addaw  #256*16,%%a2\n"
       "	dbra   %%d1,1b\n"
       "	movel  %%d3,%%a1@+\n"
       "	movel  %%d4,%%a1@+\n"
       "	movel  %%d5,%%a1@+\n"
       "	movel  %%d6,%%a1@+\n"
       "	subql  #1,%%d0\n"
       "	jgt    0b"
       :
       : "m" (table), "m" (dstblock->addr), "m" (dstblock->addr), "d" (padded_width*srcblock->height)
       : "d0","d1","d2","d3","d4","d5","d6", "a0","a1","a2","a3","a4");
#endif

  FBfree(table);

#else /* General C function */

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

