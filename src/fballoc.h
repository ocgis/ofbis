#ifndef _FBALLOC_H_
#define _FBALLOC_H_

/*
#define	FBMEMDEBUG
*/

#ifdef FBMEMDEBUG
void	*FBalloc( unsigned long size, const char *file, int line );
#define	FBalloc(s)	FBalloc( s, __FILE__, __LINE__ )
#else
void	*FBalloc( unsigned long size );
#endif
void	FBfree( void *p );

#endif
