#include <stdio.h>
#include "libfb.h"

void
FBfinfdump ( FB *f )
{
  printf("Fixed info:\n\nid: %s\n",f->finf.id);
  printf("smem_start: %p\n",f->finf.smem_start);
  printf("smem_len: %d\n",f->finf.smem_len);
  printf("type: %d\n",f->finf.type);
  printf("type_aux: %d\n",f->finf.type_aux);
  printf("visual: %d\n",f->finf.visual);
  printf("xpanstep: %hd\n",f->finf.xpanstep);
  printf("ypanstep: %hd\n",f->finf.ypanstep);
  printf("ywrapstep: %hd\n",f->finf.ywrapstep);
  printf("line length in bytes: %d\n",f->finf.line_length);
  printf("---------------------------\n");
}

void
FBvinfdump( FB *f)
{
  printf("xres is %d, yres is %d\n",f->vinf.xres,f->vinf.yres);
  printf("xres virtual is %d, yres virtual is %d\n",f->vinf.xres_virtual, f->vinf.yres_virtual);
  printf("xoffset is %d, yoffset is %d\n",f->vinf.xoffset, f->vinf.yoffset);
  printf("bits per pixel: %d\n", f->vinf.bits_per_pixel);
  printf("red length is: %d\n", f->vinf.red.length);
  printf("green length is: %d\n", f->vinf.green.length);
  printf("blue length is: %d\n", f->vinf.blue.length);
  printf("transp length is: %d\n", f->vinf.transp.length);
  printf("activate set to: %d\n", f->vinf.activate);
}

void
FBcmapdump( FBCMAP *f )
{
	int i;

	printf("start is %d, len is %d\n",f->start,f->len);
	printf("end is %d\n",f->end);
	for (i=f->start;i<f->len;i++)
	{
		printf("red entry %d is %hx\n",i,f->red[i]);
		printf("green entry %d is %hx\n",i,f->green[i]);
		printf("blue entry %d is %hx\n",i,f->blue[i]);
	}
	/*if (f->cmap.transp==NULL)
	{
		printf("transp is NULL\n");
	}*/
}
