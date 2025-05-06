#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define BIOSINT 0x15

/*
**struct WORDREGS {
**	unsigned int	ax, bx, cx, dx, si, di, cflag, flags;
**};
**
**struct BYTEREGS {
**	unsigned char	al, ah, bl, bh, cl, ch, dl, dh;
**};
**
**union	REGS	{
**	struct	WORDREGS x;
**	struct	BYTEREGS h;
**};
*/
typedef struct {
unsigned bit0 : 1;
unsigned bit1 : 1;
unsigned bit2 : 1;
unsigned bit3 : 1;
unsigned bit4 : 1;
unsigned bit5 : 1;
unsigned bit6 : 1;
unsigned bit7 : 1;
} byte;

union REGS regs;

main()
{
	byte switches;
	int i = 20;
	clear_regs(&regs);
	regs.h.ah = 0x84;
	regs.x.dx = 0x00;
	int86(BIOSINT,&regs,&regs);
	memcpy(&switches,&regs.h.al,1);
	printf("Switch 0 %d\n",switches.bit0);
	printf("Switch 1 %d\n",switches.bit1);
	printf("Switch 2 %d\n",switches.bit2);
	printf("Switch 3 %d\n",switches.bit3);
	printf("Switch 4 %d\n",switches.bit4);
	printf("Switch 5 %d\n",switches.bit5);
	printf("Switch 6 %d\n",switches.bit6);
	printf("Switch 7 %d\n",switches.bit7);

	if (regs.x.cflag)
	{
		printf("Carry flag %d",regs.x.cflag);
	}

	while (i--)
	{
		clear_regs(&regs);
		regs.h.ah = 0x84;
		regs.x.dx = 0x01;
		int86(BIOSINT,&regs,&regs);
		printf("paddle A: x= %d y= %d  paddle B: x= %d y=%d\n",regs.x.ax,
		regs.x.bx,regs.x.cx,regs.x.dx);

		if (regs.x.cflag)
		{
			printf("Carry flag %d",regs.x.cflag);
		}
	}
}

clear_regs()
{
	 regs.x.ax = 0x00;
	 regs.x.bx = 0x00;
	 regs.x.cx = 0x00;
	 regs.x.si = 0x00;
	 regs.x.di = 0x00;
}