#include "ofbis.h"
#include "common.h"

void
generic_hline( FB *f, u_int16_t x1, u_int16_t x2, u_int16_t y, u_int32_t col )
{
	u_int16_t	x = MIN(x1,x2);
	u_int16_t	j = MAX(x1,x2);
	
	while ( x <= j )
	{
		(*f->putpixel)(f, x++, y, col);
	}
}

