#ifndef _FBCONVERT_H_
#define _FBCONVERT_H_

#include "ofbis.h"

/* convert functions */

void	sp8_to_i1( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_i2( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_i4( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_i8( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_tc( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_sp8( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_p1( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_p2( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_p4( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );
void	sp8_to_p8( FB *f, FBBLOCK *srcblock, FBBLOCK *dstblock );

#endif
