#include "ofbis.h"
#include "common.h"
#include "bitblt.h"
#include "alloc.h"

FBBLTPBLK *
FBgetbltpblk( FB *f )
{
	FBBLTPBLK *fbb;
	int i;

	fbb = (FBBLTPBLK *) FBalloc( sizeof(FBBLTPBLK) );

	fbb->b_wd = f->vinf.xres_virtual;
	fbb->b_ht = f->vinf.yres_virtual;
	fbb->plane_ct = f->vinf.bits_per_pixel;
	fbb->fg_col = fbb->bg_col = 0;
	fbb->op_tab = 0x03030303; /* dest = src */
	fbb->s_form = fbb->d_form = f->sbuf;
	fbb->s_xmin = fbb->s_ymin = fbb->d_xmin = fbb->d_ymin = 0;

	/* these aren't used yet, but we could just as well set them up anyway */
	fbb->p_addr = NULL;
	fbb->p_nxln = fbb->p_nxpl = fbb->p_mask = 0;
	for( i = 0 ; i < 12 ; i++ )
		fbb->space[i] = 0;


	switch (f->finf.type)
	{
		case FB_TYPE_INTERLEAVED_PLANES:
			fbb->s_nxwd = fbb->d_nxwd = f->vinf.bits_per_pixel * 2;
			fbb->s_nxln = fbb->d_nxln = EVENWORD(f->vinf.xres_virtual) * 2 *
			     f->vinf.bits_per_pixel;
			fbb->s_nxpl = fbb->d_nxpl = 2;
			break;

		case FB_TYPE_PLANES:
			fbb->s_nxwd = fbb->d_nxwd = 2;
			fbb->s_nxln = fbb->d_nxln = EVENWORD(f->vinf.xres_virtual) * 2;
			fbb->s_nxpl = fbb->d_nxpl = EVENWORD(f->vinf.xres_virtual) * 2 *
			     f->vinf.yres_virtual;
			break;
		/* not sure what's correct or not here - Stefan */
		case FB_TYPE_PACKED_PIXELS:
			fbb->s_nxwd = fbb->d_nxwd = f->vinf.bits_per_pixel * 2;
			fbb->s_nxln = fbb->d_nxln = (f->vinf.xres_virtual * 
						     f->vinf.bits_per_pixel)/8;
			fbb->s_nxpl = fbb->d_nxpl = 2;
			break;

		/* not sure what to have when type is not any of the above */
		default:
			fbb->s_nxwd = fbb->d_nxwd = f->vinf.bits_per_pixel * 2;
			fbb->s_nxln = fbb->d_nxln = EVENWORD(f->vinf.xres_virtual) * 2 *
			     f->vinf.bits_per_pixel;
			fbb->s_nxpl = fbb->d_nxpl = 2;
			break;
	}

	return fbb;
}

void
FBfreebltpblk( FBBLTPBLK *fbb )
{
     FBfree( fbb );
}
