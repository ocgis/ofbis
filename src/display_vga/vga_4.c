/* VGA planes 4 bits per pixel */

#include <sys/types.h>
#include <string.h>

#include "ofbis.h"
#include "common.h"
#include "bitblt.h"

void
vga_4_putpixel(FB *f, u_int16_t x, u_int16_t y, u_int32_t col)
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
vga_4_getpixel(FB *f, u_int16_t x, u_int16_t y)
{
  register u_int8_t *pixel =
    (u_int8_t *)f->sbuf + ((y * f->vinf.xres_virtual)+x);

  return (u_int32_t)*pixel;
}

u_int32_t vga_4_c24_to_cnative(FB *f, u_int32_t col24)
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

u_int32_t vga_4_cnative_to_c24(FB *f, u_int32_t col)
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

void
vga_4_register_functions(FB *f)
{
  f->putpixel       = &vga_4_putpixel;
  f->getpixel       = &vga_4_getpixel;
  f->c24_to_cnative = &vga_4_c24_to_cnative;
  f->cnative_to_c24 = &vga_4_cnative_to_c24;
}
