#ifndef _FBMOUSE_H_
#define _FBMOUSE_H_
#include "fbevent.h"

extern int msefd;

void	FBmouseopen( void );
void	FBprocessmouse( FBMOUSEEVENT *ev );
void	FBmouseclose( void );

#endif
