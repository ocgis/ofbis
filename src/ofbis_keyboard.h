/*
** ofbis_keyboard.h
**
** Copyright 1999 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
*/

#ifndef _OFBIS_KEYBOARD_H
#define _OFBIS_KEYBOARD_H

#define FB_KEY(typ,val) (((typ) << 8) | (val))
#define FB_KTYP(x)	((x) >> 8)
#define FB_KVAL(x)	((x) & 0xff)
#define FB_UNICODE(x)	((x) < 0xE000 || (x) >= 0xF900)

/* types */
#define FB_KT_LATIN1	0x00
#define FB_KT_SPEC	0xE0
#define FB_KT_FN	0xE1
#define FB_KT_PAD	0xE2
#define FB_KT_MOD	0xE3
#define FB_KT_DEAD	0xE4

/* modifiers */
#define FB_KM_SHIFT	0x00
#define FB_KM_CTRL	0x01
#define FB_KM_ALT	0x02
#define FB_KM_META	0x03
#define FB_KM_SUPER	0x04
#define FB_KM_HYPER	0x05
#define FB_KM_ALTGR	0x06
#define FB_KM_CAPS	0x07
#define FB_KM_NUM	0x08
#define FB_KM_SCROLL	0x09

#define FB_KM_MASK	0x0f
#define FB_KM_RIGHT	0x40	/* additive */
#define FB_KM_LOCK	0x80	/*  - " -   (only used for labels) */

/* Bits in the "modifiers" field */
#define FB_MOD_SHIFT	(1 << FB_KM_SHIFT)
#define FB_MOD_CTRL	(1 << FB_KM_CTRL)
#define FB_MOD_ALT	(1 << FB_KM_ALT)
#define FB_MOD_META	(1 << FB_KM_META)
#define FB_MOD_SUPER	(1 << FB_KM_SUPER)
#define FB_MOD_HYPER	(1 << FB_KM_HYPER)
#define FB_MOD_ALTGR	(1 << FB_KM_ALTGR)
#define FB_MOD_CAPS	(1 << FB_KM_CAPS)
#define FB_MOD_NUM	(1 << FB_KM_NUM)
#define FB_MOD_SCROLL	(1 << FB_KM_SCROLL)

/* function keys */
#define FB_K_F0			FB_KEY(FB_KT_FN, 0)
#define FB_K_F1			FB_KEY(FB_KT_FN, 1)
#define FB_K_F2			FB_KEY(FB_KT_FN, 2)
#define FB_K_F3			FB_KEY(FB_KT_FN, 3)
#define FB_K_F4			FB_KEY(FB_KT_FN, 4)
#define FB_K_F5			FB_KEY(FB_KT_FN, 5)
#define FB_K_F6			FB_KEY(FB_KT_FN, 6)
#define FB_K_F7			FB_KEY(FB_KT_FN, 7)
#define FB_K_F8			FB_KEY(FB_KT_FN, 8)
#define FB_K_F9			FB_KEY(FB_KT_FN, 9)
#define FB_K_F10		FB_KEY(FB_KT_FN, 10)
#define FB_K_F11		FB_KEY(FB_KT_FN, 11)
#define FB_K_F12		FB_KEY(FB_KT_FN, 12)
#define FB_K_F13		FB_KEY(FB_KT_FN, 13)
#define FB_K_F14		FB_KEY(FB_KT_FN, 14)
#define FB_K_F15		FB_KEY(FB_KT_FN, 15)
#define FB_K_F16		FB_KEY(FB_KT_FN, 16)
#define FB_K_F17		FB_KEY(FB_KT_FN, 17)
#define FB_K_F18		FB_KEY(FB_KT_FN, 18)
#define FB_K_F19		FB_KEY(FB_KT_FN, 19)
#define FB_K_F20		FB_KEY(FB_KT_FN, 20)
#define FB_K_F21		FB_KEY(FB_KT_FN, 21)
#define FB_K_F22		FB_KEY(FB_KT_FN, 22)
#define FB_K_F23		FB_KEY(FB_KT_FN, 23)
#define FB_K_F24		FB_KEY(FB_KT_FN, 24)
#define FB_K_F25		FB_KEY(FB_KT_FN, 25)
#define FB_K_F26		FB_KEY(FB_KT_FN, 26)
#define FB_K_F27		FB_KEY(FB_KT_FN, 27)
#define FB_K_F28		FB_KEY(FB_KT_FN, 28)
#define FB_K_F29		FB_KEY(FB_KT_FN, 29)
#define FB_K_F30		FB_KEY(FB_KT_FN, 30)
#define FB_K_F31		FB_KEY(FB_KT_FN, 31)
#define FB_K_F32		FB_KEY(FB_KT_FN, 32)
#define FB_K_F33		FB_KEY(FB_KT_FN, 33)
#define FB_K_F34		FB_KEY(FB_KT_FN, 34)
#define FB_K_F35		FB_KEY(FB_KT_FN, 35)
#define FB_K_F36		FB_KEY(FB_KT_FN, 36)
#define FB_K_F37		FB_KEY(FB_KT_FN, 37)
#define FB_K_F38		FB_KEY(FB_KT_FN, 38)
#define FB_K_F39		FB_KEY(FB_KT_FN, 39)
#define FB_K_F40		FB_KEY(FB_KT_FN, 40)
#define FB_K_F41		FB_KEY(FB_KT_FN, 41)
#define FB_K_F42		FB_KEY(FB_KT_FN, 42)
#define FB_K_F43		FB_KEY(FB_KT_FN, 43)
#define FB_K_F44		FB_KEY(FB_KT_FN, 44)
#define FB_K_F45		FB_KEY(FB_KT_FN, 45)
#define FB_K_F46		FB_KEY(FB_KT_FN, 46)
#define FB_K_F47		FB_KEY(FB_KT_FN, 47)
#define FB_K_F48		FB_KEY(FB_KT_FN, 48)
#define FB_K_F49		FB_KEY(FB_KT_FN, 49)
#define FB_K_F50		FB_KEY(FB_KT_FN, 50)
#define FB_K_F51		FB_KEY(FB_KT_FN, 51)
#define FB_K_F52		FB_KEY(FB_KT_FN, 52)
#define FB_K_F53		FB_KEY(FB_KT_FN, 53)
#define FB_K_F54		FB_KEY(FB_KT_FN, 54)
#define FB_K_F55		FB_KEY(FB_KT_FN, 55)
#define FB_K_F56		FB_KEY(FB_KT_FN, 56)
#define FB_K_F57		FB_KEY(FB_KT_FN, 57)
#define FB_K_F58		FB_KEY(FB_KT_FN, 58)
#define FB_K_F59		FB_KEY(FB_KT_FN, 59)
#define FB_K_F60		FB_KEY(FB_KT_FN, 60)
#define FB_K_F61		FB_KEY(FB_KT_FN, 61)
#define FB_K_F62		FB_KEY(FB_KT_FN, 62)
#define FB_K_F63		FB_KEY(FB_KT_FN, 63)
#define FB_K_F64		FB_KEY(FB_KT_FN, 64)

/* special keys */
#define FB_K_VOID		FB_KEY(FB_KT_SPEC, 0)

#define FB_K_Enter		FB_UC_Return
#define FB_K_Delete		FB_UC_Delete

#define FB_K_Break		FB_KEY(FB_KT_SPEC, 5)

#define FB_K_ScrollForw		FB_KEY(FB_KT_SPEC, 10)
#define FB_K_ScrollBack		FB_KEY(FB_KT_SPEC, 11)

#define FB_K_Boot		FB_KEY(FB_KT_SPEC, 12)
#define FB_K_Compose		FB_KEY(FB_KT_SPEC, 14)
#define FB_K_SAK		FB_KEY(FB_KT_SPEC, 15)

#define FB_K_Undo		FB_KEY(FB_KT_SPEC, 23)
#define FB_K_Redo		FB_KEY(FB_KT_SPEC, 24)
#define FB_K_Menu		FB_KEY(FB_KT_SPEC, 25)
#define FB_K_Cancel		FB_KEY(FB_KT_SPEC, 26)
#define FB_K_PrintScreen	FB_KEY(FB_KT_SPEC, 27)
#define FB_K_Execute		FB_KEY(FB_KT_SPEC, 28)
#define FB_K_Find		FB_KEY(FB_KT_SPEC, 30)
#define FB_K_Begin		FB_KEY(FB_KT_SPEC, 31)
#define FB_K_Clear		FB_KEY(FB_KT_SPEC, 32)
#define FB_K_Insert		FB_KEY(FB_KT_SPEC, 34)
#define FB_K_Select		FB_KEY(FB_KT_SPEC, 35)
#define FB_K_Macro	 	FB_KEY(FB_KT_SPEC, 38)
#define FB_K_Help		FB_KEY(FB_KT_SPEC, 39)
#define FB_K_Do			FB_KEY(FB_KT_SPEC, 40)
#define FB_K_Pause	 	FB_KEY(FB_KT_SPEC, 41)
#define FB_K_Stop		FB_K_Pause
#define FB_K_SysRq		FB_KEY(FB_KT_SPEC, 42)
#define FB_K_ModeSwitch		FB_KEY(FB_KT_SPEC, 43)

#define FB_K_Up			FB_KEY(FB_KT_SPEC, 50)
#define FB_K_Down		FB_KEY(FB_KT_SPEC, 51)
#define FB_K_Left		FB_KEY(FB_KT_SPEC, 52)
#define FB_K_Right		FB_KEY(FB_KT_SPEC, 53)
#define FB_K_Prior		FB_KEY(FB_KT_SPEC, 54)
#define FB_K_PageUp		FB_K_Prior
#define FB_K_Next		FB_KEY(FB_KT_SPEC, 55)
#define FB_K_PageDown		FB_K_Next
#define FB_K_Home		FB_KEY(FB_KT_SPEC, 56)
#define FB_K_End		FB_KEY(FB_KT_SPEC, 57)

/* keys on the numeric keypad */
#define FB_K_P0			FB_KEY(FB_KT_PAD, '0')
#define FB_K_P1			FB_KEY(FB_KT_PAD, '1')
#define FB_K_P2			FB_KEY(FB_KT_PAD, '2')
#define FB_K_P3			FB_KEY(FB_KT_PAD, '3')
#define FB_K_P4			FB_KEY(FB_KT_PAD, '4')
#define FB_K_P5			FB_KEY(FB_KT_PAD, '5')
#define FB_K_P6			FB_KEY(FB_KT_PAD, '6')
#define FB_K_P7			FB_KEY(FB_KT_PAD, '7')
#define FB_K_P8			FB_KEY(FB_KT_PAD, '8')
#define FB_K_P9			FB_KEY(FB_KT_PAD, '9')
#define FB_K_PA			FB_KEY(FB_KT_PAD, 'A')
#define FB_K_PB			FB_KEY(FB_KT_PAD, 'B')
#define FB_K_PC			FB_KEY(FB_KT_PAD, 'C')
#define FB_K_PD			FB_KEY(FB_KT_PAD, 'D')
#define FB_K_PE			FB_KEY(FB_KT_PAD, 'E')
#define FB_K_PF			FB_KEY(FB_KT_PAD, 'F')

#define FB_K_PPlus		FB_KEY(FB_KT_PAD, '+')
#define FB_K_PMinus		FB_KEY(FB_KT_PAD, '-')
#define FB_K_PSlash		FB_KEY(FB_KT_PAD, '/')
#define FB_K_PAsterisk		FB_KEY(FB_KT_PAD, '*')
#define FB_K_PStar		FB_K_PAsterisk
#define FB_K_PEqual		FB_KEY(FB_KT_PAD, '=')
#define FB_K_PSeparator		FB_KEY(FB_KT_PAD, ',')
#define FB_K_PDecimal		FB_KEY(FB_KT_PAD, '.')
#define FB_K_PParenLeft		FB_KEY(FB_KT_PAD, '(')
#define FB_K_PParenRight	FB_KEY(FB_KT_PAD, ')')
#define FB_K_PSpace		FB_KEY(FB_KT_PAD, ' ')
#define FB_K_PEnter		FB_KEY(FB_KT_PAD, '\r')
#define FB_K_PTab		FB_KEY(FB_KT_PAD, '\t')

#define FB_K_PPlusMinus		FB_KEY(FB_KT_PAD, 0x80)
#define FB_K_PBegin		FB_KEY(FB_KT_PAD, 0x81)

#define FB_K_PF1		FB_KEY(FB_KT_PAD, 0x91)
#define FB_K_PF2		FB_KEY(FB_KT_PAD, 0x92)
#define FB_K_PF3		FB_KEY(FB_KT_PAD, 0x93)
#define FB_K_PF4		FB_KEY(FB_KT_PAD, 0x94)
#define FB_K_PF5		FB_KEY(FB_KT_PAD, 0x95)
#define FB_K_PF6		FB_KEY(FB_KT_PAD, 0x96)
#define FB_K_PF7		FB_KEY(FB_KT_PAD, 0x97)
#define FB_K_PF8		FB_KEY(FB_KT_PAD, 0x98)
#define FB_K_PF9		FB_KEY(FB_KT_PAD, 0x99)

/* modifier keys */
#define FB_K_Shift		FB_KEY(FB_KT_MOD, FB_KM_SHIFT)
#define FB_K_Ctrl		FB_KEY(FB_KT_MOD, FB_KM_CTRL)
#define FB_K_Alt		FB_KEY(FB_KT_MOD, FB_KM_ALT)
#define FB_K_Meta		FB_KEY(FB_KT_MOD, FB_KM_META)
#define FB_K_Super		FB_KEY(FB_KT_MOD, FB_KM_SUPER)
#define FB_K_Hyper		FB_KEY(FB_KT_MOD, FB_KM_HYPER)
#define FB_K_AltGr		FB_KEY(FB_KT_MOD, FB_KM_ALTGR)
#define FB_K_Caps		FB_KEY(FB_KT_MOD, FB_KM_CAPS)
#define FB_K_Num		FB_KEY(FB_KT_MOD, FB_KM_NUM)
#define FB_K_Scroll		FB_KEY(FB_KT_MOD, FB_KM_SCROLL)

/* modifier _labels_ */
#define FB_K_ShiftL		(FB_K_Shift  | 0)
#define FB_K_ShiftR		(FB_K_Shift  | FB_KM_RIGHT)
#define FB_K_CtrlL		(FB_K_Ctrl   | 0)
#define FB_K_CtrlR		(FB_K_Ctrl   | FB_KM_RIGHT)
#define FB_K_AltL		(FB_K_Alt    | 0)
#define FB_K_AltR		(FB_K_Alt    | FB_KM_RIGHT)
#define FB_K_MetaL		(FB_K_Meta   | 0)
#define FB_K_MetaR		(FB_K_Meta   | FB_KM_RIGHT)
#define FB_K_SuperL		(FB_K_Super  | 0)
#define FB_K_SuperR		(FB_K_Super  | FB_KM_RIGHT)
#define FB_K_HyperL		(FB_K_Hyper  | 0)
#define FB_K_HyperR		(FB_K_Hyper  | FB_KM_RIGHT)

#define FB_K_ShiftLock		(FB_K_Shift  | FB_KM_LOCK)
#define FB_K_CtrlLock		(FB_K_Ctrl   | FB_KM_LOCK)
#define FB_K_AltLock		(FB_K_Alt    | FB_KM_LOCK)
#define FB_K_MetaLock		(FB_K_Meta   | FB_KM_LOCK)
#define FB_K_SuperLock		(FB_K_Super  | FB_KM_LOCK)
#define FB_K_HyperLock		(FB_K_Hyper  | FB_KM_LOCK)
#define FB_K_AltGrLock		(FB_K_AltGr  | FB_KM_LOCK)
#define FB_K_CapsLock		(FB_K_Caps   | FB_KM_LOCK)
#define FB_K_NumLock		(FB_K_Num    | FB_KM_LOCK)
#define FB_K_ScrollLock		(FB_K_Scroll | FB_KM_LOCK)

/* miscellaneous */
#define FB_K_NIL   	0xffff	/* used to indicate "not mapped yet" */
#define FB_BUTTON_NIL	0xff	/* used for pseudo keys (composed ones) */

/* ASCII keys */
#define FB_UC_Nul		0x00
#define FB_UC_BackSpace		0x08
#define FB_UC_Tab		0x09
#define FB_UC_Linefeed		0x0a
#define FB_UC_Return		0x0d
#define FB_UC_Escape		0x1b
#define FB_UC_Delete		0x7f

#define FB_UC_Space		0x20
#define FB_UC_Exclamation	0x21
#define FB_UC_Exclam		FB_UC_Exclamation
#define FB_UC_DoubleQuote	0x22
#define FB_UC_QuoteDbl		FB_UC_DoubleQuote
#define FB_UC_NumberSign	0x23
#define FB_UC_Hash		FB_UC_NumberSign
#define FB_UC_Dollar		0x24
#define FB_UC_Percent		0x25
#define FB_UC_Ampersand		0x26
#define FB_UC_Apostrophe	0x27
#define FB_UC_ParenLeft		0x28
#define FB_UC_ParenRight	0x29
#define FB_UC_Asterisk		0x2a
#define FB_UC_Star		FB_UC_Asterisk
#define FB_UC_Plus		0x2b
#define FB_UC_Comma		0x2c
#define FB_UC_Minus		0x2d
#define FB_UC_Period		0x2e
#define FB_UC_Slash		0x2f
#define FB_UC_0			0x30
#define FB_UC_1			0x31
#define FB_UC_2			0x32
#define FB_UC_3			0x33
#define FB_UC_4			0x34
#define FB_UC_5			0x35
#define FB_UC_6			0x36
#define FB_UC_7			0x37
#define FB_UC_8			0x38
#define FB_UC_9			0x39
#define FB_UC_Colon		0x3a
#define FB_UC_Semicolon		0x3b
#define FB_UC_Less		0x3c
#define FB_UC_Equal		0x3d
#define FB_UC_Greater		0x3e
#define FB_UC_Question		0x3f
#define FB_UC_At		0x40
#define FB_UC_A			0x41
#define FB_UC_B			0x42
#define FB_UC_C			0x43
#define FB_UC_D			0x44
#define FB_UC_E			0x45
#define FB_UC_F			0x46
#define FB_UC_G			0x47
#define FB_UC_H			0x48
#define FB_UC_I			0x49
#define FB_UC_J			0x4a
#define FB_UC_K			0x4b
#define FB_UC_L			0x4c
#define FB_UC_M			0x4d
#define FB_UC_N			0x4e
#define FB_UC_O			0x4f
#define FB_UC_P			0x50
#define FB_UC_Q			0x51
#define FB_UC_R			0x52
#define FB_UC_S			0x53
#define FB_UC_T			0x54
#define FB_UC_U			0x55
#define FB_UC_V			0x56
#define FB_UC_W			0x57
#define FB_UC_X			0x58
#define FB_UC_Y			0x59
#define FB_UC_Z			0x5a
#define FB_UC_BracketLeft	0x5b
#define FB_UC_BackSlash		0x5c
#define FB_UC_BracketRight	0x5d
#define FB_UC_Circumflex	0x5e
#define FB_UC_Hat		FB_UC_Circumflex
#define FB_UC_Underscore	0x5f
#define FB_UC_Grave		0x60
#define FB_UC_a			0x61
#define FB_UC_b			0x62
#define FB_UC_c			0x63
#define FB_UC_d			0x64
#define FB_UC_e			0x65
#define FB_UC_f			0x66
#define FB_UC_g			0x67
#define FB_UC_h			0x68
#define FB_UC_i			0x69
#define FB_UC_j			0x6a
#define FB_UC_k			0x6b
#define FB_UC_l			0x6c
#define FB_UC_m			0x6d
#define FB_UC_n			0x6e
#define FB_UC_o			0x6f
#define FB_UC_p			0x70
#define FB_UC_q			0x71
#define FB_UC_r			0x72
#define FB_UC_s			0x73
#define FB_UC_t			0x74
#define FB_UC_u			0x75
#define FB_UC_v			0x76
#define FB_UC_w			0x77
#define FB_UC_x			0x78
#define FB_UC_y			0x79
#define FB_UC_z			0x7a
#define FB_UC_BraceLeft		0x7b
#define FB_UC_Bar		0x7c
#define FB_UC_Pipe		FB_UC_Bar
#define FB_UC_BraceRight	0x7d
#define FB_UC_Tilde		0x7e

#define FB_UC_NoBreakSpace	0xa0
#define FB_UC_NBSP		FB_UC_NoBreakSpace
#define FB_UC_ExclamDown	0xa1
#define FB_UC_Cent		0xa2
#define FB_UC_Sterling		0xa3
#define FB_UC_Pound		FB_UC_Sterling
#define FB_UC_Currency		0xa4
#define FB_UC_Yen		0xa5
#define FB_UC_BrokenBar		0xa6
#define FB_UC_Section		0xa7
#define FB_UC_Diaeresis		0xa8
#define FB_UC_Umlaut		FB_UC_Diaeresis
#define FB_UC_Copyright		0xa9
#define FB_UC_OrdFeminine	0xaa
#define FB_UC_GuillemotLeft	0xab
#define FB_UC_NotSign		0xac
#define FB_UC_SoftHyphen	0xad
#define FB_UC_Registered	0xae
#define FB_UC_Macron		0xaf
#define FB_UC_Degree		0xb0
#define FB_UC_PlusMinus		0xb1
#define FB_UC_TwoSuperior	0xb2
#define FB_UC_ThreeSuperior	0xb3
#define FB_UC_Acute		0xb4
#define FB_UC_Mu		0xb5
#define FB_UC_Micro		FB_UC_Mu
#define FB_UC_Paragraph		0xb6
#define FB_UC_Pilcrow		FB_UC_Paragraph
#define FB_UC_PeriodCentered	0xb7
#define FB_UC_MiddleDot		FB_UC_PeriodCentered
#define FB_UC_Cedilla		0xb8
#define FB_UC_OneSuperior	0xb9
#define FB_UC_mKuline		0xba
#define FB_UC_GuillemotRight	0xbb
#define FB_UC_OneQuarter	0xbc
#define FB_UC_OneHalf		0xbd
#define FB_UC_ThreeQuarters	0xbe
#define FB_UC_QuestionDown	0xbf
#define FB_UC_Agrave		0xc0
#define FB_UC_Aacute		0xc1
#define FB_UC_Acircumflex	0xc2
#define FB_UC_Atilde		0xc3
#define FB_UC_Adiaeresis	0xc4
#define FB_UC_Aumlaut		FB_UC_Adiaeresis
#define FB_UC_Aring		0xc5
#define FB_UC_AE		0xc6
#define FB_UC_Ccedilla		0xc7
#define FB_UC_Egrave		0xc8
#define FB_UC_Eacute		0xc9
#define FB_UC_Ecircumflex	0xca
#define FB_UC_Ediaeresis	0xcb
#define FB_UC_Eumlaut		FB_UC_Ediaeresis
#define FB_UC_Igrave		0xcc
#define FB_UC_Iacute		0xcd
#define FB_UC_Icircumflex	0xce
#define FB_UC_Idiaeresis	0xcf
#define FB_UC_Iumlaut		FB_UC_Idiaeresis
#define FB_UC_ETH		0xd0
#define FB_UC_Ntilde		0xd1
#define FB_UC_Ograve		0xd2
#define FB_UC_Oacute		0xd3
#define FB_UC_Ocircumflex	0xd4
#define FB_UC_Otilde		0xd5
#define FB_UC_Odiaeresis	0xd6
#define FB_UC_Oumlaut		FB_UC_Odiaeresis
#define FB_UC_Multiply		0xd7
#define FB_UC_Ooblique		0xd8
#define FB_UC_Ugrave		0xd9
#define FB_UC_Uacute		0xda
#define FB_UC_Ucircumflex	0xdb
#define FB_UC_Udiaeresis	0xdc
#define FB_UC_Uumlaut		FB_UC_Udiaeresis
#define FB_UC_Yacute		0xdd
#define FB_UC_THORN		0xde
#define FB_UC_ssharp		0xdf
#define FB_UC_agrave		0xe0
#define FB_UC_aacute		0xe1
#define FB_UC_acircumflex	0xe2
#define FB_UC_atilde		0xe3
#define FB_UC_adiaeresis	0xe4
#define FB_UC_aumlaut		FB_UC_adiaeresis
#define FB_UC_aring		0xe5
#define FB_UC_ae		0xe6
#define FB_UC_ccedilla		0xe7
#define FB_UC_egrave		0xe8
#define FB_UC_eacute		0xe9
#define FB_UC_ecircumflex	0xea
#define FB_UC_ediaeresis	0xeb
#define FB_UC_eumlaut		FB_UC_ediaeresis
#define FB_UC_igrave		0xec
#define FB_UC_iacute		0xed
#define FB_UC_icircumflex	0xee
#define FB_UC_idiaeresis	0xef
#define FB_UC_iumlaut		FB_UC_idiaeresis
#define FB_UC_eth		0xf0
#define FB_UC_ntilde		0xf1
#define FB_UC_ograve		0xf2
#define FB_UC_oacute		0xf3
#define FB_UC_ocircumflex	0xf4
#define FB_UC_otilde		0xf5
#define FB_UC_odiaeresis	0xf6
#define FB_UC_oumlaut		FB_UC_odiaeresis
#define FB_UC_Division		0xf7
#define FB_UC_oslash		0xf8
#define FB_UC_ugrave		0xf9
#define FB_UC_uacute		0xfa
#define FB_UC_ucircumflex	0xfb
#define FB_UC_udiaeresis	0xfc
#define FB_UC_uumlaut		FB_UC_udiaeresis
#define FB_UC_yacute		0xfd
#define FB_UC_thorn		0xfe
#define FB_UC_ydiaeresis	0xff
#define FB_UC_yumlaut		FB_UC_ydiaeresis

/* Dead keys */
#define FB_K_DeadRing			FB_KEY(FB_KT_DEAD, 0x00)
#define FB_K_DeadCaron			FB_KEY(FB_KT_DEAD, 0x01)
#define FB_K_DeadOgonek			FB_KEY(FB_KT_DEAD, 0x02)
#define FB_K_DeadIota			FB_KEY(FB_KT_DEAD, 0x03)
#define FB_K_DeadDoubleAcute		FB_KEY(FB_KT_DEAD, 0x04)
#define FB_K_DeadBreve			FB_KEY(FB_KT_DEAD, 0x05)
#define FB_K_DeadAboveDot		FB_KEY(FB_KT_DEAD, 0x06)
#define FB_K_DeadBelowDot		FB_KEY(FB_KT_DEAD, 0x07)
#define FB_K_DeadVoicedSound		FB_KEY(FB_KT_DEAD, 0x08)
#define FB_K_DeadSemiVoicedSound	FB_KEY(FB_KT_DEAD, 0x09)

#define FB_K_DeadAcute			FB_KEY(FB_KT_DEAD, FB_UC_Acute)
#define FB_K_DeadCedilla		FB_KEY(FB_KT_DEAD, FB_UC_Cedilla)
#define FB_K_DeadCircumflex		FB_KEY(FB_KT_DEAD, FB_UC_Circumflex)
#define FB_K_DeadDiaeresis		FB_KEY(FB_KT_DEAD, FB_UC_Diaeresis)
#define FB_K_DeadGrave			FB_KEY(FB_KT_DEAD, FB_UC_Grave)
#define FB_K_DeadTilde			FB_KEY(FB_KT_DEAD, FB_UC_Tilde)
#define FB_K_DeadMacron			FB_KEY(FB_KT_DEAD, FB_UC_Macron)

#endif /* _OFBIS_KEYBOARD_H */
