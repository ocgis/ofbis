#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include "fbvt.h"
#include "fballoc.h"
#include "fberror.h"
#include "fbkbd.h"
#include "fbmmap.h"

typedef	int	FBListKeyType;
typedef FB	*FBListValType;

#include "fblist.h"

static	char		*ttynames[]={"/dev/tty0", "/dev/console", NULL};
static	char		*ttyfmts[]={"/dev/tty%d", "/dev/tty%02x", "/dev/tty%x", "/dev/tty%02d", NULL};
static	int		cttyname = 0;
static	int		originaltty;
static	struct vt_mode	vtm;
/*static*/ 	unsigned short 	switching;		/* Flag to prevent reentrancy */

/*static*/	void FBVTswitch(int s);

static	FBList		ttylist;
static	unsigned short	listinit = FALSE;

void
FBVTopen(FB *f)
{
	char		ttynam[11];
	int		i = 0;
	struct vt_stat	vts;

	/* Open current console */

	while ( ( f->tty = open( ttynames[cttyname], O_WRONLY )) == -1 )
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

	if ( f->vtchoice == FB_KEEP_CURRENT_VC )
	{
		/* Get current VT number so we can switch back to it later */

		if ( ioctl( f->tty, VT_GETSTATE, &vts ) == -1 )
		{
			FBerror( FATAL | SYSERR, "FBVTopen: couldn't get VT state");
		}
		f->ttyno = vts.v_active;
	}
	else if ( f->vtchoice == FB_OPEN_NEW_VC )
	{
		/* Get number of free VT */

		if ( ioctl( f->tty, VT_OPENQRY, &f->ttyno ) == -1 )
		{
			FBerror( FATAL | SYSERR, "FBVTopen: no free ttys");
		}
	}
	else
	{
		FBerror( FATAL | SYSERR, "FBVTopen: you are very lame - pass correct option!" );
	}

	/* Close current console */

	if ( close( f->tty ) == -1 )
	{
		FBerror( WARNING | SYSERR, "FBVTopen: failed to close %s",
			 ttynames[cttyname]);
	}

	/* Open new VT */

	do
	{
		(void) sprintf( ttynam, ttyfmts[i++], f->ttyno );
	}
	while ( ( ttyfmts[i] != NULL ) &&
		( f->tty = open( ttynam, O_RDONLY )) == -1 );

	if ( f->tty == -1 )
	{
		FBerror( FATAL | SYSERR, "FBVTopen: failed to open %s", ttynam);
	}

	/* junk from Xserver... clean up later */

	/*if (!f->keeptty)
	{
		setpgrp();
	}
	chown(ttynam,getuid(),getgid());
	chown("/dev/console",getuid(),getgid());
	chown("/dev/tty0",getuid(),getgid());*/
	{
	int i;

	if (!f->keeptty)
	{
		if ((i=open("/dev/tty",O_RDWR))>=0)
		{
			ioctl(i,TIOCNOTTY,0);
			close(i);
		}
	/*setsid();*/
	/*ioctl(f->tty,TIOCSCTTY);*/
	}
	}

	/* Get current VT number so we can switch back to it later */

	if ( ioctl( f->tty, VT_GETSTATE, &vts ) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBVTopen: couldn't get VT state");
	}
	originaltty = vts.v_active;

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
	f->visible=TRUE;
	f->drawing=FALSE;

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

	/* Initialise kbd handling */

	FBkbdopen( f );
}

FB *
FBfindFB( void )
{
	struct vt_stat	vts;
	int		curcon;
	FB		*f;

	/* Open current console */

	if ( ( curcon = open( ttynames[cttyname], O_RDONLY )) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBVTswitch: open failed on %s",
			 ttynames[cttyname]);
	}

	/* Get current VT number so we can find which FB struct to use */

	if ( ioctl( curcon, VT_GETSTATE, &vts ) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBVTswitch: couldn't get VT state");
	}

	f = FBListFindKey( ttylist, vts.v_active );

	/* Close current console */

	if ( close( curcon ) == -1 )
	{
		FBerror( WARNING | SYSERR, "FBVTswitch: failed to close %s",
			 ttynames[cttyname]);
	}
	return f;
}

void
FBVTswitch(int s)
{
	static	FB 		*f;		/* Non reentrant */
	/*static 	unsigned short 	switching;*/	/* Flag to prevent reentrancy */

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
		/* Switching out, allocated new backing store */

		f->sbak = (unsigned short *) FBalloc( f->finf.smem_len );

		/* Copy framebuffer to backing store */

		(void) memcpy( f->sbak, f->sbuf, f->finf.smem_len );

		/* Unmap current framebuffer */

		FBunmap(f);

		/* Make backing store current so it can be written to */

		f->sbuf=f->sbak;
		f->visible=FALSE;

		/* Release console for switch */

		if ( ioctl(f->tty, VT_RELDISP, 1) == -1 )
		{
			FBerror( FATAL | SYSERR, "FBVTswitch: switch away from VT %d denied", f->tty);
		}
	}
	else
	{
		/* Acknowledge switch back to this VT */

		if ( ioctl(f->tty, VT_RELDISP, VT_ACKACQ) == -1 )
		{
			FBerror( FATAL | SYSERR, "FBVTswitch: switch to VT %d denied", f->tty);
		}

		/* Map framebuffer back into memory */

		FBmap(f);

		/* Copy backing store to framebuffer */

		(void) memcpy( f->sbuf, f->sbak, f->finf.smem_len );

		/* Free backing store */

		FBfree(f->sbak);
		f->sbak=NULL;
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
	/* Close keyboard */

	FBkbdclose( f );

	/* If backing store allocated, free it */

	if (f->sbak)
	{
		FBfree(f->sbak);
	}

	/* Enable cursor */

	if ( ioctl( f->tty, KDSETMODE, KD_TEXT ) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBVTclose: Couldn't set keyboard graphics mode on VT %d", f->ttyno);
	}

	/* Restore mode parameters */

	vtm.mode = VT_AUTO;

	/* Set new mode parameters */

	if ( ioctl( f->tty, VT_SETMODE, &vtm ) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBVTclose: Couldn't set mode of VT %d", f->ttyno);
	}

	/* Switch back to original VT */

	if ( ioctl( f->tty, VT_ACTIVATE, originaltty ) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBVTclose: couldn't switch to VT %d", originaltty);
	}

	/* Close VT */

	if ( close( f->tty ) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBVTclose: failed to close VT");
	}

	/* Remove from ttylist */

	ttylist = FBListRemoveKey( ttylist, f->ttyno );
}
