#ifndef _I2_COMMON_H_
#define _I2_COMMON_H_

#define writetoi2plane(x)					\
	base[x] &= ~( 0x00000001 << sc );			\
	base[x] |= ( ( ( col >> x ) & 0x00000001 ) << sc );

#define readfromi2plane(x)					\
	col |= ( ( base[x] >> sc ) & 0x00000001 ) << x;

#define setonei2(data,bpl,c) base[bpl] &= ~((data) << skew); \
			     if((c>>bpl)&1) base[bpl] |=  ((data) << skew);

#define settwoi2(data,bpl,off,c) base[bpl]     &= ~((data) >> (-skew)); \
				 base[bpl+off] &= ~((data) << (16+skew)); \
				 if((c>>bpl)&1) { base[bpl]     |=  ((data) >> (-skew)); \
						  base[bpl+off] |=  ((data) << (16+skew)); }

extern void i2_generic_bitblt(FB *f, FBBLTPBLK *fbb);
extern u_int32_t i2_generic_c24_to_cnative(FB *f, u_int32_t col24);
extern u_int32_t i2_generic_cnative_to_c24(FB *f, u_int32_t col);

#endif /* _I2_COMMON_H_ */

