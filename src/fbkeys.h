#ifndef _FBKEYS_H
#define _FBKEYS_H

#define GLYPHS_PER_KEY	4
#define NUM_KEYCODES	150
#define NUM_STD_KEYCODES 127
#define MIN_KEYCODE     8
#define MAX_KEYCODE     (NUM_KEYCODES + MIN_KEYCODE - 1)
#define MAX_STD_KEYCODE (NUM_STD_KEYCODES + MIN_KEYCODE - 1)

#define AltMask		Mod1Mask
#define NumLockMask	Mod2Mask
#define AltLangMask	Mod3Mask
#define KanaMask	Mod4Mask
#define ScrollLockMask	Mod5Mask

#define KeyPressed(k) (keyc->down[k >> 3] & (1 << (k & 7)))
#define ModifierDown(k) ((state & (k)) == (k))

#define XF86XK_ModeLock	0x1008FF01                 /* Mode Switch Lock */

#define Mode_RShift	0x0001
#define Mode_LShift	0x0002
#define Mode_Ctrl	0x0004
#define Mode_Alt	0x0008
#define Mode_Caps	0x0010
#define Mode_Home	0x0020
#define Mode_Insert	0x0040

/*
 * definition of the AT84/MF101/MF102 Keyboard:
 * ============================================================
 *       Defined             Key Cap Glyphs       Pressed value
 *      Key Name            Main       Also       (hex)    (dec)
 *      ----------------   ---------- -------    ------    ------
 */

#define KEY_Escape       /* Escape                0x01  */    1  
#define KEY_1            /* 1           !         0x02  */    2 
#define KEY_2            /* 2           @         0x03  */    3 
#define KEY_3            /* 3           #         0x04  */    4 
#define KEY_4            /* 4           $         0x05  */    5 
#define KEY_5            /* 5           %         0x06  */    6 
#define KEY_6            /* 6           ^         0x07  */    7 
#define KEY_7            /* 7           &         0x08  */    8 
#define KEY_8            /* 8           *         0x09  */    9 
#define KEY_9            /* 9           (         0x0a  */   10 
#define KEY_0            /* 0           )         0x0b  */   11 
#define KEY_Minus        /* - (Minus)   _ (Under) 0x0c  */   12
#define KEY_Equal        /* = (Equal)   +         0x0d  */   13 
#define KEY_BackSpace    /* Back Space            0x0e  */   14 
#define KEY_Tab          /* Tab                   0x0f  */   15
#define KEY_Q            /* Q                     0x10  */   16
#define KEY_W            /* W                     0x11  */   17
#define KEY_E            /* E                     0x12  */   18
#define KEY_R            /* R                     0x13  */   19
#define KEY_T            /* T                     0x14  */   20
#define KEY_Y            /* Y                     0x15  */   21
#define KEY_U            /* U                     0x16  */   22
#define KEY_I            /* I                     0x17  */   23
#define KEY_O            /* O                     0x18  */   24
#define KEY_P            /* P                     0x19  */   25
#define KEY_LBrace       /* [           {         0x1a  */   26
#define KEY_RBrace       /* ]           }         0x1b  */   27 
#define KEY_Enter        /* Enter                 0x1c  */   28
#define KEY_LCtrl        /* Ctrl(left)            0x1d  */   29
#define KEY_A            /* A                     0x1e  */   30
#define KEY_S            /* S                     0x1f  */   31
#define KEY_D            /* D                     0x20  */   32 
#define KEY_F            /* F                     0x21  */   33
#define KEY_G            /* G                     0x22  */   34
#define KEY_H            /* H                     0x23  */   35
#define KEY_J            /* J                     0x24  */   36
#define KEY_K            /* K                     0x25  */   37
#define KEY_L            /* L                     0x26  */   38
#define KEY_SemiColon    /* ;(SemiColon) :(Colon) 0x27  */   39
#define KEY_Quote        /* ' (Apostr)  " (Quote) 0x28  */   40
#define KEY_Tilde        /* ` (Accent)  ~ (Tilde) 0x29  */   41
#define KEY_ShiftL       /* Shift(left)           0x2a  */   42
#define KEY_BSlash       /* \(BckSlash) |(VertBar)0x2b  */   43
#define KEY_Z            /* Z                     0x2c  */   44
#define KEY_X            /* X                     0x2d  */   45
#define KEY_C            /* C                     0x2e  */   46
#define KEY_V            /* V                     0x2f  */   47
#define KEY_B            /* B                     0x30  */   48
#define KEY_N            /* N                     0x31  */   49
#define KEY_M            /* M                     0x32  */   50
#define KEY_Comma        /* , (Comma)   < (Less)  0x33  */   51
#define KEY_Period       /* . (Period)  >(Greater)0x34  */   52
#define KEY_Slash        /* / (Slash)   ?         0x35  */   53
#define KEY_ShiftR       /* Shift(right)          0x36  */   54
#define KEY_KP_Multiply  /* *                     0x37  */   55
#define KEY_Alt          /* Alt(left)             0x38  */   56
#define KEY_Space        /*   (SpaceBar)          0x39  */   57
#define KEY_CapsLock     /* CapsLock              0x3a  */   58
#define KEY_F1           /* F1                    0x3b  */   59
#define KEY_F2           /* F2                    0x3c  */   60
#define KEY_F3           /* F3                    0x3d  */   61
#define KEY_F4           /* F4                    0x3e  */   62
#define KEY_F5           /* F5                    0x3f  */   63
#define KEY_F6           /* F6                    0x40  */   64
#define KEY_F7           /* F7                    0x41  */   65
#define KEY_F8           /* F8                    0x42  */   66
#define KEY_F9           /* F9                    0x43  */   67
#define KEY_F10          /* F10                   0x44  */   68
#define KEY_NumLock      /* NumLock               0x45  */   69
#define KEY_ScrollLock   /* ScrollLock            0x46  */   70
#define KEY_ClrHome      /* ClrHome               0x47  */   71 
#define KEY_KP_8         /* 8           Up        0x48  */   72 
#define KEY_KP_9         /* 9           PgUp      0x49  */   73 
#define KEY_KP_Minus     /* - (Minus)             0x4a  */   74
#define KEY_KP_4         /* 4           Left      0x4b  */   75
#define KEY_KP_5         /* 5                     0x4c  */   76
#define KEY_KP_6         /* 6           Right     0x4d  */   77
#define KEY_KP_Plus      /* + (Plus)              0x4e  */   78
#define KEY_KP_1         /* 1           End       0x4f  */   79
#define KEY_KP_2         /* 2           Down      0x50  */   80
#define KEY_KP_3         /* 3           PgDown    0x51  */   81
#define KEY_Insert       /* Insert                0x52  */   82
#define KEY_KP_Decimal   /* . (Decimal) Delete    0x53  */   83 
#define KEY_SysReqest    /* SysReqest             0x54  */   84
                         /* NOTUSED               0x55  */
#define KEY_Less         /* < (Less)   >(Greater) 0x56  */   86
#define KEY_F11          /* F11                   0x57  */   87
#define KEY_F12          /* F12                   0x58  */   88

#define KEY_Prefix0      /* special               0x60  */   96
#define KEY_Undo         /* Undo                  0x61  */   97
#define KEY_Help         /* Help                  0x62  */   98

/*
 * The 'scancodes' below are generated by the server, because the MF101/102
 * keyboard sends them as sequence of other scancodes
 */
#define KEY_Home         /* Home                  0x59  */   89
#define KEY_Up           /* Up                    0x5a  */   90
#define KEY_PgUp         /* PgUp                  0x5b  */   91
#define KEY_Left         /* Left                  0x5c  */   92
#define KEY_Begin        /* Begin                 0x5d  */   93
#define KEY_Right        /* Right                 0x5e  */   94
#define KEY_End          /* End                   0x5f  */   95
#define KEY_Down         /* Down                  0x60  */   96
#define KEY_PgDown       /* PgDown                0x61  */   97
/*#define KEY_Insert*/       /* Insert                0x62  */   /*98*/
#define KEY_Delete       /* Delete                0x63  */   99
#define KEY_KP_Enter     /* Enter                 0x64  */  100
#define KEY_RCtrl        /* Ctrl(right)           0x65  */  101
#define KEY_KP_Asterisk  /* *                     0x66  */  102
#define KEY_Pause        /* Pause                 0x66  */  102
#define KEY_Print        /* Print                 0x67  */  103
#define KEY_KP_Divide    /* Didive                0x68  */  104
#define KEY_AltLang      /* AtlLang(right)        0x69  */  105
#define KEY_Break        /* Break                 0x6a  */  106
#define KEY_LMeta        /* Left Meta             0x6b  */  107
#define KEY_RMeta        /* Right Meta            0x6c  */  108
#define KEY_Menu         /* Menu                  0x6d  */  109
#define KEY_SN_KP_7      /* ServerNumLock 7       0x80  */  128
#define KEY_SN_KP_8      /* ServerNumLock 8       0x81  */  129
#define KEY_SN_KP_9      /* ServerNumLock 9       0x82  */  130
#define KEY_SN_KP_4      /* ServerNumLock 4       0x83  */  131
#define KEY_SN_KP_5      /* ServerNumLock 5       0x84  */  132
#define KEY_SN_KP_6      /* ServerNumLock 6       0x85  */  133
#define KEY_SN_KP_1      /* ServerNumLock 1       0x86  */  134
#define KEY_SN_KP_2      /* ServerNumLock 2       0x87  */  135
#define KEY_SN_KP_3      /* ServerNumLock 4       0x88  */  136
#define KEY_SN_KP_0      /* ServerNumLock 0       0x89  */  137
#define KEY_SN_KP_Dec    /* ServerNumLock Decimal 0x8a  */  138
#define KEY_SN_KP_Home   /* ServerNumLock Home    0x8b  */  139
#define KEY_SN_KP_Up     /* ServerNumLock Up      0x8c  */  140
#define KEY_SN_KP_Prior  /* ServerNumLock Prior   0x8d  */  141
#define KEY_SN_KP_Left   /* ServerNumLock Left    0x8e  */  142
#define KEY_SN_KP_Begin  /* ServerNumLock Begin   0x8f  */  143
#define KEY_SN_KP_Right  /* ServerNumLock Right   0x90  */  144
#define KEY_SN_KP_End    /* ServerNumLock End     0x91  */  145
#define KEY_SN_KP_Down   /* ServerNumLock Down    0x92  */  146
#define KEY_SN_KP_Next   /* ServerNumLock Next    0x93  */  147
#define KEY_SN_KP_Ins    /* ServerNumLock Ins     0x94  */  148
#define KEY_SN_KP_Del    /* ServerNumLock Del     0x95  */  149

#endif
