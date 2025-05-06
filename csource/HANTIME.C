/*
**
** HANTIME.C
**
** Calculates the time to do the towers of hanoi problem
**
*/


#include <stdio.h>
#include <stdlib.h>

#define NUM_DISKS  365
#define MOVES_PER_SECOND 10


main()
{
	double number_moves = 1;
	double time_years = 0;
	double time_

	int x;

	for (x=0;x<NUM_DISKS;x++)
		number_moves*= 2;

	printf("CODE: %015.0lf\n",number_moves);

	time_of_problem = ((((number_moves / MOVES_PER_SECOND)/1800)/24)/365.25)




}
