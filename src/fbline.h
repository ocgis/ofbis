#ifndef _FBLINE_H_
#define _FBLINE_H_

#include "libfb.h"

/* Line functions */

void	genln( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	i2ln1( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	i2ln2( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	i2ln4( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	i2ln8( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	lntc( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	plln1( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	plln2( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	plln4( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );
void	plln8( FB *f, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long col );

#endif
