/* When editing, please remember to add/change members to both ASSEMBLY and */
/* non-assembly sections of this file! */
#ifndef _FB_LINEAH_
#define _FB_LINEAH_

#ifndef __ASSEMBLY__

#include <linux/types.h>

typedef struct
{
	__u32	resv1;		/* -910 -$38E Reserved */
	__u32	cur_font;	/* -906 -$38A Pointer to current font hdr */
	__u16	resv2[23];	/* -902 -$386 Reserved */
	__u16	m_pos_hx;	/* -856 -$356 X offset into the mouse form of the 'hot spot' */
	__u16	m_pos_hy;	/* -854 -$354 Y offset into the mouse form of the 'hot spot' */
	__u16	m_planes;	/* -852 -$352 Writing mode for the mouse pointer */
	__u16	m_cdb_bg;	/* -850 -$350 Mouse pointer background colour */
	__u16	m_cdb_fg;	/* -848 -$34E Mouse pointer foreground colour */
	__u16	mask_form[32];	/* -846 -$34C Mouse pointer mask and image */
	__u16	inq_tab[45];	/* -782 -$30E 45 words returned by vq_extnd */
	__u16	dev_tab[45];	/* -692 -$2B4 45 words returned by v_opnwk */
	__u16	gcurx;		/* -602 -$25A Current mouse pointer X pos */
	__u16	gcury;		/* -600 -$258 Current mouse pointer Y pos */
	__u16	m_hid_ct;	/* -598 -$256 Mouse hide count */
	__u16	mouse_bt;	/* -596 -$254 Current mouse button status */
	__u16	req_col[48];	/* -594 -$252 RGB data for the first 16 VDI colours as returned by vq_color */
	__u16	siz_tab[15];	/* -498 -$1F2 Text, line and marker sizes in device coords */
	__u16	resv3;		/* -468 -$1D4 Reserved */
	__u16	resv4;		/* -466 -$1D2 Reserved */
	__u32	cur_work;	/* -464 -$1D0 Pointer to current VDI workstation attribute table */
	__u32	def_font;	/* -460 -$1CC Pointer to default font header */
	__u32	font_ring[4];	/* -456 -$1C8 Array of pointers to lists of fonts */
	__u16	font_count;	/* -440 -$1B8 Number of fonts pointed to by the font_ring pointers */
	__u16	resv5[45];	/* -438 -$1B6 Reserved */
	__u8	cur_ms_stat;	/* -348 -$15C Bitmap of mouse status since last interrupt */
	__u8	resv6;		/* -347 -$15B Reserved */
	__u16	v_hid_cnt;	/* -346 -$15A Text cursor hide count */
	__u16	cur_x;		/* -344 -$158 X pos where mouse pointer will be drawn */
	__u16	cur_y;		/* -342 -$156 Y pos where mouse pointer will be drawn */
	__u8	cur_flag;	/* -340 -$154 Mouse redraw flag (!=0 draw on next VBI */
	__u8	mouse_flag;	/* -339 -$153 Mouse interrupt flag (!=0 enable interrupt) */
	__u32	resv7;		/* -338 -$152 Reserved */
	__u32	v_sav_xy;	/* -334 -$14E X and Y pos of text cursor saved by VT52 emulator */
	__u16	save_len;	/* -330 -$14A Height of form saved in save_area */
	__u32	save_addr;	/* -328 -$148 Screen address of first word saved from screen */
	__u16	save_stat;	/* -324 -$144 Save status flags bitmap */
	__u16	save_area[128];	/* -322 -$142 Save buffer for mouse pointer, up to 4bpl */
	__u32	user_tim;	/* -066 -$042 Pointer to routine called at each timer tick set by vex_timv */
	__u32	next_tim;	/* -062 -$03E User_tim routine jumps to this address when complete */
	__u32	user_but;	/* -058 -$03A Pointer to user mouse button routine set by vex_butv */
	__u32	user_cur;	/* -054 -$036 Pointer to user mouse cursor routine set by vex_curv */
	__u32	user_mot;	/* -050 -$032 Pointer to user mouse motion routine set by vex_motv */
	__u16	v_cel_ht;	/* -046 -$02E Current text cell height */
	__u16	v_cel_mx;	/* -044 -$02C Number of text columns - 1 */
	__u16	v_cel_my;	/* -042 -$02A Number of text rows - 1 */
	__u16	v_cel_wr;	/* -040 -$028 Number of bytes between character cells */
	__u16	v_col_bg;	/* -038 -$026 Text background colour */
	__u16	v_col_fg;	/* -036 -$024 Text foreground colour */
	__u32	v_cur_ad;	/* -034 -$022 Text cursor physical address */
	__u16	v_cur_of;	/* -030 -$01E Offset in bytes from screen base to top of first cell */
	__u32	v_cur_xy;	/* -028 -$01C X and Y character position of text cursor */
	__u8	v_period;	/* -024 -$018 Cursor blink rate in frames */
	__u8	v_cur_ct;	/* -023 -$017 Countdown timer to next cursor blink */
	__u32	v_fnt_ad;	/* -022 -$016 Pointer to monospaced system font data */
	__u16	v_fnt_nd;	/* -018 -$012 Last ASCII character in font */
	__u16	v_fnt_st;	/* -016 -$010 First ASCII character in font */
	__u16	v_fnt_wd;	/* -014	-$00E Width of system font form in bytes */
	__u16	v_rez_hz;	/* -012 -$00C Horizontal pixel resolution */
	__u32	v_off_ad;	/* -010 -$00A Address of font offset table */
	__u16	resv8;		/* -006 -$006 Reserved */
	__u16	v_rez_vt;	/* -004 -$004 Vertical pixel resolution */
	__u16	bytes_lin;	/* -002 -$002 Bytes per screen line */
	__u16	planes;		/* +000 +$000 Number of bit planes in current resolution */
	__u16	width;		/* +002 +$002 Width of destination memory form (usually screen) in bytes */
	__u32	contrl;		/* +004 +$004 Pointer to CONTRL array */
	__u32	intin;		/* +008 +$008 Pointer to INTIN array */
	__u32	ptsin;		/* +012 +$00C Pointer to PTSIN array */
	__u32	intout;		/* +016 +$010 Pointer to INTOUT array */
	__u32	ptsout;		/* +020 +$014 Pointer to PTSOUT array */
	__u16	colbit0;	/* +024 +$018 Colour bit value for plane 0 */
	__u16	colbit1;	/* +026 +$01A Colour bit value for plane 1 */
	__u16	colbit2;	/* +028 +$01C Colour bit value for plane 2 */
	__u16	colbit3;	/* +030 +$01E Colour bit value for plane 3 */
	__u16	lstlin;		/* +032 +$020 Last pixel in line draw flag (0==drawn) */
	__u16	lnmask;		/* +034 +$022 Line draw pattern mask */
        __u16	wmode;		/* +036 +$024 VDI writing mode */
        __u16	x1;		/* +038 +$026 X1 coordinate */
        __u16	y1;		/* +040 +$028 Y1 coordinate */
        __u16	x2;		/* +042 +$02A X2 coordinate */
        __u16	y2;		/* +044 +$02C Y2 coordinate */
        __u32	patptr;		/* +046 +$02E Fill pattern pointer */
        __u16	patmsk;		/* +050 +$032 Fill pattern mask, ANDed with Y1 */
	__u16	mfill;		/* +052 +$034 Multiplane fill pattern flag (0==mono) */
	__u16	clip;		/* +054 +$036 Clipping flag (0==disabled) */
	__u16	xmincl;		/* +056 +$038 Minimum X clipping value */
	__u16	ymincl;		/* +058 +$03A Minimum Y clipping value */
	__u16	xmaxcl;		/* +060 +$03C Maximum X clipping value */
	__u16	ymaxcl;		/* +062 +$03E Maximum Y clipping value */
	__u16	xdda;		/* +064 +$040 Text scaling accumulator */
	__u16	ddainc;		/* +066 +$042 Scaling increment */
	__u16	scaldir;	/* +068 +$044 Scale direction flag (0==down, 1==up) */
	__u16	mono;		/* +070 +$046 Monospaced font flag */
	__u16	sourcex;	/* +072 +$048 X coord of character in font form */
	__u16	sourcey;	/* +074 +$04A Y coord of character in font form */
	__u16	destx;		/* +076 +$04C X coord of character to output on screen */
	__u16	desty;		/* +078 +$04E Y coord of character to output on screen */
	__u16	delx;		/* +080 +$050 Width of character to output */
	__u16	dely;		/* +082 +$052 Height of character to output */
	__u32	fbase;		/* +084 +$054 Pointer to the font character image block */
	__u16	fwidth;		/* +088 +$058 Width of font form in bytes */
	__u16	style;		/* +090 +$05A TextBlt special effects flag bitmap */
	__u16	litemask;	/* +092 +$05C Mask to lighten text */
	__u16	skewmask;	/* +094 +$05E Mask to skew text */
	__u16	weight;		/* +096 +$060 Width by which to thicken text */
	__u16	roff;		/* +098 +$062 Offset above baseline used for italicizing */
	__u16	loff;		/* +100 +$064 Offset below baseline used for italicizing */
	__u16	scale;		/* +102 +$066 Text scaling flag (0==no scaling */
	__u16	chup;		/* +104 +$068 Character rotation angle in 10ths of degrees (supported only in 90 degree increments) */
	__u16	textfg;		/* +106 +$06A Text foreground colour */
	__u32	scrtchp;	/* +108 +$06C Pointer to two contiguous special effects buffers for TextBlt */
	__u16	scrpt2;		/* +112 +$070 Offset from first buffer to second (in bytes) */
	__u16	textbg;		/* +114 +$072 Text background colour */
	__u16	copytran;	/* +116 +$074 Copy raster mode */
	__u32	seedabort;	/* +118 +$076 Pointer to routine called by seedfill at each line */

	/* Anything after this is undocumented, but present on my Falcon */

	__u32	drawtab;	/* +162 +$0A2 Pointer to table for current resolution */

	__u32	drawcol;	/* +1618 +$652 Drawing colour (generated from colbits) */
	__u16	colbit4;	/* +1622 +$656 Colour bit value for plane 4 */
	__u16	colbit5;	/* +1624 +$658 Colour bit value for plane 5 */
	__u16	colbit6;	/* +1626 +$65A Colour bit value for plane 6 */
	__u16	colbit7;	/* +1628 +$65C Colour bit value for plane 7 */
} FBLINEA;

#else /* __ASSEMBLY__ */

#define	LASYM(addr,symbol)	((addr)-RESV1+(symbol))

RESV1		= -0x38E	/* Reserved */
CUR_FONT	= -0x38A	/* Pointer to current font hdr */
RESV2		= -0x386	/* Reserved */
#if 0
	__u16	m_pos_hx;	/* -856 -$356 X offset into the mouse form of the 'hot spot' */
	__u16	m_pos_hy;	/* -854 -$354 Y offset into the mouse form of the 'hot spot' */
	__u16	m_planes;	/* -852 -$352 Writing mode for the mouse pointer */
	__u16	m_cdb_bg;	/* -850 -$350 Mouse pointer background colour */
	__u16	m_cdb_fg;	/* -848 -$34E Mouse pointer foreground colour */
	__u16	mask_form[32];	/* -846 -$34C Mouse pointer mask and image */
	__u16	inq_tab[45];	/* -782 -$30E 45 words returned by vq_extnd */
	__u16	dev_tab[45];	/* -692 -$2B4 45 words returned by v_opnwk */
	__u16	gcurx;		/* -602 -$25A Current mouse pointer X pos */
	__u16	gcury;		/* -600 -$258 Current mouse pointer Y pos */
	__u16	m_hid_ct;	/* -598 -$256 Mouse hide count */
	__u16	mouse_bt;	/* -596 -$254 Current mouse button status */
	__u16	req_col[48];	/* -594 -$252 RGB data for the first 16 VDI colours as returned by vq_color */
	__u16	siz_tab[15];	/* -498 -$1F2 Text, line and marker sizes in device coords */
	__u16	resv3;		/* -468 -$1D4 Reserved */
	__u16	resv4;		/* -466 -$1D2 Reserved */
	__u32	cur_work;	/* -464 -$1D0 Pointer to current VDI workstation attribute table */
	__u32	def_font;	/* -460 -$1CC Pointer to default font header */
	__u32	font_ring[4];	/* -456 -$1C8 Array of pointers to lists of fonts */
	__u16	font_count;	/* -440 -$1B8 Number of fonts pointed to by the font_ring pointers */
	__u16	resv5[45];	/* -438 -$1B6 Reserved */
	__u8	cur_ms_stat;	/* -348 -$15C Bitmap of mouse status since last interrupt */
	__u8	resv6;		/* -347 -$15B Reserved */
	__u16	v_hid_cnt;	/* -346 -$15A Text cursor hide count */
	__u16	cur_x;		/* -344 -$158 X pos where mouse pointer will be drawn */
	__u16	cur_y;		/* -342 -$156 Y pos where mouse pointer will be drawn */
	__u8	cur_flag;	/* -340 -$154 Mouse redraw flag (!=0 draw on next VBI */
	__u8	mouse_flag;	/* -339 -$153 Mouse interrupt flag (!=0 enable interrupt) */
	__u32	resv7;		/* -338 -$152 Reserved */
	__u32	v_sav_xy;	/* -334 -$14E X and Y pos of text cursor saved by VT52 emulator */
	__u16	save_len;	/* -330 -$14A Height of form saved in save_area */
	__u32	save_addr;	/* -328 -$148 Screen address of first word saved from screen */
	__u16	save_stat;	/* -324 -$144 Save status flags bitmap */
	__u16	save_area[128];	/* -322 -$142 Save buffer for mouse pointer, up to 4bpl */
	__u32	user_tim;	/* -066 -$042 Pointer to routine called at each timer tick set by vex_timv */
	__u32	next_tim;	/* -062 -$03E User_tim routine jumps to this address when complete */
	__u32	user_but;	/* -058 -$03A Pointer to user mouse button routine set by vex_butv */
	__u32	user_cur;	/* -054 -$036 Pointer to user mouse cursor routine set by vex_curv */
	__u32	user_mot;	/* -050 -$032 Pointer to user mouse motion routine set by vex_motv */
	__u16	v_cel_ht;	/* -046 -$02E Current text cell height */
	__u16	v_cel_mx;	/* -044 -$02C Number of text columns - 1 */
	__u16	v_cel_my;	/* -042 -$02A Number of text rows - 1 */
	__u16	v_cel_wr;	/* -040 -$028 Number of bytes between character cells */
	__u16	v_col_bg;	/* -038 -$026 Text background colour */
	__u16	v_col_fg;	/* -036 -$024 Text foreground colour */
	__u32	v_cur_ad;	/* -034 -$022 Text cursor physical address */
	__u16	v_cur_of;	/* -030 -$01E Offset in bytes from screen base to top of first cell */
	__u32	v_cur_xy;	/* -028 -$01C X and Y character position of text cursor */
	__u8	v_period;	/* -024 -$018 Cursor blink rate in frames */
	__u8	v_cur_ct;	/* -023 -$017 Countdown timer to next cursor blink */
	__u32	v_fnt_ad;	/* -022 -$016 Pointer to monospaced system font data */
	__u16	v_fnt_nd;	/* -018 -$012 Last ASCII character in font */
	__u16	v_fnt_st;	/* -016 -$010 First ASCII character in font */
	__u16	v_fnt_wd;	/* -014	-$00E Width of system font form in bytes */
	__u16	v_rez_hz;	/* -012 -$00C Horizontal pixel resolution */
	__u32	v_off_ad;	/* -010 -$00A Address of font offset table */
	__u16	resv8;		/* -006 -$006 Reserved */
	__u16	v_rez_vt;	/* -004 -$004 Vertical pixel resolution */
#endif
BYTES_LIN	= -0x002	/* -002 -$002 Bytes per screen line */
PLANES		= 0x000		/* Number of bit planes in current resolution */
WIDTH		= 0x002		/* Width of destination memory form (usually screen) in bytes */
CONTRL		= 0x004		/* Pointer to CONTRL array */
INTIN		= 0x008		/* Pointer to INTIN array */
PTSIN		= 0x00C		/* Pointer to PTSIN array */
INTOUT		= 0x010		/* Pointer to INTOUT array */
PTSOUT		= 0x014		/* Pointer to PTSOUT array */
COLBIT0		= 0x018		/* Colour bit value for plane 0 */
COLBIT1		= 0x01A		/* Colour bit value for plane 1 */
COLBIT2		= 0x01C		/* Colour bit value for plane 2 */
COLBIT3		= 0x01E		/* Colour bit value for plane 3 */
LSTLIN		= 0x020		/* Last pixel in line draw flag (0==drawn) */
LNMASK		= 0x022		/* Line draw pattern mask */
WMODE		= 0x024		/* VDI writing mode */
X1		= 0x026		/* X1 coordinate */
Y1		= 0x028		/* Y1 coordinate */
X2		= 0x02A		/* X2 coordinate */
Y2		= 0x02C		/* Y2 coordinate */
PATPTR		= 0x02E		/* Fill pattern pointer */
PATMSK		= 0x032		/* Fill pattern mask, ANDed with Y1 */
MFILL		= 0x034		/* Multiplane fill pattern flag (0==mono) */
#if 0
	__u16	clip;		/* +054 +$036 Clipping flag (0==disabled) */
	__u16	xmincl;		/* +056 +$038 Minimum X clipping value */
	__u16	ymincl;		/* +058 +$03A Minimum Y clipping value */
	__u16	xmaxcl;		/* +060 +$03C Maximum X clipping value */
	__u16	ymaxcl;		/* +062 +$03E Maximum Y clipping value */
	__u16	xdda;		/* +064 +$040 Text scaling accumulator */
	__u16	ddainc;		/* +066 +$042 Scaling increment */
	__u16	scaldir;	/* +068 +$044 Scale direction flag (0==down, 1==up) */
	__u16	mono;		/* +070 +$046 Monospaced font flag */
	__u16	sourcex;	/* +072 +$048 X coord of character in font form */
	__u16	sourcey;	/* +074 +$04A Y coord of character in font form */
	__u16	destx;		/* +076 +$04C X coord of character to output on screen */
	__u16	desty;		/* +078 +$04E Y coord of character to output on screen */
	__u16	delx;		/* +080 +$050 Width of character to output */
	__u16	dely;		/* +082 +$052 Height of character to output */
	__u32	fbase;		/* +084 +$054 Pointer to the font character image block */
	__u16	fwidth;		/* +088 +$058 Width of font form in bytes */
	__u16	style;		/* +090 +$05A TextBlt special effects flag bitmap */
	__u16	litemask;	/* +092 +$05C Mask to lighten text */
	__u16	skewmask;	/* +094 +$05E Mask to skew text */
	__u16	weight;		/* +096 +$060 Width by which to thicken text */
	__u16	roff;		/* +098 +$062 Offset above baseline used for italicizing */
	__u16	loff;		/* +100 +$064 Offset below baseline used for italicizing */
	__u16	scale;		/* +102 +$066 Text scaling flag (0==no scaling */
	__u16	chup;		/* +104 +$068 Character rotation angle in 10ths of degrees (supported only in 90 degree increments) */
	__u16	textfg;		/* +106 +$06A Text foreground colour */
	__u32	scrtchp;	/* +108 +$06C Pointer to two contiguous special effects buffers for TextBlt */
	__u16	scrpt2;		/* +112 +$070 Offset from first buffer to second (in bytes) */
	__u16	textbg;		/* +114 +$072 Text background colour */
	__u16	copytran;	/* +116 +$074 Copy raster mode */
	__u32	seedabort;	/* +118 +$076 Pointer to routine called by seedfill at each line */

	/* Anything after this is undocumented, but present on my Falcon */
#endif
DRAWTAB		= 0x0A2		/* Pointer to table for current resolution */
DRAWCOL		= 0x652		/* Drawing colour (generated from colbits) */
COLBIT4		= 0x656		/* Colour bit value for plane 4 */
COLBIT5		= 0x658		/* Colour bit value for plane 5 */
COLBIT6		= 0x65A		/* Colour bit value for plane 6 */
COLBIT7		= 0x65C		/* Colour bit value for plane 7 */


#endif
#endif
