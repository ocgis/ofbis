#ifndef _FBEVENT_H_
#define _FBEVENT_H_

/* FBevent - event handling for libfb */

#include "libfb.h"

/*enum	eventtype
{
	FBKeyEvent,
	FBMouseEvent
};

typedef struct fb_keyevt
{
	enum eventtype	type;
	unsigned int	state;
	unsigned int	keycode;
	unsigned int	ascii;
} FBKEYEVENT;

typedef struct fb_mseevt
{
	enum eventtype	type;
	int		x;
	int		y;
	unsigned int	state;
	unsigned int	buttons;
} FBMOUSEEVENT;

typedef union fb_event
{
	enum eventtype	type;
	FBKEYEVENT	key;
	FBMOUSEEVENT	mouse;
} FBEVENT;

int	FBgetchar( FB *f );*/
#endif
