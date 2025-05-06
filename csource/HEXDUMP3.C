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

void print_hex_line(unsigned char *,int);
int main(int, char *[]);

int main(int argc, char *argv[])
{
    FILE *in;
    unsigned char val[15];
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
	    printf("\nError opening given filename\n");
	    return(4);
	}

	printf(" Hex dump of file <%s> :\n",argv[1]);
	printf("\n");

	/* read up to sixteen bytes from the input file.
	** Number of bytes read will be assigned to i.  If EOF
	** then i = 0 and while loop breaks.
	*/

	while (i = fread(&val[0],sizeof(unsigned char),16,in))
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
    static long int disp = 0;

    /* print current displacement in file*/

    printf("%08X  ",disp);

    /* print hex codes */

    for (i=0;i<cnt;i++)
    {
	if (i == 7)
	    printf("%02X  ",val[i]);
	else
	    printf("%02X ",val[i]);

	/* keep running count of displacement for characters printed */
	disp++;
    };

    /* check for short last line and print dummy spaces to allow
    ** ASCII text to line up
    */

    if (i < 16)
    {
	for (;i<16;i++)
	{
	    if (i == 7)
		printf("    ");
	    else
		printf("   ");
	};
    };


    /* print valid ASCII equivalents                  */
    /* note: portable ASCII therefore the 32-126 range */

    for (i=0;i<cnt;i++)
    {
	if ((val[i] < 32) || (val[i] > 126))
	    printf(".");
	else
	    printf("%c",val[i]);
    };

    /* print a line feed */

    printf("\n");

}
