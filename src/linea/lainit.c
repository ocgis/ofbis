/* Line A structure initialisation */

#include "lainit.h"
#include "../fballoc.h"

FBLINEA *
LAalloc( void )
{
	return (FBLINEA *) FBalloc(sizeof(FBLINEA));
}

void
LAfree( FBLINEA *la )
{
	FBfree( la );
}

void
LAgetvars( FB *f, FBLINEA *l )
{
	int	i;
	__u16 palsize = ((1<<f->vinf.red.length) *
			 (1<<f->vinf.green.length) *
			 (1<<f->vinf.blue.length)	);

	/* Setup data for vq_extnd() */

	l->inq_tab[0] = 4;				/* screen type */
	l->inq_tab[1] = (palsize>32767)? 0 : palsize;	/* number of bg colours */
	l->inq_tab[2] = -1;				/* text effects */
	l->inq_tab[3] = 0;				/* enlarging raster */
	l->inq_tab[4] = f->vinf.bits_per_pixel;		/* num of planes */
	l->inq_tab[5] = 1;				/* look up table */
	l->inq_tab[6] = 100;	/* 16*16 rops per second - no idea! */
	l->inq_tab[7] = 0;				/* contour fill */
	l->inq_tab[8] = 0;				/* text rotation */
	l->inq_tab[9] = 4;				/* num of drawing modes */
	l->inq_tab[10] = 2;				/* highest possible input mode */
	l->inq_tab[11] = 0;				/* text alignment */
	l->inq_tab[12] = 0;				/* colour change with pens */
	l->inq_tab[13] = 0;				/* colour change with band shift */
	l->inq_tab[14] = -1;				/* max points in pline,pmarker,farea */
	l->inq_tab[15] = -1;				/* max len of intin */
	l->inq_tab[16] = 2;				/* number of mouse buttons */
	l->inq_tab[17] = 0;				/* line types for wide lines */
	l->inq_tab[18] = 0;				/* drawing modes for wide lines */
	for ( i = 19 ; i < 45 ; i++ )
	{
		l->inq_tab[i] = 0;			/* clear reserved elements */
	}

	/* Setup data for v_opnwk */

	l->dev_tab[0] = f->vinf.xres_virtual - 1;	/* Max addressable width */
	l->dev_tab[1] = f->vinf.yres_virtual - 1;	/* Max addressable height */
	l->dev_tab[2] = 0;				/* Device Coord Units flag */
	l->dev_tab[3] = 372;				/* Width of pixel in microns */
	l->dev_tab[4] = 372;				/* Height of pixel in microns */
	l->dev_tab[5] = 3;				/* Number of different text sizes */
	l->dev_tab[6] = 7;				/* Number of line types */
	l->dev_tab[7] = 0;				/* Number of line widths */
	l->dev_tab[8] = 6;				/* Number of marker types */
	l->dev_tab[9] = 0;				/* Number of marker sizes */
	l->dev_tab[10] = 1;				/* Number of fonts on device */
	l->dev_tab[11] = 24;				/* Number of patterns */
	l->dev_tab[12] = 12;				/* Number of hatch styles */
	l->dev_tab[13] = 1 << f->vinf.bits_per_pixel;	/* Number of colours, virt pens (TC) */
	l->dev_tab[14] = 10;				/* Number of GDPs */
	for ( i = 0 ; i < 10 ; i++ )
	{
		l->dev_tab[15+i] = i+1;			/* List of GDPs supported */
	}
	l->dev_tab[25] = 3;				/* GDP attributes */
	l->dev_tab[26] = 0;
	l->dev_tab[27] = 3;
	l->dev_tab[28] = 3;
	l->dev_tab[29] = 3;
	l->dev_tab[30] = 0;
	l->dev_tab[31] = 3;
	l->dev_tab[32] = 0;
	l->dev_tab[33] = 3;
	l->dev_tab[34] = 2;
	l->dev_tab[35] = ( f->vinf.bits_per_pixel > 1 ) ? 1 : 0;	/* Flag colour representation */
	l->dev_tab[36] = 0;				/* Text rotation */
	l->dev_tab[37] = 0;				/* Flag fill-out range */
	l->dev_tab[38] = 0;				/* Flag function cell-array */
	l->dev_tab[39] = (palsize>32767)? 0 : palsize;	/* Number of available colours */
	l->dev_tab[40] = 2;				/* Graphic cursor control */
	l->dev_tab[41] = 1;				/* Number-changeable inputs */
	l->dev_tab[42] = 1;				/* Key choice */
	l->dev_tab[43] = 1;				/* Alphanumeric input */
	l->dev_tab[44] = 2;				/* Type of work device */

	l->bytes_lin = f->vinf.xres_virtual * f->vinf.bits_per_pixel / 8;	/* bytes per line */
	l->planes = f->vinf.bits_per_pixel;		/* Number of planes */
	l->width = f->vinf.xres_virtual * f->vinf.bits_per_pixel / 8;	/* width in bytes */

}
