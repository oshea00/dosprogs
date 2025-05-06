/*
** ENV.C
**
** Michael O'Shea
** 393-6495 Boeing Computer Services
**
** INPUT
**
** Optional variable name
**
** OUTPUT
**
** If variable is in environment - return the string value of it
** If variable is not in environment - add it to table listing
** if no variable - existing table listing
** If existing variable followed by = and no string - delete variable
**
** MODIFICATIONS
**
** This basic routine is modified to call a child process, CHILD, which
** will alter a string in the parents environment.
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <process.h>

#define MAX_LENGTH 128
#define MAX_SIZE 20


int main(int,char *[],char *[]);
void list_env(void);
void check_variable(char*);
int existing_variable(char*,int *);
void overwrite_existing(int,char*);
void add_variable(char*);
void parse_input(char *,char *,int *,char *);
void delete_variable(int);
void pspaddr(void);

union REGS inregs,outregs;

typedef struct {
	char variable[MAX_LENGTH + 1];
	} envlist;

envlist listenv[MAX_SIZE];
int envcnt = 0;

unsigned int far *parentenvptr;
unsigned int hold;


int main(int argc,char *argv[],char *env[])
{
	int i;
	char hold_env[MAX_LENGTH + 1];


	/* Fill environment list */

	for (i=0;env[i] != NULL;i++)
	{
		if (envcnt>MAX_SIZE)
		{
			printf("DOS environment table > 20 entries");
			exit(1);
		}
		strcpy(listenv[i].variable,env[i]);
		envcnt++;
	}

	if (envcnt>MAX_SIZE)
	{
		printf("DOS environment table > 20 entries");
		exit(1);
	}

	/* based on argument count passed to main() by DOS and the user.
	   See usage: text below.*/

	switch (argc)
	{
		case 1    : pspaddr();
					printf("\nBefore: ");
					list_env();
					spawnl(P_WAIT,"child",NULL);
					printf("After: ");
					list_env();break;
		case 2    : check_variable(argv[1]); break;
		default   : printf("Usage: ENV optional_label\n");
					printf("This program allows you to view environment\n");
					printf("variables passed to it by the DOS loader.\n");
					exit(1);
	}

	return 0;
}


void list_env(void)
{
	int i;
	printf("Variables in the DOS environment table:\n\n");
	for (i=0;i<envcnt;i++)
	{
		printf("%s\n",listenv[i].variable);
	}
	printf("\n");
}

void check_variable(char *argv)
{
	int place, i;
	char string[MAX_LENGTH + 1],
		 variable[MAX_LENGTH + 1],
		 *ptr;
	int  equal_sign;

	parse_input(argv,variable,&equal_sign,string);


	if (equal_sign)
	{
		/* set or overwrite a variable */

		if (existing_variable(variable,&place))
			if (*string != NULL)
			{
				overwrite_existing(place,argv);
				list_env();
			}
			else
			{
				delete_variable(place);
				list_env();
			}
		else
		{
			add_variable(argv);
			list_env();
		}
	}
	else
	{
		/* report string associated with variable */

		if (existing_variable(variable,&place))
		{
			parse_input(listenv[place].variable,variable,&equal_sign,string);
			printf("%s\n",string);
		}
		else
		{
			printf("Variable not in environment\n");
		}
	}

}

int existing_variable(char *variable,int *place)
{
	int i;
	char work_string[MAX_LENGTH];

	for (i=0;i<envcnt;i++)
	{
		strcpy(work_string,listenv[i].variable);
		if (!strcmp(strtok(work_string,"="),variable))
		{
			*place = i;
			return 1;
		}
	}
	return 0;
}


void overwrite_existing(int place, char *argv)
{
	int i;
	for (i=place;i<envcnt;i++)
	{
		strcpy(listenv[i].variable,listenv[i+1].variable);
	}
	strcpy(listenv[envcnt-1].variable,argv);
}


void add_variable(char *argv)
{
	if (envcnt+1 > MAX_SIZE)
	{
		printf("Add variable will exceed %d line table size",MAX_SIZE);
		exit(1);
	}

	strcpy(listenv[envcnt].variable,argv);
	envcnt++;
}

void delete_variable(int place)
{
	int i;
	for (i=place;i<envcnt;i++)
	{
		strcpy(listenv[i].variable,listenv[i+1].variable);
	}
	envcnt--;
}

void parse_input(char *instring,char *variable,int *equal_sign,char *string)
{
	int i;
	char *ptr;
	char hold_instring[MAX_LENGTH + 1];

	strcpy(hold_instring,instring);

	/* check for '=' sign */

	if (strchr(instring,'='))
	{
		*equal_sign = 1;

		/* set variable in string to uppercase */

		for (i=0; '=' != (*(instring+i)=toupper(*(instring+i)));i++);

		/* parse instring */

		strcpy(hold_instring,instring);
		ptr = strtok(hold_instring,"=");
		strcpy(variable,ptr);
		ptr = strtok(NULL,"=");
		if (ptr == NULL)
			*string = NULL;
		else
			strcpy(string,ptr);

	}
	else
	{
		/* set variable to uppercase */

		for (i=0;*(instring+i)=toupper(*(instring+i));i++);

		/* set output values */

		strcpy(variable,instring);
		*equal_sign = NULL;
		*string = NULL;
	}

}

void pspaddr(void)
{
	inregs.h.ah = 0x51;
	intdos(&inregs,&outregs);
	printf("PARENT PSP: %p  ",outregs.x.bx);

	/* convert BX value to XXXX:0000 form of far pointer for parent PSP*/
	hold=outregs.h.bh*0x100+outregs.h.bl;

	/* create far pointer to parent environment */
	parentenvptr = (unsigned int far *) MK_FP(hold,0x2c);
	hold = *parentenvptr;
	printf("Parent environment = %X:0000\n",hold);
}

