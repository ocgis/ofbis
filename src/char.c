#include <stdio.h>

#include "ofbis.h"
#include "alloc.h"
#include "char.h"

void
FBfontopen( FB *f )
{
	f->font = (FBFONT *)FBalloc(sizeof(FBFONT));

	/* Default font 8x8 */
	f->font->data = fontdata_8x8;
	f->font->width = fontwidth_8x8;
	f->font->height = fontheight_8x8;
}

void
FBfontclose( FB *f )
{
	FBfree(f->font);
}


void
FBsetfont( FB *f, FBFONT *newfont )
{
	f->font->data = newfont->data;
	f->font->width = newfont->width;
	f->font->height = newfont->height;
}
