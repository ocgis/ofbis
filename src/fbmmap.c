#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include "ofbis.h"
#include "fberror.h"

void
FBmap(FB *f)
{
  off_t offset = 0;

  /* Some cards need a special mmap with offset */
  if(f->finf.accel == FB_ACCEL_SUN_CGSIX)
    offset = 0x70000000 + 0x16000;  

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




