#include <stdlib.h>
#include <stdio.h>

int extern stick(int);
int extern button(int);

main()
{
	int i = 1000;
	while (i--)
	{
		printf("x1 value = %d  ",stick(1));
		printf("button pressed : %s\n",button(32)?"yes":"no");
	}
}

