/*
** UPDATEWS.C
**
** This program uses an input file called UPDATEWS.DAT to
** know which server offerings to connect to and copy down
** to the indicated PC directory the files in the server offering.
**
** This can be used to automatically update a pc workstation
** with current versions of files.
** The network administrator simply places the updated files on the
** server.
** The UPDATEWS.DAT file contains the PC directory, network offering,
** and network offering subdirectory.
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void);
void update_ws(char *);
int version_changed(char *,char *);

int updated=0;


int main(void)
{
	FILE *in;
	char input_line[80];
	int cnt=0;
	int updated=0;
	printf("UPDATEWS: checking workstation files\n");
	if ((in = fopen("UPDATEWS.DAT","r")) == NULL)
	{
		printf("Error opening UPDATEWS.DAT\n");
		exit(1);
	}

	while (fgets(input_line,80,in) != NULL)
	{
		update_ws(input_line);
		cnt++;
	}

	if (cnt == 0)
	{
		printf("Error - UPDATEWS.DAT file empty\n");
		exit(1);
	}
	printf("UPDATEWS: update completed\n");
	if (updated == 1)
	{
		system("del null");
	}
	fclose(in);
	return(0);
}

void update_ws(char *input_line)
{
	char pcdir[40];
	char srvoffr[40];
	char srvdir[40];
	char dos_command[80];
	char work[80];
	int length;

	strcpy(pcdir,strtok(input_line,";\n"));
	strcpy(srvoffr,strtok(NULL,";\n"));
	strcpy(srvdir,strtok(NULL,";\n"));

	if (pcdir[0] == NULL ||
		srvoffr[0] == NULL ||
		srvdir[0] == NULL)
	{
		printf("Invalid UDPATEWS.DAT file format\n");
		exit(1);
	}

	dos_command[0] = NULL;
	strcat(dos_command,"use j: ");
	strcat(dos_command,srvoffr);
	system(dos_command);
	dos_command[0] = NULL;
	if (version_changed(pcdir,srvdir))
	{
		printf("UPDATEWS: updating %s files\n",pcdir);
		updated = 1;
		strcat(dos_command,"copy j:\\");
		strcat(dos_command,srvdir);
		strcat(dos_command,"\\*.* ");
		strcat(dos_command,pcdir);
		strcat(dos_command," > null");
		system(dos_command);
		dos_command[0] = NULL;
	}
	system("use j: /d");
	printf("UPDATEWS: %s files are current\n",pcdir);
}

int version_changed(char *pcdir,char *srvdir)
{
	FILE *pcin,*srvin;
	int pc,server;
	char work[80];
	work[0] = NULL;
	strcat(work,pcdir);
	strcat(work,"\\update.ver");
	if ((pcin = fopen(work,"r")) == NULL)
	{
		printf("Error opening PC UPDATE.VER\n");
		exit(1);
	}
	work[0] = NULL;
	strcat(work,"j:\\");
	strcat(work,srvdir);
	strcat(work,"\\update.ver");
	if ((srvin = fopen(work,"r")) == NULL)
	{
		printf("Error opening server UPDATE.VER\n");
		exit(1);
	}
	fscanf(pcin,"%d",&pc);
	fscanf(srvin,"%d",&server);
	if (pc == server)
	{
		fclose(pcin);
		fclose(srvin);
		return 0;
	}
	else
	{
		fclose(pcin);
		fclose(srvin);
		return 1;
	}
}




