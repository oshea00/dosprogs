/*
** DRAW.C
**
** INPUT
**  Keyboard commands
**
** OUTPUT
**  Graphic drawing program
**
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <graphics.h>

#define ENTER_KEY 0x0d
#define BACKSPACE 0x08
#define SQUARE 1
#define CIRCLE 2
#define FILE 3
#define ARROW 4
#define LINE 5
#define SQUAREXOFF 65
#define SQUAREYOFF 50
#define RADIUSX 40
#define RADIUSY 30
#define MAXSQUARES 15
#define MAXCIRCLES 15
#define MAXFILES 15
#define MAXARROWS 30
#define MAXLINES 30
#define ON 1
#define OFF 0
#define SR 1
#define FR 10



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
int textdir;
int tx;
int ty;
char text[2][14];
} arrow_type;

typedef struct {
int x1;
int y1;
int x2;
int y2;
int textdir;
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

int cursx = 6;
int cursy = 6;
int xsave = 6;
int ysave = 6;

int sqrcnt = 0;
int crccnt = 0;
int filcnt = 0;
int arwcnt = 0;
int lincnt = 0;

int main(void);
void c_break(void);
void draw_cursor(int);
void up(int);
void down(int);
void left(int);
void right(int);
void draw_square(void);
void draw_circle(void);
void draw_file(void);
void draw_arrow(void);
void draw_line(void);
void draw_object(int,int);
int  move_set(int);
int  move_line(int);
void insert_text(int,int,int,int,int,int);
void get_string(int,int,int);
void prompt(char *,char *,char *,char *);
void refresh(void);
void refresh_squares(int);
void refresh_circles(int);
void refresh_files(int);
void refresh_arrows(int);
void refresh_lines(int);
void file_store(int,int);
void line_obj(int,int);
void draw_point(int,int,int,int);
void manipulate_object(void);

int main(void)
{
	int g_driver,g_mode,g_error,c=0;
	detectgraph(&g_driver,&g_mode);
	g_mode = 1; /* Override to EGA */
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	if (registerbgifont(small_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"");
	draw_cursor(ON);
	fflush(stdin);
	prompt("Cursor Mode.","Type 'e' to quit.","","");
	while (c!='e')
	{
		c = getch();
		switch(c)
		{
			case 'w' : draw_cursor(OFF);up(SR);draw_cursor(ON);break;
			case 'W' : draw_cursor(OFF);up(FR);draw_cursor(ON);break;
			case 'x' : draw_cursor(OFF);down(SR);draw_cursor(ON);break;
			case 'X' : draw_cursor(OFF);down(FR);draw_cursor(ON);break;
			case 'a' : draw_cursor(OFF);left(SR);draw_cursor(ON);break;
			case 'A' : draw_cursor(OFF);left(FR);draw_cursor(ON);break;
			case 'd' : draw_cursor(OFF);right(SR);draw_cursor(ON);break;
			case 'D' : draw_cursor(OFF);right(FR);draw_cursor(ON);break;
			case 's' : draw_cursor(OFF);draw_square();break;
			case 'c' : draw_cursor(OFF);draw_circle();break;
			case 'f' : draw_cursor(OFF);draw_file();break;
			case 'p' : draw_cursor(OFF);draw_arrow();break;
			case 'l' : draw_cursor(OFF);draw_line();break;
			case 'm' : draw_cursor(OFF);manipulate_object();break;
			case 'r' : refresh();break;
		}
		if (c=='s'||c=='r'||c=='c'||c=='f'||c=='p'||c=='l')
			prompt("Cursor Mode.","Type 'e' to quit.","","");
	}
	c_break();
	return 0;
}

void draw_cursor(int toggle)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);

	setlinestyle(SOLID_LINE,NULL,NORM_WIDTH);
	line(cursx-3,cursy,cursx+3,cursy);
	line(cursx,cursy-3,cursx,cursy+3);
}

void draw_square(void)
{
	int ret;
	draw_object(SQUARE,ON);
	prompt("Position using cursor commands.","Type 'e' when done positioning.","'q' to quit.","");
	while (ret = move_set(SQUARE))
	{
		if (ret == 2)
			break;
	}
	if (ret != 2)
	{
		square_work.x = cursx;
		square_work.y = cursy;
		prompt("Text input mode.","Enter up to 4 lines","","");
		insert_text(2,3,9,4,10,SQUARE);
		if (sqrcnt<MAXSQUARES)
		{
			memcpy(&squares[sqrcnt],&square_work,sizeof(square_type));
			sqrcnt++;
		}
	}
	else
	{
		refresh();
	}
	prompt("","","","");
	cursx=6;cursy=6;
	draw_cursor(ON);

}

void draw_circle(void)
{
	int ret;
	draw_object(CIRCLE,ON);
	prompt("Position using cursor commands.","Type 'e' when done positioning.","","");
	while (ret = move_set(CIRCLE))
	{
		if (ret == 2)
			break;
	}
	if (ret != 2)
	{
		circle_work.x = cursx;
		circle_work.y = cursy;
		prompt("Text input mode.","Enter up to 4 lines","","");
		insert_text(-27,-20,9,4,10,CIRCLE);
		if (crccnt<MAXCIRCLES)
		{
			memcpy(&circles[crccnt],&circle_work,sizeof(circle_type));
			crccnt++;
		}
	}
	else
	{
		refresh();
	}
	prompt("","","","");
	cursx=6;cursy=6;
	draw_cursor(ON);
}

void draw_file(void)
{
	int ret;
	draw_object(FILE,ON);
	prompt("Position using cursor commands.","Type 'e' when done positioning.","","");
	while (ret = move_set(FILE))
	{
		if (ret == 2)
			break;
	}
	if (ret != 2)
	{
		file_work.x = cursx;
		file_work.y = cursy;
		prompt("Text input mode.","Enter 1 line.","","");
		insert_text(2,3,9,1,13,FILE);
		if (filcnt<MAXFILES)
		{
			memcpy(&files[filcnt],&file_work,sizeof(file_type));
			filcnt++;
		}
	}
	else
	{
		refresh();
	}
	prompt("","","","");
	cursx=6;cursy=6;
	draw_cursor(ON);
}

void draw_arrow(void)
{
	char c=0;
	int ret;
	xsave = cursx;
	ysave = cursy;
	putpixel(cursx,cursy,WHITE);
	prompt("Position using cursor commands.","Type 'e' when done positioning.","","");
	while (ret = move_set(LINE))
	{
		if (ret == 2)
			break;
	}
	if (ret != 2)
	{
		arrow_work.x1 = xsave;
		arrow_work.y1 = ysave;
		arrow_work.x2 = cursx;
		arrow_work.y2 = cursy;
		draw_point(arrow_work.x1,arrow_work.y1,arrow_work.x2,arrow_work.y2);
		prompt("Text input mode.","Position text with cursor.","'e' to set.","");
		draw_cursor(ON);
		while (c!='e')
		{
			c = getch();
			switch(c)
			{
				case 'w' : draw_cursor(OFF);up(SR);draw_cursor(ON);break;
				case 'W' : draw_cursor(OFF);up(FR);draw_cursor(ON);break;
				case 'x' : draw_cursor(OFF);down(SR);draw_cursor(ON);break;
				case 'X' : draw_cursor(OFF);down(FR);draw_cursor(ON);break;
				case 'a' : draw_cursor(OFF);left(SR);draw_cursor(ON);break;
				case 'A' : draw_cursor(OFF);left(FR);draw_cursor(ON);break;
				case 'd' : draw_cursor(OFF);right(SR);draw_cursor(ON);break;
				case 'D' : draw_cursor(OFF);right(FR);draw_cursor(ON);break;
				case 'e' : arrow_work.tx=cursx;arrow_work.ty=cursy;break;
			}
		}
		draw_cursor(OFF);
		prompt("Text input mode.","Type up to 2 lines.","","");
		insert_text(0,0,9,2,13,ARROW);
		if (arwcnt<MAXARROWS)
		{
			memcpy(&arrows[arwcnt],&arrow_work,sizeof(arrow_type));
			arwcnt++;
		}
	}
	else
	{
		refresh();
	}
	prompt("","","","");
	cursx=arrows[arwcnt-1].x2;cursy=arrows[arwcnt-1].y2;
	draw_cursor(ON);
}

void draw_line(void)
{
	int ret;
	char c=0;
	xsave = cursx;
	ysave = cursy;
	putpixel(cursx,cursy,WHITE);
	prompt("Position using cursor commands.","Type 'e' when done positioning.","","");
	while (ret = move_set(LINE))
	{
		if (ret == 2)
			break;
	}
	if (ret != 2)
	{
		line_work.x1 = xsave;
		line_work.y1 = ysave;
		line_work.x2 = cursx;
		line_work.y2 = cursy;
		prompt("Text input mode.","Position text with cursor.","'e' to set.","");
		cursx = cursx-=10;
		cursy = cursy-=10;
		draw_cursor(ON);
		while (c!='e')
		{
			c = getch();
			switch(c)
			{
				case 'w' : draw_cursor(OFF);up(SR);draw_cursor(ON);break;
				case 'W' : draw_cursor(OFF);up(FR);draw_cursor(ON);break;
				case 'x' : draw_cursor(OFF);down(SR);draw_cursor(ON);break;
				case 'X' : draw_cursor(OFF);down(FR);draw_cursor(ON);break;
				case 'a' : draw_cursor(OFF);left(SR);draw_cursor(ON);break;
				case 'A' : draw_cursor(OFF);left(FR);draw_cursor(ON);break;
				case 'd' : draw_cursor(OFF);right(SR);draw_cursor(ON);break;
				case 'D' : draw_cursor(OFF);right(FR);draw_cursor(ON);break;
				case 'e' : line_work.tx=cursx;line_work.ty=cursy;break;
			}
		}
		draw_cursor(OFF);
		prompt("Text input mode.","Type up to 2 lines.","","");
		insert_text(0,0,9,2,13,LINE);
		if (lincnt<MAXLINES)
		{
			memcpy(&lines[lincnt],&line_work,sizeof(line_type));
			lincnt++;
		}
	}
	else
	{
		refresh();
	}
	prompt("","","","");
	cursx=lines[lincnt-1].x2;cursy=lines[lincnt-1].y2;
	draw_cursor(ON);
}

void draw_point(int x1,int y1,int x2,int y2)
{
	setlinestyle(SOLID_LINE,NULL,THICK_WIDTH);
	if (x2<x1&&y2<y1)
	{
		line(x2,y2,x2,y2+3);
		line(x2,y2,x2+4,y2);
		cursx = xsave-6;
		cursy = ysave-6;
	}
	else
	if (x2==x1&&y2<y1)
	{
		line(x2,y2,x2+3,y2+3);
		line(x2,y2,x2-3,y2+3);
		cursx = xsave-6;
		cursy = ysave-6;
	}
	else
	if (x2>x1&&y2<y1)
	{
		line(x2,y2,x2-4,y2);
		line(x2,y2,x2,y2+3);
		cursx = xsave+6;
		cursy = ysave+6;
	}
	else
	if (x2<x1&&y2==y1)
	{
		line(x2,y2,x2+3,y2-3);
		line(x2,y2,x2+3,y2+3);
		cursx = xsave-6;
		cursy = ysave+6;
	}
	else
	if (x2>x1&&y2==y1)
	{
		line(x2,y2,x2-3,y2-3);
		line(x2,y2,x2-3,y2+3);
		cursx = xsave-6;
		cursy = ysave+6;
	}
	else
	if (x2<x1&&y2>y1)
	{
		line(x2,y2,x2,y2-3);
		line(x2,y2,x2+4,y2);
		cursx = xsave-6;
		cursy = ysave+6;
	}
	else
	if (x2==x1&&y2>y1)
	{
		line(x2,y2,x2-3,y2-3);
		line(x2,y2,x2+3,y2-3);
		cursx = xsave+6;
		cursy = ysave+6;
	}
	else
	if (x2>x1&&y2>y1)
	{
		line(x2,y2,x2,y2-3);
		line(x2,y2,x2-4,y2);
		cursx = xsave+6;
		cursy = ysave+6;
	}
	setlinestyle(SOLID_LINE,NULL,NORM_WIDTH);
}

void refresh(void)
{
	int i;
	setviewport(0,0,639,349,1);
	clearviewport();
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

	cursx=6;cursy=6;
	draw_cursor(ON);
}

void refresh_squares(int i)
{
	cursx = squares[i].x;
	cursy = squares[i].y;
	draw_object(SQUARE,ON);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(cursx+2,cursy+3,&squares[i].text[0][0]);
	outtextxy(cursx+2,cursy+12,&squares[i].text[1][0]);
	outtextxy(cursx+2,cursy+21,&squares[i].text[2][0]);
	outtextxy(cursx+2,cursy+30,&squares[i].text[3][0]);
}

void refresh_circles(int i)
{
	cursx = circles[i].x;
	cursy = circles[i].y;
	draw_object(CIRCLE,ON);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(cursx-27,cursy-20,&circles[i].text[0][0]);
	outtextxy(cursx-27,cursy-11,&circles[i].text[1][0]);
	outtextxy(cursx-27,cursy-2,&circles[i].text[2][0]);
	outtextxy(cursx-27,cursy+7,&circles[i].text[3][0]);
}

void refresh_files(int i)
{
	cursx = files[i].x;
	cursy = files[i].y;
	draw_object(FILE,ON);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(cursx+2,cursy+3,&files[i].text[0][0]);
}

void refresh_arrows(int i)
{
	setcolor(WHITE);
	line(arrows[i].x1,arrows[i].y1,arrows[i].x2,arrows[i].y2);
	draw_point(arrows[i].x1,arrows[i].y1,arrows[i].x2,arrows[i].y2);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(arrows[i].tx,arrows[i].ty,&arrows[i].text[0][0]);
	outtextxy(arrows[i].tx,arrows[i].ty+9,&arrows[i].text[1][0]);
	setcolor(WHITE);
}

void refresh_lines(int i)
{
	setcolor(WHITE);
	line(lines[i].x1,lines[i].y1,lines[i].x2,lines[i].y2);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(lines[i].tx,lines[i].ty,&lines[i].text[0][0]);
	outtextxy(lines[i].tx,lines[i].ty+9,&lines[i].text[1][0]);
	setcolor(WHITE);
}

void manipulate_object(void)
{
	int i;
	int object;
	int index;
	int line_end;
	double midx,midy;
	double distance;
	double min_distance = 9999999;


	if (sqrcnt > 0)
	{
		for (i=0;i<sqrcnt;i++)
		{
			distance = sqrt(pow((double)(squares[i].x-cursx),2.00)
						   +pow((double)(squares[i].y-cursy),2.00));
			if (distance < min_distance)
			{
				object = SQUARE;
				min_distance = distance;
				index = i;
			}
		}
	}

	if (crccnt > 0)
	{
		for (i=0;i<crccnt;i++)
		{
			distance = sqrt(pow((double)(circles[i].x-cursx),2.00)
						   +pow((double)(circles[i].y-cursy),2.00));
			if (distance < min_distance)
			{
				object = CIRCLE;
				min_distance = distance;
				index = i;
			}
		}
	}

	if (filcnt > 0)
	{
		for (i=0;i<filcnt;i++)
		{
			distance = sqrt(pow((double)(files[i].x-cursx),2.00)
						   +pow((double)(files[i].y-cursy),2.00));
			if (distance < min_distance)
			{
				object = FILE;
				min_distance = distance;
				index = i;
			}
		}
	}

	if (arwcnt > 0)
	{
		for (i=0;i<arwcnt;i++)
		{
			distance = sqrt(pow((double)(arrows[i].x1-cursx),2.00)
						   +pow((double)(arrows[i].y1-cursy),2.00));
			if (distance < min_distance)
			{
				object = ARROW;
				min_distance = distance;
				index = i;
				line_end = 1;
			}
			distance = sqrt(pow((double)(arrows[i].x2-cursx),2.00)
						   +pow((double)(arrows[i].y2-cursy),2.00));
			if (distance < min_distance)
			{
				object = ARROW;
				min_distance = distance;
				index = i;
				line_end = 2;
			}
		}
	}

	if (lincnt > 0)
	{
		for (i=0;i<lincnt;i++)
		{
			distance = sqrt(pow((double)(lines[i].x1-cursx),2.00)
						   +pow((double)(lines[i].y1-cursy),2.00));
			if (distance < min_distance)
			{
				object = LINE;
				min_distance = distance;
				index = i;
				line_end = 1;
			}
			distance = sqrt(pow((double)(lines[i].x2-cursx),2.00)
						   +pow((double)(lines[i].y2-cursy),2.00));
			if (distance < min_distance)
			{
				object = LINE;
				min_distance = distance;
				index = i;
				line_end = 2;
			}
		}
	}
}

void up(int offset)
{
	cursy-=offset;
	if (cursy<0)
		cursy = 350;
}

void down(int offset)
{
	cursy+=offset;
	if (cursy>350)
		cursy = 0;
}

void left(int offset)
{
	cursx-=offset;
	if (cursx<0)
		cursx = 640;
}

void right(int offset)
{
	cursx+=offset;
	if (cursx>640)
		cursx = 0;
}

void draw_object(int object,int toggle)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);
	switch(object)
	{
		case SQUARE : 	rectangle(cursx,cursy,cursx+SQUAREXOFF,cursy+SQUAREYOFF);break;
		case CIRCLE :   circle(cursx,cursy,RADIUSX);break;
		case FILE   :   file_store(cursx,cursy);break;
		case LINE   :   line_obj(cursx,cursy);break;
	}
}

void file_store(int x,int y)
{
	line(x,y,x+70,y);
	line(x,y+18,x+70,y+18);
	line(x,y,x,y+18);
}

void line_obj(int x,int y)
{
	line(xsave,ysave,x,y);
}

int move_set(int object)
{
	int c=0;
	while (c!='e'&&c!='q')
	{
		c = getch();
		switch(c)
		{
			case 'w' : draw_object(object,OFF);up(SR);draw_object(object,ON);break;
			case 'W' : draw_object(object,OFF);up(FR);draw_object(object,ON);break;
			case 'x' : draw_object(object,OFF);down(SR);draw_object(object,ON);break;
			case 'X' : draw_object(object,OFF);down(FR);draw_object(object,ON);break;
			case 'a' : draw_object(object,OFF);left(SR);draw_object(object,ON);break;
			case 'A' : draw_object(object,OFF);left(FR);draw_object(object,ON);break;
			case 'd' : draw_object(object,OFF);right(SR);draw_object(object,ON);break;
			case 'D' : draw_object(object,OFF);right(FR);draw_object(object,ON);break;
		}
		if (c!='e'&&c!='q')
			return 1;
	}
	if (c=='q')
		return 2;
	else
		return 0;
}

void insert_text(int xoff,int yoff,int yinc,int lines,int letcnt,int object)
{
	int i;
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	setfillstyle(SOLID_FILL,BLACK);
	if ((object != FILE) && (object != LINE) && (object != ARROW))
		floodfill(cursx+2,cursy+2,WHITE);
	for (i=0;i<lines;i++)
	{
		get_string(cursx+xoff,cursy+yoff+(i*yinc),letcnt);
		input[letcnt+1]=NULL;
		switch (object)
		{
			case SQUARE :  strcpy(&square_work.text[i][0],input);break;
			case CIRCLE :  strcpy(&circle_work.text[i][0],input);break;
			case FILE   :  strcpy(&file_work.text[i][0],input);break;
			case ARROW  :  strcpy(&arrow_work.text[i][0],input);break;
			case LINE   :  strcpy(&line_work.text[i][0],input);break;
		}
	}
}

void get_string(int x,int y,int max)
{
	int i=0;
	fflush(stdin);
	input[i] = getch();
	while (input[i] != ENTER_KEY)
	{
		if (input[i] == BACKSPACE)
		{
			setcolor(BLACK);
			input[i] = NULL;
			outtextxy(x,y,input);
			input[i-1] = ' ';
			input[i] = NULL;
			setcolor(LIGHTGREEN);
			outtextxy(x,y,input);
			i--;
			if (i<0)
				i=0;
		}
		else
		{
			input[i+1] = NULL;
			outtextxy(x,y,input);
			i++;
			if (i>max-1)
				break;
		}
		input[i] = getch();
	}
	input[i] = NULL;
}

void c_break(void)
{
	char string[80];
	fflush(stdin);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	setviewport(455,300,639,349,1);
	clearviewport();
	outtextxy(0,0,"Hit <ENTER> key to");
	outtextxy(0,9,"continue...");
	gets(string);
	clearviewport();
	setviewport(0,0,639,349,1);
}

void prompt(char *line1,char *line2,char *line3,char *line4)
{
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	setviewport(455,300,639,349,1);
	clearviewport();
	outtextxy(0,0,line1);
	outtextxy(0,9,line2);
	outtextxy(0,18,line3);
	outtextxy(0,27,line4);
	setviewport(0,0,639,349,1);
}

