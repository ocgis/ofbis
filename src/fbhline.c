#include "libfb.h"
#include "fbhline.h"

#define	MIN(a,b)	( (a) < (b) ? (a) : (b) )
#define MAX(a,b)	( (a) > (b) ? (a) : (b) )

#define writetoi2plane(x)					\
	base[x] &= ~( 0x00000001 << sc );			\
	base[x] |= ( ( ( col >> x ) & 0x00000001 ) << sc );

#define	writetoplplane(x)					\
	*base &= ~( 0x00000001 << sc );				\
	*base |= ( ( ( col >> x ) & 0x00000001 ) << sc );

void
genhl( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	unsigned short	x = MIN(x1,x2);
	unsigned short	j = MAX(x1,x2);
	
	while ( x <= j )
	{
		(*f->putpixel)(f, x++, y, col);
	}
}

void
i2hl1( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*1;

	while ( x <= j )
	{
		register unsigned short sc = 15 - (x & 15);

		writetoi2plane(0)
		base+=( ++x & 15 ) ? 0 : 1;
	}
}

void
i2hl2( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*2;

	while ( x <= j )
	{
		register unsigned short sc = 15 - (x & 15);

		writetoi2plane(0)
		writetoi2plane(1)
		base+=( ++x & 15 ) ? 0 : 2;
	}
}

void
i2hl4( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*4;

	while ( x <= j )
	{
		register unsigned short sc = 15 - (x & 15);

		writetoi2plane(0)
		writetoi2plane(1)
		writetoi2plane(2)
		writetoi2plane(3)
		base+=( ++x & 15 ) ? 0 : 4;
	}
}

void
i2hl8( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*8;

	while ( x <= j )
	{
		register unsigned short sc = 15 - (x & 15);

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
hltc( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short	*pixel=f->sbuf+((y*f->vinf.xres_virtual)+x);
		 unsigned char	*cp= (char *)&col;
	register unsigned short	pval = ( ( *cp++ << 8 ) & 0xF800 ) |
				( ( *++cp << 3 ) & 0x07E0 ) |
				( ( *++cp >> 3 ) & 0x001F );

	while ( x++ <= j )
	{
		*pixel++ = pval;
	}
}


/*
** Description
** Draw a horizontal line to a static psuedocolor fb
**
** To be done
** This function only works for a colourdepth of 8 (one byte per pixel) and
** needs to be updated for larger colourdepths.
*/
void
hlspc(FB *f,
      unsigned short x1,
      unsigned short x2,
      unsigned short y,
      unsigned long col)
{
  register unsigned short	x = MIN(x1,x2);
  register unsigned short	j = MAX(x1,x2);
  register unsigned char	*pixel =
    (char *)f->sbuf + ((y*f->vinf.xres_virtual) + x);
  
  while (x++ <= j) {
    *pixel++ = (unsigned char)col;
  }
}


void
plhl1( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);

	while ( x <= j )
	{
		register unsigned short sc = 15 - (x & 15);

		writetoplplane(0);
		base+=( ++x & 15 ) ? 0 : 1;
	}
}

void
plhl2( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;
	register unsigned short	*wbase=base;
	register int		pc;

	for ( pc = 0 ; pc < 2 ; pc ++,x = MIN(x1,x2),wbase+=nextpl,base=wbase )
	{
		while ( x <= j )
		{
			register unsigned short sc = 15 - (x & 15);

			writetoplplane(pc);
			base+=( ++x & 15 ) ? 0 : 1;
		}
	}
}

void
plhl4( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;
	register unsigned short	*wbase=base;
	register int		pc;

	for ( pc = 0 ; pc < 4 ; pc ++,x = MIN(x1,x2),wbase+=nextpl,base=wbase )
	{
		while ( x <= j )
		{
			register unsigned short sc = 15 - (x & 15);

			writetoplplane(pc);
			base+=( ++x & 15 ) ? 0 : 1;
		}
	}
}

void
plhl8( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col )
{
	register unsigned short	x = MIN(x1,x2);
	register unsigned short	j = MAX(x1,x2);
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;
	register unsigned short	*wbase=base;
	register int		pc;

	for ( pc = 0 ; pc < 8 ; pc ++,x = MIN(x1,x2),wbase+=nextpl,base=wbase )
	{
		while ( x <= j )
		{
			register unsigned short sc = 15 - (x & 15);

			writetoplplane(pc);
			base+=( ++x & 15 ) ? 0 : 1;
		}
	}
}
