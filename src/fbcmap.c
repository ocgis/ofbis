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
	f->cmap=fbcmap;
	return fbcmap;
}

void
FBputcmap( FB *f, FBCMAP *fbcmap )
{
	struct fb_cmap	cmap;

	cmap.start=fbcmap->start;
	cmap.len=fbcmap->end-fbcmap->start+1;
	cmap.red= &fbcmap->red[fbcmap->start];
	cmap.green= &fbcmap->green[fbcmap->start];
	cmap.blue= &fbcmap->blue[fbcmap->start];

	if ( ioctl(f->fb, FBIOPUTCMAP, &cmap) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBputcmap: Put colourmap failed" );
	}
	f->cmap=fbcmap;
}

void
FBfreecmap( FBCMAP *fbcmap )
{
	FBfree( fbcmap->red );
	FBfree( fbcmap->green );
	FBfree( fbcmap->blue );
	FBfree( fbcmap );
}

unsigned long
FBc24_to_cnative( FB *f, unsigned long col24 )
{

	if(f->finf.visual != FB_VISUAL_TRUECOLOR)
		return col24;

	switch(f->vinf.bits_per_pixel) {
	case 16:
	{
		unsigned char *cp= (char *)&col24;
		unsigned short pval = ((*++cp << 8 ) & 0xF800) |
				      ((*++cp << 3 ) & 0x07E0) |
				      ((*++cp >> 3 ) & 0x001F);
		return pval;
	}		
		break;
	default:
		return col24;
	}
}

unsigned long
FBcnative_to_c24( FB *f, unsigned long col )
{

	if(f->finf.visual != FB_VISUAL_TRUECOLOR)
		return col;

	switch(f->vinf.bits_per_pixel) {
	case 16:
	{
		unsigned long pval = ((col & 0xF800) << 8) |
				     ((col & 0x07E0) << 5) |
				     ((col & 0x001F) << 3);
		return pval;
	}		
		break;
	default:
		return col;
	}
}
