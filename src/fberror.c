#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "fberror.h"
#include "fbvt.h"

extern int errno;

void
FBerror( unsigned short flags, const char *fmt, ... )
{
	unsigned short fatal = flags & FATAL;
	unsigned short syserr = flags & SYSERR;
	va_list args;
	va_start( args, fmt );

	printf("libfb %s: ",(fatal?"fatal error":"warning") );
	vprintf( fmt, args );
	va_end( args );
	if (syserr)
	{
		printf( " : %s", strerror(errno) );
	}
	putchar('\n');
	if (fatal)
		FBshutdown();
}

/* Shutdown libfb in case of a fatal error */

void
FBshutdown( void )
{
	FB	*f;

	while ( ( f=FBfindFB() ) != NULL )
	{
		FBclose(f);
	}

	/* Now exit */

	exit(1);
}
