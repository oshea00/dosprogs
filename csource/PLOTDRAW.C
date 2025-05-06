/*
** PLOTDRAW.C
**
** INPUT
**  Keyboard commands
**
** OUTPUT
**  HP plotter commands for Graphic yourdon drawing program
**
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ENTER_KEY 0x0d
#define BACKSPACE 0x08
#define SQUARE 1
#define CIRCLE 2
#define FILEOBJ 3
#define ARROW 4
#define LINE 5
#define SQUAREXOFF 65
#define SQUAREYOFF 50
#define RADIUSX 40
#define RADIUSY 30
#define MAXSQUARES 30
#define MAXCIRCLES 30
#define MAXFILES 30
#define MAXARROWS 60
#define MAXLINES 60
#define ON 1
#define OFF 0
#define SR 1
#define FR 15
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
#define NONE 0


typedef struct {
int x;
int y;
char text[4][11];
} square_type;

typedef struct {
int x;
int y;
char text[4][11];
} circle_type;

typedef struct {
int x;
int y;
char text[1][14];
} file_type;

typedef struct {
int x1;
int y1;
int x2;
int y2;
int tx;
int ty;
char text[2][14];
} arrow_type;

typedef struct {
int x1;
int y1;
int x2;
int y2;
int tx;
int ty;
char text[2][14];
} line_type;

square_type squares[MAXSQUARES];
circle_type circles[MAXCIRCLES];
file_type files[MAXFILES];
arrow_type arrows[MAXARROWS];
line_type lines[MAXLINES];
square_type square_work;
circle_type circle_work;
file_type file_work;
arrow_type arrow_work;
line_type line_work;

char input[15];

int basex = 0;
int basey = 0;
int cursx = 6;
int cursy = 6;
int xsave = 6;
int ysave = 6;

int sqrcnt = 0;
int crccnt = 0;
int filcnt = 0;
int arwcnt = 0;
int lincnt = 0;
int max_xdistance = 0;
int max_ydistance = 0;

int main(void);
void draw_object(int);
void refresh(void);
void refresh_squares(int);
void refresh_circles(int);
void refresh_files(int);
void refresh_arrows(int);
void refresh_lines(int);
void file_store(int,int);
void print_line(int,int,int,int);
void get_save(void);
void get_drawing(void);
void get_filename(char *);
void print_circle(int,int,int);
void print_square(int,int,int,int);
void farthestxy(void);

int main(void)
{

	printf("IN;ES0,-.25;SC0,600,600,0,1;SP3;");
	get_save();
	farthestxy();
	printf("farthest x = %d y = %d",max_xdistance,max_ydistance);
	refresh();
	printf("PG;");
	return 0;
}

void draw_point(int x1,int y1,int x2,int y2)
{
	if (x2<x1&&y2<y1)
	{
		print_line(x2-basex,y2-basey,x2-basex,y2+3-basey);
		print_line(x2-basex,y2-basey,x2+4-basex,y2-basey);
	}
	else
	if (x2==x1&&y2<y1)
	{
		print_line(x2-basex,y2-basey,x2+3-basex,y2+3-basey);
		print_line(x2-basex,y2-basey,x2-3-basex,y2+3-basey);
	}
	else
	if (x2>x1&&y2<y1)
	{
		print_line(x2-basex,y2-basey,x2-4-basex,y2-basey);
		print_line(x2-basex,y2-basey,x2-basex,y2+3-basey);
	}
	else
	if (x2<x1&&y2==y1)
	{
		print_line(x2-basex,y2-basey,x2+3-basex,y2-3-basey);
		print_line(x2-basex,y2-basey,x2+3-basex,y2+3-basey);
	}
	else
	if (x2>x1&&y2==y1)
	{
		print_line(x2-basex,y2-basey,x2-3-basex,y2-3-basey);
		print_line(x2-basex,y2-basey,x2-3-basex,y2+3-basey);
	}
	else
	if (x2<x1&&y2>y1)
	{
		print_line(x2-basex,y2-basey,x2-basex,y2-3-basey);
		print_line(x2-basex,y2-basey,x2+4-basex,y2-basey);
	}
	else
	if (x2==x1&&y2>y1)
	{
		print_line(x2-basex,y2-basey,x2-3-basex,y2-3-basey);
		print_line(x2-basex,y2-basey,x2+3-basex,y2-3-basey);
	}
	else
	if (x2>x1&&y2>y1)
	{
		print_line(x2-basex,y2-basey,x2-basex,y2-3-basey);
		print_line(x2-basex,y2-basey,x2-4-basex,y2-basey);
	}
}

void refresh(void)
{
	int i;
	if (sqrcnt > 0)
	{
		for (i=0;i<sqrcnt;i++)
		{
			refresh_squares(i);
		}
	}

	if (crccnt > 0)
	{
		for (i=0;i<crccnt;i++)
		{
			refresh_circles(i);
		}
	}

	if (filcnt > 0)
	{
		for (i=0;i<filcnt;i++)
		{
			refresh_files(i);
		}
	}

	if (arwcnt > 0)
	{
		for (i=0;i<arwcnt;i++)
		{
			refresh_arrows(i);
		}
	}

	if (lincnt > 0)
	{
		for (i=0;i<lincnt;i++)
		{
			refresh_lines(i);
		}
	}
}

void refresh_squares(int i)
{
	cursx = squares[i].x;
	cursy = squares[i].y;
	draw_object(SQUARE);
	pouttextxy(cursx+2-basex,cursy+13-basey,&squares[i].text[0][0]);
	pouttextxy(0,0,&squares[i].text[1][0]);
	pouttextxy(0,0,&squares[i].text[2][0]);
	pouttextxy(0,0,&squares[i].text[3][0]);
}

void refresh_circles(int i)
{
	cursx = circles[i].x;
	cursy = circles[i].y;
	draw_object(CIRCLE);
	pouttextxy(cursx-30-basex,cursy-20-basey,&circles[i].text[0][0]);
	pouttextxy(0,0,&circles[i].text[1][0]);
	pouttextxy(0,0,&circles[i].text[2][0]);
	pouttextxy(0,0,&circles[i].text[3][0]);
}

void refresh_files(int i)
{
	cursx = files[i].x;
	cursy = files[i].y;
	draw_object(FILEOBJ);
	pouttextxy(cursx+2-basex,cursy+13-basey,&files[i].text[0][0]);
}

void refresh_arrows(int i)
{
	print_line(arrows[i].x1-basex,arrows[i].y1-basey,arrows[i].x2-basex,arrows[i].y2-basey);
	draw_point(arrows[i].x1,arrows[i].y1,arrows[i].x2,arrows[i].y2);
	pouttextxy(arrows[i].tx-basex,arrows[i].ty-basey,&arrows[i].text[0][0]);
	pouttextxy(0,0,&arrows[i].text[1][0]);
}

void refresh_lines(int i)
{
	print_line(lines[i].x1-basex,lines[i].y1-basey,lines[i].x2-basex,lines[i].y2-basey);
	pouttextxy(lines[i].tx-basex,lines[i].ty-basey,&lines[i].text[0][0]);
	pouttextxy(0,0,&lines[i].text[1][0]);
}

pouttextxy(int x,int y,char *instring)
{
	if (x==0&&y==0)
	{
		if (*instring != NULL)
			printf("LB%s\n\x03",instring);
		else
			printf("LB \n\x03");
	}
	else
	{
		if (*instring != NULL)
			printf("PU;PA%d,%d;LB%s\n\x03",x+3,y-3,instring);
		else
			printf("PU;PA%d,%d;LB \n\x03",x+3,y-3);
	}
}

void draw_object(int object)
{
	switch(object)
	{
		case SQUARE : 	print_square(cursx-basex,cursy-basey,cursx+SQUAREXOFF-basex,cursy+SQUAREYOFF-basey);break;
		case CIRCLE :   print_circle(cursx-basex,cursy-basey,RADIUSX);break;
		case FILEOBJ :  file_store(cursx,cursy);break;
	}
}

void print_square(int x,int y,int x2,int y2)
{
	printf("PU;PA%d,%d;EA%d,%d;",x,y,x2,y2);
}

void print_circle(int x,int y,int radius)
{
	printf("PU;PA%d,%d;CI%d;",x,y,radius);
}

void file_store(int x,int y)
{
	print_line(x-basex,y-basey,x+70-basex,y-basey);
	print_line(x-basex,y+18-basey,x+70-basex,y+18-basey);
	print_line(x-basex,y-basey,x-basex,y+18-basey);
}

void print_line(int x,int y,int x1,int y1)
{
	printf("PU;PA%d,%d;PD%d,%d;",x,y,x1,y1);
}

void get_save(void)
{
	fflush(stdin);
	get_drawing();
}

void get_drawing(void)
{
	FILE *sq,*cr,*fi,*ar,*li;
	int i;
	char drawname[40];
	char work[45];
	get_filename(drawname);

	strcpy(work,drawname);
	strcat(work,".sqr");
	if ((sq=fopen(work,"rb")) == NULL)
	{	printf("Cannot open input file\n");	  }
	strcpy(work,drawname);
	strcat(work,".crc");
	if ((cr=fopen(work,"rb")) == NULL)
	{	printf("Cannot open input file\n");  }
	strcpy(work,drawname);
	strcat(work,".fil");
	if ((fi=fopen(work,"rb")) == NULL)
	{	printf("Cannot open input file\n");  }
	strcpy(work,drawname);
	strcat(work,".arw");
	if ((ar=fopen(work,"rb")) == NULL)
	{	printf("Cannot open input file\n");  }
	strcpy(work,drawname);
	strcat(work,".lin");
	if ((li=fopen(work,"rb")) == NULL)
	{	printf("Cannot open input file\n");  }

	sqrcnt = 0;
	while(fread(&squares[sqrcnt],sizeof(square_type),1,sq))
	{
		sqrcnt++;
	}
	crccnt = 0;
	while(fread(&circles[crccnt],sizeof(circle_type),1,cr))
	{
		crccnt++;
	}
	filcnt = 0;
	while(fread(&files[filcnt],sizeof(file_type),1,fi))
	{
		filcnt++;
	}
	arwcnt = 0;
	while(fread(&arrows[arwcnt],sizeof(arrow_type),1,ar))
	{
		arwcnt++;
	}
	lincnt = 0;
	while(fread(&lines[lincnt],sizeof(line_type),1,li))
	{
		lincnt++;
	}
	if (sq!=NULL)
		fclose(sq);
	if (cr!=NULL)
		fclose(cr);
	if (fi!=NULL)
		fclose(fi);
	if (ar!=NULL)
		fclose(ar);
	if (li!=NULL)
		fclose(li);
}

void get_filename(char *nametext)
{
	gets(nametext);
}


void farthestxy(void)
{
	int i;
	int xdistance;
	int ydistance;

	/* Locate farthest object */

	if (sqrcnt > 0)
	{
		for (i=0;i<sqrcnt;i++)
		{
			xdistance = squares[i].x;
			ydistance = squares[i].y;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}

	if (crccnt > 0)
	{
		for (i=0;i<crccnt;i++)
		{
			xdistance = circles[i].x;
			ydistance = circles[i].y;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}

	if (filcnt > 0)
	{
		for (i=0;i<filcnt;i++)
		{
			xdistance = files[i].x;
			ydistance = files[i].y;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}

	if (arwcnt > 0)
	{
		for (i=0;i<arwcnt;i++)
		{
			xdistance = arrows[i].x1;
			ydistance = arrows[i].y1;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
			xdistance = arrows[i].x2;
			ydistance = arrows[i].y2;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}

	if (lincnt > 0)
	{
		for (i=0;i<lincnt;i++)
		{
			xdistance = lines[i].x1;
			ydistance = lines[i].y1;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
			xdistance = lines[i].x2;
			ydistance = lines[i].y2;
			if (xdistance > max_xdistance)
			{
				max_xdistance = xdistance;
			}
			if (ydistance > max_ydistance)
			{
				max_ydistance = ydistance;
			}
		}
	}
}

