#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>

#define  MENUSIZE 5
#define  SUB1SIZE 4
#define  SUBSIZE 10
#define  TEXTLENGTH 25
#define  UP 0x48
#define  DOWN 0x50
#define  LEFT 0x4b
#define  RIGHT 0x4d
#define  ENTER 0x0d
#define  ESC 0x1B

typedef struct {
	int x;
	int y;
	char choice[TEXTLENGTH];
	} choicetype;

struct text_info r;
union REGS regs;
union REGS inregs,outregs;

char screenbuffer[80*25*2];

choicetype mainmenulst[MENUSIZE] =
{ {2,2,"Choice1   "},
  {12,2,"Choice2   "},
  {22,2,"Choice3   "},
  {32,2,"Choice4   "},
  {42,2,"Quit   "} };

choicetype sub1menulst[SUB1SIZE] =
{ {2,2,"Subchoice1"},
  {2,3,"Subchoice2"},
  {2,4,"Subchoice3"},
  {2,5,"Subchoice4"} };

int main(void);
int mainmenu(void);
int sub1menu(void);
int getchoicelr(int,int,int,int,int,choicetype []);
int getchoiceud(int,int,int,int,int,choicetype []);
void drawmainmenu(void);
void border(int,int,int,int,char *);
void placechar(int,int,int);
void placestr(int,int,char *);
void box(int,int,int,int);
void cursoroff(void);
void cursoron(void);
int mgetch(int *,char *);

int main(void)
{
	textmode(C80);
	gettextinfo(&r);
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	clrscr();
	drawmainmenu();
	while (mainmenu());
	textattr(r.attribute);
	clrscr();
	cursoron();
	return 0;
}

int mainmenu(void)
{
	int choice,quit=9;
	choice = getchoicelr(MENUSIZE,BLACK,LIGHTCYAN,BLUE,LIGHTCYAN,mainmenulst);
	switch (choice)
	{
		case  1  :  sub1menu();break;
		case  2  :  break;
		case  3  :  break;
		case  4  :  break;
		case  5  :  quit=0;break;
	}
	return quit;
}

int sub1menu(void)
{
	int i,choice;
	gettext(1,1,80,25,screenbuffer);
	window(2,3,21,8);
	textcolor(YELLOW);
	textbackground(BLUE);
	clrscr();
	box(1,1,19,6);
	for (i=0;i<SUB1SIZE;i++)
	{
		placestr(sub1menulst[i].x,sub1menulst[i].y,sub1menulst[i].choice);
	}
	choice = getchoiceud(SUB1SIZE,BLUE,YELLOW,LIGHTGRAY,YELLOW,sub1menulst);
	window(1,1,80,25);
	puttext(1,1,80,25,screenbuffer);
	switch (choice)
	{
		case  0  :  break;   /*  ESC taken */
		case  1  :  break;
		case  2  :  break;
		case  3  :  break;
		case  4  :  break;
	}
}

void drawmainmenu(void)
{
	int i;
	char string[80];
	textbackground(BLACK);
	textcolor(CYAN);
	border(1,1,80,25," Module Template ");
	textcolor(LIGHTCYAN);
	textbackground(BLACK);
	for (i=0;i<MENUSIZE;i++)
	{
		placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
	}
	/* Help line */
	textcolor(RED);
	textbackground(LIGHTGRAY);
	placestr(2,25,"F1");
	textcolor(BLACK);
	placestr(4,25,"-HELP");
}

int getchoicelr(int num,int orgback,int orgfore,int selback,int selfore,choicetype mainmenulst[MENUSIZE])
{
	int i=0,keycode;
	char ascii=0;
	textcolor(selfore);
	textbackground(selback);
	placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
	cursoroff();
	while (ascii!=ENTER)
	{
		mgetch(&keycode,&ascii);
		switch (keycode)
		{
			case LEFT :  textcolor(orgfore);
					   textbackground(orgback);
					   placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
					   i-=1;
					   if (i<0)
						   i=num-1;
					   textcolor(selfore);
					   textbackground(selback);
					   placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);break;
			case RIGHT :  textcolor(orgfore);
					   textbackground(orgback);
					   placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);
					   i+=1;
					   if (i>num-1)
						   i=0;
					   textcolor(selfore);
					   textbackground(selback);
					   placestr(mainmenulst[i].x,mainmenulst[i].y,mainmenulst[i].choice);break;
			default   : break;
		}
	}
	return i+1;
}

int getchoiceud(int num,int orgback,int orgfore,int selback,int selfore,choicetype submenulst[SUBSIZE])
{
	int i=0,keycode;
	char ascii=0;
	textcolor(selfore);
	textbackground(selback);
	placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);
	cursoroff();
	while (ascii!=ENTER&&ascii!=ESC)
	{
		mgetch(&keycode,&ascii);
		switch (keycode)
		{
			case UP :  textcolor(orgfore);
					   textbackground(orgback);
					   placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);
					   i-=1;
					   if (i<0)
						   i=num-1;
					   textcolor(selfore);
					   textbackground(selback);
					   placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);break;
			case DOWN :  textcolor(orgfore);
					   textbackground(orgback);
					   placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);
					   i+=1;
					   if (i>num-1)
						   i=0;
					   textcolor(selfore);
					   textbackground(selback);
					   placestr(submenulst[i].x,submenulst[i].y,submenulst[i].choice);break;
			case RIGHT : ascii=ESC;break;
			case LEFT  : ascii=ESC;break;
			default   : break;
		}
	}
	if (ascii==ESC)
		return 0;
	return i+1;
}


/*
** This creates a titled box by specifying the top left and bottom
** right coordinates and the title.
** Note: be sure that your coordinates do not exceed the
**       current text window, it will cause truncation.
**
**
*/

void border(int a,int b,int c,int d,char *str)
{
	int i;
	for (i=a;i<c;i++) placechar(i,b,205);
	for (i=a;i<c;i++) placechar(i,d-1,205);
	for (i=b;i<d-1;i++) placechar(a,i,186);
	for (i=b;i<d-1;i++) placechar(c,i,186);
	placechar(a,b,201);
	placechar(c,b,187);
	placechar(a,d-1,200);
	placechar(c,d-1,188);
	placechar(a,b+2,204);
	placechar(c,b+2,185);
	for (i=a+1;i<c;i++) placechar(i,b+2,205);
	gotoxy((c-strlen(str)+a)/2,b);
	textcolor(YELLOW);
	textbackground(BLACK);
	cprintf("%s",str);
}


/*  puts a character at coords a,b - character is passed as int */

void placechar(int a,int b,int c)
{
	gotoxy(a,b);
	cprintf("%c",c);
};

void placestr(int a,int b,char *str)
{
	gotoxy(a,b);
	cprintf(str);
}


/* Same as border only without the Title option */

void box(int a,int b,int c,int d)
{
    int i;
	for (i=a;i<=c;i++) placechar(i,b,196);
	for (i=a;i<=c;i++) placechar(i,d,196);
	for (i=b;i<=d;i++) placechar(a,i,179);
	for (i=b;i<=d;i++) placechar(c,i,179);
	placechar(a,b,218);
	placechar(c,b,191);
	placechar(a,d,192);
	placechar(c,d,217);
}

void cursoroff(void)
{
	inregs.h.ah = 0x01;
	inregs.h.ch = 0x20;
	int86(0x10,&inregs,&outregs);
}

int mgetch(int *scan,char *ascii)
{
	regs.h.ah = 0x00;
	int86(0x16,&regs,&regs);
	*scan=regs.h.ah;
	*ascii=regs.h.al;
}

void cursoron(void)
{
	inregs.h.ah = 0x01;
	inregs.h.ch = 6;
	inregs.h.cl = 7;
	int86(0x10,&inregs,&outregs);
}


