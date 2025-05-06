/*
** CALC2.C
**
** Calculates like a calculator
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DX 30
#define DY 6
#define DX1 DX+14
#define DY1 DY+3
#define TX DX+1
#define TY DY+1
#define LX 3
#define LY 5
#define LX1 16
#define LY1 23
#define IX 20
#define IY 20

int main(void);
int calculate(void);
void border(int,int,int,int,char *);
void placechar(int,int,int);
void placestr(int,int,char *);
void box(int,int,int,int);
void init_screen(void);
void add_to_list(double);
void print_list(void);

double accumulator=0;
double list[18];
int listcnt=0;


int main(void)
{
	init_screen();
	while (calculate());
	clrscr();
	exit(0);
}

void init_screen(void)
{
	clrscr();
	border(1,1,60,25,"Calculator");
	box(DX,DY,DX1,DY1);
	gotoxy(DX,DY-1);
	cprintf("Total:");
	box(LX,LY-1,LX1+1,LY1+1);
	gotoxy(IX,IY);
	cprintf("C = clear entry");
	gotoxy(IX,IY+1);
	cprintf("R = reset and clear tape");
	gotoxy(IX,IY+2);
	cprintf("E = exit program");
	box(IX-1,IY-1,IX+24,IY+4);
	gotoxy(TX,TY);
	cprintf("%12.2lf",0l);
}

int calculate(void)
{
	double numin;
	double *numptr;
	char keysin[30];
	int i=0,decflag=0;
	char c;
	numptr = &numin;
	c = getch();
	while (isdigit(c)||c=='.')
	{
		if (decflag==1&&c=='.')
		{}
		else
		{
			if (c=='.')
				decflag=1;
			keysin[i] = c;
			keysin[i+1]=NULL;
			gotoxy(TX,TY);
			*numptr = strtod(keysin,NULL);
			cprintf("%12.2lf",numin);
			i++;
		}
		c=getch();
	}
	decflag=0;
	switch (c)
	{
		case 'C' :
		case 'c' :  numin=0;
					gotoxy(TX,TY);
					cprintf("%12.2lf",numin);
					break;
		case 'R' :
		case 'r' :  numin=0;
					accumulator=0;
					gotoxy(TX,TY);
					cprintf("%12.2f",accumulator);
					listcnt=0;
					print_list();
					break;
		case '+' :  accumulator+=numin;
					gotoxy(TX,TY);
					cprintf("%12.2f",accumulator);
					add_to_list(numin);
					print_list();
					break;
		case '-' :  accumulator-=numin;
					gotoxy(TX,TY);
					cprintf("%12.2f",accumulator);
					numin*=-1;
					add_to_list(numin);
					numin*=-1;
					print_list();
					break;
		case '/' :
					if (numin==0)
						numin=1;
					accumulator/=numin;
					gotoxy(TX,TY);
					cprintf("%12.2f",accumulator);
					break;
		case '*' :  accumulator*=numin;
					gotoxy(TX,TY);
					cprintf("%12.2f",accumulator);
					break;
		case 'E' :
		case 'e' :  return 0;
	}
	return 1;
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
	for (i=2;i<c;i++) placechar(i,b,205);
	for (i=2;i<c;i++) placechar(i,d-1,205);
	for (i=2;i<d-1;i++) placechar(a,i,186);
	for (i=2;i<d-1;i++) placechar(c,i,186);
	placechar(a,b,201);
	placechar(c,b,187);
	placechar(a,d-1,200);
	placechar(c,d-1,188);
	placechar(a,b+2,204);
	placechar(c,b+2,185);
	for (i=2;i<c;i++) placechar(i,b+2,205);
	gotoxy((c-strlen(str))/2,b+1);
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

void add_to_list(double entry)
{
	if (listcnt>17)
		listcnt=0;
	list[listcnt]=entry;
	listcnt++;
}

void print_list(void)
{
	int i,j,x,y;
	x = LX+1;
	y = LY;
	window(LX+1,LY,LX1,LY1-1);
	clrscr();
	window(1,1,80,25);
	for (i=0;i<listcnt;i++)
	{
		gotoxy(x,y++);
		cprintf("%+12.2lf",list[i]);
	}
}