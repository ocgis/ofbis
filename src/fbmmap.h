#ifndef __FBMMAP_H_
#define __FBMMAP_H_

#include <unistd.h>
#include <sys/mman.h>
#include "ofbis.h"

inline static void
FBmap(FB *f)
{
	/* Map fb into memory */

	if ( (f->sbuf=(unsigned short *)mmap((void *)0,f->finf.smem_len, 
					     PROT_READ | PROT_WRITE,
					     MAP_SHARED, f->fb, (off_t)0)) == (void *)-1 )
	{
		FBerror( FATAL | SYSERR, "FBopen: fb mmap failed" );
	}
}

inline static void
FBunmap(FB *f)
{
	/* Unmap framebuffer from memory */

	if ( ( f->visible ) && ( munmap ( (void *)f->sbuf, f->finf.smem_len ) == -1 ) )
	{
		FBerror( WARNING | SYSERR, "FBclose: munmap failed");
	}
}

#endif
