/* Packed pixels 8 bits per pixel */

#include <sys/types.h>
#include <string.h>

#include "ofbis.h"
#include "common.h"
#include "bitblt.h"

void
pp_8_putpixel(FB *f, u_int16_t x, u_int16_t y, u_int32_t col)
{
  register u_int8_t *pixel =
    (u_int8_t *)f->sbuf + ((y * f->vinf.xres_virtual)+x);

  switch (f->writemode) {
  case FB_XOR:
    *pixel ^= (u_int8_t)col;
    break;

  default:
    *pixel = (u_int8_t)col;
  }
}

u_int32_t
pp_8_getpixel(FB *f, u_int16_t x, u_int16_t y)
{
  register u_int8_t *pixel =
    (u_int8_t *)f->sbuf + ((y * f->vinf.xres_virtual)+x);

  return (u_int32_t)*pixel;
}

void
pp_8_hline(FB *f, u_int16_t x1, u_int16_t x2, u_int16_t y, u_int32_t col)
{
  register u_int16_t	x = MIN(x1,x2);
  register u_int16_t	j = MAX(x1,x2);
  register u_int8_t	*pixel =
    (u_int8_t *)f->sbuf + ((y*f->vinf.xres_virtual) + x);

  /* FIXME
  ** Maybe there should be individual functions that are set in FB instead? */
  switch (f->writemode) {
  case FB_XOR:
    while (x++ <= j) {
      *pixel++ ^= (u_int8_t)col;
    }
    break;

  default:
    while (x++ <= j) {
      *pixel++ = (u_int8_t)col;
    }
  }
}

void
pp_8_putchar(FB *f, u_int16_t x, u_int16_t y, u_int32_t fgcol, u_int32_t bgcol, u_int8_t ch)
{
  int row;
  register u_int8_t *base = ((u_int8_t *)f->sbuf) + 
    (x + y * f->vinf.xres_virtual);
  register u_int8_t *chardata = &f->font->data[f->font->height * ch];

  for (row = 0; row < f->font->height; row++, chardata++) {
    register u_int8_t data = *chardata;
    int p;

    for (p = 0; p < f->font->width; p++) {
      if (data & 0x80) {
	*base = fgcol;
      } else if (!(bgcol & 0x80000000)) {
	*base = bgcol;
      }

      base++;
      data <<= 1;
    }

    base += f->vinf.xres_virtual - f->font->width;
  }
}

static __inline__
void copyline(u_int8_t *dst, u_int8_t *src, size_t num, int dir, int logicop)
{
  register int i;
  
  if (dir == BITBLT_FORWARD) {
    for(i=0; i<num; i++) {
      LOGICOP((u_int32_t)dst[i], (u_int32_t)src[i], logicop);
    }
  } else {
    for(i=num-1; i>=0; i--) {
      LOGICOP((u_int32_t)dst[i], (u_int32_t)src[i], logicop);
    }
  }
}

void
pp_8_bitblt( FB *f, FBBLTPBLK *fbb )
{
  u_int8_t *sbase = ((u_int8_t *) fbb->s_form) +
    fbb->s_xmin + (fbb->s_ymin * fbb->s_nxln);
  u_int8_t *dbase = ((u_int8_t *) fbb->d_form) +
    fbb->d_xmin + (fbb->d_ymin * fbb->d_nxln);
  u_int8_t *tdbase, *tsbase;
  int hcnt;

  /* If the logic operations aren't used, we can speed up the
   * process a lot by using memmove() and memcpy().
   */
  if((fbb->op_tab & 0xf) == 0x3) {
    /* Special optimized case if the bitblt covers the whole width. */
    if(fbb->b_wd == f->vinf.xres_virtual &&
       fbb->s_nxln == fbb->d_nxln &&
       fbb->s_nxln == f->vinf.xres_virtual) {
      if(sbase < dbase) {
	memmove(dbase, sbase, fbb->b_wd * fbb->b_ht);
      } else if (sbase > dbase) {
	memcpy(dbase, sbase, fbb->b_wd * fbb->b_ht);
      }
      return;
    }

    /* No logicop, but only part of the display. */
    if (sbase < dbase) {
      tsbase = sbase + fbb->s_nxln * (fbb->b_ht-1);
      tdbase = dbase + fbb->d_nxln * (fbb->b_ht-1);
      for(hcnt = fbb->b_ht-1; hcnt >= 0; hcnt--) {
	memmove(tdbase, tsbase, fbb->b_wd);
	tsbase -= fbb->s_nxln;
	tdbase -= fbb->d_nxln;
      }
    } else if (sbase > dbase) {
      for(hcnt = 0; hcnt < fbb->b_ht; hcnt++) {
	memcpy(dbase, sbase, fbb->b_wd);
	sbase += fbb->s_nxln;
	dbase += fbb->d_nxln;
      }
    }
  } else {
    /* Use logicop, to some basic extent. */
    if (sbase < dbase) {
      tsbase = sbase + fbb->s_nxln * (fbb->b_ht-1);
      tdbase = dbase + fbb->d_nxln * (fbb->b_ht-1);
      for(hcnt = fbb->b_ht-1; hcnt >= 0; hcnt--) {
	copyline(tdbase, tsbase, fbb->b_wd,
		 BITBLT_BACKWARD, fbb->op_tab & 0xf);
	tsbase -= fbb->s_nxln;
	tdbase -= fbb->d_nxln;
      }
    } else if (sbase > dbase) {
      for(hcnt = 0; hcnt < fbb->b_ht; hcnt++) {
	copyline(dbase, sbase, fbb->b_wd,
		 BITBLT_FORWARD, fbb->op_tab & 0xf);
	sbase += fbb->s_nxln;
	dbase += fbb->d_nxln;
      }
    }
  }
}

u_int32_t pp_8_c24_to_cnative(FB *f, u_int32_t col24)
{
  int best_diff, best_match, i, diff;
  int red, green, blue;

  /* For palette based modes, we try to find the best matching
   * colour index.
   */
  if(f->cmap == NULL)
    return 0;

  best_diff = 1024;
  best_match = 255;
  for(i = 0 ; i < 256 ; i++) {
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

u_int32_t pp_8_cnative_to_c24(FB *f, u_int32_t col)
{
  int index;
  u_int32_t colour;

  /* This picks out the colour from the palette. */
  if(f->cmap == NULL)
    return 0;

  index = col < 256 ? col : 255;
  index = index >= 0 ? index : 0;
  colour = ((((f->cmap->red[index] >> 8) & 0xff) << 16) |
	    (((f->cmap->green[index] >> 8) & 0xff) <<  8) |
	    (((f->cmap->blue[index] >> 8) & 0xff) <<  0));
  return colour;
}

/* The converting routines has to be made in a different way later, if 
 * they should even be in oFBis at all.
 */

/* 1998-10-27 Tomas Berndtsson
 *
 * Convert 8-bit static pseudocolour to 
 * 8-bit static pseudocolour. (i.e. do nothing)
 * Sets width and height for destination block.
 */
void pp_8_sp8_convert( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock )
{
  dstblock->width = srcblock->width;
  dstblock->height = srcblock->height;

  memcpy(dstblock->addr, srcblock->addr, srcblock->width*srcblock->height);
}

void
pp_8_register_functions(FB *f)
{
  f->putpixel       = &pp_8_putpixel;
  f->getpixel       = &pp_8_getpixel;
  f->hline          = &pp_8_hline;
  f->putchar        = &pp_8_putchar;
  f->bitblt         = &pp_8_bitblt;
  f->c24_to_cnative = &pp_8_c24_to_cnative;
  f->cnative_to_c24 = &pp_8_cnative_to_c24;
  f->sp8_convert    = &pp_8_sp8_convert;
}
