/* Packed pixels 32 bits per pixel */

#include <sys/types.h>
#include <string.h>

#include "ofbis.h"
#include "common.h"
#include "bitblt.h"

void
pp_32_putpixel( FB *f, u_int16_t x, u_int16_t y, u_int32_t col )
{
  register u_int32_t *pixel=(u_int32_t *)f->sbuf+((y*f->vinf.xres_virtual)+x);
  
  switch(f->writemode) {
  case FB_XOR:
    *pixel = ~(u_int32_t)*pixel;
    break;
    
  default:
    *pixel = (u_int32_t)col;
  }
}

u_int32_t
pp_32_getpixel( FB *f, u_int16_t x, u_int16_t y )
{
  register u_int32_t *pixel=(u_int32_t *)f->sbuf+((y*f->vinf.xres_virtual)+x);

  return *pixel;
}

void
pp_32_hline( FB *f, u_int16_t x1, u_int16_t x2, u_int16_t y, u_int32_t col )
{
  register u_int16_t x = MIN(x1,x2);
  register u_int16_t j = MAX(x1,x2);
  register u_int32_t *pixel=(u_int32_t *)f->sbuf+((y*f->vinf.xres_virtual)+x);
  
  switch (f->writemode) {
  case FB_XOR:
    while (x++ <= j) {
      *pixel++ = ~(u_int32_t)*pixel;
    }
    break;
    
  default:
    while (x++ <= j) {
      *pixel++ = (u_int32_t)col;
    }
  }
}

void
pp_32_putchar(FB *f, u_int16_t x, u_int16_t y, u_int32_t fgcol, u_int32_t bgcol, u_int8_t ch)
{
  int row;
  register u_int32_t *base = (u_int32_t *)f->sbuf + 
    (x + y * f->vinf.xres_virtual);
  register u_int8_t *chardata = &f->font->data[f->font->height * ch];

  for (row = 0; row < f->font->height; row++, chardata++) {
    register u_int8_t data = *chardata;
    int p;

    for (p = 0; p < f->font->width; p++) {
      if (data & 0x80) {
	*base = (u_int32_t)fgcol;
      } else if (!(bgcol & 0x80000000)) {
	*base = (u_int32_t)bgcol;
      }

      base++;
      data <<= 1;
    }

    base += f->vinf.xres_virtual - f->font->width;
  }
}

static __inline__
void copyline(u_int32_t *dst, u_int32_t *src, size_t num, int dir, int logicop)
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
pp_32_bitblt( FB *f, FBBLTPBLK *fbb )
{
  u_int8_t *sbase = ((u_int8_t *) fbb->s_form) +
    fbb->s_xmin * 4 + (fbb->s_ymin * fbb->s_nxln);
  u_int8_t *dbase = ((u_int8_t *) fbb->d_form) +
    fbb->d_xmin * 4 + (fbb->d_ymin * fbb->d_nxln);
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
	memmove(dbase, sbase, fbb->b_wd * fbb->b_ht * 4);
      } else if (sbase > dbase) {
	memcpy(dbase, sbase, fbb->b_wd * fbb->b_ht * 4);
      }
      return;
    }

    /* No logicop, but only part of the display. */
    if (sbase < dbase) {
      tsbase = sbase + fbb->s_nxln * (fbb->b_ht-1);
      tdbase = dbase + fbb->d_nxln * (fbb->b_ht-1);
      for(hcnt = fbb->b_ht-1; hcnt >= 0; hcnt--) {
	memmove(tdbase, tsbase, fbb->b_wd * 4);
	tsbase -= fbb->s_nxln;
	tdbase -= fbb->d_nxln;
      }
    } else if (sbase > dbase) {
      for(hcnt = 0; hcnt < fbb->b_ht; hcnt++) {
	memcpy(dbase, sbase, fbb->b_wd * 4);
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
	copyline((u_int32_t *)tdbase, (u_int32_t *)tsbase, fbb->b_wd * 4,
		 BITBLT_BACKWARD, fbb->op_tab & 0xf);
	tsbase -= fbb->s_nxln;
	tdbase -= fbb->d_nxln;
      }
    } else if (sbase > dbase) {
      for(hcnt = 0; hcnt < fbb->b_ht; hcnt++) {
	copyline((u_int32_t *)dbase, (u_int32_t *)sbase, fbb->b_wd * 4,
		 BITBLT_FORWARD, fbb->op_tab & 0xf);
	sbase += fbb->s_nxln;
	dbase += fbb->d_nxln;
      }
    }
  }
}

u_int32_t
pp_32_c24_to_cnative( FB *f, u_int32_t col24)
{
  u_int32_t pval;

  pval = ((((col24 >> (16 + 8 - f->vinf.red.length))   & ((1 << f->vinf.red.length)   - 1)) << f->vinf.red.offset) |
	  (((col24 >> ( 8 + 8 - f->vinf.green.length)) & ((1 << f->vinf.green.length) - 1)) << f->vinf.green.offset) |
	  (((col24 >> ( 0 + 8 - f->vinf.blue.length))  & ((1 << f->vinf.blue.length)  - 1)) << f->vinf.blue.offset));
  return pval;

}

u_int32_t
pp_32_cnative_to_c24( FB *f, u_int32_t col)
{
  u_int32_t colour;

  colour = ((((col >> f->vinf.red.offset) & ((1 << f->vinf.red.length) - 1)) << (8 - f->vinf.red.length)) |
	    (((col >> f->vinf.green.offset) & ((1 << f->vinf.green.length) - 1)) << (8 - f->vinf.green.length)) |
	    (((col >> f->vinf.blue.offset) & ((1 << f->vinf.blue.length) - 1)) << (8 - f->vinf.blue.length)));
  return colour;
}

void
pp_32_register_functions(FB *f)
{
  f->putpixel       = &pp_32_putpixel;
  f->getpixel       = &pp_32_getpixel;
  f->hline          = &pp_32_hline;
  f->putchar        = &pp_32_putchar;
  f->bitblt         = &pp_32_bitblt;
  f->c24_to_cnative = &pp_32_c24_to_cnative;
  f->cnative_to_c24 = &pp_32_cnative_to_c24;
}
