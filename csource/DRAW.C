/*
** DRAW.C
**
** INPUT
**  Keyboard commands
**
** OUTPUT
**  Graphic yourdon drawing program
**
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <graphics.h>
#include <dos.h>

#define ENTER_KEY 0x0d
#define ENTER '\r'
#define ESC 0x01
#define BACKSPACE 0x08
#define SQUARE 1
#define CIRCLE 2
#define FILEOBJ 3
#define ARROW 4
#define LINE 5
#define SQUAREXOFF 65
#define SQUAREYOFF 50
#define RADIUSX 40
#define RADIUSY 40
#define MAXSQUARES 30
#define MAXCIRCLES 30
#define MAXFILES 30
#define MAXARROWS 60
#define MAXLINES 60
#define ON 1
#define OFF 0
#define SR 1
#define FR 15
#define UP 73
#define DOWN 81
#define RIGHT 116
#define LEFT 115
#define FUP 72
#define FDOWN 80
#define FRIGHT 77
#define FLEFT 75
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
union REGS regs;

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

char mainmenu[11][20] = { {"Square"},
						 {"Circle"},
						 {"Fileobj"},
						 {"Arrow"},
						 {"Line"},
						 {"Manip"},
						 {"Getsave"},
						 {"Rescale"},
						 {"rEfresh"},
						 {"Home"},
						 {"Quit"} };

char quick_main[12] = "SCFALMGREHQ";

char manip_menu[3][20] = { {"Quit"},
						   {"Move"},
						   {"Delete"} };
char quick_manip[4] = "QMD";

char quit_menu[3][20] = { {"Save"},
						   {"Get"},
						   {"Quit"} };

char quick_quit[4] = "SMQ";

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
void highlight_handle(int,int,int);
void delete_object(int,int);
void move_object(int,int,int);
void move_square(int);
void move_circle(int);
void move_file(int);
void move_arrow(int,int);
void move_line(int,int);
void disp_square(int,int,int,int);
void disp_circle(int,int,int,int);
void disp_file(int,int,int,int);
void disp_arrow(int,int,int,int,int);
void disp_line(int,int,int,int,int);
void get_save(void);
void save_drawing(void);
void get_drawing(void);
void get_filename(char *);
void rescale(void);
void mgetch(int *,char*);
int get_choice(int,int,char[][],char[]);
void erase_menu(void);

int main(void)
{
	int choice=0;
	int scan;
	char ascii=0;
	int g_driver,g_mode,g_error;
	detectgraph(&g_driver,&g_mode);
	g_mode = 1; /* Override to EGA */
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	if (registerbgifont(small_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"");
	regs.h.ah = 0x03;
	regs.h.al = 0x05;
	regs.h.bh = 0x00;
	regs.h.bl = 0x00;
	int86(0x16,&regs,&regs);
	draw_cursor(ON);
	fflush(stdin);
	prompt("Cursor Mode.","","","");
	while (choice!=11)
	{
		while (ascii!='/')
		{
			mgetch(&scan,&ascii);
			switch (scan)
			{
				case FUP   : draw_cursor(OFF);up(FR);draw_cursor(ON);break;
				case FDOWN : draw_cursor(OFF);down(FR);draw_cursor(ON);break;
				case FLEFT : draw_cursor(OFF);left(FR);draw_cursor(ON);break;
				case FRIGHT : draw_cursor(OFF);right(FR);draw_cursor(ON);break;
				case UP   : draw_cursor(OFF);up(SR);draw_cursor(ON);break;
				case DOWN : draw_cursor(OFF);down(SR);draw_cursor(ON);break;
				case LEFT : draw_cursor(OFF);left(SR);draw_cursor(ON);break;
				case RIGHT : draw_cursor(OFF);right(SR);draw_cursor(ON);break;
			}
		}
		choice = get_choice(7,11,mainmenu,quick_main);
		switch(choice)
		{
			case 1 : draw_cursor(OFF);draw_square();break;
			case 2 : draw_cursor(OFF);draw_circle();break;
			case 3 : draw_cursor(OFF);draw_file();break;
			case 4 : draw_cursor(OFF);draw_arrow();break;
			case 5 : draw_cursor(OFF);draw_line();break;
			case 6 : draw_cursor(OFF);manipulate_object();break;
			case 7 : draw_cursor(OFF);get_save();break;
			case 8 : draw_cursor(OFF);rescale();break;
			case 9 : refresh();break;
			case 10 : basex=0;basey=0;cursx=6;cursy=6;refresh();break;
		}
		ascii=0;
		prompt("Cursor Mode.","","","");
		erase_menu();
	}
	regs.h.ah = 0x03;
	regs.h.al = 0x05;
	regs.h.bh = 0x01;
	regs.h.bl = 0x0C;
	int86(0x16,&regs,&regs);
	restorecrtmode();
	return 0;
}

void draw_cursor(int toggle)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);

	setlinestyle(SOLID_LINE,NULL,NORM_WIDTH);
	line(cursx-3-basex,cursy-basey,cursx+3-basex,cursy-basey);
	line(cursx-basex,cursy-3-basey,cursx-basex,cursy+3-basey);
}

void draw_square(void)
{
	int ret;
	erase_menu();
	draw_cursor(OFF);
	draw_object(SQUARE,ON);
	prompt("Position using cursor commands.","<enter> when done positioning.","'q' to quit.","");
	while (ret = move_set(SQUARE))
	{
		if (ret == 2)
			break;
	}
	if (ret != 2)
	{
		square_work.x = cursx;
		square_work.y = cursy;
		prompt("Text input mode.","Enter up to 4 lines.","","");
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
	cursx=basex+6;cursy=basey+6;
	draw_cursor(ON);

}

void draw_circle(void)
{
	int ret;
	erase_menu();
	draw_cursor(OFF);
	draw_object(CIRCLE,ON);
	prompt("Position using cursor commands.","<enter> when done positioning.","","");
	while (ret = move_set(CIRCLE))
	{
		if (ret == 2)
			break;
	}
	if (ret != 2)
	{
		circle_work.x = cursx;
		circle_work.y = cursy;
		prompt("Text input mode.","Enter up to 4 lines.","","");
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
	cursx=basex+6;cursy=basey+6;
	draw_cursor(ON);
}

void draw_file(void)
{
	int ret;
	erase_menu();
	draw_cursor(OFF);
	draw_object(FILEOBJ,ON);
	prompt("Position using cursor commands.","<enter> when done positioning.","","");
	while (ret = move_set(FILEOBJ))
	{
		if (ret == 2)
			break;
	}
	if (ret != 2)
	{
		file_work.x = cursx;
		file_work.y = cursy;
		prompt("Text input mode.","Enter 1 line.","","");
		insert_text(2,3,9,1,13,FILEOBJ);
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
	cursx=basex+6;cursy=basey+6;
	draw_cursor(ON);
}


void draw_arrow(void)
{
	int scan;
	char ascii=0;
	int ret;
	erase_menu();
	draw_cursor(OFF);
	xsave = cursx;
	ysave = cursy;
	putpixel(cursx-basex,cursy-basey,WHITE);
	prompt("Position using cursor commands.","<enter> when done positioning.","","");
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
		prompt("Position text with cursor.","<enter> to set.","","");
		draw_cursor(ON);
		while (ascii!=ENTER)
		{
			mgetch(&scan,&ascii);
			switch (scan)
			{
				case UP     : draw_cursor(OFF);up(SR);draw_cursor(ON);break;
				case FUP    : draw_cursor(OFF);up(FR);draw_cursor(ON);break;
				case DOWN   : draw_cursor(OFF);down(SR);draw_cursor(ON);break;
				case FDOWN  : draw_cursor(OFF);down(FR);draw_cursor(ON);break;
				case LEFT   : draw_cursor(OFF);left(SR);draw_cursor(ON);break;
				case FLEFT  : draw_cursor(OFF);left(FR);draw_cursor(ON);break;
				case RIGHT  : draw_cursor(OFF);right(SR);draw_cursor(ON);break;
				case FRIGHT : draw_cursor(OFF);right(FR);draw_cursor(ON);break;
			}
			switch (ascii)
			{
				case ENTER : arrow_work.tx=cursx;arrow_work.ty=cursy;break;
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
	int ret,scan;
	char ascii=0;
	erase_menu();
	draw_cursor(OFF);
	xsave = cursx;
	ysave = cursy;
	putpixel(cursx-basex,cursy-basey,WHITE);
	prompt("Position using cursor commands.","Type <enter> when done positioning.","","");
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
		prompt("Position text with cursor.","<enter> to set.","","");
		cursx = cursx-=10;
		cursy = cursy-=10;
		draw_cursor(ON);
		while (ascii!=ENTER)
		{
			mgetch(&scan,&ascii);
			switch(scan)
			{
				case UP     : draw_cursor(OFF);up(SR);draw_cursor(ON);break;
				case FUP    : draw_cursor(OFF);up(FR);draw_cursor(ON);break;
				case DOWN   : draw_cursor(OFF);down(SR);draw_cursor(ON);break;
				case FDOWN  : draw_cursor(OFF);down(FR);draw_cursor(ON);break;
				case LEFT   : draw_cursor(OFF);left(SR);draw_cursor(ON);break;
				case FLEFT  : draw_cursor(OFF);left(FR);draw_cursor(ON);break;
				case RIGHT  : draw_cursor(OFF);right(SR);draw_cursor(ON);break;
				case FRIGHT : draw_cursor(OFF);right(FR);draw_cursor(ON);break;
			}
			switch(ascii)
			{
				case ENTER : line_work.tx=cursx;line_work.ty=cursy;break;
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
		line(x2-basex,y2-basey,x2-basex,y2+3-basey);
		line(x2-basex,y2-basey,x2+4-basex,y2-basey);
		cursx = xsave-6;
		cursy = ysave-6;
	}
	else
	if (x2==x1&&y2<y1)
	{
		line(x2-basex,y2-basey,x2+3-basex,y2+3-basey);
		line(x2-basex,y2-basey,x2-3-basex,y2+3-basey);
		cursx = xsave-6;
		cursy = ysave-6;
	}
	else
	if (x2>x1&&y2<y1)
	{
		line(x2-basex,y2-basey,x2-4-basex,y2-basey);
		line(x2-basex,y2-basey,x2-basex,y2+3-basey);
		cursx = xsave+6;
		cursy = ysave+6;
	}
	else
	if (x2<x1&&y2==y1)
	{
		line(x2-basex,y2-basey,x2+3-basex,y2-3-basey);
		line(x2-basex,y2-basey,x2+3-basex,y2+3-basey);
		cursx = xsave-6;
		cursy = ysave+6;
	}
	else
	if (x2>x1&&y2==y1)
	{
		line(x2-basex,y2-basey,x2-3-basex,y2-3-basey);
		line(x2-basex,y2-basey,x2-3-basex,y2+3-basey);
		cursx = xsave-6;
		cursy = ysave+6;
	}
	else
	if (x2<x1&&y2>y1)
	{
		line(x2-basex,y2-basey,x2-basex,y2-3-basey);
		line(x2-basex,y2-basey,x2+4-basex,y2-basey);
		cursx = xsave-6;
		cursy = ysave+6;
	}
	else
	if (x2==x1&&y2>y1)
	{
		line(x2-basex,y2-basey,x2-3-basex,y2-3-basey);
		line(x2-basex,y2-basey,x2+3-basex,y2-3-basey);
		cursx = xsave+6;
		cursy = ysave+6;
	}
	else
	if (x2>x1&&y2>y1)
	{
		line(x2-basex,y2-basey,x2-basex,y2-3-basey);
		line(x2-basex,y2-basey,x2-4-basex,y2-basey);
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

	cursx=basex+6;cursy=basey+6;
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
	outtextxy(cursx+2-basex,cursy+3-basey,&squares[i].text[0][0]);
	outtextxy(cursx+2-basex,cursy+12-basey,&squares[i].text[1][0]);
	outtextxy(cursx+2-basex,cursy+21-basey,&squares[i].text[2][0]);
	outtextxy(cursx+2-basex,cursy+30-basey,&squares[i].text[3][0]);
}

void refresh_circles(int i)
{
	cursx = circles[i].x;
	cursy = circles[i].y;
	draw_object(CIRCLE,ON);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(cursx-27-basex,cursy-20-basey,&circles[i].text[0][0]);
	outtextxy(cursx-27-basex,cursy-11-basey,&circles[i].text[1][0]);
	outtextxy(cursx-27-basex,cursy-2-basey,&circles[i].text[2][0]);
	outtextxy(cursx-27-basex,cursy+7-basey,&circles[i].text[3][0]);
}

void refresh_files(int i)
{
	cursx = files[i].x;
	cursy = files[i].y;
	draw_object(FILEOBJ,ON);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(cursx+2-basex,cursy+3-basey,&files[i].text[0][0]);
}

void refresh_arrows(int i)
{
	setcolor(WHITE);
	line(arrows[i].x1-basex,arrows[i].y1-basey,arrows[i].x2-basex,arrows[i].y2-basey);
	draw_point(arrows[i].x1,arrows[i].y1,arrows[i].x2,arrows[i].y2);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(arrows[i].tx-basex,arrows[i].ty-basey,&arrows[i].text[0][0]);
	outtextxy(arrows[i].tx-basex,arrows[i].ty+9-basey,&arrows[i].text[1][0]);
	setcolor(WHITE);
}

void refresh_lines(int i)
{
	setcolor(WHITE);
	line(lines[i].x1-basex,lines[i].y1-basey,lines[i].x2-basex,lines[i].y2-basey);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(LIGHTGREEN);
	outtextxy(lines[i].tx-basex,lines[i].ty-basey,&lines[i].text[0][0]);
	outtextxy(lines[i].tx-basex,lines[i].ty+9-basey,&lines[i].text[1][0]);
	setcolor(WHITE);
}

void manipulate_object(void)
{
	int choice=0;
	char c;
	int i;
	int object;
	int index;
	int line_end;
	double midx,midy;
	double distance;
	double min_distance = 9999999;
	draw_cursor(OFF);

	/* Locate closest object */

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
				object = FILEOBJ;
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
	/*  Highlight Object handle  */

	prompt("Manipulate Object.","","","");
	highlight_handle(object,index,line_end);

	/* Check for quit, move or delete */

	fflush(stdin);
	choice = get_choice(3,3,manip_menu,quick_manip);
	switch (choice)
	{
		case 1 :  refresh();prompt("","","","");cursx=6;cursy=6;draw_cursor(ON);return;
		case 3 :  delete_object(object,index);break;
		case 2 :  move_object(object,index,line_end);break;
	}
	refresh();
	prompt("","","","");
	cursx=basex+6;cursy=basey+6;
	draw_cursor(ON);
}

void highlight_handle(int object,int index,int line_end)
{
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,WHITE);
	switch (object)
	{
		case SQUARE  :  rectangle(squares[index].x-basex,squares[index].y-basey,squares[index].x+4-basex,squares[index].y+4-basey);
						floodfill(squares[index].x+1-basex,squares[index].y+1-basey,WHITE);break;
		case CIRCLE  :  rectangle(circles[index].x-basex,circles[index].y-basey,circles[index].x+4-basex,circles[index].y+4-basey);
						floodfill(circles[index].x+1-basex,circles[index].y+1-basey,WHITE);break;
		case FILEOBJ    :  rectangle(files[index].x-basex,files[index].y-basey,files[index].x+4-basex,files[index].y+4-basey);
						floodfill(files[index].x+1-basex,files[index].y+1-basey,WHITE);break;
		case ARROW   :  if (line_end == 1)
						{
							rectangle(arrows[index].x1-basex,arrows[index].y1-basey,arrows[index].x1+4-basex,arrows[index].y1+4-basey);
							floodfill(arrows[index].x1+1-basex,arrows[index].y1+1-basey,WHITE);
						}
						else
						{
							rectangle(arrows[index].x2-basex,arrows[index].y2-basey,arrows[index].x2+4-basex,arrows[index].y2+4-basey);
							floodfill(arrows[index].x2+1-basex,arrows[index].y2+1-basey,WHITE);
						}
						break;
		case LINE    :  if (line_end == 1)
						{
							rectangle(lines[index].x1-basex,lines[index].y1-basey,lines[index].x1+4-basex,lines[index].y1+4-basey);
							floodfill(lines[index].x1+1-basex,lines[index].y1+1-basey,WHITE);
						}
						else
						{
							rectangle(lines[index].x2-basex,lines[index].y2-basey,lines[index].x2+4-basex,lines[index].y2+4-basey);
							floodfill(lines[index].x2+1-basex,lines[index].y2+1-basey,WHITE);
						}
						break;
	}
}

void delete_object(int object,int index)
{
	int i;
	i = index;
	switch (object)
	{
		case SQUARE  :  for (i=index;i<sqrcnt;i++)
						{
							squares[i].x = squares[i+1].x;
							squares[i].y = squares[i+1].y;
							strcpy(&squares[i].text[0][0],&squares[i+1].text[0][0]);
							strcpy(&squares[i].text[1][0],&squares[i+1].text[1][0]);
							strcpy(&squares[i].text[2][0],&squares[i+1].text[2][0]);
							strcpy(&squares[i].text[3][0],&squares[i+1].text[3][0]);
						}
						sqrcnt = i-1;break;
		case CIRCLE  :  for (i=index;i<crccnt;i++)
						{
							circles[i].x = circles[i+1].x;
							circles[i].y = circles[i+1].y;
							strcpy(&circles[i].text[0][0],&circles[i+1].text[0][0]);
							strcpy(&circles[i].text[1][0],&circles[i+1].text[1][0]);
							strcpy(&circles[i].text[2][0],&circles[i+1].text[2][0]);
							strcpy(&circles[i].text[3][0],&circles[i+1].text[3][0]);
						}
						crccnt = i-1;break;
		case FILEOBJ    :  for (i=index;i<filcnt;i++)
						{
							files[i].x = files[i+1].x;
							files[i].y = files[i+1].y;
							strcpy(&files[i].text[0][0],&files[i+1].text[0][0]);
						}
						filcnt = i-1;break;
		case ARROW   :  for (i=index;i<arwcnt;i++)
						{
							arrows[i].x1 = arrows[i+1].x1;
							arrows[i].y1 = arrows[i+1].y1;
							arrows[i].x2 = arrows[i+1].x2;
							arrows[i].y2 = arrows[i+1].y2;
							arrows[i].tx = arrows[i+1].tx;
							arrows[i].ty = arrows[i+1].ty;
							strcpy(&arrows[i].text[0][0],&arrows[i+1].text[0][0]);
							strcpy(&arrows[i].text[1][0],&arrows[i+1].text[1][0]);
						}
						arwcnt = i-1;break;
		case LINE    :  for (i=index;i<lincnt;i++)
						{
							lines[i].x1 = lines[i+1].x1;
							lines[i].y1 = lines[i+1].y1;
							lines[i].x2 = lines[i+1].x2;
							lines[i].y2 = lines[i+1].y2;
							lines[i].tx = lines[i+1].tx;
							lines[i].ty = lines[i+1].ty;
							strcpy(&lines[i].text[0][0],&lines[i+1].text[0][0]);
							strcpy(&lines[i].text[1][0],&lines[i+1].text[1][0]);
						}
						lincnt = i-1;break;
	}
}

void move_object(int object,int index,int line_end)
{
	switch (object)
	{
		case SQUARE  :  move_square(index);break;
		case CIRCLE  :  move_circle(index);break;
		case FILEOBJ :  move_file(index);break;
		case ARROW   :  move_arrow(index,line_end);break;
		case LINE    :  move_line(index,line_end);break;
	}
}

void move_square(int index)
{
	char ascii=0;
	int scan;
	fflush(stdin);
	prompt("Move Mode.","Type <enter> to quit.","","");
	while (ascii!=ENTER)
	{
		mgetch(&scan,&ascii);
		switch(scan)
		{
			case UP     : disp_square(index,OFF,NONE,NONE);disp_square(index,ON,UP,SR);break;
			case FUP    : disp_square(index,OFF,NONE,NONE);disp_square(index,ON,UP,FR);break;
			case DOWN   : disp_square(index,OFF,NONE,NONE);disp_square(index,ON,DOWN,SR);break;
			case FDOWN  : disp_square(index,OFF,NONE,NONE);disp_square(index,ON,DOWN,FR);break;
			case LEFT   : disp_square(index,OFF,NONE,NONE);disp_square(index,ON,LEFT,SR);break;
			case FLEFT  : disp_square(index,OFF,NONE,NONE);disp_square(index,ON,LEFT,FR);break;
			case RIGHT  : disp_square(index,OFF,NONE,NONE);disp_square(index,ON,RIGHT,SR);break;
			case FRIGHT : disp_square(index,OFF,NONE,NONE);disp_square(index,ON,RIGHT,FR);break;
		}
	}
}

void move_circle(int index)
{
	char ascii=0;
	int scan;
	fflush(stdin);
	prompt("Move Mode.","Type <enter> to quit.","","");
	while (ascii!=ENTER)
	{
		mgetch(&scan,&ascii);
		switch(scan)
		{
			case UP     : disp_circle(index,OFF,NONE,NONE);disp_circle(index,ON,UP,SR);break;
			case FUP    : disp_circle(index,OFF,NONE,NONE);disp_circle(index,ON,UP,FR);break;
			case DOWN   : disp_circle(index,OFF,NONE,NONE);disp_circle(index,ON,DOWN,SR);break;
			case FDOWN  : disp_circle(index,OFF,NONE,NONE);disp_circle(index,ON,DOWN,FR);break;
			case LEFT   : disp_circle(index,OFF,NONE,NONE);disp_circle(index,ON,LEFT,SR);break;
			case FLEFT  : disp_circle(index,OFF,NONE,NONE);disp_circle(index,ON,LEFT,FR);break;
			case RIGHT  : disp_circle(index,OFF,NONE,NONE);disp_circle(index,ON,RIGHT,SR);break;
			case FRIGHT : disp_circle(index,OFF,NONE,NONE);disp_circle(index,ON,RIGHT,FR);break;
		}
	}
}

void move_file(int index)
{
	char ascii=0;
	int scan;
	fflush(stdin);
	prompt("Move Mode.","Type <enter> to quit.","","");
	while (ascii!=ENTER)
	{
		mgetch(&scan,&ascii);
		switch(scan)
		{
			case UP     : disp_file(index,OFF,NONE,NONE);disp_file(index,ON,UP,SR);break;
			case FUP    : disp_file(index,OFF,NONE,NONE);disp_file(index,ON,UP,FR);break;
			case DOWN   : disp_file(index,OFF,NONE,NONE);disp_file(index,ON,DOWN,SR);break;
			case FDOWN  : disp_file(index,OFF,NONE,NONE);disp_file(index,ON,DOWN,FR);break;
			case LEFT   : disp_file(index,OFF,NONE,NONE);disp_file(index,ON,LEFT,SR);break;
			case FLEFT  : disp_file(index,OFF,NONE,NONE);disp_file(index,ON,LEFT,FR);break;
			case RIGHT  : disp_file(index,OFF,NONE,NONE);disp_file(index,ON,RIGHT,SR);break;
			case FRIGHT : disp_file(index,OFF,NONE,NONE);disp_file(index,ON,RIGHT,FR);break;
		}
	}
}

void move_arrow(int index,int line_end)
{
	char ascii=0;
	int  scan;
	fflush(stdin);
	prompt("Move Mode.","Type <enter> to quit.","","");
	while (ascii!=ENTER)
	{
		mgetch(&scan,&ascii);
		switch(scan)
		{
			case UP     : disp_arrow(index,OFF,NONE,NONE,NONE);disp_arrow(index,ON,UP,SR,line_end);break;
			case FUP    : disp_arrow(index,OFF,NONE,NONE,NONE);disp_arrow(index,ON,UP,FR,line_end);break;
			case DOWN   : disp_arrow(index,OFF,NONE,NONE,NONE);disp_arrow(index,ON,DOWN,SR,line_end);break;
			case FDOWN  : disp_arrow(index,OFF,NONE,NONE,NONE);disp_arrow(index,ON,DOWN,FR,line_end);break;
			case LEFT   : disp_arrow(index,OFF,NONE,NONE,NONE);disp_arrow(index,ON,LEFT,SR,line_end);break;
			case FLEFT  : disp_arrow(index,OFF,NONE,NONE,NONE);disp_arrow(index,ON,LEFT,FR,line_end);break;
			case RIGHT  : disp_arrow(index,OFF,NONE,NONE,NONE);disp_arrow(index,ON,RIGHT,SR,line_end);break;
			case FRIGHT : disp_arrow(index,OFF,NONE,NONE,NONE);disp_arrow(index,ON,RIGHT,FR,line_end);break;
		}
	}
}

void move_line(int index,int line_end)
{
	char ascii=0;
	int scan;
	fflush(stdin);
	prompt("Move Mode.","Type <enter> to quit.","","");
	while (ascii!=ENTER)
	{
		mgetch(&scan,&ascii);
		switch(scan)
		{
			case UP     : disp_line(index,OFF,NONE,NONE,NONE);disp_line(index,ON,UP,SR,line_end);break;
			case FUP    : disp_line(index,OFF,NONE,NONE,NONE);disp_line(index,ON,UP,FR,line_end);break;
			case DOWN   : disp_line(index,OFF,NONE,NONE,NONE);disp_line(index,ON,DOWN,SR,line_end);break;
			case FDOWN  : disp_line(index,OFF,NONE,NONE,NONE);disp_line(index,ON,DOWN,FR,line_end);break;
			case LEFT   : disp_line(index,OFF,NONE,NONE,NONE);disp_line(index,ON,LEFT,SR,line_end);break;
			case FLEFT  : disp_line(index,OFF,NONE,NONE,NONE);disp_line(index,ON,LEFT,FR,line_end);break;
			case RIGHT  : disp_line(index,OFF,NONE,NONE,NONE);disp_line(index,ON,RIGHT,SR,line_end);break;
			case FRIGHT : disp_line(index,OFF,NONE,NONE,NONE);disp_line(index,ON,RIGHT,FR,line_end);break;
		}
	}
}

void disp_square(int index,int toggle,int dir,int speed)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);

	switch (dir)
	{
		case UP   :  squares[index].y-=speed;
					 if (squares[index].y<basey)
						 squares[index].y=basey+350;break;
		case DOWN :  squares[index].y+=speed;
					 if (squares[index].y>basey+350)
						 squares[index].y=basey;break;
		case LEFT :  squares[index].x-=speed;
					 if (squares[index].x<basex)
						 squares[index].x=basex+640;break;
		case RIGHT :  squares[index].x+=speed;
					 if (squares[index].x>basex+640)
						 squares[index].x=basex;break;
	}
	cursx = squares[index].x;
	cursy = squares[index].y;
	draw_object(SQUARE,toggle);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	outtextxy(cursx+2-basex,cursy+3-basey,&squares[index].text[0][0]);
	outtextxy(cursx+2-basex,cursy+12-basey,&squares[index].text[1][0]);
	outtextxy(cursx+2-basex,cursy+21-basey,&squares[index].text[2][0]);
	outtextxy(cursx+2-basex,cursy+30-basey,&squares[index].text[3][0]);
}

void disp_circle(int index,int toggle,int dir,int speed)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);

	switch (dir)
	{
		case UP   :  circles[index].y-=speed;
					 if (circles[index].y<basey)
						 circles[index].y=basey+350;break;
		case DOWN :  circles[index].y+=speed;
					 if (circles[index].y>basey+350)
						 circles[index].y=basey;break;
		case LEFT :  circles[index].x-=speed;
					 if (circles[index].x<basex)
						 circles[index].x=basex+640;break;
		case RIGHT :  circles[index].x+=speed;
					 if (circles[index].x>basex+640)
						 circles[index].x=basex;break;
	}
	cursx = circles[index].x;
	cursy = circles[index].y;
	draw_object(CIRCLE,toggle);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	outtextxy(cursx-27-basex,cursy-20-basey,&circles[index].text[0][0]);
	outtextxy(cursx-27-basex,cursy-11-basey,&circles[index].text[1][0]);
	outtextxy(cursx-27-basex,cursy-2-basey,&circles[index].text[2][0]);
	outtextxy(cursx-27-basex,cursy+7-basey,&circles[index].text[3][0]);
}

void disp_file(int index,int toggle,int dir,int speed)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);

	switch (dir)
	{
		case UP   :  files[index].y-=speed;
					 if (files[index].y<basey)
						 files[index].y=basey+350;break;
		case DOWN :  files[index].y+=speed;
					 if (files[index].y>basey+350)
						 files[index].y=basey;break;
		case LEFT :  files[index].x-=speed;
					 if (files[index].x<basex)
						 files[index].x=basex+640;break;
		case RIGHT :  files[index].x+=speed;
					 if (files[index].x>basex+640)
						 files[index].x=basex;break;
	}
	cursx = files[index].x;
	cursy = files[index].y;
	draw_object(FILEOBJ,toggle);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	outtextxy(cursx+2-basex,cursy+3-basey,&files[index].text[0][0]);
}

void disp_arrow(int index,int toggle,int dir,int speed,int line_end)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);

	switch (dir)
	{
		case UP   :  if (line_end == 1)
					 {
						arrows[index].y1-=speed;
						if (arrows[index].y1<basey)
							arrows[index].y1=basey+350;
					 }
					 else
					 {
						arrows[index].y2-=speed;
						if (arrows[index].y2<basey)
							arrows[index].y2=basey+350;
					 };
					 arrows[index].ty-=speed;
					 if (arrows[index].ty<basey)
						arrows[index].ty=basey+350;
					 break;
		case DOWN   :  if (line_end == 1)
					 {
						arrows[index].y1+=speed;
						if (arrows[index].y1>basey+350)
							arrows[index].y1=basey;
					 }
					 else
					 {
						arrows[index].y2+=speed;
						if (arrows[index].y2>basey+350)
							arrows[index].y2=basey;
					 };
					 arrows[index].ty+=speed;
					 if (arrows[index].ty>basey+350)
						arrows[index].ty=basey;
					 break;
		case LEFT   :  if (line_end == 1)
					 {
						arrows[index].x1-=speed;
						if (arrows[index].x1<basex)
							arrows[index].x1=basex+640;
					 }
					 else
					 {
						arrows[index].x2-=speed;
						if (arrows[index].x2<basex)
							arrows[index].x2=basex+640;
					 };
					 arrows[index].tx-=speed;
					 if (arrows[index].tx<basex)
						arrows[index].tx=basex+640;
					 break;
		case RIGHT  :  if (line_end == 1)
					 {
						arrows[index].x1+=speed;
						if (arrows[index].x1>basex+640)
							arrows[index].x1=basex;
					 }
					 else
					 {
						arrows[index].x2+=speed;
						if (arrows[index].x2>basex+640)
							arrows[index].x2=basex;
					 };
					 arrows[index].tx+=speed;
					 if (arrows[index].tx>basex+640)
						arrows[index].tx=basex;
					 break;
	}
	line(arrows[index].x1-basex,arrows[index].y1-basey,arrows[index].x2-basex,arrows[index].y2-basey);
	draw_point(arrows[index].x1,arrows[index].y1,arrows[index].x2,arrows[index].y2);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	outtextxy(arrows[index].tx-basex,arrows[index].ty-basey,&arrows[index].text[0][0]);
	outtextxy(arrows[index].tx-basex,arrows[index].ty+9-basey,&arrows[index].text[1][0]);
}

void disp_line(int index,int toggle,int dir,int speed,int line_end)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);

	switch (dir)
	{
		case UP   :  if (line_end == 1)
					 {
						lines[index].y1-=speed;
						if (lines[index].y1<basey)
							lines[index].y1=basey+350;
					 }
					 else
					 {
						lines[index].y2-=speed;
						if (lines[index].y2<basey)
							lines[index].y2=basey+350;
					 };
					 lines[index].ty-=speed;
					 if (lines[index].ty<basey)
						lines[index].ty=basey+350;
					 break;
		case DOWN   :  if (line_end == 1)
					 {
						lines[index].y1+=speed;
						if (lines[index].y1>basey+350)
							lines[index].y1=basey;
					 }
					 else
					 {
						lines[index].y2+=speed;
						if (lines[index].y2>basey+350)
							lines[index].y2=basey;
					 };
					 lines[index].ty+=speed;
					 if (lines[index].ty>basey+350)
						lines[index].ty=basey;
					 break;
		case LEFT   :  if (line_end == 1)
					 {
						lines[index].x1-=speed;
						if (lines[index].x1<basex)
							lines[index].x1=basex+640;
					 }
					 else
					 {
						lines[index].x2-=speed;
						if (lines[index].x2<basex)
							lines[index].x2=basex+640;
					 };
					 lines[index].tx-=speed;
					 if (lines[index].tx<basex)
						lines[index].tx=basex+640;
					 break;
		case RIGHT  :  if (line_end == 1)
					 {
						lines[index].x1+=speed;
						if (lines[index].x1>basex+640)
							lines[index].x1=basex;
					 }
					 else
					 {
						lines[index].x2+=speed;
						if (lines[index].x2>basex+640)
							lines[index].x2=basex;
					 };
					 lines[index].tx+=speed;
					 if (lines[index].tx>basex+640)
						lines[index].tx=basex;
					 break;
	}
	line(lines[index].x1-basex,lines[index].y1-basey,lines[index].x2-basex,lines[index].y2-basey);
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	outtextxy(lines[index].tx-basex,lines[index].ty-basey,&lines[index].text[0][0]);
	outtextxy(lines[index].tx-basex,lines[index].ty+9-basey,&lines[index].text[1][0]);
}

void up(int offset)
{
	cursy-=offset;
	if (cursy<basey)
		cursy = basey+350;
}

void down(int offset)
{
	cursy+=offset;
	if (cursy>basey+350)
		cursy = basey;
}

void left(int offset)
{
	cursx-=offset;
	if (cursx<basex)
		cursx = basex+640;
}

void right(int offset)
{
	cursx+=offset;
	if (cursx>basex+640)
		cursx = basex;
}

void draw_object(int object,int toggle)
{
	if (toggle == ON)
		setcolor(WHITE);
	else
		setcolor(BLACK);
	switch(object)
	{
		case SQUARE : 	rectangle(cursx-basex,cursy-basey,cursx+SQUAREXOFF-basex,cursy+SQUAREYOFF-basey);break;
		case CIRCLE :   ellipse(cursx-basex,cursy-basey,0,360,RADIUSX,RADIUSY);break;
		case FILEOBJ   :   file_store(cursx,cursy);break;
		case LINE   :   line_obj(cursx,cursy);break;
	}
}

void file_store(int x,int y)
{
	line(x-basex,y-basey,x+70-basex,y-basey);
	line(x-basex,y+18-basey,x+70-basex,y+18-basey);
	line(x-basex,y-basey,x-basex,y+18-basey);
}

void line_obj(int x,int y)
{
	line(xsave-basex,ysave-basey,x-basex,y-basey);
}

int move_set(int object)
{
	char ascii=0;
	int scan=0;
	while (ascii!=ENTER&&scan!=ESC)
	{
		mgetch(&scan,&ascii);
		switch(scan)
		{
			case UP     : draw_object(object,OFF);up(SR);draw_object(object,ON);break;
			case FUP    : draw_object(object,OFF);up(FR);draw_object(object,ON);break;
			case DOWN   : draw_object(object,OFF);down(SR);draw_object(object,ON);break;
			case FDOWN  : draw_object(object,OFF);down(FR);draw_object(object,ON);break;
			case LEFT   : draw_object(object,OFF);left(SR);draw_object(object,ON);break;
			case FLEFT  : draw_object(object,OFF);left(FR);draw_object(object,ON);break;
			case RIGHT  : draw_object(object,OFF);right(SR);draw_object(object,ON);break;
			case FRIGHT : draw_object(object,OFF);right(FR);draw_object(object,ON);break;
		}
		if (ascii!=ENTER&&scan!=ESC)
			return 1;
	}
	if (scan==ESC)
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
	if ((object != FILEOBJ) && (object != LINE) && (object != ARROW))
		floodfill(cursx+2-basex,cursy+2-basey,WHITE);
	for (i=0;i<lines;i++)
	{
		get_string(cursx+xoff,cursy+yoff+(i*yinc),letcnt);
		input[letcnt+1]=NULL;
		switch (object)
		{
			case SQUARE :  strcpy(&square_work.text[i][0],input);break;
			case CIRCLE :  strcpy(&circle_work.text[i][0],input);break;
			case FILEOBJ   :  strcpy(&file_work.text[i][0],input);break;
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
			outtextxy(x-basex,y-basey,input);
			input[i-1] = ' ';
			input[i] = NULL;
			setcolor(LIGHTGREEN);
			outtextxy(x-basex,y-basey,input);
			i--;
			if (i<0)
				i=0;
		}
		else
		{
			input[i+1] = NULL;
			outtextxy(x-basex,y-basey,input);
			i++;
			if (i>max-1)
				break;
		}
		input[i] = getch();
	}
	input[i] = NULL;
}


void prompt(char *line1,char *line2,char *line3,char *line4)
{
	settextstyle(SMALL_FONT,0,0);
	setusercharsize(1,1,1,1);
	setcolor(YELLOW);
	setviewport(455,300,639,329,1);
	clearviewport();
	outtextxy(4,4,line1);
	outtextxy(4,13,line2);
	setviewport(0,0,639,349,1);
}

void get_save(void)
{
	int choice;
	choice = get_choice(3,3,quit_menu,quick_quit);
	switch (choice)
	{
			case 1 : save_drawing();break;
			case 2 : get_drawing();break;
	}
	draw_cursor(ON);
}

void save_drawing(void)
{
	FILE *sq,*cr,*fi,*ar,*li;
	int i;
	char drawname[40];
	char work[45];
	get_filename(drawname);

	strcpy(work,drawname);
	strcat(work,".sqr");
	if ((sq=fopen(work,"wb")) == NULL)
	{	printf("Cannot open output file\n");  }
	strcpy(work,drawname);
	strcat(work,".crc");
	if ((cr=fopen(work,"wb")) == NULL)
	{	printf("Cannot open output file\n");  }
	strcpy(work,drawname);
	strcat(work,".fil");
	if ((fi=fopen(work,"wb")) == NULL)
	{	printf("Cannot open output file\n");  }
	strcpy(work,drawname);
	strcat(work,".arw");
	if ((ar=fopen(work,"wb")) == NULL)
	{	printf("Cannot open output file\n");  }
	strcpy(work,drawname);
	strcat(work,".lin");
	if ((li=fopen(work,"wb")) == NULL)
	{	printf("Cannot open output file\n");  }

	for (i=0;i<sqrcnt;i++)
	{
		fwrite(&squares[i],sizeof(square_type),1,sq);
	}
	for (i=0;i<crccnt;i++)
	{
		fwrite(&circles[i],sizeof(circle_type),1,cr);
	}
	for (i=0;i<filcnt;i++)
	{
		fwrite(&files[i],sizeof(file_type),1,fi);
	}
	for (i=0;i<arwcnt;i++)
	{
		fwrite(&arrows[i],sizeof(arrow_type),1,ar);
	}
	for (i=0;i<lincnt;i++)
	{
		fwrite(&lines[i],sizeof(line_type),1,li);
	}
	fclose(sq);
	fclose(cr);
	fclose(fi);
	fclose(ar);
	fclose(li);
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
	{ 	printf("Cannot open input file\n");  }
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
	refresh();
}

void get_filename(char *nametext)
{
	restorecrtmode();
	printf("Enter a drawing name up to eight characters long. \n: ");
	gets(nametext);
	setgraphmode(EGAHI);
	refresh();
}

void rescale(void)
{
	char ascii=0;
	int scan;
	erase_menu();
	prompt("Cursors to move window.","<enter> to set.","","");
	while (ascii!=ENTER)
	{
		mgetch(&scan,&ascii);
		switch(scan)
		{
			case UP :
			case FUP : basey-=160;
					   if (basey<0)
						   basey=32000;break;
			case DOWN  :
			case FDOWN : basey+=160;
					   if (basey>32000)
						   basey=0;break;
			case LEFT :
			case FLEFT : basex-=320;
					   if (basex<0)
						   basex=32000;break;
			case RIGHT :
			case FRIGHT : basex+=320;
					   if (basex>32000)
						   basex=0;break;
		}
		refresh();
		prompt("Cursors to move window.","<enter> to set.","","");
	}
}

void mgetch(int *scan,char *ascii)
{
	regs.h.ah = 0x00;
	int86(0x16,&regs,&regs);
	*scan=regs.h.ah;
	*ascii=regs.h.al;
}

int get_choice(int choicelim,int menusize,char menu[][20],char quick[])
{
	int i,j,selx1=2,selx2=70;
	int menupos=0;
	int scan=0;
	int cont=0;
	int contchg=0;
	char ascii=0;
	setcolor(WHITE);
	setviewport(1,330,600,349,1);
	clearviewport();
	rectangle(0,0,544,18);
	settextstyle(DEFAULT_FONT,0,0);
	setcolor(LIGHTCYAN);
	for (i=8;i<choicelim*75;i+=75)
	{
		outtextxy(i,6,&menu[menupos][0]);
		menupos++;
	}
	setcolor(LIGHTRED);
	rectangle(selx1,4,selx2,14);
	while (ascii!=ENTER)
	{
		mgetch(&scan,&ascii);
		switch (scan)
		{
			case FLEFT   :  setcolor(BLACK);
						   rectangle(selx1,4,selx2,14);
						   selx1-=75;selx2-=75;
						   if (selx1<0)
						   {
							   selx1=(choicelim-1)*75+2;selx2=(choicelim-1)*75+70;
							   if (menupos!=0)
							   {
									menupos=0;
									cont=0;
									clearviewport();
							   }
						   }
						   break;
			case FRIGHT  :  setcolor(BLACK);
						   rectangle(selx1,4,selx2,14);
						   selx1+=75;selx2+=75;
						   if (selx1>choicelim*75)
						   {
							   selx1=2;selx2=70;
							   menupos=choicelim;
							   cont++;
							   if (cont>1)
							   {
								   cont=0;
								   contchg=1;
							   }
						   }
						   break;
			case ESC     : clearviewport();
						   settextstyle(SMALL_FONT,0,0);
						   setviewport(0,0,639,349,1);
						   return 0;
		}
		if (ascii==ENTER)
		{
			settextstyle(SMALL_FONT,0,0);
			setviewport(0,0,639,349,1);
			return ((cont*choicelim+selx1/75)+1);
		}
		ascii=toupper(ascii);
		for (j=0;j<menusize;j++)
		{
			if (quick[j]==ascii)
				return j+1;
		}
		if (cont!=0||contchg==1)
		{
			clearviewport();
			contchg=0;
		}
		setcolor(WHITE);
		rectangle(0,0,544,18);
		setcolor(LIGHTCYAN);
		if (cont==0)
			menupos=0;
		else
			menupos=cont*choicelim;
		for (i=8;i<choicelim*75;i+=75)
		{
			if (menupos<menusize)
			{
				outtextxy(i,6,&menu[menupos][0]);
				menupos++;
			}
		}
		setcolor(LIGHTRED);
		rectangle(selx1,4,selx2,14);
	}
}

void erase_menu(void)
{
	setviewport(1,330,600,349,1);
	clearviewport();
	setviewport(0,0,639,349,1);
}
