/*
** MAINMEN.C
**
** Demo program for HOLE-IN-THE-WALL video system...
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <dos.h>

int main(void);
void border(int,int,int,int,char *);
void placechar(int,int,int);
void placestr(int,int,char *);
void box(int,int,int,int);
void init_screen(void);
void rentals();
void membership();
void tapes();
void inventory();
void reports();
void options();
void bad_menu_option();
void rent_help();
void return_trans();
void rent_trans();
void wait(int);
void scan_member_code();
void scan_tape_code(int);
void type_member_code();
void type_tape_code(int);
void tape_help();

#define TRUE 1
#define FALSE 0

int main(void)
{
	init_screen();
	system("cls");
	exit(0);
}

void init_screen(void)
{
	int c;
	int quit=FALSE;
	textmode(C80);
	while (quit==FALSE)
	{
		clrscr();
		border(1,1,79,25,"Hole-In-The-Wall Video");
		box(19,8,60,20);
		placestr(35,9,"Main Menu");
		placestr(25,11,"1 ......... Rentals/Returns");
		placestr(25,12,"2 ......... Membership");
		placestr(25,13,"3 ......... Tape Status (in/out)");
		placestr(25,14,"4 ......... Inventory");
		placestr(25,15,"5 ......... Reports");
		placestr(25,16,"6 ......... System Options");
		placestr(25,17,"7 ......... Quit");

		c=getch();
		switch (c)
		{
			case  '1'   :  rentals();break;
			case  '2'   :  membership();break;
			case  '3'   :  tapes();break;
			case  '4'   :  inventory();break;
			case  '5'   :  reports();break;
			case  '6'   :  options();break;
			case  '7'   :  quit=TRUE;break;
			default     :  bad_menu_option();
		}
	}
}

void rentals()
{
	char c=' ';
	while ((c!='q')&&(c!='Q'))
	{
		clrscr();
		placestr(27,2,"VIDEO RENTALS AND RETURNS");
		placestr(27,3,"-------------------------");
		placestr(2,4,"ID: 999999");
		placestr(2,6,"FIRST NAME   LAST NAME    MI   HOME PHONE WORK PHONE");
		placestr(2,8,"XXXXXXXXXXXX XXXXXXXXXXXX X    XXX-XXXX   XXX-XXXX");
		placestr(2,10,"CITY XXXXXXXXXXXXXXX  STATE XX  ZIP CODE 99999");
		placestr(2,12,"                               TAPES RENTED");
		placestr(2,13,"                               ------------");
		placestr(2,15,"  ID       TITLE       RENTED   RETURNED DAYS   FEE        SUB-TOTAL");
		placestr(2,17,"9999999 XXXXXXXXXXXXXX 99/99/99 99/99/99 99     9.99         99.99");
		placestr(2,18,"9999999 XXXXXXXXXXXXXX 99/99/99 99/99/99 99     9.99         99.99");
		placestr(2,19,"9999999 XXXXXXXXXXXXXX 99/99/99 99/99/99 99     9.99         99.99");
		placestr(2,20,"9999999 XXXXXXXXXXXXXX 99/99/99 99/99/99 99     9.99         99.99");
		placestr(2,22,"                                               SALES TAX:    99.99");
		placestr(2,23,"                                                   TOTAL:    99.99");
		placestr(2,24,"Menu: Rent rEturn Help Cancel Total Quit");
		c=getch();
		switch (c)
		{
			case 'r' :
			case 'R' :  rent_trans();break;
			case 'e' :
			case 'E' :  return_trans();break;
			case 'h' :
			case 'H' :  rent_help();break;
			case 'q' :
			case 'Q' :  break;
			case 'c' :
			case 'C' :  gotoxy(2,24);clreol();
						placestr(2,24,"Cancel current transaction");
						wait(2500);
						placestr(2,24,"Menu: Rent reTurn Help Cancel Total Quit");
						break;
			case 't' :
			case 'T' :  gotoxy(2,24);clreol();
						placestr(2,24,"Total transaction and print receipt");
						wait(2500);
						placestr(2,24,"Main Menu: Rent reTurn Help Cancel Total Quit");
						break;
			default  :  gotoxy(2,24);clreol();
						placestr(2,24,"Invalid Selection");
						wait(1000);
						placestr(2,24,"Main Menu: Rent reTurn Help Cancel Total Quit");
		}
	}
}

void tapes()
{
	char c=' ';
	while ((c!='q')&&(c!='Q'))
	{
		clrscr();
		placestr(27,2,"TAPE STATUS INQUIRY SCREEN");
		placestr(27,3,"--------------------------");
		placestr(2,5,"  ID       TITLE       RENTED   RETURNED DAYS   FEE     ");
		placestr(2,7,"9999999 XXXXXXXXXXXXXX 99/99/99 99/99/99 99     9.99    ");
		placestr(2,9,"                               MEMBER RENTING");
		placestr(2,10,"                               --------------");
		placestr(2,12,"MEMBER ID: 999999");
		placestr(2,14,"FIRST NAME   LAST NAME    MI   HOME PHONE WORK PHONE");
		placestr(2,16,"XXXXXXXXXXXX XXXXXXXXXXXX X    XXX-XXXX   XXX-XXXX");
		placestr(2,18,"CITY XXXXXXXXXXXXXXX  STATE XX  ZIP CODE 99999");
		placestr(2,24,"Menu: Scan-code Type-code Help Quit ");
		c=getch();
		switch (c)
		{
			case 's' :
			case 'S' :  gotoxy(2,24);clreol();scan_tape_code(2);break;
			case 't' :
			case 'T' :  gotoxy(2,24);clreol();type_tape_code(2);break;
			case 'q' :
			case 'Q' :  break;
			case 'h' :
			case 'H' :  tape_help();break;
			default  :  gotoxy(2,24);clreol();
						placestr(2,24,"Invalid Selection");
						wait(1000);
						placestr(2,24,"Menu: Scan-code Type-code Help Quit");
		}
	}
}


void return_trans()
{
	char c;
	placestr(2,24,"Return Menu: Scan-codes Type-in-codes Cancel ");
	c=getch();
	switch (c)
	{
		case 's' :
		case 'S' :  scan_member_code();scan_tape_code(18);break;
		case 't' :
		case 'T' :  type_member_code();type_tape_code(18);break;
		case 'd' :
		case 'D' :
		case 'c' :
		case 'C' :  break;
	}
}

void rent_trans()
{
	char c;
	placestr(2,24,"Rental Menu: Scan-codes Type-in-codes Cancel ");
	c=getch();
	switch (c)
	{
		case 's' :
		case 'S' :  scan_member_code();scan_tape_code(18);break;
		case 't' :
		case 'T' :  type_member_code();type_tape_code(18);break;
		case 'd' :
		case 'D' :
		case 'c' :
		case 'C' :  break;
	}
}

void scan_member_code()
{
	gotoxy(2,24);
	clreol();
	placestr(2,24,"Scan member card: (hit enter to simulate)");
	getch();
	gotoxy(2,24);clreol();
	placestr(2,24,"Member: 999999 ");
	wait(1500);
}

void scan_tape_code(int d)
{
	placestr(d,24,"Scan tapes <enter> when done: ");
	wait(2000);
	gotoxy(d,24);clreol();
	placestr(d,24,"  Tapes: 9999999                   ");
	wait(1500);
}

void type_member_code()
{
	gotoxy(2,24);
	clreol();
	placestr(2,24,"Type member id: (hit enter to simulate)");
	getch();
	gotoxy(2,24);clreol();
	placestr(2,24,"Member: 999999 ");
	wait(1500);
}

void type_tape_code(int d)
{
	placestr(d,24,"Type Tape ID's:");
	wait(2000);
	gotoxy(d,24);clreol();
	placestr(d,24,"Tapes: 9999999");
	wait(1500);
}

void rent_help()
{
	clrscr();
	border(1,1,79,25,"RENTAL HELP SCREEN");
	placestr(10,4,"This screen would provide help information.");
	wait(2500);
}

void tape_help()
{
	clrscr();
	border(1,1,79,25,"TAPE INQUIRY HELP SCREEN");
	placestr(10,4,"This screen would provide help information.");
	wait(2500);
}

void membership()
{
	clrscr();
	placestr(30,12,"Membership Option");
	wait(1500);
}

void inventory()
{
	clrscr();
	placestr(30,12,"Inventory Option");
	wait(1500);
}

void reports()
{
	clrscr();
	placestr(30,12,"Reports Option");
	wait(1500);
}

void options()
{
	clrscr();
	placestr(30,12,"System Options");
	wait(1500);
}

void bad_menu_option()
{
	clrscr();
	placestr(30,12,"Menu choice is not valid");
	wait(1500);
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

void wait(int i)
{
	int j=2;
	double e=100,f=3.4;
	j*=i;
	while (j--)
	{  e/=f;  }
}
