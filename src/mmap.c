#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include "error.h"
#include "mmap.h"
#include "ofbis.h"

void
FBmap(FB *f)
{
  off_t offset = 0;

  /* Older kernel headers doesn't define this */
  /* Some cards need a special mmap with offset */
#ifdef FB_ACCEL_SUN_CGSIX
  if(f->finf.accel == FB_ACCEL_SUN_CGSIX)
    offset = 0x70000000 + 0x16000;
#endif
#ifdef FB_ACCEL_SUN_CGTHREE
  if(f->finf.accel == FB_ACCEL_SUN_CGTHREE)
    offset = 0x04000000; /* Untested!! */
#endif
#ifdef FB_ACCEL_SUN_TCX
  if(f->finf.accel == FB_ACCEL_SUN_TCX && 
     f->vinf.bits_per_pixel == 24)
    offset = 0x01000000; /* Untested!! */
#endif

	/* Map fb into memory */

	if ( (f->sbuf=(unsigned short *)mmap((void *)0,f->finf.smem_len, 
					     PROT_READ | PROT_WRITE,
					     MAP_SHARED, f->fb, offset)) == (void *)-1 )
	{
		FBerror( FATAL | SYSERR, "FBopen: fb mmap failed" );
	}
}

void
FBunmap(FB *f)
{
	/* Unmap framebuffer from memory */

	if ( ( f->visible ) && ( munmap ( (void *)f->sbuf, f->finf.smem_len ) == -1 ) )
	{
		FBerror( WARNING | SYSERR, "FBclose: munmap failed");
	}
}




