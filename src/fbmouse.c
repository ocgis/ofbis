#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "fbmouse.h"
#include "fberror.h"

int	msefd;
int	calls = 0;

void
FBmouseopen( void )
{
	if ( ++calls > 1 )
	{
		return;
	}

	if ( ( msefd = open ( "/dev/mouse", O_RDWR | O_NDELAY )) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBmouseopen: Error opening /dev/mouse" );
	}

	/* Flush input */

	tcflush( msefd, TCIFLUSH );
}

int
FBmouseget( void )
{
	char	buf[3];

	if ( read( msefd, &buf, sizeof(buf) ) == -1 )
	{
		return -1;
	}
	else
	{
/*		printf("%d %d %d\n",(int) buf[0],(int) buf[1],(int) buf[2]); */
		return (  ( ( buf[0] << 16 ) & 0xFF0000 )
			| ( ( buf[1] << 8 )  & 0xFF00 )
			| ( ( buf[2] ) & 0xFF ) );
	}
}

void
FBprocessmouse( FBMOUSEEVENT *ev )
{
	int	mousecode = FBmouseget();

	ev->buttons = ( mousecode >> 16 ) & 0x07;
	ev->x = (int)( (char) (( mousecode >> 8 ) & 0xFF ));
	ev->y = (int)( - (char) ( mousecode & 0xFF ));
	ev->state = 0;
}

void
FBmouseclose( void )
{
	if ( --calls == 0 )
	{
		return;
	}

	if ( close( msefd ) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBmouseclose: Error closing /dev/mouse" );
	}
}
