#ifndef _ALLOC_H_
#define _ALLOC_H_

/*
#define	FBMEMDEBUG
*/

#ifdef FBMEMDEBUG
void	*FBalloc( size_t size, const char *file, int line );
#define	FBalloc(s)	FBalloc( s, __FILE__, __LINE__ )
#else
void	*FBalloc( size_t size );
#endif
void	FBfree( void *p );

#endif
