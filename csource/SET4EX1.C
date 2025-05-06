#include <stdio.h>
#include <stdlib.h>

int main(void);
void get_info(void);
void print_info(void);

char fullname[21];
char street[21];
char citystatezip[26];

int main(void)
{
	get_info();
	print_info();
	exit(0);
}

void get_info(void)
{
	printf("Enter the following information:\n");
	printf("Full name: ");
	gets(fullname);
	printf("Street: ");
	gets(street);
	printf("City, state and zip code: ");
	gets(citystatezip);
}

void print_info(void)
{
	printf("\n\n\n");
	puts(fullname);
	puts(street);
	puts(citystatezip);
}
