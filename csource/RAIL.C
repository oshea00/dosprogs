/*
**
** RAIL.C  does the rail fence cipher
**
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


void main(void);
void get_text(char *,int *);
void rail_encode(char [],int,char [],int *);
void rail_decode(char [],int, char [],int *);
void print_text(char *,int);

#define MAXCNT 500
char in_text[MAXCNT + 1];
char out_text[MAXCNT + 1];

void main(void)
{
	char s;
	int incnt,outcnt;
	printf("Rail fence cipher program.\n");
	printf("\nD .... Decode\n");
	printf("E .... Encode\n");
	printf("\nType letter of choice:\n");
	s = getch();
	switch (s)
	{
		case 'd'  :
		case 'D'  : get_text(in_text,&incnt);
					rail_decode(in_text,incnt,out_text,&outcnt);
					print_text(out_text,outcnt);
					break;
		case 'e'  :
		case 'E'  : get_text(in_text,&incnt);
					rail_encode(in_text,incnt,out_text,&outcnt);
					print_text(out_text,outcnt);
					break;
		default   : break;
	}
}

void get_text(char *in_text,int *incnt)
{
	int i=1;
	char c;
	printf("Type in text, end with '/' and return:\n");
	c = getchar();
	while (c != '/')
	{
		if (isalnum(c))
		{
			*(in_text + i) = c;
			i++;
		}
		c = getchar();
	}
	*incnt = i-1;
}

void rail_encode(char in_text[],int incnt,char out_text[],int *outcnt)
{
	int l=0,d,h,i,r,j;
	printf("encode:\n");
	printf("Input number of rails, 3 to 20: ");
	scanf("%d",&h);
	printf("\n");
	d=2*h-2;
	j = incnt;
	/* Do top rail */
	for (i=1;i<j+1;i+=d)
	{
		l++;
		out_text[l]=in_text[i];
	}
	/* Do rails 2 thru h-1 */
	for (r=1;r<(h-2)+1;r++)
	{
		l++;
		out_text[l]=in_text[r+1];
		for (i=d+1;i<j+1;i+=d)
		{
			l++;
			out_text[l]=in_text[i-r];
			if (i+r>j)
				continue;
			l++;
			out_text[l]=in_text[i+r];
		}
	}
	/* Do last rail */
	for (i=h;i<j+1;i+=d)
	{
		l++;
		out_text[l]=in_text[i];
	}
	*outcnt = i;
}

void rail_decode(char in_text[],int incnt,char out_text[],int *outcnt)
{
	int l=0,d,h,i,r,j;
	printf("decode:\n");
	printf("Input number of rails, 3 to 20: ");
	scanf("%d",&h);
	printf("\n");
	d=2*h-2;
	j = incnt;
	/* Do top rail */
	for (i=1;i<j+1;i+=d)
	{
		l++;
		out_text[i]=in_text[l];
	}
	/* Do rails 2 thru h-1 */
	for (r=1;r<(h-2)+1;r++)
	{
		l++;
		out_text[r+1]=in_text[l];
		for (i=d+1;i<j+1;i+=d)
		{
			l++;
			out_text[i-r]=in_text[l];
			if (i+r>j)
				continue;
			l++;
			out_text[i+r]=in_text[l];
		}
	}
	/* Do last rail */
	for (i=h;i<j+1;i+=d)
	{
		l++;
		out_text[i]=in_text[l];
	}
	*outcnt = i;
}

void print_text(char *out_text,int outcnt)
{
	int i,j;
	printf("printed text:\n");
	j = 0;
	for (i=1;i<outcnt+1;i++)
	{
		if ( j < 5 )
		{
			printf("%c",*(out_text + i));
			j++;
		}
		else
		{
			printf(" %c",*(out_text + i));
			j = 1;
		}
	}
}
