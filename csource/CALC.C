/*
** CALC.C
**
** Mike O'Shea  Monday night 'C' class level II
** 393-6495 m/s 9C-51
**
** Exercise #4
**
** INPUT:
** Prompts for an operator and two doubles, value1 and value2
**
** OUTPUT:
** value1 operator value2 = result
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR1 "Error 1: Three values were expected."
#define ERROR2 "Error 2: Invalid operator."
#define ERROR3 "Error 3: Second value must be non-zero if operator is /."

#define ADD      '+'
#define SUBTRACT '-'
#define MULTIPLY '*'
#define DIVIDE   '/'

#define MAX_ARGUMENTS 3
#define MAX_NUM_STRING 80 /* Allows for large number strings */
#define MAX_LINE_BUFFER 256    /* Prevents overflow */

int main(void);
void get_valid_input(char *, double *, double *);
void validate_input(char *, double *);
void error(char *);
double add(double, double);
double subtract(double, double);
double multiply(double, double);
double divide(double, double);
double calculate(double (*)(double,double), double, double);

/* double (*)(double,double) : in the previous prototype refers
   to a pointer (*) to a function (double,double) that returns
   a double value
*/

int main(void)
{
    char operator;
    double value1, value2, result;

    get_valid_input(&operator, &value1, &value2);

    switch (operator)
    {
	case ADD      : result = calculate(add,value1,value2); break;
	case SUBTRACT : result = calculate(subtract,value1,value2); break;
	case MULTIPLY : result = calculate(multiply,value1,value2); break;
	case DIVIDE   : result = calculate(divide,value1,value2); break;
	default       : break;
    };

    printf("\n %-8.3f %c %-8.3f = %-8.3f\n",value1,operator,value2,result);
    return(0);
}

void get_valid_input(char *operator, double *value1, double *value2)
{
    typedef struct {
		    char value[MAX_NUM_STRING];
		   } argument;

    argument  arg[MAX_ARGUMENTS];

    char input_string[MAX_LINE_BUFFER],
	 *ptr;

    int argc = 0;

    printf("CALC: ");
    gets(input_string);

    /* Tokenize input string into argument value list
       and count number of arguments. */

    ptr = strtok(input_string," ");
    while (ptr != NULL)
    {
	if (argc < MAX_ARGUMENTS) /* Keeps from exceeding list size */
	    strcpy(arg[argc].value,ptr);
	ptr = strtok(NULL," ");
	argc++;
    };

    if (argc != MAX_ARGUMENTS)
	error(ERROR1);  /* invalid number of arguments */
    else
    {
	/* Convert string arguments to input values */
	*operator = *arg[0].value;
	*value1 = strtod(arg[1].value,NULL);
	*value2 = strtod(arg[2].value,NULL);
	validate_input(operator,value2);
    };
}

void validate_input(char *operator, double *value2)
{
    switch (*operator)
    {
	case ADD      :
	case SUBTRACT :
	case MULTIPLY :
	case DIVIDE   : break;
	default       : error(ERROR2); /* Invalid operator */
    };

    if ( (*operator == DIVIDE) && (*value2 == 0) )
	error(ERROR3);  /* Prevent divide by zero */
}

double calculate(double (*func_ptr)(double,double), double value1, double value2)
{
    return ((*func_ptr)(value1,value2));
}

double add(double value1, double value2)
{
    return (value1 + value2);
}

double subtract(double value1, double value2)
{
    return (value1 - value2);
}

double multiply(double value1, double value2)
{
    return (value1 * value2);
}

double divide(double value1, double value2)
{
    return (value1 / value2);
}

void error(char *error_message)
{
    printf("%s\n\n",error_message);
    printf("At 'CALC:' prompt enter: ");
    printf("valid_operator  numeric_value1  numeric_value2\n");
    printf("where valid operator is: + - / *.\n");
    exit(1);
}

