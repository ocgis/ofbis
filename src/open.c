#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ofbis.h"
#include "alloc.h"
#include "error.h"
#include "mmap.h"
#include "mouse.h"
#include "char.h"

FB *
FBopen( const char *fbname, unsigned short opts )
{
  /* Allocate new FB handle */
  
  FB	*f=(FB *)FBalloc(sizeof(FB));
  
  /* Set open options */
  
  f->vtchoice = opts & 0x01;
  f->handle_kbd = (opts & FB_NO_KBD) == 0;
  f->use_backing = (opts & FB_NO_BACKING_STORE) == 0;
  
  /* Open framebuffer fbname */
  
  if ( !fbname ) {
    /* Try to set name from the environment */
    
    fbname = getenv("FRAMEBUFFER");
    if ( !fbname ) {
      if ( access("/dev/.devfsd", F_OK) == 0 )  /* devfs detected */
	fbname = "/dev/fb/0";
      else
	fbname = "/dev/fb0";
    }
  }

  if ( (f->fb = open( fbname, O_RDONLY/*WR*/ | O_NONBLOCK )) == -1 ) {
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

  f->cmap = NULL;
  
  FBputvar(f);
  
  /* Map fb into memory */
  
  FBmap(f); 
  f->sbak = NULL;

  /*
  fprintf(stderr, "f->sbuf: %p\n", f->sbuf);
  fprintf(stderr, "f->finf.smem_start: %p\n", f->finf.smem_start);
  fprintf(stderr, "f->finf.smem_len: %d\n", f->finf.smem_len);
  */

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
  
  /* Free the internal palette. */
  if(f->cmap) {
    FBfree(f->cmap->red);
    FBfree(f->cmap->green);
    FBfree(f->cmap->blue);
    FBfree(f->cmap);
  }
    
  /* Free FB */

  FBfree(f);
  
  return(1);
}

