/* Must define FBListKeyType and FBListValType before including this file! */

#include "alloc.h"

typedef struct fb_lnde
{
	FBListKeyType	key;
	FBListValType	val;
	struct fb_lnde	*next;
} *FBListNode;

typedef FBListNode	*FBList;

static inline void
FBListDeleteNode( FBListNode n )
{
	FBfree( n );
}

static inline void
FBListDelete( FBList lst )
{
	FBListNode	n = (FBListNode) lst;
	FBListNode	i;

	while ( n != NULL )
	{
		i = n->next;
		FBListDeleteNode( n );
		n = i;
	}
}

static inline FBList
FBListInit( FBList lst )
{
	if ( lst != NULL )
		FBListDelete( lst );
	return NULL;
}

static inline int
FBListEmpty( FBList lst )
{
	return ( lst == NULL );
}

static inline FBListNode
FBListNewNode( FBListKeyType k, FBListValType v )
{
	FBListNode	n = (FBListNode) FBalloc( sizeof(struct fb_lnde) );

	n->key = k;
	n->val = v;
	n->next = NULL;
	return n;
}

static inline FBList
FBListInsertNode( FBList lst, FBListNode n )
{
	if ( lst != NULL )
	{
		n->next = (FBListNode) lst;
	}
	return (FBList) n;
}

static inline FBList
FBListAdd( FBList lst, FBListKeyType k, FBListValType v )
{
	return FBListInsertNode( lst, FBListNewNode( k, v ) );
}

static inline FBListNode
FBListGetAnyNode( FBList lst )
{
	return (FBListNode) lst;
}

static inline FBListValType
FBListFindKey( FBList lst, FBListKeyType k )
{
	FBListNode	n = (FBListNode) lst;

	while ( ( n != NULL ) && ( n->key != k ) )
	{
		n = n->next;
	}
	if ( n != NULL )
	{
		return n->val;
	}
	else
	{
		return (FBListValType) 0;
	}
}

static inline FBList
FBListRemoveKey( FBList lst, FBListKeyType k )
{
	FBListNode	n = (FBListNode) lst;
	FBListNode	p = n;

	if ( ( n->next == NULL ) && ( n->key == k ) )
	{
		FBListDeleteNode( p );
		return NULL;
	}
	else if ( n->key == k )
	{
		n=n->next;
		FBListDeleteNode( p );
		return (FBList) n;
	}
	else
	{
		while ( ( n->next != NULL ) && ( n->next->key != k ) )
		{
			n = n->next;
		}
		p = n->next;
		n->next = n->next->next;
		FBListDeleteNode( p );
		return lst;
	}
}

static inline FBList
FBListRemoveNode( FBList lst, FBListKeyType k )
{
	FBListNode	n = (FBListNode) lst;
	FBListNode	p = n;

	if ( ( n->next == NULL ) && ( n->key == k ) )
	{
		return NULL;
	}
	else if ( n->key == k )
	{
		return (FBList) n->next;
	}
	else
	{
		while ( ( n->next != NULL ) && ( n->next->key != k ) )
		{
			n = n->next;
		}
		p = n->next;
		n->next = n->next->next;
		return lst;
	}
}
