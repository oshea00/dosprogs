#include <stdlib.h>
#include <stdio.h>

main(int argc, char *argv[])
{
	int i,j=0;
	int hexflag=0;
	if (argc>1)
		hexflag=1;
	for (i=0;i<256;i++)
	{
		if (i==0||i==10||i==9||i==26||i==27||i==7||i==13||i==8||i==255)
		{}
		else
		{
			if (hexflag)
				printf("%02X=%c ",i,i);
			else
				printf("%03d=%c ",i,i);
			j++;
			if (j>12)
			{
				printf("\n");
				j=0;
			}
		}
	}
}