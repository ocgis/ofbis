#ifndef _KBD_H_
#define _KBD_H_

#include "ofbis.h"
#include "event.h"

void	FBkbdopen( FB *f );
void	FBkbdclose( FB *f );
char	FBkbdgetkey( FB *f );
void	FBprocesskey( FB *f, FBKEYEVENT *ev );

#endif
