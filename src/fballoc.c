#include <stdlib.h>
#include "fballoc.h"
#include "fberror.h"

#ifdef FBMEMDEBUG

typedef	struct fb_memdebug
{
	void	*start;
	char	*file;
	int	line;
	unsigned long size;
} FBMD;

typedef	void	*FBListKeyType;
typedef FBMD	*FBListValType;

#include <stdio.h>
#include "fblist.h"
#undef	FBalloc

static	FBList		md;
static	unsigned short 	debuginit=0;

static
void
FBdebugreport( void )
{
	FBListNode	ln;
	FBMD		*fbmd;

	printf("libfb: Memory leaks:\n");
	if (FBListEmpty(md))
	{
		printf("libfb: No leaks.\n");
		return;
	}
	while (!FBListEmpty(md))
	{
		ln=FBListGetAnyNode(md);
		fbmd=ln->val;
		fprintf(stderr,"libfb: Address %p : %ld bytes at line %d of %s\n",fbmd->start,fbmd->size,fbmd->line,fbmd->file);
		md=FBListRemoveNode(md,ln->key);
		free(fbmd->start);
	}
}

void *
FBalloc( unsigned long size, const char *file, int line )
{
	void	*s;
	FBListNode	ln;
	FBMD		*fbmd;

	/*printf("FBalloc start: %ld bytes called by %s line %d\n",size,file,line);*/
	if (!debuginit)
	{
		debuginit=1;
		md=FBListInit(md);
		if ( atexit(FBdebugreport) != 0 )
		{
			FBerror( FATAL | SYSERR, "FBalloc (debug): atexit registration failed" );
		}
	}
	if ( ( s = malloc( sizeof(struct fb_lnde) + sizeof(FBMD) + sizeof(char) + size ) ) == NULL )
	{
		FBerror( FATAL | SYSERR, "FBalloc: couldn't get %ld bytes", size );
	}
	ln= (FBListNode) s;
	ln->key= (s + sizeof(struct fb_lnde) + sizeof(FBMD) );
	ln->val= fbmd = (FBMD *) (s + sizeof(struct fb_lnde) );
	ln->next=NULL;
	fbmd->start=s;
	fbmd->size=size;
	fbmd->file=(char *)file;
	fbmd->line=line;
	md=FBListInsertNode(md,ln);
	return ln->key;
}

void
FBfree( void *p )
{
	FBMD	*fbmd= FBListFindKey(md,p);

	md=FBListRemoveNode(md,p);
	(void) free(fbmd->start);
}
#else
void *
FBalloc( unsigned long size )
{
	void	*s;

	if ( ( s = malloc( size ) ) == NULL )
	{
		FBerror( FATAL | SYSERR, "FBalloc: couldn't get %ld bytes", size );
	}
	return (void *)s;
}

void
FBfree( void *p )
{
	(void) free(p);
}
#endif
