#ifndef _FBKBD_H_
#define _FBKBD_H_

#include "libfb.h"
#include "fbevent.h"

void	FBkbdopen( FB *f );
void	FBkbdclose( FB *f );
char	FBkbdgetkey( FB *f );
void	FBprocesskey( FB *f, FBKEYEVENT *ev );

#endif
