/* Framebuffer library headers and prototypes */

#ifndef _OFBIS_H_
#define _OFBIS_H_

#include <linux/fb.h>

/* Options for FBsetwritemode */
typedef enum {
  FB_REPLACE,
  FB_TRANS,
  FB_XOR,
  FB_ERASE
} FBWRITEMODE;

typedef struct fbcmap
{
  int              start;
  int              end;
  int              len;
  unsigned short * red;
  unsigned short * green;
  unsigned short * blue;
} FBCMAP;

typedef struct fbinf FB;

/* bitblt declarations */
typedef struct  bbpb
{
  unsigned short   b_wd;       /* width of block to blit (pixels) */
  unsigned short   b_ht;       /* height of block to blit (pixels) */
  unsigned short   plane_ct;   /* number of planes to blit */
  unsigned short   fg_col;     /* foreground colour */
  unsigned short   bg_col;     /* background colour */
  unsigned int     op_tab;     /* logic op table */
  unsigned short   s_xmin;     /* minimum X source */
  unsigned short   s_ymin;     /* minimum Y source */
  unsigned short * s_form;     /* source form base addr */
  unsigned short   s_nxwd;     /* offset to next word in line (bytes) */
  unsigned short   s_nxln;     /* offset to next line in plane (bytes) */
  unsigned short   s_nxpl;     /* offset from start of current plane to next */
  unsigned short   d_xmin;     /* minimum X destination */
  unsigned short   d_ymin;     /* minimum Y destination */
  unsigned short * d_form;     /* destination form base addr */
  unsigned short   d_nxwd;     /* offset to next word in line (bytes) */
  unsigned short   d_nxln;     /* offset to next line in plane (bytes) */
  unsigned short   d_nxpl;     /* offset from start of current plane to next */
  unsigned short * p_addr;     /* address of pattern buffer */
  unsigned short   p_nxln;     /* offset to next line in pattern (bytes) */
  unsigned short   p_nxpl;     /* offset to next plane in pattern (bytes) */
  unsigned short   p_mask;     /* pattern index mask */
  unsigned short   space[12];  /* reserved */
} FBBLTPBLK;

typedef struct fbblock
{
  void          *addr;          /* Pointer to start of data block */
  int           width;          /* Width of block */
  int           height;         /* Height of block */
} FBBLOCK;

typedef struct fbfont
{
  unsigned char *data;
  int           width;
  int           height;
} FBFONT;

struct fbinf
{
  int                      fb;             /* fb file descriptor */
  int                      tty;            /* tty file desc. */
  int                      ttyno;          /* VT number */
  unsigned short           vtchoice;       /* Current VT or new one? */
  unsigned short           handle_kbd;     /* Open raw kbd and handle vt 
                                           ** switching? */
  unsigned short           use_backing;    /* Draw to backing store when
                                           ** vt is not visible ? */
  unsigned short           keeptty;        /* Keep current tty (debug) */
  unsigned short           visible;        /* Is VT visible? */
  unsigned short           drawing;        /* Drawing now? */
  unsigned short *         sbuf;           /* addr of frame buf */
  unsigned short *         sbak;           /* addr of backing store */
  struct fb_fix_screeninfo finf;           /* fixed screen info */
  struct fb_var_screeninfo vinf;           /* variable screen info */
  FBCMAP *                 cmap;           /* current colourmap */
  FBFONT *                 font;           /* current font */
  FBWRITEMODE              writemode;      /* writemode */

  /* Drawing functions */

  void          (*putpixel) (FB *           f,
                             unsigned short x,
                             unsigned short y,
                             unsigned long  col);
  unsigned long (*getpixel) (FB *           f,
                             unsigned short x,
                             unsigned short y);
  void          (*hline)    (FB *           f,
                             unsigned short x1,
                             unsigned short x2,
                             unsigned short y,
                             unsigned long  col);
  void          (*line)     (FB *           f,
                             unsigned short x1,
                             unsigned short x2,
                             unsigned short y1,
                             unsigned short y2,
                             unsigned long  col);
  void          (*bitblt)   (FB *        f,
                             FBBLTPBLK * fbb);
  void          (*putchar)  (FB *f,
                             unsigned short x,
                             unsigned short y,
                             unsigned long  fgcol,
                             unsigned long  bgcol,
                             unsigned char  ch);
  void          (*sp8_convert) (FB *      f,
                                FBBLOCK * srcblock,
                                FBBLOCK * dstblock);
};

/* misc defines */

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

/* init/deinit functions */

/* options for FBopen */

#define FB_KEEP_CURRENT_VC  0x0000
#define FB_OPEN_NEW_VC      0x0001
#define FB_NO_KBD           0x0002
#define FB_NO_BACKING_STORE 0x0004

FB      *FBopen(const char *fbname, unsigned short opts);
int     FBclose(FB *f);

/* VT handling functions */

void    FBVTopen(FB *f);
void    FBVTclose(FB *f);
void    FBVTswitch(int s);
void    FBVTswitchoriginal(FB *f);
void    FBVTswitchfb(FB *f);
extern unsigned short switching;

/* lowlevel drawing functions */

void FBsetwritemode (FB *        f,
                     FBWRITEMODE mode);

#define FB_ATOMIC(f,x)          { f->drawing=TRUE; x; f->drawing=FALSE; if (switching) FBVTswitch(0); }

#define FBputpixel(f,x,y,col)   FB_ATOMIC(f,(*(f->putpixel))(f,x,y,col))

static inline unsigned long
FBgetpixel( FB *f, unsigned short x, unsigned short y)
{ unsigned long col; f->drawing=TRUE; col=(*(f->getpixel))(f,x,y); f->drawing=FALSE; if (switching) FBVTswitch(0); return col; }

#define FBhline(f,x1,x2,y,col)  FB_ATOMIC(f,(*(f->hline))(f,x1,x2,y,col))

#define FBline(f,x1,y1,x2,y2,col)       FB_ATOMIC(f,(*(f->line))(f,x1,y1,x2,y2,col))

#define FBbitblt(f,fbb) FB_ATOMIC(f,(*(f->bitblt))(f,fbb))

#define FBputchar(f,x,y,fgcol,bgcol,ch) FB_ATOMIC(f,(*(f->putchar))(f,x,y,fgcol,bgcol,ch))

#define FBchunk_to_native(f,srcblock,dstblock)  FB_ATOMIC(f,(*(f->sp8_convert))(f,srcblock,dstblock))

/* screen settings functions */

void    FBgetfix(FB *f);
void    FBgetvar(FB *f);
void    FBputvar(FB *f);

/* colourmap functions */

FBCMAP  *FBgetcmap(FB *f);
void    FBputcmap(FB *f, FBCMAP *fbcmap);
void    FBfreecmap(FBCMAP *fbcmap);

unsigned long FBc24_to_cnative(FB *f, unsigned long col24);
unsigned long FBcnative_to_c24(FB *f, unsigned long col);

/* bitblt functions */

FBBLTPBLK       *FBgetbltpblk(FB *f);
void    FBfreebltpblk(FBBLTPBLK *fbb);

/* font function */

void    FBsetfont(FB *f, FBFONT *newfont);

/* built-in font declarations */

extern unsigned char fontdata_6x11[];
extern int fontwidth_6x11, fontheight_6x11;

extern unsigned char fontdata_8x8[];
extern int fontwidth_8x8, fontheight_8x8;

extern unsigned char fontdata_8x16[];
extern int fontwidth_8x16, fontheight_8x16;

/* debug functions */

void    FBfinfdump(FB *f);
void    FBvinfdump(FB *f);
void    FBcmapdump(FBCMAP *f);

/* event declarations and functions */

enum    eventtype
{
  FBNoEvent,
  FBKeyEvent,
  FBMouseEvent
};

typedef struct fb_keyevt
{
  enum eventtype  type;
  unsigned int    state;
  unsigned int    keycode;
  unsigned int    ascii;
} FBKEYEVENT;

typedef struct fb_mseevt
{
  enum eventtype  type;
  int             x;
  int             y;
  unsigned int    state;
  unsigned int    buttons;
} FBMOUSEEVENT;

typedef union fb_event
{
  enum eventtype  type;
  FBKEYEVENT      key;
  FBMOUSEEVENT    mouse;
} FBEVENT;

void    FBgetevent( FB *f, FBEVENT *ev );
void    FBcheckevent( FB *f, FBEVENT *ev, int timeout );
int     FBgetchar( FB *f );

#endif /* _OFBIS_H_ */
