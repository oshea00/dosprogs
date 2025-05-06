
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main();
void initstack();
void push(int);
void print_stack();
int pop();

struct node
{
	int key;
	struct node *next;
};

struct node *head,*x,*tail;


int main()
{
	char string[80];
	int i;
	printf("Enter Expression:\n");
	gets(string);
	initstack();
	for (i=0;i<strlen(string);i++)
	{
		switch (string[i])
		{
			case '(' : break;
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' : printf("%c",(char)string[i]);break;
			case '+' :
			case '*' : push((int)string[i]);break;
			case ')' : printf("%c",(char)pop());break;
		}
	}
	return 0;
}

void initstack()
{
	head = (struct node *) malloc(sizeof(*head));
	tail = (struct node *) malloc(sizeof(*tail));
	head->next = tail;
	tail->next = tail;
}

void push(int val)
{
	x = (struct node *) malloc(sizeof(*x));
	x->key = val;
	x->next = head->next;
	head->next = x;
}

int pop()
{
	int val;
	x=head->next;
	val=x->key;
	head->next=x->next;
	free(x);
	return val;
}

int stackempty()
{
	return x==head->next;
}

void print_stack()
{
	x = head->next;
	while (x!=(x->next))
	{
		printf("%d\n",x->key);
		x=x->next;
	}
}



