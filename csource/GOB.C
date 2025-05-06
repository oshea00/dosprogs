#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <process.h>
#include <string.h>

main()
{
	char input[80];
	char c;
	int i;

	system("cls");

	printf("Type password: ");
	c = getch();
	for (i=0;c!=0x0d;i++)
	{
		input[i] = c;
		putchar('*');
		c = getch();
	}
	input[i] = NULL;
	if (!strcmpi(input,"donutke"))
	{
		system("copy c:\\data\\bad.dat > null");
		system("ren bad.dat cbook.exe > null");
		spawnl(P_WAIT,"cbook",NULL);
		system("del cbook.exe > null");
	}
	else
	{
		printf("\nNaughty, Naughty you can't look without the password!\n");
	}
}
