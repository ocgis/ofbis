#ifndef _FB_LAINIT_H_
#define _FB_LAINIT_H_

#include "../libfb.h"
#include "linea.h"

FBLINEA	*LAalloc( void );
void	LAfree( FBLINEA * );
void	LAgetvars( FB *, FBLINEA * );

#endif
