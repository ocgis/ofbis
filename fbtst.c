#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "src/libfb.h"

#define BLITTEST
#define CHARTEST
#define CMAPTEST
#undef EVENTTEST

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

FB	*f;

static void signal_handler(int sig)
{
  fprintf(stderr,"fbtst: signal %d caught and exits program!\n", sig);
  FBclose(f);;
  exit(1);
}
      
static void setup_signals()
{
  sigset_t mask;
  struct sigaction sigs;
          
  sigemptyset(&mask);
            
  sigs.sa_handler = signal_handler;
  sigs.sa_mask = mask;
  sigs.sa_flags = SA_ONESHOT;
  sigs.sa_restorer = NULL;
             
  sigaction(SIGHUP, &sigs, NULL);
  sigaction(SIGINT, &sigs, NULL);
  sigaction(SIGQUIT, &sigs, NULL);
  sigaction(SIGILL, &sigs, NULL);
  sigaction(SIGTRAP, &sigs, NULL);
  sigaction(SIGBUS, &sigs, NULL);
  sigaction(SIGFPE, &sigs, NULL);
  sigaction(SIGSEGV, &sigs, NULL);
  sigaction(SIGTERM, &sigs, NULL);
}
                                      

int
main( int argc, char **argv )
{
#ifdef BLITTEST
  int	i,j=0;
  long	c;
#endif /* BLITTEST */
  
#ifdef CMAPTEST
  FBCMAP *cmap;
  int    k;
#endif 

#ifdef EVENTTEST
#define FB_OPT FB_OPEN_NEW_VC
#else
#define FB_OPT FB_OPEN_NEW_VC | FB_NO_KBD
#endif

  if ((f=FBopen(NULL,FB_OPT /*FB_KEEP_CURRENT_VC*/))==NULL) {
    printf("FBinit f failed");
  }

  fprintf (stderr, "xres_virtual=%d\n", f->vinf.xres_virtual);

  setup_signals();
  

#ifdef BLITTEST
  
  printf("fbtst\n");
  /*for (i=0,c=0L;i<320;c++,i++) {
    for (k=0;k<480;k++)
      FBputpixel(f,i,k,c);
  }*/

  /*for (i=0,c=0L;i<344;c++,i++) {
    for (k=0;k<462;k++)
      FBputpixel(f,i,k,c);
  }*/

  /*for (i=0,c=0L;i<231;c++,i++) {
    FBputpixel(f,344,i,c);
  }*/

  for ( i=0, c=0L; i<480;c++,i++) {
    FBhline(f,0,319,i,c);
  }
  
  /*for ( i=0, c=0L; i<210 ; i+=10,c++) {
    FBline(f,320,0,639,i,c);
  }*/

  /*for (i=0;i<64;i++) {
    printf("%ld\n",FBgetpixel(f,i,10));
  }*/
	
  /*FBfinfdump(f);
  FBvinfdump(f);*/

  {
    unsigned short sx,sy,dx,dy,wd,ht;
    unsigned short bbtest[42][6] = {
      /* no skew, no endmasks */
      { 16, 16, 336, 336, 64, 64 },
      /* no skew, endmasks needed */
      { 24, 16, 344, 336, 64, 64 },
      /* skew, no endmasks */
      { 10, 10, 336, 336, 17, 17 },
      /* half screen blit */
      { 1, 1, 320, 1, 318, 478 },
      { 0, 0, 320, 0, 320, 480 },
      { 0, 0, 0, 100, 320, 100 },
      /* small r->l blit, aligned, no skew */
      { 256, 136, 320, 136, 64, 16 },
      /* small r->l blit, endmasks needed, no skew */
      { 264, 136, 328, 136, 64, 16 },
      /* small r->l blit, no endmasks, skew */
      { 255, 136, 320, 136, 64, 16 },
      { 241, 136, 336, 136, 64, 16 },
      /* small byte blit */
      { 308, 136, 324, 136, 8, 16 },
      { 236, 136, 324, 136, 8, 16 },
      { 308, 136, 332, 136, 8, 16 },
      { 0, 10, 320, 136, 64, 20 },
      { 0, 10, 319, 136, 64, 20 },
      { 0, 10, 321, 136, 64, 20 },
      /* overlapping ones */
      { 16, 16, 7, 2, 64, 64 },
      { 16, 16, 28, 2, 64, 64 },
      { 16, 16, 0, 30, 64, 64 },
      { 16, 16, 48, 30, 64, 64 },
      
      { 19, 6, 280, 380, 100, 100 },
      { 0, 6, 281, 380, 100, 100 },
      { 1, 6, 281, 380, 100, 100 },
      { 2, 6, 281, 380, 100, 100 },
      { 3, 6, 281, 380, 100, 100 },
      { 4, 6, 281, 380, 100, 100 },
      { 5, 6, 281, 380, 100, 100 },
      { 6, 6, 281, 380, 100, 100 },
      { 7, 6, 281, 380, 100, 100 },
      { 8, 6, 281, 380, 100, 100 },
      { 9, 6, 281, 380, 100, 100 },
      { 10, 6, 281, 380, 100, 100 },
      { 11, 6, 281, 380, 100, 100 },
      { 12, 6, 281, 380, 100, 100 },
      { 13, 6, 281, 380, 100, 100 },
      { 14, 6, 281, 380, 100, 100 },
      { 15, 6, 281, 380, 100, 100 },
      { 16, 6, 281, 380, 100, 100 },
      { 17, 6, 281, 380, 100, 100 },
      { 18, 6, 281, 380, 100, 100 }
    };
    
    if (argc > 1) {
      j = atoi(argv[1]);
    }
    
    if(j < 0) {
      sx=atoi(argv[2]);
      sy=atoi(argv[3]);
      dx=atoi(argv[4]);
      dy=atoi(argv[5]);
      wd=atoi(argv[6]);
      ht=atoi(argv[7]);
    } else {
      j %= 40;
      sx=bbtest[j][0];
      sy=bbtest[j][1];
      dx=bbtest[j][2];
      dy=bbtest[j][3];
      wd=bbtest[j][4];
      ht=bbtest[j][5];
    }
    
    {
      /*
      FBBLTPBLK fbb={ wd, ht, 0, 0, 0, 0x03030303,
		      sx, sy, NULL, 0, 0, 0,
		      dx, dy, NULL, 0, 0, 0,
		      NULL, 0, 0, 0,
		      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
      */
      
      FBBLTPBLK *fbb;
      
      
      FBline(f,sx,sy,sx+(wd-1),sy,10L);
      FBline(f,sx+(wd-1),sy,sx+(wd-1),sy+(ht-1),10L);
      FBline(f,sx+(wd-1),sy+(ht-1),sx,sy+(ht-1),10L);
      FBline(f,sx,sy+(ht-1),sx,sy,10L);
      FBline(f,sx,sy,sx+(wd-1),sy+(ht-1),10L);
      FBline(f,sx,sy+(ht-1),sx+(wd-1),sy,10L);
      
      fbb = FBgetbltpblk(f);
      fbb->b_wd = wd;
      fbb->b_ht = ht;
      fbb->s_xmin = sx;
      fbb->s_ymin = sy;
      fbb->d_xmin = dx;
      fbb->d_ymin = dy;
      FBbitblt(f,fbb);
      FBfreebltpblk(fbb);
      
      FBputpixel(f,dx,dy,10L);
      FBputpixel(f,dx+(wd-1),dy,10L);
      FBputpixel(f,dx+(wd-1),dy+(ht-1),10L);
      FBputpixel(f,dx,dy+(ht-1),10L);
      
    }
  }
#endif /* BLITTEST */
  
#ifdef CHARTEST
#define puttext(x,y)  \
  FBputchar(f, fnt.width*0+x,y, 1,0, 'N'); \
  FBputchar(f, fnt.width*1+x,y, 2,0, 'o'); \
  FBputchar(f, fnt.width*2+x,y, 3,0, 'C'); \
  FBputchar(f, fnt.width*3+x,y, 4,0, 'r'); \
  FBputchar(f, fnt.width*4+x,y, 5,0, 'e'); \
  FBputchar(f, fnt.width*5+x,y, 6,0, 'w'); \
  FBputchar(f, fnt.width/2+fnt.width*0+x,fnt.height+y, 7,0, 'R'); \
  FBputchar(f, fnt.width/2+fnt.width*1+x,fnt.height+y, 8,0, 'u'); \
  FBputchar(f, fnt.width/2+fnt.width*2+x,fnt.height+y, 9,0, 'l'); \
  FBputchar(f, fnt.width/2+fnt.width*3+x,fnt.height+y, 10,0, 'e'); \
  FBputchar(f, fnt.width/2+fnt.width*4+x,fnt.height+y, 11,0, 's');
  {  
    FBFONT fnt;
    
    fnt.data = fontdata_6x11;
    fnt.width = fontwidth_6x11;
    fnt.height = fontheight_6x11;
    FBsetfont(f, &fnt);
    puttext(0,0);
    
    fnt.data = fontdata_8x8;
    fnt.width = fontwidth_8x8;
    fnt.height = fontheight_8x8;
    FBsetfont(f, &fnt);
    puttext(100,100);
    
    fnt.data = fontdata_8x16;
    fnt.width = fontwidth_8x16;
    fnt.height = fontheight_8x16;
    FBsetfont(f, &fnt);
    puttext(200,50);
    
  }
#endif /* CHARTEST */

#ifdef CMAPTEST  
  cmap=FBgetcmap(f);
  FBcmapdump(cmap);
  FBfreecmap(cmap);
  /*getchar();*/
#endif /* CMAPTEST */

#ifdef EVENTTEST
  {
    int  i = 0;
    char ch;
    fprintf(stderr, "fbtst: before FBgetchar\n");

    while (((ch=FBgetchar(f)) != 'a') && (i < 5)) {
      fprintf(stderr, "fbtst: char=0x%x %c\n", ch, ch);
      i++;
    }

    fprintf(stderr, "fbtst: after FBgetchar\n");
  }
#else
  sleep (5);
#endif /* EVENTTEST */

  FBclose(f);
  
  return 0;
}
