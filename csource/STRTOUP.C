
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(void);
void strtoup(char*);


int main(void)
{
	char in_string[51];

	gets(in_string);
	printf("%s\n",in_string);
	strtoup(in_string);
	printf("%s\n",in_string);
}



void strtoup(char *in_string)
{
	int i;
	for (i=0;*(in_string + i) = toupper(*(in_string + i));i++);
}
