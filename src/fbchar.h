#ifndef _FBCHAR_H_
#define _FBCHAR_H_

#include "ofbis.h"

/* Putchar functions */

void	genpc( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	i2pc1( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	i2pc2( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	i2pc4( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	i2pc8( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	pctc( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	pcsp8( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch);
void	plpc1( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	plpc2( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	plpc4( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );
void	plpc8( FB *f, unsigned short x, unsigned short y, unsigned long fgcol, unsigned long bgcol, unsigned char ch );

/* Font handling functions */

void	FBfontopen( FB *f );
void	FBfontclose( FB *f );
void	FBsetfont( FB *f, FBFONT *newfont );

#endif
