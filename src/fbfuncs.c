#include <stdio.h>
#include "ofbis.h"
#include "fballoc.h"
#include "fberror.h"
#include "fbpixel.h"
#include "fbhline.h"
#include "fbline.h"
#include "fbbitblt.h"
#include "fbchar.h"
#include "fbconvert.h"
#include "fbfuncs.h"

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
settruecolour( FB *f )
{
  f->putpixel = &pptc;
  f->getpixel = &gptc;
  f->hline = &hltc;
  f->line = &genln;
  f->putchar = &pctc;
  f->sp8_convert = &sp8_to_tc;
  f->bitblt = &pp_bitblt;
}

static void
setpackedpixel8( FB *f )
{
  f->putpixel = &ppspc;
  f->getpixel = &gpspc;
  f->hline = &hlspc;
  f->line = &genln;
  f->putchar = &pcsp8;
  f->sp8_convert = &sp8_to_sp8;
  f->bitblt = &pp_bitblt;
}

static void
setplanes1( FB *f )
{
  f->putpixel = &plpp1;
  f->getpixel = &plgp1;
  f->hline = &plhl1;
  f->line = &genln;
  f->putchar = &genpc;
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
  f->putchar = &genpc;
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
  f->putchar = &genpc;
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
  f->putchar = &genpc;
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
  int length;

  /*
  fprintf(stderr, "finf.type: %d\n", f->finf.type);
  fprintf(stderr, "finf.visual: %d\n", f->finf.visual);
  fprintf(stderr, "vinf.bits_per_pixel: %d\n", f->vinf.bits_per_pixel);
  */

  /* If there's reserved space already, free it. 
   * We must do this, because the resolution may have changed. 
   */
  if(f->cmap) {
    FBfree(f->cmap->red);
    FBfree(f->cmap->green);
    FBfree(f->cmap->blue);
    FBfree(f->cmap);
  }

  /* Reserve memory for the internal palette. */
  f->cmap = (FBCMAP *) FBalloc( sizeof(FBCMAP) );
  length = 1<<f->vinf.bits_per_pixel;
  f->cmap->red=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
  f->cmap->green=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
  f->cmap->blue=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
  

  switch(f->finf.type) { /* FB_TYPE_* */
  case FB_TYPE_INTERLEAVED_PLANES:
    switch(f->finf.visual) { /* FB_VISUAL_* */
    case FB_VISUAL_PSEUDOCOLOR:
      switch(f->vinf.bits_per_pixel) {
      case 1:           /* Atari interleaved 1 plane */
        setinterleave1(f);
        break;

      case 2:           /* Atari interleaved 2 planes */
        setinterleave2(f);
        break;

      case 4:           /* Atari interleaved 4 planes */
        setinterleave4(f);
        break;

      case 8:           /* Atari interleaved 8 planes */
        setinterleave8(f);
        break;

      default:
        FBerror(FATAL, "FBsetfuncs: unsupported depth in interleaved planes mode (%d).",
                f->vinf.bits_per_pixel);
      }
      break;

    default:
      FBerror(FATAL, "FBsetfuncs: unsupported interleaved planes mode (%d).",
              f->finf.visual);
    }
    break;

  case FB_TYPE_PLANES:
    switch(f->finf.visual) { /* FB_VISUAL_* */
    case FB_VISUAL_PSEUDOCOLOR:
      switch(f->vinf.bits_per_pixel) {
      case 1:           /* Amiga 1 plane */
        setplanes1(f);
        break;

      case 2:           /* Amiga 2 planes */
        setplanes2(f);
        break;

      case 4:           /* Amiga 4 planes */
        setplanes4(f);
        break;

      case 8:           /* Amiga 8 planes */
        setplanes8(f);
        break;

      default:
        FBerror(FATAL, "FBsetfuncs: unsupported depth in planes mode (%d).",
                f->vinf.bits_per_pixel);
      }
      break;

    default:
      FBerror(FATAL, "FBsetfuncs: unsupported planes mode (%d).",
              f->finf.visual);
    }
    break;

  case FB_TYPE_PACKED_PIXELS:
    switch (f->finf.visual) { /* FB_VISUAL_* */
    case FB_VISUAL_MONO01:
      setinterleave1(f);
      break;    /* need to fix colours here? */
          
    case FB_VISUAL_MONO10:
      setinterleave1(f);
      break;

    case FB_VISUAL_TRUECOLOR:
    case FB_VISUAL_DIRECTCOLOR:
      settruecolour(f);
      {
        FBCMAP *fbcmap;
	int i;
	
	fbcmap = FBgetcmap(f);
	
	if(fbcmap == NULL)
	  return;
#if 0	
	for(i=0;i<(1<<f->vinf.red.length);i++) {
	  fbcmap->red[i] = i<<(16-f->vinf.red.length);
	}
	for(i=0;i<(1<<f->vinf.green.length);i++) {
	  fbcmap->green[i] = i<<(16-f->vinf.green.length);
	}
	for(i=0;i<(1<<f->vinf.blue.length);i++) {
	  fbcmap->blue[i] = i<<(16-f->vinf.blue.length);
	}
#else
	for(i=0;i<32;i++) {
	  fbcmap->red[i] = i<<11;
	  fbcmap->green[i] = i<<11;
	  fbcmap->blue[i] = i<<11;
	}
#endif	
	fbcmap->start = 0;
	fbcmap->end = 31;
	fbcmap->len = 32;
	FBputcmap(f, fbcmap);
	FBfreecmap(fbcmap);
      }
      break;
          
    case FB_VISUAL_PSEUDOCOLOR:
    case FB_VISUAL_STATIC_PSEUDOCOLOR:
      switch(f->vinf.bits_per_pixel) {
      case 1:           /* Atari mono/duocolour */
        setinterleave1(f);
        break;

      case 8:           /* 8-bit chunk mode */
        setpackedpixel8(f);
        break;

      case 2:                           /* exists? */
      case 4:                           /* exists? */
      default:
        FBerror(FATAL, "FBsetfuncs: unsupported depth in pseudocolor mode (%d).",
                f->vinf.bits_per_pixel);
      }
      break;

    default:
      FBerror(FATAL, "FBsetfuncs: unsupported visual type in packed pixels (%d).",
              f->finf.visual);
    }
    break;

  default:
    FBerror(FATAL, "FBsetfuncs: unsupported type (%d).",
            f->finf.type);
  }
}
