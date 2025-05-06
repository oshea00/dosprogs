/*
** ELIPSE.C
**
** This program is a tool for drawing mathematical
** graphs.
**
** INPUT
**   The formula is coded in "formula" function
**   A range of X values is asked for and the
**   graph is drawn.
**
** OUTPUT
**   X,Y graph of a function.
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <graphics.h>

typedef struct
{
	int x;
	int y;
} coord_array;

int main(int,char *[]);
void c_break(void);
void draw_graph(void);
void formula(void);
int func(int,int,int);

int maxx=0,maxy=0,orgx=0,orgy=0;

int main(int argc,char *argv[])
{
	int i,j,x,y;
	int g_driver,g_mode,g_error;
	detectgraph(&g_driver,&g_mode);
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(small_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"");
	maxx = getmaxx();
	maxy = getmaxy();
	orgx = maxx/2;
	orgy = maxy/2;
	settextstyle(SMALL_FONT,0,0);
	formula();
	c_break();
}

void draw_graph(void)
{
	int i;
	setcolor(WHITE);
	line(0,orgy,maxx,orgy);
	line(orgx,0,orgx,maxy);
	for (i=orgx;i<maxx;i+=10)
	{
		line(i,orgy-3,i,orgy+3);
	}
	for (i=orgx;i>0;i-=10)
	{
		line(i,orgy-3,i,orgy+3);
	}
	for (i=orgy;i<maxy;i+=10)
	{
		line(orgx-3,i,orgx+3,i);
	}
	for (i=orgy;i<maxy;i-=10)
	{
		line(orgx-3,i,orgx+3,i);
	}
}

void formula(void)
{
	int i,ar,br,xmin,xmax,y;
	printf("Enter the min and max values of X to test: ");
	scanf("%d,%d",&xmin,&xmax);

	/* enter constants here */

	printf("Enter aradius, bradius: ");
	scanf("%d,%d",&ar,&br);

	/* Setup graph */

	clearviewport();
	draw_graph();

	/* Plot graph */

	for (i=xmin;i<=xmax;i++)
	{
		y = func(i,ar,br);
		putpixel(i+orgx,orgy-y,WHITE);

		/* If plus/or minus function */
		y*=-1;
		putpixel(i+orgx,orgy-y,WHITE);
	}
}

int func(int x,int ar,int br)
{
	double dy=0,dx=0,dr=0;
	dx = (double) x;
	dr = (double) ar;
	return (int) ( br * sqrt( ( 1.0 - pow(dx,2.0)/pow(dr,2.0) )  ) );
}

void c_break(void)
{
	char string[80];
	setcolor(LIGHTGREEN);
	fflush(stdin);
	outtextxy(500,450,"Hit <ENTER> key to");
	outtextxy(500,459,"return to DOS.");
	gets(string);
	closegraph();
	restorecrtmode();
	printf("Program aborted....\n");
	exit(0);
}




