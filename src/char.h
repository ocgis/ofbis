#ifndef _CHAR_H_
#define _CHAR_H_

#include "ofbis.h"

/* Font handling functions */

void	FBfontopen( FB *f );
void	FBfontclose( FB *f );
void	FBsetfont( FB *f, FBFONT *newfont );

#endif
