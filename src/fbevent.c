/* FBevent - event handling for libfb */

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include "libfb.h"
#include "fberror.h"
#include "fbevent.h"
#include "fbkbd.h"
#include "fbmouse.h"

void
FBgetevent( FB *f, FBEVENT *ev )
{
	int interrupted;
	fd_set	in;

	FD_ZERO( &in );
	FD_SET( f->tty, &in );
	if ( f->sbuf != f->sbak )
	{
		FD_SET( msefd, &in );
		tcflush( msefd, TCIFLUSH );
	}

	do
	{
		interrupted = FALSE;

		if ( select( FD_SETSIZE, &in, NULL, NULL, NULL ) == -1)
		{
			if (errno==EINTR)
			{
				interrupted = TRUE;
				if ( f->sbuf == f->sbak )
				{
					FD_CLR( msefd, &in );
				}
				else
				{
					FD_SET( msefd, &in );
					tcflush( msefd, TCIFLUSH );
				}
			}
			else
			{
				FBerror( FATAL | SYSERR, "FBgetevent: select() returned" );
			}
		}
	} while ( interrupted );

	if ( FD_ISSET( f->tty, &in ) )
	{
		ev->type = FBKeyEvent;
		FBprocesskey( f, &ev->key );
	}
	else if ( FD_ISSET( msefd, &in ) )
	{
		ev->type = FBMouseEvent;
		FBprocessmouse( &ev->mouse );		
/*		printf("buttons: %d x: %d y: %d\n",ev->mouse.buttons, ev->mouse.x, ev->mouse.y ); */
	}
}

void
FBcheckevent( FB *f, FBEVENT *ev, struct timeval *tv )
{
	fd_set	in;

	FD_ZERO( &in );
	FD_SET( f->tty, &in );
	if ( f->sbuf != f->sbak )
	{
		FD_SET( msefd, &in );
		tcflush( msefd, TCIFLUSH );
	}

	if ( select( FD_SETSIZE, &in, NULL, NULL, tv ) != -1)
	{
		if ( FD_ISSET( f->tty, &in ) )
		{
			ev->type = FBKeyEvent;
			FBprocesskey( f, &ev->key );
		}
		else if ( FD_ISSET( msefd, &in ) )
		{
			ev->type = FBMouseEvent;
			FBprocessmouse( &ev->mouse );		
/*			printf("buttons: %d x: %d y: %d\n",ev->mouse.buttons, ev->mouse.x, ev->mouse.y ); */
		}
		else
		{
			ev->type = FBNoEvent;
		}
	}
	else
	{
		ev->type = FBNoEvent;
	}
}

int
FBgetchar( FB *f )
{
	FBEVENT	ev;

	do
	{
		FBgetevent( f, &ev );
	} while ( ( ev.type != FBKeyEvent ) ||
		  ( ( ev.key.keycode & 0x80)==0x80) );

	return((int)ev.key.ascii);
}
