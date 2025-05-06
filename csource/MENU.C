
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <graphics.h>
#include <string.h>
#include <math.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER '\r'

int main(void);
int get_choice(int,int,char[][]);
void mgetch(int *,char *);


char mainmenu[9][20] = { {"Choice1"},
						 {"Choice2"},
						 {"Choice3"},
						 {"Choice4"},
						 {"Choice5"},
						 {"Choice6"},
						 {"Choice7"},
						 {"CHoice8"},
						 {"Choice9"} };

union REGS regs;

int main(void)
{
	int choice;
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
	choice=get_choice(7,9,mainmenu);
	restorecrtmode();
	printf("Choice # = %d",choice);
	getch();
	return 0;
}

int get_choice(int choicelim,int menusize,char menu[][20])
{
	int i,selx1=2,selx2=70;
	int menupos=0;
	int scan=0;
	int cont=0;
	char ascii=0;
	setcolor(WHITE);
	setviewport(1,330,600,348,1);
	rectangle(0,0,544,18);
	setcolor(LIGHTCYAN);
	for (i=8;i<choicelim*75;i+=75)
	{
		outtextxy(i,6,&mainmenu[menupos][0]);
		menupos++;
	}
	setcolor(LIGHTRED);
	rectangle(selx1,4,selx2,14);
	while (ascii!=ENTER)
	{
		mgetch(&scan,&ascii);
		switch (scan)
		{
			case LEFT   :  setcolor(BLACK);
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
			case RIGHT  :  setcolor(BLACK);
						   rectangle(selx1,4,selx2,14);
						   selx1+=75;selx2+=75;
						   if (selx1>choicelim*75)
						   {
							   selx1=2;selx2=70;
							   menupos=choicelim;
							   cont++;
							   if (cont>1)
								   cont=0;
						   }
						   break;
		}
		if (ascii==ENTER)
		{
			return ((cont*choicelim+selx1/75)+1);
		}
		if (cont!=0)
			clearviewport();
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

void mgetch(int *scan,char *ascii)
{
	regs.h.ah = 0x00;
	int86(0x16,&regs,&regs);
	*scan=regs.h.ah;
	*ascii=regs.h.al;
}


