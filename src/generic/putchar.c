#include "ofbis.h"
#include "common.h"

void
generic_putchar( FB *f, u_int16_t x, u_int16_t y, u_int32_t fgcol, u_int32_t bgcol, unsigned char ch )
{
  int ox, oy;
  register unsigned char *chardata = &f->font->data [f->font->height * ch];
  register unsigned char data;

  for (oy = 0; oy < f->font->height; oy++) {
    data = *chardata++;

    for (ox = 0; ox < f->font->width; ox++) {
      if (data & 0x80) {
	(*f->putpixel)(f, x+ox, y+oy, fgcol);
      }

      data <<= 1;
    }

  }
}

