#include <stdio.h>
#include <assert.h>
#include "libfb.h"
#include "fballoc.h"
#include "fbevent.h"	/* for debugging */

/* 0, 1, 2 or 3 */
#define BBDEBUG 0

#define SWAP(s)	s=0x0000 | (s<<16)
#define RSWAP(s) s=0x00000000 | (s>>16)
#define	WSWAP(s) ((s<<16)|(s>>16))
#define UPPER(s) (((s)<<16) & 0xFFFF0000)

/* This is quite general, could go in libfb.h instead */
#define EVENWORD(s) (((s)+15) >> 4)


/* just to reduce possible mistakes */
#define ALL_WHITE(s,d)	(0x0000)
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
#define ALL_BLACK(s,d)	(0xffff)

 
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

void
pp_bitblt( FB *f, FBBLTPBLK *fbb )
{
  unsigned char *sbase = ((__u8 *) fbb->s_form) + 
			  (fbb->s_xmin * fbb->plane_ct) / 8 + 
			  (fbb->s_ymin * fbb->s_nxln);
  unsigned char *dbase = ((__u8 *) fbb->d_form) + 
			  (fbb->d_xmin * fbb->plane_ct) / 8 + 
			  (fbb->d_ymin * fbb->d_nxln);
  int hcnt;

  for(hcnt = 0; hcnt < fbb->b_ht; hcnt++) {
    memmove((void *) dbase, (void *) sbase, (fbb->b_wd * fbb->plane_ct) / 8);
    sbase += fbb->s_nxln;
    dbase += fbb->d_nxln;
  }
}

void 
bitblt( FB *f, FBBLTPBLK *fbb )
{
	register unsigned short *dbase=(__u16 *)((__u8 *)fbb->d_form + (( fbb->d_ymin * fbb->d_nxln ) + ( (fbb->d_xmin/16) * fbb->d_nxwd )));
	register unsigned short *sbase=(__u16 *)((__u8 *)fbb->s_form + (( fbb->s_ymin * fbb->s_nxln ) + ( (fbb->s_xmin/16) * fbb->s_nxwd )));
	short pc;
	unsigned short op_tabidx,logicop=0;
	unsigned short *s,*d,*lsbase,*ldbase;
	unsigned short i,j;
	int snxline,dnxline;
	short	snxword,dnxword;
	unsigned short s_xmax=fbb->s_xmin+fbb->b_wd-1;
	unsigned short d_xmax=fbb->d_xmin+fbb->b_wd-1;
	unsigned short lf_endmask[]={ 	0xFFFF, 0x7FFF, 0x3FFF, 0x1FFF,
					0x0FFF, 0x07FF, 0x03FF, 0x01FF,
					0x00FF, 0x007F, 0x003F, 0x001F,
					0x000F, 0x0007, 0x0003, 0x0001, 0x0000 };
	unsigned short *rt_endmask=&lf_endmask[1];
	unsigned short lendmsk=lf_endmask[fbb->d_xmin%16];
	unsigned short rendmsk=~rt_endmask[d_xmax%16];
	unsigned short endmask1,endmask3;
	unsigned short skew=(fbb->d_xmin - fbb->s_xmin) & 0x000F;
	int prefetch = (fbb->d_xmin%16 < fbb->s_xmin%16);

#if BBDEBUG > 0
	printf("s_form: %p d_form: %p\n",fbb->s_form,fbb->d_form);
	printf("sbase: %p dbase: %p\n",sbase,dbase);
	printf("skew: %hd\n",skew);
	printf("prefetch: %d\n",prefetch);
#endif
	if ( ( fbb->b_wd <= 16 ) && ( ( fbb->d_xmin / 16 ) == ( d_xmax / 16 ) ) )
	{
#if BBDEBUG > 0
		printf("fbbitblt: top to bottom, left to right, wd<16, same dst word\n");
#endif
		/* Blit left to right */

		/* Adjust sbase and dbase to top of block */

		/* No adjustment needed */

		/* Set next line offsets */

		snxline = fbb->s_nxln>>1;
		dnxline = fbb->d_nxln>>1;

		/* Set next word offsets */

		snxword = ( (fbb->s_xmin / 16) != (s_xmax / 16) ) ? fbb->s_nxwd>>1 : 0;
		dnxword = 0;

		/* Set endmask */
		endmask1 = lendmsk & rendmsk;
		endmask3 = 0;
	}
	else if ( ( fbb->b_wd <= 16 ) && ( ( fbb->s_xmin / 16 ) == ( s_xmax / 16 ) ) )
	{
#if BBDEBUG > 0
		printf("fbbitblt: top to bottom, left to right, wd<16, same src word\n");
#endif
		/* Blit left to right */

		/* Adjust sbase and dbase to top of block */

		/* No adjustment needed */

		/* Set next line offsets */

		snxline = fbb->s_nxln>>1;
		dnxline = fbb->d_nxln>>1;

		/* Set next word offsets */

		snxword = 0;
		dnxword = ( (fbb->d_xmin / 16) != (d_xmax / 16) ) ? fbb->d_nxwd>>1 : 0;

		/* Set endmask */
		endmask1 = lendmsk;
		endmask3 = rendmsk;
	}
	else if ( fbb->s_ymin < fbb->d_ymin )
	{
#if BBDEBUG > 0
		printf("fbbitblt: bottom to top\n");
#endif
		/* Blit bottom to top */

		/* Adjust sbase and dbase to bottom of block */

		(__u8 *) dbase += ( fbb->d_nxln * ( fbb->b_ht - 1 ) );
		(__u8 *) sbase += ( fbb->s_nxln * ( fbb->b_ht - 1 ) );

		/* Set next line offsets */

		snxline = - (fbb->s_nxln>>1);
		dnxline = - (fbb->d_nxln>>1);

		/* Set next word offsets */

		snxword = fbb->s_nxwd>>1;
		dnxword = fbb->d_nxwd>>1;

		/* Set endmasks */

		endmask1 = lendmsk;
		endmask3 = rendmsk;
	}
	else if ( fbb->s_ymin > fbb->d_ymin )
	{
#if BBDEBUG > 0
		printf("fbbitblt: top to bottom\n");
#endif
		/* Blit top to bottom */

		/* Adjust sbase and dbase */

		/* no adjustment needed, already set to top of block */

		/* Set next line offsets */

		snxline = fbb->s_nxln>>1;
		dnxline = fbb->d_nxln>>1;

		/* Set next word offsets */

		snxword = fbb->s_nxwd>>1;
		dnxword = fbb->d_nxwd>>1;

		/* Set endmasks */

		endmask1 = lendmsk;
		endmask3 = rendmsk;
	}
	else if ( fbb->s_xmin < fbb->d_xmin )
	{
#if BBDEBUG > 0
		printf("fbbitblt: top to bottom, right to left\n");
#endif
		/* Blit top to bottom, right to left */

		/* Adjust sbase and dbase to top right of block */

		(__u8 *) dbase += ( ( ( ( fbb->b_wd - 1 ) / 16 ) + ( ( fbb->d_xmin % 16 ) != 0 )) * fbb->d_nxwd );
		(__u8 *) sbase += ( ( ( ( fbb->b_wd - 1 ) / 16 ) + ( ( fbb->s_xmin % 16 ) != 0 )) * fbb->s_nxwd );

		/* Set next line offsets */

		snxline = fbb->s_nxln>>1;
		dnxline = fbb->d_nxln>>1;

		/* Set next word offsets */

		snxword = - (fbb->s_nxwd>>1);
		dnxword = - (fbb->d_nxwd>>1);

		/* Set endmasks */

		endmask1 = rendmsk;
		endmask3 = lendmsk;
	}
	else
	{
		/* Blit top to bottom, left to right */

		/* Adjust sbase and dbase */

		/* no adjustment needed, already set to top of block */

		/* Set next line offsets */

		snxline = fbb->s_nxln>>1;
		dnxline = fbb->d_nxln>>1;

		/* Set next word offsets */

		snxword = fbb->s_nxwd>>1;
		dnxword = fbb->d_nxwd>>1;

		/* Set endmasks */

		endmask1 = lendmsk;
		endmask3 = rendmsk;
	}

#if BBDEBUG > 0
		printf("endmask1 is %x endmask3 is %x\n",endmask1,endmask3);
#endif

	/* Blit each plane starting from the last */

	for (	pc=fbb->plane_ct-1, sbase+=(pc*fbb->s_nxpl)>>1,
		  dbase+=(pc*fbb->d_nxpl)>>1;
		pc >= 0 ;
		pc--,sbase-=fbb->s_nxpl>>1,dbase-=fbb->d_nxpl>>1 )
	{
		/* Calculate index into op_tab */

		op_tabidx = ( ( ( fbb->fg_col>>pc ) & 0x0001 ) <<1 ) |
				(( fbb->bg_col>>pc ) & 0x0001 );
#if BBDEBUG > 1
		printf("op_tabidx is %hd\n",op_tabidx);
#endif
		/* Get logic op for this plane from op_tab */

		logicop = ((char *) &fbb->op_tab)[op_tabidx] & 0x000f;
#if BBDEBUG > 1
		printf("logicop is %hd\n",logicop);
#endif
		/* Do for each line in the blit */

	if (snxword>=0)
	{
#if BBDEBUG > 1
		printf("left to right blit entered\n");
#endif
		for (	j=0,lsbase=sbase,ldbase=dbase ;
			j<fbb->b_ht ;
			j++,lsbase+=snxline,ldbase+=dnxline )
		{
			unsigned int sourcebuf=0x00000000;
			
#if BBDEBUG > 2
			printf("lsbase is %p ldbase is %p snxline is %d dnxline is %d\n",lsbase,ldbase,snxline,dnxline);
#endif
			s=lsbase;
			d=ldbase;

			/* If source pixel on word boundary, don't prefetch */

			if ( prefetch )
			{
#if BBDEBUG > 2
				printf("prefetching\n");
#endif
				sourcebuf|=*s;
				s += snxword;
				SWAP(sourcebuf);
			}

			sourcebuf|=*s;

			/* Do first word */

			LOGICOPEND(*d,(unsigned short)(sourcebuf>>skew),logicop,endmask1)

			/* One word blit */

			if (dnxword==0)
				continue;

			s += snxword;
			d += dnxword;
			SWAP(sourcebuf);
			sourcebuf|=*s;

			/* Do middle words */

			for (	i=0 ;
				/*i< ((fbb->b_wd /16) + (!equalspans) ((fbb->b_wd % 16) != 0 ) - 2 ) ;*/
				i< (((d_xmax/16) - (fbb->d_xmin/16)) - 1 ) ;
				i++,s+=snxword,d+=dnxword,SWAP(sourcebuf),sourcebuf|=*s )
			{
				LOGICOP(*d,(unsigned short)(sourcebuf>>skew),logicop)
			}

			/* Do last word */

			LOGICOPEND(*d,(unsigned short)(sourcebuf>>skew),logicop,endmask3)
		}
	}
	else
	{
#if BBDEBUG > 1
		printf("right to left blit entered\n");
		printf("endmask1 is %x endmask3 is %x\n",endmask1,endmask3);
#endif
		for (	j=0,lsbase=sbase,ldbase=dbase ;
			j<fbb->b_ht ;
			j++,lsbase+=snxline,ldbase+=dnxline )
		{
			unsigned int sourcebuf=0x00000000;
			
#if BBDEBUG > 2
			printf("lsbase is %p ldbase is %p snxline is %d dnxline is %d\n",lsbase,ldbase,snxline,dnxline);
#endif
			s=lsbase;
			d=ldbase;
			sourcebuf|= UPPER(*s);

			s += snxword;
			RSWAP(sourcebuf);
			sourcebuf|= UPPER(*s);


			/* Do first word */

			LOGICOPEND(*d,(unsigned short)(sourcebuf>>skew),logicop,endmask1)

			s += snxword;
			d += dnxword;
			RSWAP(sourcebuf);
			sourcebuf|=UPPER(*s);

			/* Do middle words */

			for (	i=0 ;
				/*i< ((fbb->b_wd /16) + (!equalspans) ((fbb->b_wd % 16) != 0 ) - 2 ) ;*/
				i< (((d_xmax/16) - (fbb->d_xmin/16)) - 1 ) ;
				i++,s+=snxword,d+=dnxword,RSWAP(sourcebuf),sourcebuf|=UPPER(*s) )
			{
				LOGICOP(*d,(unsigned short)(sourcebuf>>skew),logicop)
			}

			/* Do last word */

			LOGICOPEND(*d,(unsigned short)(sourcebuf>>skew),logicop,endmask3)
		}
	}
	}
}

FBBLTPBLK *
FBgetbltpblk( FB *f )
{
	FBBLTPBLK *fbb;
	int i;

	fbb = (FBBLTPBLK *) FBalloc( sizeof(FBBLTPBLK) );

	fbb->b_wd = f->vinf.xres_virtual;
	fbb->b_ht = f->vinf.yres_virtual;
	fbb->plane_ct = f->vinf.bits_per_pixel;
	fbb->fg_col = fbb->bg_col = 0;
	fbb->op_tab = 0x03030303; /* dest = src */
	fbb->s_form = fbb->d_form = f->sbuf;
	fbb->s_xmin = fbb->s_ymin = fbb->d_xmin = fbb->d_ymin = 0;

	/* these aren't used yet, but we could just as well set them up anyway */
	fbb->p_addr = NULL;
	fbb->p_nxln = fbb->p_nxpl = fbb->p_mask = 0;
	for( i = 0 ; i < 12 ; i++ )
		fbb->space[i] = 0;


	switch (f->finf.type)
	{
		case FB_TYPE_INTERLEAVED_PLANES:
			fbb->s_nxwd = fbb->d_nxwd = f->vinf.bits_per_pixel * 2;
			fbb->s_nxln = fbb->d_nxln = EVENWORD(f->vinf.xres_virtual) * 2 *
			     f->vinf.bits_per_pixel;
			fbb->s_nxpl = fbb->d_nxpl = 2;
			break;

		case FB_TYPE_PLANES:
			fbb->s_nxwd = fbb->d_nxwd = 2;
			fbb->s_nxln = fbb->d_nxln = EVENWORD(f->vinf.xres_virtual) * 2;
			fbb->s_nxpl = fbb->d_nxpl = EVENWORD(f->vinf.xres_virtual) * 2 *
			     f->vinf.yres_virtual;
			break;
		/* not sure what's correct or not here - Stefan */
		case FB_TYPE_PACKED_PIXELS:
			fbb->s_nxwd = fbb->d_nxwd = f->vinf.bits_per_pixel * 2;
			fbb->s_nxln = fbb->d_nxln = (f->vinf.xres_virtual * 
						     f->vinf.bits_per_pixel)/8;
			fbb->s_nxpl = fbb->d_nxpl = 2;
			break;

		/* not sure what to have when type is not any of the above */
		default:
			fbb->s_nxwd = fbb->d_nxwd = f->vinf.bits_per_pixel * 2;
			fbb->s_nxln = fbb->d_nxln = EVENWORD(f->vinf.xres_virtual) * 2 *
			     f->vinf.bits_per_pixel;
			fbb->s_nxpl = fbb->d_nxpl = 2;
			break;
	}

	return fbb;
}

void
FBfreebltpblk( FBBLTPBLK *fbb )
{
     FBfree( fbb );
}
