/*
**  Mike O'Shea 393-6495 m/s 9C-51 G-3933
**
**  HELP.C
**
**  INPUT:
**  Takes a file name from the command prompt
**  OUTPUT:
**  File printed on screen
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main(int, char *[]);
void report(char *);

int main(int argcnt, char *argval[])
{
	char screenbuffer[80*25*2];
	textmode(C80);
	gettext(1,1,80,25,screenbuffer);
	textcolor(YELLOW);
	textbackground(BLUE);
	clrscr();
	report(argval[1]);
	puttext(1,1,80,25,screenbuffer);return 0;
}

void report(char *filename)
{
	FILE *in;
	char ch;
	if ((in = fopen(filename,"r")) == NULL)
		printf("\nError trying to open: %s\n",filename);
	else
	{
		while ((ch = getc(in)) != EOF)
		{
			if (ch=='\n')
				gotoxy(1,wherey()+1);
			else
			if (ch=='\t')
				cprintf("    ");
			else
				putchar(ch);   /* writes char to standard output */
		}
	fclose(in);
	gotoxy(1,wherey());
	cprintf("Press any key to return to application");
	getch();
	}
}

