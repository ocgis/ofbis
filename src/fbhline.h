#ifndef _FBHLINE_H_
#define _FBHLINE_H_

#include "libfb.h"

/* Horizontal line functions */

void	genhl( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	i2hl1( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	i2hl2( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	i2hl4( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	i2hl8( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	hltc( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	plhl1( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	plhl2( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	plhl4( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );
void	plhl8( FB *f, unsigned short x1, unsigned short x2, unsigned short y, unsigned long col );

#endif
