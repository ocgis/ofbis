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
	static int in_fatal = 0;
	va_list args;
	va_start( args, fmt );

	printf("ofbis %s: ",(fatal?"fatal error":"warning") );
	vprintf( fmt, args );
	va_end( args );
	if (syserr)
	{
		printf( " : %s", strerror(errno) );
	}
	putchar('\n');
	if (fatal) {
		if(in_fatal)
		  exit(1);
		in_fatal = 1;
		FBshutdown();
	}
}

/* Shutdown ofbis in case of a fatal error */

void
FBshutdown( void )
{
	FB	*f;

	/* Close all opened framebuffers */
	while ( ( f=FBfindFB() ) != NULL )
	{
		FBclose(f);
	}

	/* Now exit */

	exit(1);
}
