#include <stdlib.h>
#include <stdio.h>

#define ESC 0x1b

main()
{
	printf("Yes%c[1;37;40mTest Test\n",ESC);
}
