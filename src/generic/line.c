#include "ofbis.h"
#include "common.h"

/* Use Bresenham's line algorithm instead */
/* I think it's faster, and it's definitely shorter code
   and easier to understand. */
void
generic_line( FB *f, u_int16_t a1, u_int16_t b1, u_int16_t a2, u_int16_t b2, u_int32_t col)
{
	/* short is faster, at least on m68k */
	int16_t x, y, xend, yend, dx, dy, c1, c2, p;
	int16_t step;


	dx = abs(a2-a1);
	dy = abs(b2-b1);

	if (dx > dy) { /* slope < 1 => step in x direction */
		x = MIN(a1,a2);
		xend = MAX(a1,a2);
		if (x == a1) {
			y = b1;
			step = ((b2-y)<0 ? -1 : 1);
		} else {
			y = b2;
			step = ((b1-y)<0 ? -1 : 1);
		}

		p = 2*dy - dx;
		c1 = 2*dy;
		c2 = 2*(dy-dx);
		
		(*f->putpixel)(f, x, y, col);
		while (x < xend) {
			x++;
			if (p < 0) {
				p += c1;
			} else {
				y += step;
				p += c2;
			}
			(*f->putpixel)(f, x, y, col);
		}
	} else { /* slope > 1 => step in y direction */
		y = MIN(b1,b2);
		yend = MAX(b1,b2);
		if (y == b1) {
			x = a1;
			step = ((a2-x)<0 ? -1 : 1);
		} else {
			x = a2;
			step = ((a1-x)<0 ? -1 : 1);
		}

		p = 2*dx - dy;
		c1 = 2*dx;
		c2 = 2*(dx-dy);
		
		(*f->putpixel)(f, x, y, col);
		while (y < yend) {
			y++;
			if (p < 0) {
				p += c1;
			} else {
				x += step;
				p += c2;
			}
			(*f->putpixel)(f, x, y, col);
		}
	}
}



