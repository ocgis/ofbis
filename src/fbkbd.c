#include <errno.h>
#include <stdio.h>      /* for printf */
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/keyboard.h>
#include "fberror.h"
#include "fbkbd.h"
#include "fbkeys.h"
#include "fbkeytbl.h"

static  int             oldkbdmode;
static  struct termios  oldttysets;

static int keymap_index[MAX_NR_KEYMAPS];
static int number_of_keymaps;

static
void
get_keymaps (int fd) {
  int i, j;
  struct kbentry ke;

  for (i = 0; i < MAX_NR_KEYMAPS; i++) {
    ke.kb_index = 0;
    ke.kb_table = i;
    j = ioctl(fd, KDGKBENT, (unsigned long)&ke);
    if (j && errno != EINVAL) {
      fprintf(stderr, "KDGKBENT at index 0 in table %d: ", i);
      perror("");
      exit(1);
    }
    
    if (!j && (ke.kb_value != K_NOSUCHMAP)) {
      keymap_index[number_of_keymaps++] = i;
    }
  }

  fprintf (stderr,
           "ofbis: fbkbd.c: get_keymaps: number_of_keymaps=%d\n",
           number_of_keymaps);
}


void
FBkbdopen( FB *f )
{
  struct termios  nttysets;

  /* Build keyboard translation tables */
  get_keymaps (f->tty);

  /* Initialise: save old kbd state and tty settings */

  ioctl( f->tty, KDGKBMODE, &oldkbdmode );
  tcgetattr ( f->tty, &oldttysets );

  /* Install new kbd mode and tty settings */

  ioctl( f->tty, KDSKBMODE, K_MEDIUMRAW );        /* Change to full K_RAW someday */
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
  char    buf;

  if ( read( f->tty, &buf, sizeof(char) ) == -1 )
  {
    return -1;
  }
  else
  {
    return buf;
  }
}

#define SETORCLEAR(x)           \
        if (pressed)            \
        {                       \
                state |= (x);   \
        }                       \
        else                    \
        {                       \
                state &= ~(x);  \
        }


/*
** Description
** Translate raw scancode into keycode
**
** 1999-03-10 CG
*/
static
int
translate_key (int  fd,
               char table,
               char index) {
  struct kbentry ke;

  ke.kb_index = index;
  ke.kb_table = table;
  if (ioctl(fd, KDGKBENT, (unsigned long)&ke)) {
    fprintf(stderr, "KDGKBENT at index %d in table %d: ",
            index, table);
    perror("");
    exit(1);
  }
  return ke.kb_value;
}


/*
** Description
** Process a keypress
**
** 1999-03-10 CG
** 1999-04-05 CG
*/
void
FBprocesskey( FB *f, FBKEYEVENT *ev )
{
  unsigned int    key = FBkbdgetkey( f );
  int             keycode;
  int             scancode = ( 0x7f & key );
  int             pressed = ( ( 0x80 & key ) ? FALSE : TRUE );
  static int      state = 0;
  int             table;

  /*
  printf("Got scancode %d %s\n",scancode, pressed?"pressed":"released");
  printf("State before is %x\n", state);
  printf("Key is 0x%04x\n", key);
  */

  /* Choose the correct table */
  if (ModifierDown (Mode_RShift) || ModifierDown (Mode_LShift)) {
    if (ModifierDown (Mode_Alt)) {
      table = K_ALTSHIFTTAB;
    } else {
      table = K_SHIFTTAB;
    }
  } else if (ModifierDown (Mode_Alt)) {
    /* With K_ALTTAB, vt switching doesn't work */
    table = K_NORMTAB; /*K_ALTTAB;*/ 
  } else if (ModifierDown (Mode_Caps)) {
    table = K_SHIFTTAB; /* Where is the capsmap? */
  } else {
    table = K_NORMTAB;
  }

  /*
  fprintf (stderr, "ofbis: fbkbd.c: FBprocesskey: pid %d\n", getpid ());
  */

  keycode = translate_key (f->tty, table, scancode);

  /*
  fprintf (stderr, "ofbis: FBprocesskey: keycode=0x%x 0x%x\n", keycode, K_ALT);
  */
  switch (keycode)
  {
  case K_SHIFTR :
    SETORCLEAR(Mode_RShift);
    break;
  case K_SHIFT  :
  case K_SHIFTL :
    SETORCLEAR(Mode_LShift);
    break;
  case K_CTRL:
  case K_CTRLL:
  case K_CTRLR:
    SETORCLEAR(Mode_Ctrl);
    break;
  case K_ALT   :
  case K_ALTGR :
    SETORCLEAR(Mode_Alt);
    break;
  case K_CAPS:
    if (!pressed) {
      break;
    }

    if (!ModifierDown(Mode_Caps) )
    {
      state |= Mode_Caps;
    }
    else
    {
      state &= ~Mode_Caps;
    }
    break;
  case K_INSERT:
    SETORCLEAR(Mode_Insert);
    break;
    /*
      What is the linux version of "home" called?
      case KEY_ClrHome:
      SETORCLEAR(Mode_Home);
      */
    break;
  default:
  }
  ev->keycode = key;
  ev->state = state;

  if (KTYP (keycode) == KT_SPEC) {
    switch (keycode) {
    case K_ENTER :
      ev->ascii = 10;
      break;
    default :
      ev->ascii = KVAL (keycode);
    }
  } else {
    ev->ascii = KVAL (keycode);
  }

  /* Search for special keys now */

  /* Kill ofbis */

  if (ModifierDown (Mode_Ctrl) && ModifierDown (Mode_RShift)  &&
      (keycode == K_PSTAR))
  {
    FBerror( FATAL, "FB kill key pressed." );
  }

  /* Console switching */

  /*
  fprintf (stderr, 
	   "ofbis: fbkbd.c: FBProcesskey: state=0x%x\n"
	   "                              ascii=0x%x\n"
	   "                              keycode=0x%x\n", 
	   ev->state, ev->ascii, ev->keycode);
	   */

  if ((ModifierDown (Mode_Alt) && ModifierDown (Mode_Ctrl)) &&
       ((keycode>=K_F1 ) && (keycode <= K_F10)))
  {
    int     vt = keycode - K_F1 + 1;

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
  /* 
     printf("State after is %x\n", state); 
     printf("Got ascii %c %x\n", ev->ascii, (int) ev->ascii); 
     */
}

void
FBkbdclose( FB *f )
{
  /* Restore old kbd state and tty settings */

  ioctl( f->tty, KDSKBMODE, oldkbdmode );
  tcsetattr( f->tty, TCSANOW, &oldttysets );
}

