#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>  /* for errno variable */
#include <string.h> /* for strerror */
#include "mouse.h"
#include "error.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#define GPMDATADEVICE    "/dev/gpmdata"
#define IMDEVICE         "/dev/intellimouse"
#define MOUSEDEVICE      "/dev/mouse"

typedef enum
{
  DEV_TYPE_NONE,
  DEV_TYPE_GPMDATA,
  DEV_TYPE_IM,
  DEV_TYPE_MOUSE
} devType;

int            msefd;
static int     calls = 0;
static devType devtype = DEV_TYPE_NONE;

/*
** Description
** Open the mouse device.
*/
void
FBmouseopen(void)
{
  if(calls == 0)
  {
    calls++;

    if((msefd = open(GPMDATADEVICE, O_RDONLY | O_NDELAY )) != -1)
    {
      devtype = DEV_TYPE_GPMDATA;
      
      /* Flush input */
      tcflush( msefd, TCIFLUSH );
    }
    else if((msefd = open(IMDEVICE, O_RDONLY | O_NDELAY )) != -1)
    {
      devtype = DEV_TYPE_IM;
      
      /* Flush input */
      tcflush(msefd, TCIFLUSH);
    }
    else if((msefd = open(MOUSEDEVICE, O_RDONLY | O_NDELAY )) != -1)
    {
      devtype = DEV_TYPE_MOUSE;
      
      /* Flush input */
      tcflush(msefd, TCIFLUSH);
    }
    else
    {
      devtype = DEV_TYPE_NONE;
      fprintf (stderr, "FBmouseopen: Error opening mouse\n");
    }
  }
}


void
FBprocessmouse( FBMOUSEEVENT *ev )
{
  char buf[6];
  int  n;
  int  bytes_read;
  int  size_to_read;
  
  bytes_read = 0;
  size_to_read = (devtype == DEV_TYPE_GPMDATA) ? 6 : 3;
 
  while(size_to_read > 0)
  {
    n = read(msefd, &buf[bytes_read], size_to_read);
    if(n == -1)
    {
      if(errno == EAGAIN)
      {
        /* No data was immediately available for reading.
           We'll keep trying. */
      }
      else
      {
        FBerror(WARNING, strerror(errno));
        return;
      }
    }
    else if (n == 0)
    {
      FBerror(FATAL | SYSERR,
              "End-of-file occurred while reading from mouse device");
      return;
    }
    else
    {
      if((devtype == DEV_TYPE_IM) && (bytes_read == 0))
      {
        int index;

        index = 0;

        while(index < n)
        {
          if((buf[index] & 0xc0) == 0xc0)
          {
            break;
          }
          index++;
        }

        if((index > 0) && (index < n))
        {
          memmove(buf, &buf[index], n - index);
        }

        bytes_read += n - index;
        size_to_read -= n - index;
      }
      else
      {
        bytes_read += n;
        size_to_read -= n;
      }
    }
  }
  
  if(devtype == DEV_TYPE_IM)
  {
    /* printf("%02x %02x %02x\n", buf[0], buf[1], buf[2]); */

    ev->buttons = (buf[0] >> 4) & 0x3;
    ev->x = (int)(signed char)(((buf[0] << 6) & 0xc0) | (buf[1] & 0x3f));
    ev->y = (int)(signed char)(((buf[0] << 4) & 0xc0) | (buf[2] & 0x3f));
    ev->state = 0;
  }
  else
  {
    if(devtype == DEV_TYPE_GPMDATA)
    {
      buf[0] = buf[0];
      buf[1] = buf[3] * 2;
      buf[2] = buf[4] * 2;
    }
    
    /*	printf("%02x %02x %02x\n",buf[0]&0x07,buf[1]&0xff,buf[2]&0xff); */
    
    ev->buttons = buf[0] & 0x07;
    ev->x = (int)(   (signed char) ( buf[1] & 0xFF ));
    ev->y = (int)( - (signed char) ( buf[2] & 0xFF ));
    ev->state = 0;
  }
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
