/*  Assignment #1 Monday 'C' class level II
**  Mike O'Shea 393-6495 m/s 9C-51 G-3933
**
**  ASS1.C (No reflection on the programmer hopefully)
**
**  INPUT:
**  Takes a list of file names from the command prompt
**  OUTPUT:
**  Each file printed in order given with a count of each brace type:
**  {, }, (, )
**  and indicate if counts are equal
**
*/

#include <stdio.h>
#include <stdlib.h>

#define MAXNAME 30

int main(int, char *[]);
void report(char *);

int main(int argcnt, char *argval[])
{
    int i;
    char filename[MAXNAME + 1];

    if (argcnt == 1)
    {
	printf("Usage is:\n ASS1 file.name file2.name .....");
	printf("\n\nProgram will type given file list and provide counts");
	printf("\nof the brackets within the file text.\n");
	return 0;
    }
    else
	for (i=1;i<argcnt;i++)
	    report(argval[i]);

    return 0;
}

void report(char *filename)
{
    FILE *in;
    int leftcurl, rightcurl, leftparen, rightparen;
    char ch;

    leftcurl = rightcurl = leftparen = rightparen = 0;

    if ((in = fopen(filename,"r")) == NULL)
	printf("\nError trying to open: %s\n",filename);
    else
    {
	printf("\n File: %s\n\n",filename);
	while ((ch = getc(in)) != EOF)
	{
	    if (ch == '{')              /* count looked for braces */
		leftcurl++;
	    else
	    if (ch == '}')
		rightcurl++;
	    else
	    if (ch == '(')
		leftparen++;
	    else
	    if (ch == ')')
		rightparen++;

	    putchar(ch);               /* writes char to standard output */
	};

    fclose(in);
			      /* report results */

    printf("\nThere are an %s number of curly brackets.\n",
			   (leftcurl == rightcurl) ? "equal":"unequal");
    printf("Left curly = %d   Right curly = %d\n\n",leftcurl,rightcurl);
    printf("There are an %s number of parentheses.\n",
			   (leftparen == rightparen) ? "equal":"unequal");
    printf("Left paren = %d  Right paren = %d\n\n",leftparen,rightparen);
    };
}
