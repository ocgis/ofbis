#include <stdio.h>
#include <sys/ioctl.h>
#include "ofbis.h"
#include "alloc.h"
#include "error.h"
#include "common.h"

FBCMAP *
FBgetcmap( FB *f )
{
	int	length=0;	/* length of colourmap */
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
	length = 1<<(MAX(f->vinf.red.length, MAX(f->vinf.green.length,
						 f->vinf.blue.length)));
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
	if(f->cmap) {
	  f->cmap->start = fbcmap->start;
	  f->cmap->end = fbcmap->end;
	  for(i = fbcmap->start ; i <= fbcmap->end ; i++) {
	    f->cmap->red[i] = fbcmap->red[i];
	    f->cmap->green[i] = fbcmap->green[i];
	    f->cmap->blue[i] = fbcmap->blue[i];
	  }
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
	if(f->cmap) {
	  for(i = fbcmap->start ; i <= fbcmap->end ; i++) {
	    f->cmap->red[i] = fbcmap->red[i];
	    f->cmap->green[i] = fbcmap->green[i];
	    f->cmap->blue[i] = fbcmap->blue[i];
	  }
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
