#include <stdio.h>
#include <sys/ioctl.h>
#include "ofbis.h"
#include "fballoc.h"
#include "fberror.h"

FBCMAP *
FBgetcmap( FB *f )
{
	static int	length=0;	/* length of colourmap */
	struct fb_cmap	cmap;
	FBCMAP		*fbcmap;	/* new colourmap */
	int i;

	fbcmap = (FBCMAP *) FBalloc( sizeof(FBCMAP) );

	/* nasty stuff... tidy this up */

	/*if ( length != (1<<f->vinf.bits_per_pixel) && (length != 0 ))
	{
		FBfree(cmap.red);
		FBfree(cmap.green);
		FBfree(cmap.blue);
		length = 0;
	}
	if ( length == 0 )*/

	{
	length = 1<<f->vinf.bits_per_pixel;
	cmap.red=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
	cmap.green=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
	cmap.blue=(unsigned short *) FBalloc( length * sizeof(unsigned short) );
	}

	/* Set values for get colourmap ioctl */

	cmap.start=0;
	cmap.len=length;
	cmap.transp=NULL;

	if ( ioctl(f->fb, FBIOGETCMAP, &cmap) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBgetcmap: Get colourmap failed" );
	}
	fbcmap->start=cmap.start;
	fbcmap->end=cmap.len-1;
	fbcmap->len=cmap.len;
	fbcmap->red=cmap.red;
	fbcmap->green=cmap.green;
	fbcmap->blue=cmap.blue;

	/* Update the internal palette. */
	f->cmap->start = fbcmap->start;
	f->cmap->end = fbcmap->end;
	for(i = fbcmap->start ; i <= fbcmap->end ; i++) {
	  f->cmap->red[i] = fbcmap->red[i];
	  f->cmap->green[i] = fbcmap->green[i];
	  f->cmap->blue[i] = fbcmap->blue[i];
	}

	return fbcmap;
}

void
FBputcmap( FB *f, FBCMAP *fbcmap )
{
	struct fb_cmap	cmap;
	int i;

	cmap.start=fbcmap->start;
	cmap.len=fbcmap->end-fbcmap->start+1;
	cmap.red= &fbcmap->red[fbcmap->start];
	cmap.green= &fbcmap->green[fbcmap->start];
	cmap.blue= &fbcmap->blue[fbcmap->start];

	if ( ioctl(f->fb, FBIOPUTCMAP, &cmap) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBputcmap: Put colourmap failed" );
	}

	/* Update the internal palette too. */
	for(i = fbcmap->start ; i <= fbcmap->end ; i++) {
	  f->cmap->red[i] = fbcmap->red[i];
	  f->cmap->green[i] = fbcmap->green[i];
	  f->cmap->blue[i] = fbcmap->blue[i];
	}
}

void
FBfreecmap( FBCMAP *fbcmap )
{
	FBfree( fbcmap->red );
	FBfree( fbcmap->green );
	FBfree( fbcmap->blue );
	FBfree( fbcmap );
}

u_int32_t
FBc24_to_cnative( FB *f, u_int32_t col24 )
{
  int nr_of_colours, best_diff, best_match, i, diff;
  int red, green, blue;
  u_int32_t pval;

  /* For palette based modes, we try to find the best matching
   * colour index.
   */
  if(f->finf.visual == FB_VISUAL_PSEUDOCOLOR) {
    nr_of_colours = 1 << f->vinf.bits_per_pixel;

    best_diff = 1024;
    best_match = nr_of_colours - 1;
    for(i = 0 ; i < nr_of_colours ; i++) {
      red   = ((col24 >> 16) & 0xff) - (f->cmap->red[i] >> 8);
      green = ((col24 >>  8) & 0xff) - (f->cmap->green[i] >> 8);
      blue  = ((col24      ) & 0xff) - (f->cmap->blue[i] >> 8);
      diff = red * red + green * green + blue * blue;
      if(diff < best_diff) {
	best_diff = diff;
	best_match = i;
      }
    }

    return best_match;
  }

  /* Truecolour. */  
  pval = ((((col24 >> (16 + 8 - f->vinf.red.length))   & ((1 << f->vinf.red.length)   - 1)) << f->vinf.red.offset) |
	  (((col24 >> ( 8 + 8 - f->vinf.green.length)) & ((1 << f->vinf.green.length) - 1)) << f->vinf.green.offset) |
	  (((col24 >> ( 0 + 8 - f->vinf.blue.length))  & ((1 << f->vinf.blue.length)  - 1)) << f->vinf.blue.offset));
  return pval;
}

u_int32_t
FBcnative_to_c24( FB *f, u_int32_t col )
{
  int index, nr_of_colours;
  u_int32_t colour;

  /* This picks out the colour from the palette. */
  if(f->finf.visual == FB_VISUAL_PSEUDOCOLOR) {
    nr_of_colours = 1 << f->vinf.bits_per_pixel;
    index = col < nr_of_colours ? col : nr_of_colours-1;
    index = index >= 0 ? index : 0;
    colour = ((((f->cmap->red[index] >> 8) & 0xff) << 16) |
	      (((f->cmap->green[index] >> 8) & 0xff) <<  8) |
	      (((f->cmap->blue[index] >> 8) & 0xff) <<  0));
    return colour;
  }

  /* Truecolour */  
  colour = ((((col >> f->vinf.red.offset) & ((1 << f->vinf.red.length) - 1)) << (8 - f->vinf.red.length)) |
	    (((col >> f->vinf.green.offset) & ((1 << f->vinf.green.length) - 1)) << (8 - f->vinf.green.length)) |
	    (((col >> f->vinf.blue.offset) & ((1 << f->vinf.blue.length) - 1)) << (8 - f->vinf.blue.length)));
  return colour;
}
