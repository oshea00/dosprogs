/*
** HEXDUMP.C
**
** Mike O'Shea  Monday night 'C' class level II
** 393-6495  m/s 9C-51
**
** Exercise #2
**
** INPUT
** takes a filename from the command prompt when program is
** executed (binary or text)
**
** OUTPUT
** Provides a hex dump listing of the file with displacement line
** numbers.  The printable ASCII (32 - 126) characters are printed
** alongside the hex code line.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define HEXLINE_SIZE 16  /* number of hex codes per detail line */

void print_hex_line(unsigned char *,int);
int main(int, char *[]);

int main(int argc, char *argv[])
{
    FILE *in;
    unsigned char val[HEXLINE_SIZE];
    int i = 0;

    if (argc == 1)
    {
	printf("\nUsage is:\n");
	printf("HEXDUMP filename.ext\n");
	return(1);
    }
    else
    {
	if ((in = fopen(argv[1],"rb")) == NULL)
	{
	    printf("\nError opening given filename %s\n",argv[1]);
	    return(4);
	}

	printf(" Hex dump of file <%s> :\n",argv[1]);
	printf("\n");

	/* read up to HEXLINE_SIZE bytes from the input file.
	** Number of bytes read will be assigned to i.
	** Print bytes read and continue until
	** i = 0 causing while loop to break.
	*/

	while (i = fread(&val[0],sizeof(unsigned char),HEXLINE_SIZE,in))
	{
	    print_hex_line(&val[0],i);
	};

	fclose(in);
	return(0);
    };
}

void print_hex_line(unsigned char *val,int cnt)
{
    int i;
    static long int disp = 0l;

    /* print current displacement in file*/

    printf("%08lX  ",disp);

    /* print hex codes */

    for (i=0;i<cnt;i++)
    {
	if (i == (HEXLINE_SIZE/2) - 1)
	    printf("%02X  ",val[i]);
	else
	    printf("%02X ",val[i]);

	/* keep running count of displacement for characters printed */
	disp++;
    };

    /* check for short last line and print dummy spaces to allow
    ** ASCII text to line up
    */

    if (i < HEXLINE_SIZE)
    {
	for (;i<HEXLINE_SIZE;i++)
	{
	    if (i == (HEXLINE_SIZE/2) - 1)
		printf("    ");
	    else
		printf("   ");
	};
    };


    /* print valid ASCII equivalents                  */

    for (i=0;i<cnt;i++)
    {
	if (!isprint(val[i]))
	    printf(".");
	else
	    printf("%c",val[i]);
    };

    /* print a line feed */

    printf("\n");

}

