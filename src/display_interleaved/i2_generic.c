#include <stdio.h>

#include "ofbis.h"
#include "bitblt.h"

/* We use a generic bitblt for all interleaved resolutions, since they
 * are the same in this regard.
 */
void 
i2_generic_bitblt( FB *f, FBBLTPBLK *fbb )
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

u_int32_t i2_generic_c24_to_cnative(FB *f, u_int32_t col24)
{
  int nr_of_colours, best_diff, best_match, i, diff;
  int red, green, blue;

  /* For palette based modes, we try to find the best matching
   * colour index.
   */
  if(f->cmap == NULL)
    return 0;

  nr_of_colours = 1 << f->vinf.bits_per_pixel;
  best_diff = 1024;
  best_match = nr_of_colours - 1;
  for(i = 0 ; i < nr_of_colours ; i++) {
    red   = ((col24 >> 16) & 0xff) - (f->cmap->red[i] >> 8);
    green = ((col24 >>  8) & 0xff) - (f->cmap->green[i] >> 8);
    blue  = ((col24      ) & 0xff) - (f->cmap->blue[i] >> 8);
    diff = red * red + green * green + blue * blue;
    if(diff < best_diff) {
      best_diff = diff;
      best_match = i;
    }
  }

  return best_match;
}

u_int32_t i2_generic_cnative_to_c24(FB *f, u_int32_t col)
{
  int index, nr_of_colours;
  u_int32_t colour;

  /* This picks out the colour from the palette. */
  if(f->cmap == NULL)
    return 0;

  nr_of_colours = 1 << f->vinf.bits_per_pixel;
  index = col < nr_of_colours ? col : nr_of_colours-1;
  index = index >= 0 ? index : 0;
  colour = ((((f->cmap->red[index] >> 8) & 0xff) << 16) |
	    (((f->cmap->green[index] >> 8) & 0xff) <<  8) |
	    (((f->cmap->blue[index] >> 8) & 0xff) <<  0));
  return colour;
}
