#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

char screenbuffer[80];
union REGS inregs,outregs;

char input[80];

main()
{
	textmode(C80);
	textcolor(WHITE);
	textbackground(BLUE);
	clrscr();
	gotoxy(20,20);
	cprintf("ABCDEFG");
	gettext(20,20,26,20,screenbuffer);
	getscreenchars(20,20,7);
	clrscr();
}

getscreenchars(int x,int y,int length)
{
	int i;
	inregs.h.ah = 0x08;
	inregs.h.bh = 0x00;
	for (i=0;i<length;i++)
	{
		gotoxy(x+i,y);
		int86(0x10,&inregs,&outregs);
		input[i] = outregs.h.al;
	}
	input[i]=NULL;
}