#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#define BIOSINT 0x15

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
	unsigned bytein;
	outportb(0x201,0xFF);
	bytein = inportb(0x201);
	memcpy(&switches,&bytein,1);
	printf("%d %d %d %d %d %d %d %d\n",
						   switches.bit0,
						   switches.bit1,
						   switches.bit2,
						   switches.bit3,
						   switches.bit4,
						   switches.bit5,
						   switches.bit6,
						   switches.bit7);
	bytein = inportb(0x201);
	memcpy(&switches,&bytein,1);
	while (i--)
	{
	printf("%d %d %d %d %d %d %d %d\n",
						   switches.bit0,
						   switches.bit1,
						   switches.bit2,
						   switches.bit3,
						   switches.bit4,
						   switches.bit5,
						   switches.bit6,
						   switches.bit7);
	bytein = inportb(0x201);
	memcpy(&switches,&bytein,1);
	}
}
