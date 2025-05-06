#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <dos.h>

union REGS reg;

main()
{
	int i = 2000;
	while (i--)
	{
		reg.h.ah = 0x00;
		int86(0x16,&reg,&reg);
		printf("AH = %d\n",reg.h.ah);
		printf("AL = %d\n",reg.h.al);
	}
}


