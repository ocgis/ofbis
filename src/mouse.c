#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h> /* for errno variable */
#include <string.h> /* for strerror */
#include "mouse.h"
#include "error.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#define GPMDATADEVICE "/dev/gpmdata"
#define MOUSEDEVICE "/dev/mouse"
#define DEV_TYPE_NONE 0
#define DEV_TYPE_GPMDATA 1
#define DEV_TYPE_MOUSE 2

int	msefd;
static int calls=0, devtype=0;

/*
** Description
** Open the mouse device.
**
** 1998-08-06 CG
** 1999-05-20 Tomas
*/
void
FBmouseopen( void )
{
  if ( ++calls > 1 ) {
    return;
  }

  devtype = DEV_TYPE_GPMDATA;
  if ( ( msefd = open ( GPMDATADEVICE, O_RDWR | O_NDELAY )) == -1 ) {
    devtype = DEV_TYPE_MOUSE;
    fprintf (stderr, "FBmouseopen: Error opening /dev/gpmdata, trying /dev/mouse\n");
    if ( ( msefd = open ( MOUSEDEVICE, O_RDWR | O_NDELAY )) == -1 ) {
      devtype = DEV_TYPE_NONE;
      fprintf (stderr, "FBmouseopen: Error opening /dev/mouse\n");
      return;
    }
  }
  /* Flush input */
  tcflush( msefd, TCIFLUSH );
}


void
FBprocessmouse( FBMOUSEEVENT *ev )
{
	char	buf[6];
	int n, bytes_read = 0, size_to_read = (devtype==DEV_TYPE_GPMDATA?6:3);
	
	while ( size_to_read > 0 )
	{
		n = read( msefd, &buf[bytes_read], size_to_read );
		if ( n == -1 )
		{
			if ( errno == EAGAIN )
			{
				/* No data was immediately available for reading. */
				/* We'll keep trying. */
			}
			else
			{
				FBerror( WARNING, strerror( errno ) );
				return;
			}
		}
		else if ( n == 0 )
		{
			FBerror( FATAL | SYSERR, "End-of-file occurred while reading from mouse device" );
			return;
		}
		else
		{
			bytes_read += n;
			size_to_read -= n;
		}
	}

	if(devtype == DEV_TYPE_GPMDATA) {
	  buf[0] = buf[0];
	  buf[1] = buf[3]*2;
	  buf[2] = buf[4]*2;
	}
	
	/*	printf("%02x %02x %02x\n",buf[0]&0x07,buf[1]&0xff,buf[2]&0xff); */

	ev->buttons = buf[0] & 0x07;
	ev->x = (int)(   (signed char) ( buf[1] & 0xFF ));
	ev->y = (int)( - (signed char) ( buf[2] & 0xFF ));
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
		FBerror( FATAL | SYSERR, "FBmouseclose: Error closing mouse device" );
	}
}
