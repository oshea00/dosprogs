/*
** MAKENAME.C
**
** This routine generates all names with the pattern SRV01xxx
**
**
*/

#include <stdlib.h>
#include <stdio.h>

int main(void);

int main(void)
{
    char net_name[9] = "SRV01";
    char p1,p2,p3;
    for (p1='A';p1 < 'Z';p1++)
	for (p2='A';p2 < 'Z';p2++)
	    for (p3='A';p3 < 'Z';p3++)
	    {
		net_name[5] = p1;
		net_name[6] = p2;
		net_name[7] = p3;
		net_name[8] = NULL;
		printf("%s\n",net_name);
	    };
}
