#ifndef _PL_COMMON_H_
#define _PL_COMMON_H_

#define	writetoplplane(x)					\
	*base &= ~( 0x00000001 << sc );				\
	*base |= ( ( ( col >> x ) & 0x00000001 ) << sc );

#define	readfromplplane(x)					\
	col |= ( ( *base >> sc ) & 0x00000001 ) << x;

extern void pl_generic_bitblt(FB *, FBBLTPBLK *);
extern u_int32_t pl_generic_c24_to_cnative(FB *f, u_int32_t col24);
extern u_int32_t pl_generic_cnative_to_c24(FB *f, u_int32_t col);

#endif /* _PL_COMMON_H_ */
