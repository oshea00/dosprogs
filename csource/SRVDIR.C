/*
** SRVDIR.C
**
** Mike O'Shea  'C' class project
**
** INPUT
** reads a file that contains the information returned from a
** status command of the remote server session.
**
** OUTPUT
** list the server offering shortnames in wide format
** similar to a directory.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

int main(int,char*[]);
void scan_for_offering(int,char*);
void fill_to_9_spaces(int);


typedef struct {
	char token[41];
	} value;

value   input[40];

int main(int argc,char *argv[])
{
    FILE *in,*out;
    char input_string[81], dos_command[81] = "", password[10];
    char *ptr;
    int i, cnt = 0;
    if ((argc < 2) || (argc > 3))
    {
	printf("\nUsage: srvdir server_name optional_password\n");
	exit(1);
    }
    else
    if (argc == 2)
    {
	strcpy(password,"");
    }
    else
    if (argc == 3)
    {
	strcpy(password,argv[2]);
    }

    if ((out = fopen("SRVDIR.DAT","w")) == NULL)
    {
	printf("\nError creating command file.  Disk may be full.\n");
	exit(1);
    }
    else
    {
	fprintf(out,"status\n");
	fprintf(out,"q\n");
	fclose(out);
    }

    strcat(dos_command,"rserver ");
    strcat(dos_command,argv[1]);
    strcat(dos_command," ");
    strcat(dos_command,password);
    strcat(dos_command," < srvdir.dat > tempfil.txt");
    system(dos_command);
    if ((in = fopen("TEMPFIL.TXT","r")) == NULL )
    {
	printf("\nServer name given was probably incorrect.\n");
	exit(1);
    }
    else
    {
	printf("\n");
	printf("Directory of server <%s> offerings:\n",argv[1]);
	while (fgets(input_string,81,in) != NULL) /* read status file line */
	{
	    /* tokenize status file line */
	    ptr = strtok(input_string," ");
	    while (ptr != NULL)
	    {
		strcpy(input[cnt].token,ptr);
		ptr = strtok(NULL," ");
		cnt++;
	    }

	    /* search line for offering name */
	    if (cnt > 1)
		scan_for_offering(1,input[1].token);

	    /* set token count to zero for next line */
	    cnt = 0;
	}
	fclose(in);
	printf("\n");
	system("del tempfil.txt");
	system("del srvdir.dat");
	return 0;
    }
}

void scan_for_offering(int i,char *token)
{
    static tab = 0;
    if (i == 1)
    {
	if ((strpbrk(token,"\\")) != NULL)
	{
	    if (tab < 4)
	    {
		printf("%s  ",token);
		fill_to_9_spaces(strlen(token));
		tab++;
	    }
	    else
	    {
		printf("%s\n",token);
		tab = 0;
	    }
	}
    }
}

void    fill_to_9_spaces(int i)
{
    for(;i<10;i++)
	printf(" ");
}
