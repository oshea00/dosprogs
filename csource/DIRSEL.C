#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dir.h>
#include <string.h>

#define FILELIM 20

char files[FILELIM][13];


main()
{
	textmode(C80);
	window(
	if (dirtolist())
	{
		choosefile();
	}
	else
	{
		getfilename();
	}
}

int dirtolist()
{
	struct ffblk ffblk;
	int done,i=0;
	done = findfirst("*.adr",&ffblk,0);
	if (!done)
	{
		while (i<FILELIM&&!done)
		{
			strcpy(files[i],ffblk.ff_name);
			i++;
			done = findnext(&ffblk);
		}
		return 1;
	}
	else
	{
		return 0;
	}

}

choosefile