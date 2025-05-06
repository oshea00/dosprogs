#include <stdio.h>
#include <stdlib.h>

void main(void);
int total_complexity(int,int,int,int);

int inherent,scope,design,pgm;

void main(void)
{
	printf("Enter values for Inherent,Scope,Design and Program complexity:\n");
	scanf("%d %d %d %d",&inherent,&scope,&design,&pgm);
	printf("Total complexity = %d\n",total_complexity(inherent,scope,design,pgm));
}

int total_complexity(int inherent,int scope,int design,int pgm)
{
	return inherent*scope*(1+design+(pgm*design));
}
