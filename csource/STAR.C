/*
** STAR.C
**
** INPUT
**  None
**
** OUTPUT
**  Graphic demo of fractal
**
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <graphics.h>
#include <netbios.h>
#include <dos.h>



int main(int argc,char *argv[])
{
	int g_driver,g_mode,g_error;
	int x,y,r;
	detectgraph(&g_driver,&g_mode);
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	printf("Enter r:\n");
	scanf("%d",&r);
	initgraph(&g_driver,&g_mode,"");
	x = getmaxx()/2;
	y = getmaxy()/2;
	star(x,y,r);
	delay(0);
	getch();
	restorecrtmode();
}

star(int x,int y, int r)
{
	if (r>2)
	{
		star(x-r,y+r,r/2);
		star(x+r,y+r,r/2);
		star(x-r,y-r,r/2);
		star(x+r,y-r,r/2);
		box(x,y,r);
	}
}

box(int x,int y,int r)
{
	rectangle(x-r,y-r,x+r,y+r);
}