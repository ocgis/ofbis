#ifndef _BITBLT_H_
#define _BITBLT_H_

#define BITBLT_BACKWARD 0
#define BITBLT_FORWARD 1

#define SWAP(s)	s=0x0000 | (s<<16)
#define RSWAP(s) s=0x00000000 | (s>>16)
#define	WSWAP(s) ((s<<16)|(s>>16))
#define UPPER(s) (((s)<<16) & 0xFFFF0000)

#define EVENWORD(s) (((s)+15) >> 4)

/* just to reduce possible mistakes */
#define ALL_WHITE(s,d)	(0x00000000)
#define S_AND_D(s,d)	((s) & (d))
#define S_AND_NOTD(s,d)	((s) & ~(d))
#define S_ONLY(s,d)	(s)
#define NOTS_AND_D(s,d)	(~(s) & (d))
#define D_ONLY(s,d)		
#define S_XOR_D(s,d)	((s) ^ (d))
#define S_OR_D(s,d)	((s) | (d))
#define NOT_SORD(s,d)	(~((s) | (d)))
#define NOT_SXORD(s,d)	(~((s) ^ (d)))
#define NOT_D(s,d)	(~(d))
#define S_OR_NOTD(s,d)	((s) | ~(d))
#define NOT_S(s,d)	(~(s))
#define NOTS_OR_D(s,d)	(~(s) | (d))
#define NOT_SANDD(s,d)	(~((s) & (d)))
#define ALL_BLACK(s,d)	(0xffffffff)

 
#define	LOGICOP(d,s,logicop)			\
	switch(logicop)				\
	{					\
	case 0x0: d = ALL_WHITE(s,d);	break;	\
	case 0x1: d = S_AND_D(s,d);	break;	\
	case 0x2: d = S_AND_NOTD(s,d);	break;	\
	case 0x3: d = S_ONLY(s,d);	break;	\
	case 0x4: d = NOTS_AND_D(s,d);	break;	\
	case 0x5:     /* D_ONLY(s,d) */	break;	\
	case 0x6: d = S_XOR_D(s,d);	break;	\
	case 0x7: d = S_OR_D(s,d);	break;	\
	case 0x8: d = NOT_SORD(s,d);	break;	\
	case 0x9: d = NOT_SXORD(s,d);	break;	\
	case 0xA: d = NOT_D(s,d);	break;	\
	case 0xB: d = S_OR_NOTD(s,d);	break;	\
	case 0xC: d = NOT_S(s,d);	break;	\
	case 0xD: d = NOTS_OR_D(s,d);	break;	\
	case 0xE: d = NOT_SANDD(s,d);	break;	\
	case 0xF: d = ALL_BLACK(s,d);	break;	\
	}

#define	LOGICOPEND(d,s,logicop,endmask)						\
	switch(logicop)								\
	{									\
	case 0x0: d = (d & ~endmask) | (ALL_WHITE(s,d)	& endmask);	break;	\
	case 0x1: d = (d & ~endmask) | (S_AND_D(s,d)	& endmask);	break;	\
	case 0x2: d = (d & ~endmask) | (S_AND_NOTD(s,d)	& endmask);	break;	\
	case 0x3: d = (d & ~endmask) | (S_ONLY(s,d)	& endmask);	break;	\
	case 0x4: d = (d & ~endmask) | (NOTS_AND_D(s,d)	& endmask);	break;	\
	case 0x5:	/* D_ONLY(s,d) */				break;	\
	case 0x6: d = (d & ~endmask) | (S_XOR_D(s,d)	& endmask);	break;	\
	case 0x7: d = (d & ~endmask) | (S_OR_D(s,d)	& endmask);	break;	\
	case 0x8: d = (d & ~endmask) | (NOT_SORD(s,d)	& endmask);	break;	\
	case 0x9: d = (d & ~endmask) | (NOT_SXORD(s,d)	& endmask);	break;	\
	case 0xA: d = (d & ~endmask) | (NOT_D(s,d)	& endmask);	break;	\
	case 0xB: d = (d & ~endmask) | (S_OR_NOTD(s,d)	& endmask);	break;	\
	case 0xC: d = (d & ~endmask) | (NOT_S(s,d)	& endmask);	break;	\
	case 0xD: d = (d & ~endmask) | (NOTS_OR_D(s,d)	& endmask);	break;	\
	case 0xE: d = (d & ~endmask) | (NOT_SANDD(s,d)	& endmask);	break;	\
	case 0xF: d = (d & ~endmask) | (ALL_BLACK(s,d)	& endmask);	break;	\
	}


#endif /* _BITBLT_H_ */
