#ifndef _FBPIXEL_H_
#define _FBPIXEL_H_

#include "ofbis.h"

/* Put pixel functions */

void	i2pp1( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	i2pp2( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	i2pp4( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	i2pp8( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	pptc( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	ppspc( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	plpp1( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	plpp2( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	plpp4( FB *f, unsigned short x, unsigned short y, unsigned long col );
void	plpp8( FB *f, unsigned short x, unsigned short y, unsigned long col );

/* Get pixel functions */

unsigned long	i2gp1( FB *f, unsigned short x, unsigned short y );
unsigned long	i2gp2( FB *f, unsigned short x, unsigned short y );
unsigned long	i2gp4( FB *f, unsigned short x, unsigned short y );
unsigned long	i2gp8( FB *f, unsigned short x, unsigned short y );
unsigned long	gptc( FB *f, unsigned short x, unsigned short y );
unsigned long	gpspc( FB *f, unsigned short x, unsigned short y );
unsigned long	plgp1( FB *f, unsigned short x, unsigned short y );
unsigned long	plgp2( FB *f, unsigned short x, unsigned short y );
unsigned long	plgp4( FB *f, unsigned short x, unsigned short y );
unsigned long	plgp8( FB *f, unsigned short x, unsigned short y );

#endif
