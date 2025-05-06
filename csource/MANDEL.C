/*
** MANDEL.C
**
** INPUT
**  None
**
** OUTPUT
**  Graphic demo of mandlebrot set.
**
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <graphics.h>


#define XMAX 100
#define YMAX 100
#define MAXCOUNT 16


int main()
{
	int test,g_driver,g_mode,g_error;
	int x,y;
	float xscale,yscale;
	float left,top;
	float xside,yside;
	float zx,zy;
	float cx,cy;
	float tempx;
	int count;

	left = -1.3;      /* entire set -2 to 0.5 on x and -1.25 to 1.25 on y */
	top = -.23;       /* change parms to explore set */
	xside=-0.25;
	yside = 0.4;
	xscale = xside/XMAX;
	yscale = yside/YMAX;
	detectgraph(&g_driver,&g_mode);
	g_mode = 1; /* Override to EGA */
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"");
	delay(0);
	rectangle(0,0,XMAX+1,YMAX+1);

	for (y=1;y<=YMAX;y++)
	{
		for (x=1;x<=XMAX;x++)
		{
			cx=x*xscale+left;
			cy=y*yscale+top;
			zx=zy=0;
			count=0;

			while (zx*zx+zy*zy<4 && count<MAXCOUNT)
			{
				tempx = zx*zx - zy*zy +cx;
				zy = 2*zx*zy + cy;
				zx = tempx;
				count++;
			}
			if (count<MAXCOUNT)
			{
				putpixel(x,y,count);
			}
			if (kbhit())
				break;
		}
	}
	getch();
	closegraph();
}



