#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "libfb.h"
#include "fballoc.h"
#include "fberror.h"
#include "fbmmap.h"
#include "fbmouse.h"
#include "fbchar.h"

FB *
FBopen( const char *fbname, unsigned short opts )
{
  /* Allocate new FB handle */
  
  FB	*f=(FB *)FBalloc(sizeof(FB));
  
  /* Set open options */
  
  f->vtchoice = opts & 0x01;
  f->handle_kbd = (opts & FB_NO_KBD) == 0;
  f->keeptty = FALSE;
  
  /* Open framebuffer fbname */
  
  if ( !fbname ) {
    /* Try to set name from the environment */
    
    fbname = getenv("FRAMEBUFFER");
    if ( !fbname ) {
      fbname = "/dev/fb0current";	
    }
  }

  if ( (f->fb = open( fbname, O_RDONLY/*WR*/ )) == -1 ) {
    FBerror( FATAL | SYSERR, "FBopen: open failed on %s", fbname );
  }
  
  /* Get screen info */

  FBgetfix(f);
  FBgetvar(f);
  
  /* Open VT */
  
  (void)close(f->fb);

  FBVTopen(f);
  
  /* Open mouse */
  
  FBmouseopen();
  
  /* Setup fontinfo */
  
  FBfontopen(f);
  
  /*f->fb=open(fbname,O_RDWR);*/
  if ( (f->fb = open( fbname, O_RDWR )) == -1 ) {
    FBerror( FATAL | SYSERR, "FBopen: open failed on %s", fbname );
  }

  FBgetfix(f);
  /*
    FBfinfdump(f);
    FBvinfdump(f);
    */
  
  /* Disable ywrap */
  
  f->vinf.xoffset = 0;
  f->vinf.yoffset = 0;
  f->vinf.vmode &= ~FB_VMODE_YWRAP;
  
  /* Update screen info */
  
  FBputvar(f);
  
  /* Map fb into memory */
  
  FBmap(f);
  f->sbak = NULL;

  return(f);
}

int
FBclose( FB *f ) {
  /* Unmap framebuffer from memory */
  
  FBunmap(f);
  
  /* Free fontinfo */
  
  FBfontclose(f);
  
  /* Close mouse */
  
  FBmouseclose();
  
  /* Close VT */
  
  FBVTclose(f);
  
  /* Close framebuffer device */
  
  if ( close ( f->fb ) == -1 ) {
    FBerror( WARNING | SYSERR, "FBclose: close fb failed" );
  }
  
  /* Free FB */
  
  FBfree(f);
  
  return(1);
}

