/* FBevent - event handling for ofbis */

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <sys/poll.h>
#include "ofbis.h"
#include "error.h"
#include "event.h"
#include "kbd.h"
#include "mouse.h"


#define POLLMASK (POLLIN)

/*
** Description
** Check for keyboard or mouse event with possible timeout
** Timeout value in milliseconds, -1 means forever
**
** 1998-08-06 CG
** 1999-05-20 Tomas
*/
void
FBcheckevent (FB *f, FBEVENT *ev, int timeout) {
  int interrupted;
  struct pollfd in[2];
  int nfds=1, ret;

  in[0].fd = f->tty;
  in[0].events = POLLMASK;

  if ((f->sbuf != f->sbak) && (msefd != -1)) {
    in[1].fd = msefd;
    in[1].events = POLLMASK;
    tcflush( msefd, TCIFLUSH );
    nfds = 2;
  }

  do {
    interrupted = FALSE;

    ret = poll(in, nfds, timeout);
    if(ret == -1) {
      if(errno == EINTR) {
	interrupted = TRUE;
	
	if (msefd != -1) {
	  if (f->sbuf == f->sbak) {
	    nfds = 1;
	  } else {
	    in[1].fd = msefd;
	    in[1].events = POLLMASK;
	    tcflush( msefd, TCIFLUSH );
	    nfds = 2;
	  }
	}
      } else {
	/*
	fprintf(stderr,"ofbis: fbevent.c: tty fd=%d\n", in[0].fd);
	fprintf(stderr,"ofbis: fbevent.c: tty revents=0x%x\n",in[0].revents);
	fprintf(stderr,"ofbis: fbevent.c: mouse fd=%d\n", in[1].fd);
	fprintf(stderr,"ofbis: fbevent.c: mouse revents=0x%x\n",in[1].revents);
	*/
	FBerror( FATAL | SYSERR, "FBcheckevent: poll() returned" );
      }
    }
  } while(interrupted);

  if(ret == 0) { /* if poll() timed out */
    ev->type = FBNoEvent;
  } else if(in[0].revents & POLLIN) {
    ev->type = FBKeyEvent;
    FBprocesskey (f, &ev->key);
  } else if(in[1].revents & POLLIN) {
    ev->type = FBMouseEvent;
    FBprocessmouse (&ev->mouse);		
    /*		printf("buttons: %d x: %d y: %d\n",ev->mouse.buttons, ev->mouse.x, ev->mouse.y ); */
  }
}

/*
** Description
** Get one keyboard or mouse event
** This is the same as FBcheckevent, but with infinite timeout
**
** 1998-08-06 CG
** 1999-05-20 Tomas
*/
void
FBgetevent (FB *f, FBEVENT *ev) {
  FBcheckevent(f, ev, -1);
}

int
FBgetchar( FB *f )
{
  FBEVENT	ev;
  
  do {
    FBgetevent( f, &ev );
  } while ((ev.type != FBKeyEvent) ||
	   ((ev.key.keycode & 0x80)==0x80));

  return((int)ev.key.ascii);
}
