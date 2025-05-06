/*
** DECODE.C
**
** This program decodes IEW userid password file USERIDS.SYS
**
**
** to encode a userid and/or password:
** 1.  +8|7|6|5|4|3|2|1
**      p a u l
**      x h a q
** 2. reversed= QAHX
**
** To decode reverse the process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void);
void decode_record(void);

typedef struct {
		char fill[6];
		char userid[8];
		char password[8];
		char fill2[600];
		} record_template;

record_template in_record;

int main(void)
{
	FILE *in;
	if ((in = fopen("userids.sys","rb")) == NULL)
	{
		printf("Error opening file\n");
		printf("Useage: decode <filename>\n");
		exit(1);
	}

	fread(&in_record,512,1,in);
	while (!feof(in))
	{
		if (isalnum(in_record.userid[0])||isspace(in_record.userid[0]))
		{
			decode_record();
		}
		fread(&in_record,512,1,in);
	}
	fclose(in);
}

void decode_record(void)
{
	int i,j;
	char work[9];
	char user[9];
	char pass[9];
	j = 0;
	work[8] = NULL;
	for (i=7;i>=0;i--)
	{
		if (in_record.userid[i] != ' ')
		{
			work[j] = in_record.userid[i] - (i+1);
			if (work[j] < 0x41)
				work[j]+=0x1a;
		}
		else
		{
			work[j] = ' ';
		}
		j++;
	}
	strcpy(user,work);
	j = 0;
	work[8] = NULL;
	for (i=7;i>=0;i--)
	{
		if (in_record.password[i] != ' ')
		{
			work[j] = in_record.password[i] - (i+1);
			if (work[j] < 0x41)
				work[j]+=0x1a;
		}
		else
		{
			work[j] = ' ';
		}
		j++;
	}
	strcpy(pass,work);
	printf("%s = %s\n",user,pass);
}

