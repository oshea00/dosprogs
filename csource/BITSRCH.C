/*
** BITSRCH.C
**
** Mike O'Shea  Monday night 'C' class level II
** 393-6495  m/s 9C-51
**
** Exercise #3
**
** INPUT
** Integers up to 32 bits in size
**
** takes a Target integer:    ex. 11100011 01110010
**         Source search:     ex.  11011111
**  n right bits of source:   ex. 3    (would give 111)
**
** OUTPUT
** Searches target with n bits of source and returns bit position
** where pattern is found within target.  If not found returns message
** to indicate it.
** Bits are counted left to right with the leftmost bit being 0.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define FIELD_SIZE sizeof(unsigned long int) * 8 /* field size in bits */
#define HIGH_BIT 0x80000000

int main(int, char *[]);
void dectobin(unsigned long int);

int main(int argc, char *argv[])
{
    unsigned long int mask, sourcen, source, target, n, target_hold;
    int i,j;
    mask = 0xffffffff;


    if (argc != 4)
    {
	printf("\nUsage is:\n");
	printf("BITSRCH Target_int  Source_int  n_right_bits_of_source\n");
	return(1);
    }
    else
    {
	target = strtoul(argv[1],NULL,10);
	source = strtoul(argv[2],NULL,10);
	n = strtoul(argv[3],NULL,10);

	if (n < 1)
	{
	    printf("n_right_bits_of_source must be > 0\n");
	    return(1);
	}

	/* left justify source bits */
	sourcen = source<<(FIELD_SIZE - n);

	/* create bit mask left justified */
	mask <<=(FIELD_SIZE - n);

	target_hold = target;

	/* left shift  target_hold and compare until match or end
	   of eligible target field */

	for (i=0;i < (FIELD_SIZE-n+1);i++)
	{
	    if (sourcen == (mask&target_hold))
		break;
	    target_hold <<=1;
	}

	/* report result of search.  (FIELD_SIZE-n+1) kept from searching
	   off the end of the target  */

	if (i == (FIELD_SIZE-n+1) )
	{
	    printf("%ld bit pattern in source not found.\n",n);
	    printf("SOURCE = ");
	    dectobin(source);
	    printf("TARGET = ");
	    dectobin(target);
	}
	else
	{
	    printf("%ld bit pattern in source found at bit %d.\n",n,i);
	    printf("SOURCE = ");
	    dectobin(source);
	    printf("TARGET = ");
	    dectobin(target);
	    for (j=0;j<i+9;j++,putchar(' '));
	    printf("^\n");
	}
    }
}

void dectobin(unsigned long int num)
{
    int i;
    for (i=0;i<FIELD_SIZE;++i,num<<=1)
	if ((num & HIGH_BIT) != 0)
	    putchar('1');
	else
	    putchar('0');
    putchar('\n');
}