#include <stdio.h>	/* for printf */
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include "fberror.h"
#include "fbkbd.h"
#include "fbkeys.h"
#include "fbkeytbl.h"

static	int		oldkbdmode;
static	struct termios	oldttysets;

void
FBkbdopen( FB *f )
{
	struct termios	nttysets;

	/* Initialise: save old kbd state and tty settings */

	ioctl( f->tty, KDGKBMODE, &oldkbdmode );
	tcgetattr ( f->tty, &oldttysets );

	/* Install new kbd mode and tty settings */

	ioctl( f->tty, KDSKBMODE, K_MEDIUMRAW );	/* Change to full K_RAW someday */
	nttysets = oldttysets;
	nttysets.c_iflag = ( IGNPAR | IGNBRK ) & ( ~PARMRK ) & ( ~ISTRIP );
	nttysets.c_oflag = 0;
	nttysets.c_cflag = CREAD | CS8;
	nttysets.c_lflag = 0;
	nttysets.c_cc[VTIME] = 0;
	nttysets.c_cc[VMIN] = 1;
	cfsetispeed( &nttysets, B9600 );
	cfsetospeed( &nttysets, B9600 );
	tcsetattr( f->tty, TCSANOW, &nttysets );
}

char
FBkbdgetkey( FB *f )
{
	char	buf;

	if ( read( f->tty, &buf, sizeof(char) ) == -1 )
	{
		return -1;
	}
	else
	{
		return buf;
	}
}

#define	SETORCLEAR(x)		\
	if (pressed)		\
	{			\
		state |= (x);	\
	}			\
	else			\
	{			\
		state &= ~(x);	\
	}

void
FBprocesskey( FB *f, FBKEYEVENT *ev )
{
	unsigned int	key = FBkbdgetkey( f );
	int		scancode = ( 0x7f & key );
	int		pressed = ( ( 0x80 & key ) ? FALSE : TRUE );
	static int	state = 0;

	/*printf("Got scancode %d %s\n",scancode, pressed?"pressed":"released");
	printf("State before is %x\n", state);*/
	switch (scancode)
	{
	case KEY_ShiftR:	SETORCLEAR(Mode_RShift)	break;
	case KEY_ShiftL:	SETORCLEAR(Mode_LShift)	break;
	case KEY_LCtrl:		SETORCLEAR(Mode_Ctrl)	break;
	case KEY_Alt:		SETORCLEAR(Mode_Alt)	break;
	case KEY_CapsLock:	if (!pressed)		break;
				if (!ModifierDown(Mode_Caps) )
				{
					state |= Mode_Caps;
				}
				else
				{
					state &= ~Mode_Caps;
				}
				break;
	case KEY_Insert:	SETORCLEAR(Mode_Insert)	break;
	case KEY_ClrHome:	SETORCLEAR(Mode_Home)	break;
	default:
	}
	ev->keycode = key;
	ev->state = state;
	if ( ModifierDown(Mode_Caps) )
	{
		ev->ascii = capslock[scancode];
	}
	else if ( (!ModifierDown(Mode_RShift)) && (!ModifierDown(Mode_LShift)) )
	{
		ev->ascii = unshifted[scancode];
	}
	else if ( (ModifierDown(Mode_RShift)) || (ModifierDown(Mode_LShift)) )
	{
		ev->ascii = shifted[scancode];
	}
	else
	{
		FBerror( FATAL, "Unknown keyboard state!");
	}

	/* Search for special keys now */

	/* Kill libfb */

	if ( ModifierDown(Mode_Ctrl) && ModifierDown(Mode_RShift)  &&
		( scancode == KEY_KP_Asterisk ) )
	{
		FBerror( FATAL, "FB kill key pressed." );
	}

	/* Console switching */

	if ( (ModifierDown(Mode_Alt) && ModifierDown(Mode_Ctrl)) &&
		( ( scancode>=KEY_F1 ) && ( scancode <= KEY_F10 ) ) )
	{
		int	vt = scancode - KEY_F1 + 1;

		if ( ModifierDown(Mode_RShift) || ModifierDown(Mode_LShift) )
		{
			vt += 10;
		}
		ev->state = 0xFFFFFFFF;
		if ( ioctl( f->tty, VT_ACTIVATE, vt ) == -1 )
		{
			FBerror( WARNING | SYSERR, "FBprocesskey: VT switch failed" );
		}
	}
	/*printf("State after is %x\n", state); 
	printf("Got ascii %c %x\n", ev->ascii, (int) ev->ascii);*/
}

void
FBkbdclose( FB *f )
{
	/* Restore old kbd state and tty settings */

	ioctl( f->tty, KDSKBMODE, oldkbdmode );
	tcsetattr( f->tty, TCSANOW, &oldttysets );
}

