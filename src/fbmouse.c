#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "fbmouse.h"
#include "fberror.h"

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
	int size_to_read = (devtype==DEV_TYPE_GPMDATA?6:3);

	if ( read( msefd, &buf, size_to_read ) == -1 )
	{
		return;
	}

	if(devtype == DEV_TYPE_GPMDATA) {
	  buf[0] = ~buf[0];
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
