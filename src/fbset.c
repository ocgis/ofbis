#include <sys/ioctl.h>
#include "ofbis.h"
#include "fberror.h"
#include "fbfuncs.h"

/*
** void FBgetfix( FB *f )
**
** Get fixed screen info into f->finf
*/
void
FBgetfix( FB *f )
{
	if (ioctl(f->fb,FBIOGET_FSCREENINFO, &f->finf) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBgetfix: Get fixed screen settings failed %d",f->fb );
	}
}

/*
** void FBgetvar( FB *f )
**
** Get variable screen info into f->var
*/
void
FBgetvar( FB *f )
{
	if (ioctl(f->fb,FBIOGET_VSCREENINFO, &f->vinf) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBgetvar: Get variable screen settings failed" );
	}
	FBsetfuncs(f);
}

/*
** void FBputvar( FB *f )
**
** Put variable screen info into framebuffer and update f->var
*/
void
FBputvar( FB *f )
{
	if (ioctl(f->fb,FBIOPUT_VSCREENINFO, &f->vinf) == -1 )
	{
	  /* Don't quit, because the sparc crashes then. */
	  /*		FBerror( FATAL | SYSERR, "FBputvar: Put variable screen settings failed" ); */
	  	FBerror( SYSERR, "FBputvar: Put variable screen settings failed" );
	}

	if (ioctl(f->fb,FBIOGET_FSCREENINFO, &f->finf) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBputvar: Get fixed screen settings failed" );
	}

	if (ioctl(f->fb,FBIOGET_VSCREENINFO, &f->vinf) == -1 )
	{
		FBerror( FATAL | SYSERR, "FBputvar: Update variable screen settings failed" );
	}
	FBsetfuncs(f);
}

/*
** Description
** Set writemode for graphical operations
**
** 1998-12-26 CG
*/
void
FBsetwritemode (FB *        f,
                FBWRITEMODE mode) {
  f->writemode = mode;
}
