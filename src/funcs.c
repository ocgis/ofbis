#include <stdio.h>
#include "ofbis.h"
#include "error.h"
#include "alloc.h"
#include "common.h"
#include "generic/funcs.h"
#include "display_interleaved/funcs.h"
#include "display_planes/funcs.h"
#include "display_packed/funcs.h"
/* #include "convert.h" */
#include "funcs.h"

static void
setgeneric( FB *f )
{
  /* putpixel and getpixel are
   * required to exists as a
   * native function
   */

  f->hline = &generic_hline;
  f->line = &generic_line;
  f->putchar = &generic_putchar;
  f->bitblt = &generic_bitblt;

  /* Unsure how to deal with convert.
   * The generic function is really just a dummy function which does 
   * nothing right now. */
  f->sp8_convert = &generic_sp8_convert;
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
  FBCMAP *fbcmap;
  int i, length, step;

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
  length = 1<<(MAX(f->vinf.red.length, MAX(f->vinf.green.length,
					   f->vinf.blue.length)));
  f->cmap = (FBCMAP *) FBalloc( sizeof(FBCMAP) );
  f->cmap->red=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
  f->cmap->green=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
  f->cmap->blue=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
  
  /* We begin by setting generic functions */
  setgeneric(f);

  switch(f->finf.type) { /* FB_TYPE_* */
  case FB_TYPE_INTERLEAVED_PLANES:
    switch(f->finf.visual) { /* FB_VISUAL_* */
    case FB_VISUAL_PSEUDOCOLOR:
      switch(f->vinf.bits_per_pixel) {
      case 1:           /* Atari interleaved 1 plane */
        i2_1_register_functions(f);
        break;

      case 2:           /* Atari interleaved 2 planes */
        i2_2_register_functions(f);
        break;

      case 4:           /* Atari interleaved 4 planes */
        i2_4_register_functions(f);
        break;

      case 8:           /* Atari interleaved 8 planes */
        i2_8_register_functions(f);
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
	pl_1_register_functions(f);
        break;

      case 2:           /* Amiga 2 planes */
	pl_2_register_functions(f);
        break;

      case 4:           /* Amiga 4 planes */
	pl_4_register_functions(f);
        break;

      case 8:           /* Amiga 8 planes */
	pl_8_register_functions(f);
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
      i2_1_register_functions(f);
      break;    /* need to fix colours here? */
          
    case FB_VISUAL_MONO10:
      i2_1_register_functions(f);
      break;

    case FB_VISUAL_TRUECOLOR:
    case FB_VISUAL_DIRECTCOLOR:
      switch(f->vinf.bits_per_pixel) {
      case 15:
      case 16:
	pp_16_register_functions(f);
	break;
	
      case 24: /* not done yet */
	FBerror(FATAL, "FBsetfuncs: 24-bit truecolour unsupported.");
	break;

      case 32:
	pp_32_register_functions(f);
	break;

      default:
	FBerror(FATAL, "FBsetfuncs: unsupported number of bits per pixel (%d).",
		f->vinf.bits_per_pixel);
	break;
      }

      fbcmap = FBgetcmap(f);
      if(fbcmap == NULL)
	break;

      length = MAX(f->vinf.red.length, MAX(f->vinf.green.length, 
					   f->vinf.blue.length));

      step = 65535 / ((1 << f->vinf.red.length) - 1);
      for(i = 0 ; i < (1<<f->vinf.red.length) ; i++) {
	fbcmap->red[i] = step * i;
      }
      step = 65535 / ((1 << f->vinf.green.length) - 1);
      for(i = 0 ; i < (1<<f->vinf.green.length) ; i++) {
	fbcmap->green[i] = step * i;
      }
      step = 65535 / ((1 << f->vinf.blue.length) - 1);
      for(i = 0 ; i < (1<<f->vinf.blue.length) ; i++) {
	fbcmap->blue[i] = step * i;
      }
      fbcmap->start = 0;
      fbcmap->len = 1 << length;
      fbcmap->end = fbcmap->len - 1;
      FBputcmap(f, fbcmap);
      FBfreecmap(fbcmap);
      break;
      
    case FB_VISUAL_PSEUDOCOLOR:
    case FB_VISUAL_STATIC_PSEUDOCOLOR:
      switch(f->vinf.bits_per_pixel) {
      case 1:           /* Atari mono/duocolour */
	i2_1_register_functions(f);
        break;

      case 8:           /* 8-bit chunk mode */
	pp_8_register_functions(f);
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

  /* Read the current colourmap, to store it in the internal palette. */

  fbcmap = FBgetcmap(f);
  if(fbcmap)
    FBfreecmap(fbcmap);
}
