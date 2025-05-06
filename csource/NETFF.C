

/*
** NETFF.C
**
** This program will attempt to locate a given file
** name in the local server group.
**
** INPUT
**
** Will take the filename from the command prompt
** Will get the server group from the system environment
** table.
** All entries in the dos environment table denoting a server
** should begin with SRV.  Example SRV1=SRV01CSQ
**                                 SRV2=SRV02CSQ etc.
**
** OUTPUT
**
** Will list the server and offering under which file is found
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

int main(int,char*[],char*[]);
int connect_ok();
void scan_for_offering(int,char *,char *,char *);
void get_offering_list(char *,char *);
void search_for_file(char *,char *);
void create_command_file();
void check_arg_count(int);

typedef struct {
	char token[41];
	} value;

typedef struct {
	char server[20];
	} srvlist;

typedef struct {
	char offer[10];
	} offrlist;


value   input[40];
srvlist listsrv[30];

int offercnt = 0;
int srvcnt = 0;

int main(int argc,char *argv[],char *env[])
{
	int i;
	char hold_string[128];
	char hold_env[128];
	check_arg_count(argc);
	create_command_file();
	system("use r: /d");


	/* Fill server list */
	printf("Servers denoted in the DOS environment table:\n");

	for (i=0;env[i] != NULL;i++)
	{
		strcpy(hold_env,env[i]);
		strcpy(hold_string,strtok(hold_env,"="));
		if (!strncmp(hold_string,"SRV",3))
		{
			strcpy(listsrv[srvcnt].server,strtok(NULL,"="));
			printf("%s\n",listsrv[srvcnt].server);
			srvcnt++;
		}
	}
	printf("\n");

	/* Loop through server list for file find */

	while (srvcnt--)
	{
		search_for_file(listsrv[srvcnt].server,argv[1]);
	}

	system("del srvdir.dat");
	system("del tempfil.txt");
	system("del connect.txt");
	return 0;


}

void search_for_file(char *server, char *filename)
{
	printf("Searching for <%s> in server <%s>\n",filename,server);
	get_offering_list(server,filename);
}



void get_offering_list(char *server,char *filename)
{
	FILE *in;
    int i, cnt = 0;
	char input_string[81], dos_command[81], *ptr;
	dos_command[0] = NULL;
    strcat(dos_command,"rserver ");
	strcat(dos_command,server);
	strcat(dos_command," < srvdir.dat > tempfil.txt");
	system(dos_command);
    if ((in = fopen("TEMPFIL.TXT","r")) == NULL )
    {
		printf("\nServer name given was probably incorrect.\n");
		exit(1);
    }
    else
    {
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
				scan_for_offering(1,input[1].token,server,filename);

			/* set token count to zero for next line */
			cnt = 0;
		}
	fclose(in);
    }
}

void scan_for_offering(int i,char *token,char *server,char *filename)
{
	char dos_command[81] = "";
    if (i == 1)
		if ((strpbrk(token,"\\")) != NULL)
		{
			printf("Server: %s Offer: %s - being searched.\n",server,token);
			strcat(dos_command,"use r: \\\\");
			strcat(dos_command,server);
			strcat(dos_command,"\\");
			strcat(dos_command,strtok(token,"\\"));
			strcat(dos_command," > connect.txt");
			system(dos_command);
			if (connect_ok())
			{
				dos_command[0] = NULL;
				strcat(dos_command,"ff r:");
				strcat(dos_command,filename);
				system(dos_command);
				system("use r: /d");
				printf("\n");
			}
			else
			{
				printf("Connection refused\n\n");
			}
		}
}

void create_command_file()
{
	FILE *out;
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
}

void check_arg_count(int argc)
{
	if (argc != 2)
    {
	printf("\nUsage: NETFF file_name\nnote: filename may contain wild cards\n");
	exit(1);
	}
}


connect_ok()
{
	FILE *in;
	char input_string[81];
	char token[41];
	if ((in = fopen("CONNECT.TXT","r")) == NULL)
	{
		printf("Error opening temporary result file\n");
		exit(1);
	}
	if (fgets(input_string,81,in) != NULL)
	{
		fclose(in);
		strcpy(token,strtok(input_string," "));
		if (!strcmpi(token,"Connection"))
			return 0;
		else
		if (!strcmpi(token,"Network"))
			return 0;
		else
			return 1;
	}
	else
	{
		fclose(in);
		return 1;
	}
}

