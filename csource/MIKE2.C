/*  set 2 excercises
**  Mike O'Shea Monday class
*/

#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <ctype.h>


int main(void);
void print_address(void);
int passing_grade(int);
float ave_five_ints(int, int, int, int, int);
unsigned long square_it(int);
void ex5(void);
void ex6(void);


int main()
{

	int grade;
	int passing_grade();
	float ave_five_ints();
	unsigned long square_it();
	printf("Exercise set 2\n");

/* exercise 1 */
	print_address();

/* exercise 2 */
	printf("Please type a grade value 0 - 100: ");
	scanf("%d",&grade);
	printf("Grade %d is a %s grade\n",grade,(passing_grade(grade)) ?
		  ("passing") : ("failing"));

/* exercise 3 */
	printf("average of 23, 45, 56, 23, 12 is: %g\n",
		  ave_five_ints(23,45,56,23,12));

/* exercise 4 */
	printf("square of 5 = %lu\n",square_it(5));

/* execrise 5 */
	ex5();

/* exercise 6 */
	ex6();
	return(0);
}

void print_address(void)
{
	printf("Michael R. O'Shea\n");
	printf("4810 O'Malley Rd.\n");
	printf("Anchorage, Alaska  99517\n\n");
}

int passing_grade(int grade)
{
	return (grade >= 70) ? (TRUE) : (FALSE);
}

float ave_five_ints(int i1,int i2,int i3,int i4,int i5)
{
	return ((float) i1 + i2 + i3 + i4 + i5) / 5;
}

unsigned long square_it(int numin)
{
	return (unsigned long) numin * numin;
}

void ex5(void)
{
	int ch;
	int counter = 0;
	printf("Please type characters followed by a ctrl-Z\n");
	while ((ch = getchar()) != EOF)
		if (!isspace(ch))
			++counter;
	printf("Total non-blank characters = %d\n",counter);
}

void ex6(void)
{
	int ch;
	long int lcase, ucase, digits, spaces, tabs, newlines,
	others, total;
	lcase = ucase = digits = spaces = tabs = newlines = others
	= total = 0L;
	printf("\nPlease type characters followed by a ctrl-Z\n");
	while ((ch = getchar()) != EOF)
		{
			++total;
			if (islower(ch))
				++lcase;
			else
				if (isupper(ch))
					++ucase;
				else
					if (isdigit(ch))
						++digits;
					else
						if (ch == ' ')
							++spaces;
						else
							if (ch == '\t')
								++tabs;
							else
								if (ch == '\n')
									++newlines;
								else
									++others;
		}
		printf("Lower case = %ld\nUpper case = %ld\nDigits = %ld\n",lcase,ucase,digits);
		printf("Spaces = %ld\nTabs = %ld\nNewlines = %ld\nOthers = %ld\n",spaces,tabs,newlines,others);
		printf("Total = %ld\n",total);
}


