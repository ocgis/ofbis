#include "libfb.h"
#include "fberror.h"
#include "fbpixel.h"
#include "fbhline.h"
#include "fbline.h"
#include "fbbitblt.h"
#include "fbchar.h"
#include "fbconvert.h"

static void
setinterleave1( FB *f )
{
	f->putpixel = &i2pp1;
	f->getpixel = &i2gp1;
	f->hline = &i2hl1;
	f->line = &genln;
	f->putchar = &i2pc1;
	f->sp8_convert = &sp8_to_i1;
	f->bitblt = &bitblt;
}

static void
setinterleave2( FB *f )
{
	f->putpixel = &i2pp2;
	f->getpixel = &i2gp2;
	f->hline = &i2hl2;
	f->line = &genln;
	f->putchar = &i2pc2;
	f->sp8_convert = &sp8_to_i2;
	f->bitblt = &bitblt;
}

static void
setinterleave4( FB *f )
{
	f->putpixel = &i2pp4;
	f->getpixel = &i2gp4;
	f->hline = &i2hl4;
	f->line = &genln;
	f->putchar = &i2pc4;
	f->sp8_convert = &sp8_to_i4;
	f->bitblt = &bitblt;
}

static void
setinterleave8( FB *f )
{
	f->putpixel = &i2pp8;
	f->getpixel = &i2gp8;
	f->hline = &i2hl8;
	f->line = &genln;
	f->putchar = &i2pc8;
	f->sp8_convert = &sp8_to_i8;
	f->bitblt = &bitblt;
}

static void
settruecolor( FB *f )
{
	f->putpixel = &pptc;
	f->getpixel = &gptc;
	f->hline = &hltc;
	f->line = &genln;
	f->putchar = &pctc;
	f->sp8_convert = &sp8_to_tc;
	f->bitblt = &bitblt;
}

static void
setpackedpixel8( FB *f )
{
	f->putpixel = &ppspc;
	f->getpixel = &gpspc;
	f->hline = &hlspc;
	f->line = &genln;
	f->putchar = &pcspc;
	f->sp8_convert = &sp8_to_sp8;
	f->bitblt = &sp8_bitblt;
}

static void
setplanes1( FB *f )
{
	f->putpixel = &plpp1;
	f->getpixel = &plgp1;
	f->hline = &plhl1;
	f->line = &genln;
	f->putchar = &plpc1;
	f->sp8_convert = &sp8_to_p1;
	f->bitblt = &bitblt;
}

static void
setplanes2( FB *f )
{
	f->putpixel = &plpp2;
	f->getpixel = &plgp2;
	f->hline = &plhl2;
	f->line = &genln;
	f->putchar = &plpc2;
	f->sp8_convert = &sp8_to_p2;
	f->bitblt = &bitblt;
}

static void
setplanes4( FB *f )
{
	f->putpixel = &plpp4;
	f->getpixel = &plgp4;
	f->hline = &plhl4;
	f->line = &genln;
	f->putchar = &plpc4;
	f->sp8_convert = &sp8_to_p4;
	f->bitblt = &bitblt;
}

static void
setplanes8( FB *f )
{
	f->putpixel = &plpp8;
	f->getpixel = &plgp8;
	f->hline = &plhl8;
	f->line = &genln;
	f->putchar = &plpc8;
	f->sp8_convert = &sp8_to_p8;
	f->bitblt = &bitblt;
}


/*
** Description
** Setup which functions to use for standard operations
**
** 1998-08-08 CG
*/
void
FBsetfuncs( FB *f )
{
	/* I believe only one type of bitblt is needed (Tomas) */
        /* You believe wrong.. */
        /* f->bitblt = &bitblt; */

	if ( f->finf.type == FB_TYPE_INTERLEAVED_PLANES )
	{
		switch (f->vinf.bits_per_pixel)
		{
			case 1: setinterleave1(f); break;
			case 2:	setinterleave2(f); break;
			case 4: setinterleave4(f); break;
			case 8: setinterleave8(f); break;
			default: FBerror( FATAL, "FBsetfuncs: unsupported "
						"interleaved planes mode.");
		}
	}
	else if ( f->finf.type == FB_TYPE_PACKED_PIXELS )
	{
	  switch (f->finf.visual) {
	  case FB_VISUAL_MONO01:
	    setinterleave1(f);
	    break;	/* need to fix colours here? */
	  
	  case FB_VISUAL_MONO10:
	    setinterleave1(f);
	    break;

	  case FB_VISUAL_TRUECOLOR:
	    settruecolor(f);
	    break;
	  
	  case FB_VISUAL_PSEUDOCOLOR:
	  case FB_VISUAL_STATIC_PSEUDOCOLOR:
	    /*
	    ** TBD
	    ** Check bits per pixel and call the right setup function
	    */
	    setpackedpixel8 (f);
	    break;

	  default:
	    FBerror( FATAL, "FBsetfuncs: unsupported "
		     "visual type in packed pixels (0x%x).",
		     f->finf.visual);
	  }
	}
	else if ( f->finf.type == FB_TYPE_PLANES )
	{
		switch (f->finf.visual)
		{
			case FB_VISUAL_PSEUDOCOLOR:
			{
				switch ( f->vinf.bits_per_pixel )
				{
					case 1: setplanes1(f); break;		/* Amiga OCS */
					case 2: setplanes2(f); break;		/* Amiga OCS */
					case 4: setplanes4(f); break;		/* Amiga OCS */
					case 8: setplanes8(f); break;		/* Amiga OCS */
					default: FBerror( FATAL, "FBsetfuncs: unsupported depth in planes mode.");
				}
			} break;
			default: FBerror( FATAL, "FBsetfuncs: unsupported "
						"visual type in planes mode.");
		}
	}
}
