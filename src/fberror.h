#ifndef _FBERROR_H_
#define _FBERROR_H_

#include "ofbis.h"

/* FBError defines */

#define SYSERR	0x0002
#define	FATAL	0x0001
#define WARNING	0x0000

/* Error handling function */

void	FBerror(unsigned short flags, const char *fmt, ...);
void	FBshutdown( void );

#endif
