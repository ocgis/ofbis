#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "event.h"

extern int msefd;

void	FBmouseopen( void );
void	FBprocessmouse( FBMOUSEEVENT *ev );
void	FBmouseclose( void );

#endif
