#include <string.h>

#include "libfb.h"
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
 */
void sp8_to_i8( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
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
}

/* 1998-10-27 Tomas Berndtsson
 *
 * Convert 8-bit static pseudocolour to 
 * 8-bit static pseudocolour. (i.e. do nothing)
 * Sets width and height for destination block.
 */
void sp8_to_sp8( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
  dstblock->width = srcblock->width;
  dstblock->height = srcblock->height;

  memcpy(dstblock->addr, srcblock->addr, srcblock->width*srcblock->height);
}

