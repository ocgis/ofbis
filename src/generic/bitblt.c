#include "ofbis.h"

/* The slowest form of bitblt, get one pixel, put one pixel. 
 * This only supports a solid block so far, and only within the
 * framebuffer display.
 */
void
generic_bitblt(FB *f, FBBLTPBLK *fbb)
{
  int sx, sy, dx, dy;

  for(sy = fbb->s_ymin, dy = fbb->d_ymin ; sy < fbb->b_ht ; sy++, dy++) {
    for(sx = fbb->s_xmin, dx = fbb->d_xmin ; sx < fbb->b_wd ; sx++, dx++) {
      FBputpixel(f, dx, dy, FBgetpixel(f, sx, sy));
    }
  }  
}
