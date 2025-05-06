/*
** MYGREP.C
**
** Michael O'Shea  'C' class level II Monday night
**
** INPUT
** This program receives four command line arguments:
** arg1 is a match pattern
** arg2 is a replace pattern
** arg3 is an input file
** arg4 is an output file
**
** OUTPUT
** The the replace pattern will be applied to the input file an written
** to the output file.
**
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 2048
#define MATCH_SIZE 50
#define FILENAME_SIZE 20

#define ERROR1 "Error 1 - Invalid number of command arguments\n"
#define ERROR2 "Error 2 - opening input file\n"
#define ERROR3 "Error 3 - opening output file\n"
#define ERROR4 "Error 4 - Work buffer overflow - possibly too many matches on one line\n"
#define ERROR5 "Error 5 - Matching pattern too large ( > 50 )\n"

int main(int,char *[]);
void create_match_replace(char *, char *,int *);
void open_files(FILE **,char *,FILE **,char *);
int grep_input(FILE **,FILE **);
void match_and_replace(int);
void match_loop(int *,int *);
int error(char *);

typedef struct
{
	char match[MATCH_SIZE + 1];
	char replace[MATCH_SIZE + 1];
} matrep;

char work_buffer[MAX_BUFFER + 1];
char out_buffer[MAX_BUFFER + 1];
matrep match_table[2];
int match_tabl_cnt;



int main(int argc,char *argv[])
{
	FILE *in,*out;
	char file1[FILENAME_SIZE + 1];
	char file2[FILENAME_SIZE + 1];
	char match[MATCH_SIZE + 1];
	char replace[MATCH_SIZE + 1];
	int i;

	if (argc != 5)
	{
		error(ERROR1);
	}

	strcpy(match,argv[1]);
	strcpy(replace,argv[2]);
	strcpy(file1,argv[3]);
	strcpy(file2,argv[4]);

	if (strlen(match) > MATCH_SIZE)
		error(ERROR5);
	if (strlen(replace) > MATCH_SIZE)
		error(ERROR5);

	create_match_replace(match,replace,&match_tabl_cnt);
	open_files(&in,file1,&out,file2);
	while(grep_input(&in,&out));
	fclose(in);
	fclose(out);
	return 0;
}

/*
** This is a quick and dirty routine for extracting the match and replace
** strings from the command line.  If the given rules: that a
** pattern would be in the first character and only 2 characters
** long, had not been in effect - I would have written a more elegant
** parser.
*/

void create_match_replace(char *match, char *replace,int *match_tabl_cnt)
{
	if (*(match + 0) == '[' && *(match + 3) == ']')
	{
		*match_tabl_cnt = 2;
		match_table[0].match[0] = *(match + 1);
		match_table[0].match[1] = NULL;
		match_table[1].match[0] = *(match + 2);
		match_table[1].match[1] = NULL;
		strcat(match_table[0].match,match + 4);
		strcat(match_table[1].match,match + 4);
		if (*(replace + 0) == '[' && *(replace + 3) == ']')
		{
			match_table[0].replace[0] = *(replace + 1);
			match_table[0].replace[1] = NULL;
			match_table[1].replace[0] = *(replace + 2);
			match_table[1].replace[1] = NULL;
			strcat(match_table[0].replace,replace + 4);
			strcat(match_table[1].replace,replace + 4);
		}
		else
		{
			strcpy(match_table[0].replace,replace);
			strcpy(match_table[1].replace,replace);
		}
	}
	else
	{
		*match_tabl_cnt = 1;
		strcpy(match_table[0].match,match);
		strcpy(match_table[0].replace,replace);
	}
}

void open_files(FILE **in,char *file1,FILE **out,char *file2)
{
	if ((*in = fopen(file1,"r")) == NULL)
	{
		error(ERROR2);
	}
	if ((*out = fopen(file2,"w")) == NULL)
	{
		error(ERROR3);
	}
}

/* Executes by line through input file. */

int grep_input(FILE **in,FILE **out)
{
	int input_length;
	if (fgets(work_buffer,MAX_BUFFER + 1,*in) == NULL)
		return 0;
	input_length = strlen(work_buffer);
	match_and_replace(input_length);
	fputs(out_buffer,*out);
	return 1;
}

/* Executes over input line */

void match_and_replace(int input_length)
{
	int i,curr_pos = 0,out_pos = 0;
	*out_buffer = NULL;
	for (curr_pos=0;curr_pos<input_length;curr_pos++)
	{
		match_loop(&curr_pos,&out_pos);
	}
}

void match_loop(int *curr_pos,int *out_pos)
{
	int compare_length,replace_length,i,matched=0;

	for (i=0;i<match_tabl_cnt;i++)
	{
		compare_length = strlen(match_table[i].match);
		replace_length = strlen(match_table[i].replace);
		if (strncmp(work_buffer+*curr_pos,match_table[i].match,compare_length) == 0)
		{
			/* matched from current input position to compare string */
			if (*out_pos + replace_length > MAX_BUFFER)
				error(ERROR4);
			else
			{
				/* concat replace string to end of out buffer.
				   Increment out buffer position and input
				   buffer position.*/

				strcat(out_buffer,match_table[i].replace);
				*out_pos += replace_length;
				*curr_pos += compare_length - 1;
				matched = 1;
				break;
			}
		}
		else
		{
			/*
			   No match for the current comparison string.
			   No action.
			*/
		}
	}

	/* At this point, all comparison strings have been
	   tried at the current position of the input buffer.
	   If a match occurred, then no further action, otherwise
	   copy the current character of the input buffer to
	   the output buffer and adjust the buffer pointers */

	if (matched)
		; /* no action */
	else
	{
		if (*out_pos + 2 > MAX_BUFFER + 1)
			error(ERROR4);
		else
		{
			*(out_buffer + *out_pos) = *(work_buffer + *curr_pos);
				 /* make sure out buffer is a 'C' string */
			*(out_buffer + *out_pos + 1) = NULL;
			*out_pos+=1;
		}
	}
}


int error(char *error_message)
{
	printf("%s",error_message);
	printf("\nUsage:\n");
	printf("MYGREP match replace file1 file2\n");
	printf("Match and replace characters in file1 sending output\n");
	printf("to file2.  Primitive pattern matching is allowed on first\n");
	printf("character of match. Example: [Hh]ello [Hh]i file1 file2\n");
	printf("Would match and replace Hello with Hi and hello with hi.\n");
	exit(1);
}