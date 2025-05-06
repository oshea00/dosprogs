#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <process.h>
#include <ctype.h>

#define MENUSIZE  7
#define TEXTLENGTH 25
#define UP 0x48
#define DOWN 0x50
#define ENTER 0x0d

typedef struct {
	int highlight;
	char choice[TEXTLENGTH];
} menutype;


int main(void);
int mainmenu(void);
int getchoice(int,int,int,int,int,int,int,menutype[]);
void drawmenu(void);
void border(int,int,int,int,char *);
void placechar(int,int,int);
void placestr(int,int,char *);
void box(int,int,int,int);
void cursoroff(void);
void cursoron(void);
int mgetch(int *,char*);

struct text_info r;
union REGS regs;
union REGS inregs,outregs;

char menubuffer[80*25*2];

menutype mainmenulst[MENUSIZE] =
		{ {0,"Itinerary"},
		  {0,"Phone & address list"},
		  {1,"mEssages & memos"},
		  {0,"Mileage"},
		  {1,"eXpenses"},
		  {4,"checKlist"},
		  {0,"Quit"} };
char quickmain[10] = "IPEMXKQ";

int main(void)
{
	textmode(C80);
	gettextinfo(&r);
	textbackground(BLACK);
	textcolor(CYAN);
	clrscr();
	while (mainmenu());
	textattr(r.attribute);
	clrscr();
	cursoron();
	return 0;
}

int mainmenu(void)
{
	int choice,quit=9;
	drawmenu();
	choice = getchoice(33,8,MENUSIZE,BLACK,LIGHTCYAN,BLUE,LIGHTCYAN,mainmenulst);
	switch (choice)
	{
		case  1  :  spawnl(P_WAIT,"modtemp",NULL);break;
		case  2  :  spawnl(P_WAIT,"phoneno",NULL);break;
		case  3  :  spawnl(P_WAIT,"message",NULL);break;
		case  4  :  break;
		case  5  :  break;
		case  6  :  break;
		case  7  :  quit=0;break;
	}
	return quit;
}

void drawmenu(void)
{
	int i;
	static int init=0;
	char string[80];
	if (init)
	{
		puttext(1,1,80,25,menubuffer);
	}
	else
	{
		border(1,1,80,25," Travel Manager ");
		textcolor(LIGHTCYAN);
		textbackground(BLACK);
		for (i=8;i<MENUSIZE+8;i++)
		{
			placestr(33,i,mainmenulst[i-8].choice);
			textcolor(WHITE);
			gotoxy(33+mainmenulst[i-8].highlight,i);
			putch(mainmenulst[i-8].choice[mainmenulst[i-8].highlight]);
			textcolor(LIGHTCYAN);
		}
		/* Help line */
		textcolor(RED);
		textbackground(LIGHTGRAY);
		placestr(2,25,"F1");
		textcolor(BLACK);
		placestr(4,25,"-HELP");

		/* save for subsequent redrawing */
		gettext(1,1,80,25,menubuffer);
		init = 1;
	}
}

int getchoice(int x,int y,int num,int orgback,int orgfore,int selback,int selfore,menutype mainmenulst[MENUSIZE])
{
	int i=0,j,keycode;
	char ascii=0;
	textcolor(selfore);
	textbackground(selback);
	placestr(x,y+i,mainmenulst[i].choice);
	textcolor(WHITE);
	gotoxy(x+mainmenulst[i].highlight,y+i);
	putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
	textcolor(selfore);
	cursoroff();
	while (ascii!=ENTER)
	{
		mgetch(&keycode,&ascii);
		switch (keycode)
		{
			case UP :  textcolor(orgfore);
					   textbackground(orgback);
					   placestr(x,y+i,mainmenulst[i].choice);
					   textcolor(WHITE);
					   gotoxy(x+mainmenulst[i].highlight,y+i);
					   putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
					   textcolor(orgfore);
					   i-=1;
					   if (i<0)
						   i=num-1;
					   textcolor(selfore);
					   textbackground(selback);
					   placestr(x,y+i,mainmenulst[i].choice);
					   textcolor(WHITE);
					   gotoxy(x+mainmenulst[i].highlight,y+i);
					   putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
					   textcolor(selfore);break;
			case DOWN :  textcolor(orgfore);
					   textbackground(orgback);
					   placestr(x,y+i,mainmenulst[i].choice);
					   textcolor(WHITE);
					   gotoxy(x+mainmenulst[i].highlight,y+i);
					   putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
					   textcolor(orgfore);
					   i+=1;
					   if (i>num-1)
						   i=0;
					   textcolor(selfore);
					   textbackground(selback);
					   placestr(x,y+i,mainmenulst[i].choice);
					   textcolor(WHITE);
					   gotoxy(x+mainmenulst[i].highlight,y+i);
					   putch(mainmenulst[i].choice[mainmenulst[i].highlight]);
					   textcolor(selfore);break;
			default   : break;
		}
		if (isalpha(ascii))
		{
			for (j=0;j<MENUSIZE;j++)
			{
				if (quickmain[j]==toupper(ascii))
				{
					return j+1;
				}
			}
		}
	}
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
	gotoxy((c-strlen(str)+a)/2,b+1);
	textcolor(YELLOW);
	textbackground(BLUE);
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
	for (i=a;i<c;i++) placechar(i,b,205);
	for (i=a;i<c;i++) placechar(i,d-1,205);
	for (i=b;i<d-1;i++) placechar(a,i,186);
	for (i=b;i<d-1;i++) placechar(c,i,186);
    placechar(a,b,201);
    placechar(c,b,187);
    placechar(a,d-1,200);
    placechar(c,d-1,188);
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

