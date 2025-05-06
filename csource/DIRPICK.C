
#include <stdio.h>
#include <stdlib.h>
#include <dir.h>
#include <conio.h>
#include <string.h>


struct ffblk ffblk;
char files[20][13];
int filescnt;
char selected_file[13];
char screenbuffer[80*25*2];

void main(void);
int fillfilelist(void);
void select_file_name(void);
void draw_file_box(void);
void placechar(int,int,char);
void box(int,int,int,int);
void list_files(int);

void main()
{
	textmode(C80);
	textcolor(YELLOW);
	textbackground(BLUE);
	clrscr();
	fillfilelist();
	select_file_name();
	textcolor(YELLOW);
	textbackground(BLUE);
	clrscr();
	cprintf("File = %s",selected_file);
}

int fillfilelist()
{
	int done,i=0;
	done = findfirst("*.adr",&ffblk,0);
	if (!done)
	{
		while (!done)
		{
			strcpy(files[i],ffblk.ff_name);
			i++;
			done = findnext(&ffblk);
		}
		filescnt=i;
		return 1;
	}
	else
	{
		return 0;
	}
}

void select_file_name()
{
	gettext(1,1,80,25,screenbuffer);
	draw_file_box();
	list_files(0);
	gotoxy(3,2);
	textcolor(WHITE);
	cprintf("Enter file name: ");

	/* The following simulates getting a string via getstrfld */
	gotoxy(20,2);
	textbackground(BLACK);
	cprintf("             ");
	gotoxy(20,2);
	gets(selected_file);

	/**************/

	window(1,1,80,25);
	puttext(1,1,80,25,screenbuffer);
}

void list_files(int starting)
{
	int i,j,c=0,end;
	j=starting;
	end=j+20;
	if (end>filescnt)
		end=filescnt;
	while (j<end)
	{
		for (i=0;i<5;i++)
		{
			gotoxy(14*c+3,i+4);
			cprintf("%s",files[j]);
			j++;
		}
		c++;
	}
}

void draw_file_box()
{
	window(5,5,76,13);
	textbackground(RED);
	clrscr();
	box(1,1,71,9);
}


/* Same as border only without the Title option */

void box(int a,int b,int c,int d)
{
    int i;
	for (i=a;i<=c;i++)
		placechar(i,b,196);  /* top --- */
	for (i=a;i<=c;i++)
		placechar(i,d,196);  /* bottom --- */
	for (i=b;i<=d;i++)
		placechar(a,i,179);  /* left | */
	for (i=b;i<=d;i++)
		placechar(c,i,179);  /* right | */
	placechar(a,b,218);
	placechar(c,b,191);
	placechar(a,d,192);
	placechar(c,d,217);
}

void placechar(int x,int y,char chr)
{
	gotoxy(x,y);
	putch(chr);
}



