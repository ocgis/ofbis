#include "ofbis.h"
#include "fbline.h"

#define	MIN(a,b)	( (a) < (b) ? (a) : (b) )
#define MAX(a,b)	( (a) > (b) ? (a) : (b) )

#define writetoi2plane(x)					\
	base[x] &= ~( 0x00000001 << sc );			\
	base[x] |= ( ( ( col >> x ) & 0x00000001 ) << sc );

#define	writetoplplane(x)					\
	*base &= ~( 0x00000001 << sc );				\
	*base |= ( ( ( col >> x ) & 0x00000001 ) << sc );

#if 0
void
genln( FB *f, unsigned short x1, unsigned short x2, unsigned short y1, unsigned short y2, unsigned long col )
{
	/* implemented below for now */
}
#endif

void
i2ln1( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

void
i2ln2( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

void
i2ln4( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

void
i2ln8( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

void
lntc( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

void
plln1( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

void
plln2( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

void
plln4( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

void
plln8( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col )
{
}

/*
Symmetric Double Step Line Algorithm
by Brian Wyvill
from "Graphics Gems", Academic Press, 1990
Converted to ANSI C by Martin Mitchell
*/

#define swap(a,b)           {a^=b; b^=a; a^=b;}
#define absolute(i,j,k)     ( (i-j)*(k = ( (i-j)<0 ? -1 : 1)))

#define plot_normal(x,y) (*f->putpixel)(f, (x), (y), col)
#define plot_reverse(x,y) (*f->putpixel)(f, (y), (x), col)

/* Use Bresenham's line algorithm instead */
/* I think it's faster, and it's definitely shorter code
   and easier to understand. */
void
genln( FB *f, unsigned short a1, unsigned short b1, unsigned short a2, unsigned short b2, unsigned long col)
{
	/* short is faster, at least on m68k */
	short x, y, xend, yend, dx, dy, c1, c2, p;
	short step;


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

#if 0
/*int*/void
/*symwuline*/genln( FB *f, unsigned short a1, unsigned short b1, unsigned short a2, unsigned short b2, unsigned long col)
{
	int           dx, dy, incr1, incr2, D, x, y, xend, c, pixels_left;
	int           x1, y1;
	int           sign_x, sign_y, step, reverse, i;

	dx = absolute(a2, a1, sign_x);
	dy = absolute(b2, b1, sign_y);
	/* decide increment sign by the slope sign */
	if (sign_x == sign_y)
		step = 1;
	else
		step = -1;

	if (dy > dx) {		/* chooses axis of greatest movement (make
				 		 * dx) */
		swap(a1, b1);
		swap(a2, b2);
		swap(dx, dy);
		reverse = 1;
	} else
		reverse = 0;
	/* note error check for dx==0 should be included here */
	if (a1 > a2) {		/* start from the smaller coordinate */
		x = a2;
		y = b2;
		x1 = a1;
		y1 = b1;
	} else {
		x = a1;
		y = b1;
		x1 = a2;
		y1 = b2;
	}

	/* same algorithm twice, but plotting with x and y switched
	   if reverse==1 */
	if(reverse == 0) { /* plot normal */
		
		/* Note dx=n implies 0 - n or (dx+1) pixels to be set */
		/* Go round loop dx/4 times then plot last 0,1,2 or 3 pixels */
		/* In fact (dx-1)/4 as 2 pixels are already plottted */
		xend = (dx - 1) / 4;
		pixels_left = (dx - 1) % 4;	/* number of pixels left over at the
					 			 * end */
		plot_normal(x, y);
		plot_normal(x1, y1);	/* plot first two points */
		incr2 = 4 * dy - 2 * dx;
		if (incr2 < 0) {	/* slope less than 1/2 */
			c = 2 * dy;
			incr1 = 2 * c;
			D = incr1 - dx;

			for (i = 0; i < xend; i++) {	/* plotting loop */
				++x;
				--x1;
				if (D < 0) {
                  			/* pattern 1 forwards */
					plot_normal(x, y);
					plot_normal(++x, y);
					/* pattern 1 backwards */
					plot_normal(x1, y1);
					plot_normal(--x1, y1);
					D += incr1;
				} else {
					if (D < c) {
						/* pattern 2 forwards */
						plot_normal(x, y);
						plot_normal(++x, y += step);
						/* pattern 2 backwards */
						plot_normal(x1, y1);
						plot_normal(--x1, y1 -= step);	
					} else {
						/* pattern 3 forwards */
						plot_normal(x, y += step);
						plot_normal(++x, y);
						/* pattern 3 backwards */
						plot_normal(x1, y1 -= step);
						plot_normal(--x1, y1);
					}
					D += incr2;
				}
			}		/* end for */

			/* plot last pattern */
			if (pixels_left) {
				if (D < 0) {
					plot_normal(++x, y);	/* pattern 1 */
					if (pixels_left > 1)
						plot_normal(++x, y);
					if (pixels_left > 2)
						plot_normal(--x1, y1);
				} else {
					if (D < c) {
						plot_normal(++x, y);	/* pattern 2  */
						if (pixels_left > 1)
							plot_normal(++x, y += step);
						if (pixels_left > 2)
							plot_normal(--x1, y1);
					} else {
						/* pattern 3 */
						plot_normal(++x, y += step);
						if (pixels_left > 1)
							plot_normal(++x, y);
						if (pixels_left > 2)
							plot_normal(--x1, y1 -= step);
					}
				}
			}		/* end if pixels_left */
		}
		/* end slope < 1/2 */
		else {			/* slope greater than 1/2 */
			c = 2 * (dy - dx);
			incr1 = 2 * c;
			D = incr1 + dx;
			for (i = 0; i < xend; i++) {
				++x;
				--x1;
				if (D > 0) {
					/* pattern 4 forwards */
					plot_normal(x, y += step);
					plot_normal(++x, y += step);
					/* pattern 4 backwards */
					plot_normal(x1, y1 -= step);
					plot_normal(--x1, y1 -= step);
					D += incr1;
				} else {
					if (D < c) {
						/* pattern 2 forwards */
						plot_normal(x, y);
						plot_normal(++x, y += step);

						/* pattern 2 backwards */
						plot_normal(x1, y1);
						plot_normal(--x1, y1 -= step);
					} else {
						/* pattern 3 forwards */
						plot_normal(x, y += step);
						plot_normal(++x, y);
						/* pattern 3 backwards */
						plot_normal(x1, y1 -= step);
						plot_normal(--x1, y1);
					}
					D += incr2;
				}
			}		/* end for */
			/* plot last pattern */
			if (pixels_left) {
				if (D > 0) {
					plot_normal(++x, y += step);	/* pattern 4 */
					if (pixels_left > 1)
						plot_normal(++x, y += step);
					if (pixels_left > 2)
						plot_normal(--x1, y1 -= step);
				} else {
					if (D < c) {
						plot_normal(++x, y);	/* pattern 2  */
						if (pixels_left > 1)
							plot_normal(++x, y += step);
						if (pixels_left > 2)
							plot_normal(--x1, y1);
					} else {
						/* pattern 3 */
						plot_normal(++x, y += step);
						if (pixels_left > 1)
							plot_normal(++x, y);
						if (pixels_left > 2) {
#if 1
							if (D > c) /* step 3 */
								{
									plot_normal(--x1, y1 -= step);
								}
							else /* step 2 */
								{
									plot_normal(--x1, y1);
								}
#endif
						}
					}
				}
			}
		}
	} else { /* plot reversed */
		/* Note dx=n implies 0 - n or (dx+1) pixels to be set */
		/* Go round loop dx/4 times then plot last 0,1,2 or 3 pixels */
		/* In fact (dx-1)/4 as 2 pixels are already plottted */
		xend = (dx - 1) / 4;
		pixels_left = (dx - 1) % 4;	/* number of pixels left over at the
					 			 * end */
		plot_reverse(x, y);
		plot_reverse(x1, y1);	/* plot first two points */
		incr2 = 4 * dy - 2 * dx;
		if (incr2 < 0) {	/* slope less than 1/2 */
			c = 2 * dy;
			incr1 = 2 * c;
			D = incr1 - dx;

			for (i = 0; i < xend; i++) {	/* plotting loop */
				++x;
				--x1;
				if (D < 0) {
                  			/* pattern 1 forwards */
					plot_reverse(x, y);
					plot_reverse(++x, y);
					/* pattern 1 backwards */
					plot_reverse(x1, y1);
					plot_reverse(--x1, y1);
					D += incr1;
				} else {
					if (D < c) {
						/* pattern 2 forwards */
						plot_reverse(x, y);
						plot_reverse(++x, y += step);
						/* pattern 2 backwards */
						plot_reverse(x1, y1);
						plot_reverse(--x1, y1 -= step);	
					} else {
						/* pattern 3 forwards */
						plot_reverse(x, y += step);
						plot_reverse(++x, y);
						/* pattern 3 backwards */
						plot_reverse(x1, y1 -= step);
						plot_reverse(--x1, y1);
					}
					D += incr2;
				}
			}		/* end for */

			/* plot last pattern */
			if (pixels_left) {
				if (D < 0) {
					plot_reverse(++x, y);	/* pattern 1 */
					if (pixels_left > 1)
						plot_reverse(++x, y);
					if (pixels_left > 2)
						plot_reverse(--x1, y1);
				} else {
					if (D < c) {
						plot_reverse(++x, y);	/* pattern 2  */
						if (pixels_left > 1)
							plot_reverse(++x, y += step);
						if (pixels_left > 2)
							plot_reverse(--x1, y1);
					} else {
						/* pattern 3 */
						plot_reverse(++x, y += step);
						if (pixels_left > 1)
							plot_reverse(++x, y);
						if (pixels_left > 2)
							plot_reverse(--x1, y1 -= step);
					}
				}
			}		/* end if pixels_left */
		}
		/* end slope < 1/2 */
		else {			/* slope greater than 1/2 */
			c = 2 * (dy - dx);
			incr1 = 2 * c;
			D = incr1 + dx;
			for (i = 0; i < xend; i++) {
				++x;
				--x1;
				if (D > 0) {
					/* pattern 4 forwards */
					plot_reverse(x, y += step);
					plot_reverse(++x, y += step);
					/* pattern 4 backwards */
					plot_reverse(x1, y1 -= step);
					plot_reverse(--x1, y1 -= step);
					D += incr1;
				} else {
					if (D < c) {
						/* pattern 2 forwards */
						plot_reverse(x, y);
						plot_reverse(++x, y += step);

						/* pattern 2 backwards */
						plot_reverse(x1, y1);
						plot_reverse(--x1, y1 -= step);
					} else {
						/* pattern 3 forwards */
						plot_reverse(x, y += step);
						plot_reverse(++x, y);
						/* pattern 3 backwards */
						plot_reverse(x1, y1 -= step);
						plot_reverse(--x1, y1);
					}
					D += incr2;
				}
			}		/* end for */
			/* plot last pattern */
			if (pixels_left) {
				if (D > 0) {
					plot_reverse(++x, y += step);	/* pattern 4 */
					if (pixels_left > 1)
						plot_reverse(++x, y += step);
					if (pixels_left > 2)
						plot_reverse(--x1, y1 -= step);
				} else {
					if (D < c) {
						plot_reverse(++x, y);	/* pattern 2  */
						if (pixels_left > 1)
							plot_reverse(++x, y += step);
						if (pixels_left > 2)
							plot_reverse(--x1, y1);
					} else {
						/* pattern 3 */
						plot_reverse(++x, y += step);
						if (pixels_left > 1)
							plot_reverse(++x, y);
						if (pixels_left > 2) {
#if 1
							if (D > c) /* step 3 */
								{
									plot_reverse(--x1, y1 -= step);
								}
							else /* step 2 */
								{
									plot_reverse(--x1, y1);
								}
#endif
						}
					}
				}
			}
		}
	}
}

#endif /* 0 */
