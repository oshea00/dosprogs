

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

main()
{
	double num;
	double *numptr;
	char try[15];
	numptr = &num;
	strcpy(try,"23.3");
	*numptr = strtod(try,NULL);
	printf("%12.2lf",num);
}

