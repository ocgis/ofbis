/* VGA planes 4 bits per pixel */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include "ofbis.h"
#include "error.h"

/* Not having sys/io.h means we can't support this display mode. 
 * Therefore, we just skip this whole file, except for a different
 * register function at the bottom, which prints undefined display.
 */
#ifdef HAVE_SYS_IO_H
#include <unistd.h>
#include <sys/types.h>
#include <sys/io.h>

#include "common.h"
#include "bitblt.h"

/* Initialize the VGA controller. Return 0 on error */
static int
vga_4_init(FB *f)
{
  if(ioperm(0x3c0, 0x20, 1) == -1)
    return 0;

  /* Set the Enable Set/Reset Register. */
  outb(1, 0x3ce);
  outb(0xf, 0x3cf);

  /* Set the Data Rotate Register to 0*/
  outb(3, 0x3ce);
  outb(0, 0x3cf);

  /* Set the Graphics Mode Register to 0. */
  outb(5, 0x3ce);
  outb(0, 0x3cf);

  return 1;
}

void
vga_4_putpixel(FB *f, u_int16_t x, u_int16_t y, u_int32_t col)
{
  volatile unsigned char *pixel=(unsigned char *)f->sbuf + 
    x / 8 + y * f->finf.line_length;

  /* Set/Reset Register for drawing in (color) for write mode 0 */
  outb(0, 0x3ce);
  outb((int) col, 0x3cf);

  /* Select the Bit Mask Register on the Graphics Controller. */
  outb(8, 0x3ce);
  
  /* Only effect the pixels selected in Mask Register */
  outb((0x80 >> (x % 8)), 0x3cf);
  
  /* Read-modify-write the specified memory byte. */
  *pixel |= 1;
}

u_int32_t
vga_4_getpixel(FB *f, u_int16_t x, u_int16_t y)
{
  volatile unsigned char *src = (unsigned char *)f->sbuf + 
    x / 8 + y * f->finf.line_length;
  int             plane;
  u_int32_t       colour = 0;
  
  for(plane = 0 ; plane < 4 ; ++plane) {
    /* Set the Read Map Select register. */
    outb(4, 0x3ce);
    outb(plane, 0x3cf);
    if(*src & (0x80 >> (x % 8)))
      colour |= 1 << plane;
  }

  return colour;
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
  if(vga_4_init(f) == 0) {
    FBerror(FATAL | SYSERR, "vga_4: init failed.");
  }

  f->putpixel       = &vga_4_putpixel;
  f->getpixel       = &vga_4_getpixel;
  f->c24_to_cnative = &vga_4_c24_to_cnative;
  f->cnative_to_c24 = &vga_4_cnative_to_c24;
}

#else /* HAVE_SYS_IO_H */

void
vga_4_register_functions(FB *f)
{
  FBerror(FATAL | SYSERR, "vga_4: unsupported mode on this platform.");
}

#endif /* HAVE_SYS_IO_H */
