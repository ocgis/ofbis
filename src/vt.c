#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include "vt.h"
#include "alloc.h"
#include "error.h"
#include "kbd.h"
#include "mmap.h"

typedef int     FBListKeyType;
typedef FB      *FBListValType;

#include "list.h"

static  char            *ttynames[]={"/dev/vc/0", "/dev/tty0", "/dev/tty", NULL};
static  char            *ttyfmts[]={"/dev/vc/%d", "/dev/tty%d", "/dev/tty%02x", "/dev/tty%x", "/dev/tty%02d", NULL};
static  int             cttyname = 0;
static  int             ctty;
static  int             originaltty;
static  int             kd_mode;
static  struct vt_mode  vt_mode;
/*static*/      unsigned short  switching;              /* Flag to prevent reentrancy */

/*static*/      void FBVTswitch(int s);
static void FBVTdetach(FB *f);

static  FBList          ttylist;
static  unsigned short  listinit = FALSE;

void
FBVTopen(FB *f)
{
  char            ttynam[11];
  int             i = 0;
  struct vt_stat  vts;
  struct vt_mode  vtm;

  /* Open current console */

  while ( ( ctty = open( ttynames[cttyname], O_RDWR ) ) == -1 )
  {
    FBerror( WARNING | SYSERR, "FBVTopen: open failed on %s",
             ttynames[cttyname]);

    if ( ttynames[++cttyname] == NULL )
    {
      FBerror( FATAL, "FBVTopen: failed to open a tty");
    }
    else
    {
      FBerror( WARNING, "FBVTopen: trying %s", ttynames[cttyname]);
    }
  }

  /* Get current VT number so we can switch back to it later */

  if ( ioctl( ctty, VT_GETSTATE, &vts ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBVTopen: couldn't get VT state");
  }
  originaltty = vts.v_active;

  if ( f->vtchoice == FB_KEEP_CURRENT_VC )
  {
    f->tty = 0;
    f->ttyno = originaltty;
    f->keeptty = TRUE;

    if ( close( ctty ) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTopen: failed to close controlling VT");
    }
 }
  else if ( f->vtchoice == FB_OPEN_NEW_VC )
  {
    /* Get number of free VT */

    if ( ioctl( ctty, VT_OPENQRY, &f->ttyno) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTopen: no free ttys");
    }
    f->keeptty = FALSE;

    /* Find an accessible VT */

    while ( ttyfmts[i] != NULL )
    {
      (void) sprintf( ttynam, ttyfmts[i++], f->ttyno );
      chown( ttynam, getuid(), getgid() );
      if ( access( ttynam, R_OK | W_OK ) != -1 )
      {
        break;
      }
    }

    if ( close( ctty ) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTopen: failed to close controlling VT");
    }

    /* Detach from our controlling terminal */

    FBVTdetach( f );

    /* Open new VT */

    close( 0 );
    close( 1 );
    close( 2 );

    f->tty = open( ttynam, O_RDWR );
    if ( f->tty == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTopen: failed to open %s", ttynam);
    }
    dup( 0 );
    dup( 0 );

    /* Switch to new VT */

    if ( ioctl( f->tty, VT_ACTIVATE, f->ttyno ) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTopen: couldn't switch to VT %d", f->ttyno);
    }

    /* Wait for new VT to become active */

    if ( ioctl( f->tty, VT_WAITACTIVE, f->ttyno ) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTopen: VT %d didn't become active", f->ttyno);
    }
  }
  else
  {
    FBerror( FATAL | SYSERR, "FBVTopen: please pass correct options!" );
  }

  /* Get mode of new VT */

  if ( ioctl( f->tty, VT_GETMODE, &vtm ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBVTopen: Couldn't get mode of VT %d", f->ttyno);
  }

  /* Adjust mode parameters */

  vtm.mode = VT_PROCESS;
  vtm.relsig = SIGUSR2;
  vtm.acqsig = SIGUSR2;

  /* Set signal handler for VT switches */

  (void) signal( SIGUSR2, FBVTswitch );

  /* Set new mode parameters */

  if ( ioctl( f->tty, VT_SETMODE, &vtm ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBVTopen: Couldn't set mode of VT %d", f->ttyno);
  }

  /* Save current keybard mode */

  if ( ioctl( f->tty, KDGETMODE, &kd_mode ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBVTopen: Couldn't get keyboard mode on VT %d", f->ttyno);
  }

  /* Disable cursor */

  if ( ioctl( f->tty, KDSETMODE, KD_GRAPHICS ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBVTopen: Couldn't set keyboard graphics mode on VT %d", f->ttyno);
  }

  /* Initialise tty list if not already done */

  if ( listinit == FALSE )
  {
    ttylist = FBListInit( ttylist );
  }

  /* Add new FB to tty list */

  ttylist = FBListAdd( ttylist, f->ttyno, f );

  if (f->handle_kbd) {
    /* Initialise kbd handling */

    FBkbdopen( f );
  }

  f->visible=TRUE;
  f->drawing=FALSE;

}

static void
FBVTdetach(FB *f)
{
#if 1
  int child;

  /* Fork to background */

  child = fork();

  if ( child < 0 )
  {
    FBerror( FATAL | SYSERR, "FBVTdetach: Failed to fork" );
  }
  else if ( child )
  {
    exit( 0 );
  }

  setsid();
#else
  int ctty;
  struct vt_stat vts;

  ctty = open( "/dev/tty", O_RDONLY );
  if ( ctty < 0 )
  {
    FBerror( FATAL | SYSERR, "FBVTdetach: Couldn't open /dev/tty");
  }

  if ( ioctl( ctty, VT_GETSTATE, &vts ) == -1 )
  {
    return;
  }

  /* Detach from our controlling terminal, and start a new session */

  if ( ioctl( ctty, TIOCNOTTY, 0) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBVTdetach: Couldn't detach from controlling terminal");
  }

  close( ctty );
#endif
}

FB *
FBfindFB( void )
{
  struct vt_stat  vts;
  int             curcon;
  FB              *f;

  /* Open current console */

  if ( ( curcon = open( ttynames[cttyname], O_RDONLY ) ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBfindFB: open failed on %s",
             ttynames[cttyname]);
  }

  /* Get current VT number so we can find which FB struct to use */

  /*  fprintf (stderr, "ofbis: FBfindFB: pid %d\n", getpid ()); */

  if ( ioctl( curcon, VT_GETSTATE, &vts ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBfindFB: couldn't get VT state");
  }

  f = FBListFindKey( ttylist, vts.v_active );

  /* Close current console */

  if ( close( curcon ) == -1 )
  {
    FBerror( WARNING | SYSERR, "FBfindFB: failed to close %s",
             ttynames[cttyname]);
  }

  /*  fprintf (stderr, "ofbis: FBfindFB: returning %p\n", f); */

  return f;
}

void
FBVTswitch(int s)
{
  static  FB              *f;             /* Non reentrant */
  /*static        unsigned short  switching;*/    /* Flag to prevent reentrancy */

  /*
  fprintf(stderr,"FBVTswitch: signalnum=%d\n",s);
  fprintf(stderr,"FBVTswitch: switching=%d\n",switching);
  */

  /* Set this handler again, otherwise signal reverts to default handling */

  (void) signal( SIGUSR2, FBVTswitch );

  /* If the switch has already been acknowledged, and the user tries to */
  /* switch again, ignore until the alarm signal is raised. */

  if ( (switching) && (s==SIGUSR2) )
  {
    return;
  }

  if ( switching == FALSE )
  {
    switching = TRUE;

    /* Find current FB struct */

    f = FBfindFB();
  }

  /* If drawing, a change in the screen buffer will cause problems: */
  /* delay switch and hope next time it isn't drawing. */

  if (f->drawing)
  {
    /*(void) signal( SIGALRM, FBVTswitch );
      (void) alarm(1);*/
    return;
  }

  if (f->visible)
  {
    if (f->use_backing) {
      /* Switching out, allocate new backing store */
      f->sbak = (unsigned short *) FBalloc (f->finf.smem_len);
      
      if (f->sbak == NULL) {
        FBerror( FATAL | SYSERR, "FBVTswitch: failed to allocate backing store");
      }
      
      /*
      fprintf (stderr, "ofbis: fbvt.c: FBVTswitch: 23 sbak=%p sbuf=%p smem_len=0x%lx (%ld)\n",
               f->sbak, f->sbuf, f->finf.smem_len, f->finf.smem_len);
      */

      /* Copy framebuffer to backing store */
      
      (void) memcpy( f->sbak, f->sbuf, f->finf.smem_len );
    }

    /* Unmap current framebuffer */

    FBunmap(f);

    /* Make backing store current so it can be written to */

    f->sbuf=f->sbak;
    f->visible=FALSE;

    /* Release console for switch */

    if ( ioctl(f->tty, VT_RELDISP, 1) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTswitch: switch away from VT %d denied", f->ttyno);
    }
  }
  else
  {
    /* Acknowledge switch back to this VT */
    if ( ioctl(f->tty, VT_RELDISP, VT_ACKACQ) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTswitch: switch to VT %d denied", f->ttyno);
    }

    /* Map framebuffer back into memory */
    FBmap(f);

    if (f->use_backing) {
      /* Copy backing store to framebuffer */
      (void) memcpy( f->sbuf, f->sbak, f->finf.smem_len );
      
      /* Free backing store */
      /*      fprintf (stderr, "ofbis: FBclose: calling FBfree with sbak = %p\n", f->sbak); */
      
      FBfree (f->sbak);
      /*      fprintf (stderr, "ofbis: FBclose: called FBfree with sbak = %p\n", f->sbak); */
      f->sbak=NULL;
    }

    /* Set the palette as it was when switching away. */
    FBputcmap(f, f->cmap);

    f->visible=TRUE;
  }

  switching=FALSE;
}

void
FBVTswitchoriginal(FB *f)
{
  if ( ioctl( f->tty, VT_ACTIVATE, originaltty ) == -1 )
  {
    FBerror( WARNING | SYSERR, "FBVTswitchoriginal: VT switch failed" );
  }
}

void
FBVTswitchfb(FB *f)
{
  if ( ioctl( f->tty, VT_ACTIVATE, f->ttyno ) == -1 )
  {
    FBerror( WARNING | SYSERR, "FBVTswitchfb: VT switch failed" );
  }
}

void
FBVTclose(FB *f)
{
  /* Enable cursor */

  if ( ioctl( f->tty, KDSETMODE, kd_mode ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBVTclose: Couldn't restore keyboard mode on VT %d", f->ttyno);
  }

  /* Restore mode parameters */

  if ( ioctl( f->tty, VT_SETMODE, &vt_mode ) == -1 )
  {
    FBerror( FATAL | SYSERR, "FBVTclose: Couldn't restore mode of VT %d", f->ttyno);
  }

  if (f->handle_kbd) {
    /* Close keyboard */

    FBkbdclose( f );
  }

  /* If backing store allocated, free it */

  if (f->sbak)
  {
    FBfree(f->sbak);
  }

  if ( !f->keeptty )
  {
    /* Switch back to original VT */

    if ( ioctl( f->tty, VT_ACTIVATE, originaltty ) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTclose: couldn't switch to VT %d", originaltty);
    }
    if ( ioctl( f->tty, VT_WAITACTIVE, originaltty ) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTclose: couldn't wait for VT %d", originaltty);
    }

    /* Close VT */

    if ( close( f->tty ) == -1 )
    {
      FBerror( FATAL | SYSERR, "FBVTclose: failed to close VT");
    }
  }

  /* Remove from ttylist */

  ttylist = FBListRemoveKey( ttylist, f->ttyno );
}

