#include "libfb.h"
#include "fberror.h"
#include "fbpixel.h"
#include "fbhline.h"
#include "fbline.h"
#include "fbbitblt.h"
#include "fbchar.h"

static void
setinterleave1( FB *f )
{
	f->putpixel = &i2pp1;
	f->getpixel = &i2gp1;
	f->hline = &i2hl1;
	f->line = &genln;
	f->putchar = &i2pc1;
}

static void
setinterleave2( FB *f )
{
	f->putpixel = &i2pp2;
	f->getpixel = &i2gp2;
	f->hline = &i2hl2;
	f->line = &genln;
	f->putchar = &i2pc2;
}

static void
setinterleave4( FB *f )
{
	f->putpixel = &i2pp4;
	f->getpixel = &i2gp4;
	f->hline = &i2hl4;
	f->line = &genln;
	f->putchar = &i2pc4;
}

static void
setinterleave8( FB *f )
{
	f->putpixel = &i2pp8;
	f->getpixel = &i2gp8;
	f->hline = &i2hl8;
	f->line = &genln;
	f->putchar = &i2pc8;
}

static void
settruecolor( FB *f )
{
	f->putpixel = &pptc;
	f->getpixel = &gptc;
	f->hline = &hltc;
	f->line = &genln;
	f->putchar = &pctc;
}

static void
setplanes1( FB *f )
{
	f->putpixel = &plpp1;
	f->getpixel = &plgp1;
	f->hline = &plhl1;
	f->line = &genln;
	f->putchar = &plpc1;
}

static void
setplanes2( FB *f )
{
	f->putpixel = &plpp2;
	f->getpixel = &plgp2;
	f->hline = &plhl2;
	f->line = &genln;
	f->putchar = &plpc2;
}

static void
setplanes4( FB *f )
{
	f->putpixel = &plpp4;
	f->getpixel = &plgp4;
	f->hline = &plhl4;
	f->line = &genln;
	f->putchar = &plpc4;
}

static void
setplanes8( FB *f )
{
	f->putpixel = &plpp8;
	f->getpixel = &plgp8;
	f->hline = &plhl8;
	f->line = &genln;
	f->putchar = &plpc8;
}

void
FBsetfuncs( FB *f )
{
	/* I believe only one type of bitblt is needed (Tomas) */
	f->bitblt = &bitblt;

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
		switch (f->finf.visual)
		{
			case FB_VISUAL_MONO01: setinterleave1(f); break;	/* need to fix colours here? */
			case FB_VISUAL_MONO10: setinterleave1(f); break;
			case FB_VISUAL_PSEUDOCOLOR: setinterleave1(f); break;
			case FB_VISUAL_TRUECOLOR: settruecolor(f); break;
			default: FBerror( FATAL, "FBsetfuncs: unsupported "
						"visual type in packed pixels.");
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
