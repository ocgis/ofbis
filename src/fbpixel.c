#include "libfb.h"
#include "fbpixel.h"
#include "fberror.h"

#define writetoi2plane(x)					\
	base[x] &= ~( 0x00000001 << sc );			\
	base[x] |= ( ( ( col >> x ) & 0x00000001 ) << sc );

#define readfromi2plane(x)					\
	col |= ( ( base[x] >> sc ) & 0x00000001 ) << x;

#define	writetoplplane(x)					\
	*base &= ~( 0x00000001 << sc );				\
	*base |= ( ( ( col >> x ) & 0x00000001 ) << sc );

#define	readfromplplane(x)					\
	col |= ( ( *base >> sc ) & 0x00000001 ) << x;

void
i2pp1( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
  register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*1;
  register unsigned short sc = 15 - (x & 15);

	writetoi2plane(0)
}

void
i2pp2( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
  register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*2;
  register unsigned short sc = 15 - (x & 15);

	writetoi2plane(0)
	writetoi2plane(1)
}

void
i2pp4( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
  register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*4;
  register unsigned short sc = 15 - (x & 15);

	writetoi2plane(0)
	writetoi2plane(1)
	writetoi2plane(2)
	writetoi2plane(3)
}

void
i2pp8( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
  register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*8;
  register unsigned short sc = 15 - (x & 15);

	writetoi2plane(0)
	writetoi2plane(1)
	writetoi2plane(2)
	writetoi2plane(3)
	writetoi2plane(4)
	writetoi2plane(5)
	writetoi2plane(6)
	writetoi2plane(7)
}

void
pptc( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
  register unsigned short	*pixel=f->sbuf+((y*f->vinf.xres_virtual)+x);
  register unsigned char	*cp= (char *)&col;

	*pixel = (( *cp++ << 8 ) & 0xF800 ) |
		 (( *++cp << 3 ) & 0x07E0 ) |
		 (( *++cp >> 3 ) & 0x001F );
}

void
plpp1( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short sc = 15 - (x & 15);

	writetoplplane(0)
}

void
plpp2( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short sc = 15 - (x & 15);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;

	writetoplplane(0)
	base += nextpl;
	writetoplplane(1)
}

void
plpp4( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short sc = 15 - (x & 15);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;

	writetoplplane(0)
	base += nextpl;
	writetoplplane(1)
	base += nextpl;
	writetoplplane(2)
	base += nextpl;
	writetoplplane(3)
}

void
plpp8( FB *f, unsigned short x, unsigned short y, unsigned long col )
{
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short sc = 15 - (x & 15);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;

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

unsigned long
i2gp1( FB *f, unsigned short x, unsigned short y )
{
  register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*1;
  register unsigned short sc = 15 - (x & 15);
  register unsigned long col=0x00000000;

	readfromi2plane(0);

	return col;
}

unsigned long
i2gp2( FB *f, unsigned short x, unsigned short y )
{
  register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*2;
  register unsigned short sc = 15 - (x & 15);
  register unsigned long col=0x00000000;

	readfromi2plane(0);
	readfromi2plane(1);

	return col;
}

unsigned long
i2gp4( FB *f, unsigned short x, unsigned short y )
{
  register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*4;
  register unsigned short sc = 15 - (x & 15);
  register unsigned long col=0x00000000;

	readfromi2plane(0);
	readfromi2plane(1);
	readfromi2plane(2);
	readfromi2plane(3);

	return col;
}

unsigned long
i2gp8( FB *f, unsigned short x, unsigned short y )
{
  register unsigned short *base=f->sbuf+(((y*f->vinf.xres_virtual)+x)>>4)*8;
  register unsigned short sc = 15 - (x & 15);
  register unsigned long col=0x00000000;

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

unsigned long
gptc( FB *f, unsigned short x, unsigned short y )
{
  register unsigned long col=0x00000000;

	return col;
}

unsigned long
plgp1( FB *f, unsigned short x, unsigned short y )
{
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short sc = 15 - (x & 15);
	register unsigned long col=0x00000000;

	readfromplplane(0)

	return col;
}

unsigned long
plgp2( FB *f, unsigned short x, unsigned short y )
{
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short sc = 15 - (x & 15);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;
	register unsigned long col=0x00000000;

	readfromplplane(0)
	base += nextpl;
	readfromplplane(1)

	return col;
}

unsigned long
plgp4( FB *f, unsigned short x, unsigned short y )
{
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short sc = 15 - (x & 15);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;
	register unsigned long col=0x00000000;

	readfromplplane(0)
	base += nextpl;
	readfromplplane(1)
	base += nextpl;
	readfromplplane(2)
	base += nextpl;
	readfromplplane(3)

	return col;
}

unsigned long
plgp8( FB *f, unsigned short x, unsigned short y )
{
	register unsigned short	*base=f->sbuf+(((y*f->finf.line_length)+((x/16)<<1))>>1);
	register unsigned short sc = 15 - (x & 15);
	register unsigned short nextpl = (f->vinf.xres_virtual*f->vinf.yres_virtual/8)>>1;
	register unsigned long col=0x00000000;

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
