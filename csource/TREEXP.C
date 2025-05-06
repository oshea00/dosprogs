
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <graphics.h>
#include <ctype.h>

int main();
void initstack();
void initstack2();
void push(struct node *);
void push2(int);
void print_stack();
struct node *pop();
int pop2();
void depth_first(struct node *);
void visit(struct node *);
void traverse(struct node *);
void draw_tree();
void draw_seg();
void convert_exp();

struct node
{
	int info;
	int x;
	int y;
	struct node *l,*r;
};

struct stack_rec
{
	struct node *ptr;
	struct stack_rec *next;
};

struct stack2_rec
{
	int idx;
	struct stack2_rec *next;
};

struct stack_rec *head,*top,*tail;

struct stack2_rec *head2,*top2,*tail2;

struct node *new,*null;

char input[80];
char stringexp[80];


typedef struct {
	int x,y;
	char info;
} coord_type;

coord_type coords[100];
int coordsmax = 0;

int x=0,y=0;

int main()
{
	char c;
	int strmax,i;
	struct node *ptr;

	/* Initialize stacks */
	initstack();
	initstack2();

	/* Initialize null tree node to point to itself */
	null = (struct node *) malloc(sizeof(*null));
	null->l = null;
	null->r = null;

	/* Get input expression */
	convert_exp();
	strmax = strlen(input);

	/* Parse input into tree nodes */

	for (i=0;i<strmax;i++)
	{
		c=input[i];
		new = (struct node *) malloc(sizeof(*new));
		new->info=c;
		new->l=null;
		new->r=null;

		/*  If an operator then pop two operands off stack
			and point to them. */

		switch (c)
		{
			case '/'  :
			case '-'  :
			case '+'  :
			case '*'  :   new->l=pop();new->r=pop();break;
		}

		/*  push new tree node onto stack */

		push(new);
	}

	print_stack();

	/* save the top pointer for other routines.
	   The depth first routine affects the node stack */

	ptr = pop();

	/* Does an inorder traverse of the expression tree and
	   adds the relative coordinates to the node records */

	traverse(ptr);

	/*  Does a depth first traversal and reads out the node
		coordinates and character into an array */

	depth_first(ptr);

	/*  Processes the array produced by traverse to draw the tree
		structure.  Note:  traverse() assigned the coordinate values,
		and the depth first retrieval of the coordinates into
		the coords[] array allows this stack based algorithm to work. */

	draw_tree();

	return 0;
}

void initstack()
{
	head = (struct stack_rec *) malloc(sizeof(*head));
	tail = (struct stack_rec *) malloc(sizeof(*tail));
	head->next = tail;
	tail->next = tail;
}

void initstack2()
{
	head2 = (struct stack2_rec *) malloc(sizeof(*head2));
	tail2 = (struct stack2_rec *) malloc(sizeof(*tail2));
	head2->next = tail2;
	tail2->next = tail2;
}

void push(struct node *ptr)
{
	top = (struct stack_rec *) malloc(sizeof(*top));
	top->next = head->next;
	top->ptr = ptr;
	head->next = top;
}

void push2(int val)
{
	top2 = (struct stack2_rec *) malloc(sizeof(*top2));
	top2->next = head2->next;
	top2->idx = val;
	head2->next = top2;
}

struct node *pop()
{
	struct node *ptr;
	top=head->next;
	ptr=top->ptr;
	head->next=top->next;
	free(top);
	return ptr;
}

int pop2()
{
	int val;
	top2=head2->next;
	val=top2->idx;
	head2->next=top2->next;
	free(top2);
	return val;
}

int stackempty()
{
	return top->next==top;
}

void depth_first(struct node *ptr)
{
	int i=0;
	push(ptr);
	while (!(head->next==tail))
	{
		ptr = pop();
		coords[i].y=ptr->x;
		coords[i].x=ptr->y;
		coords[i].info=ptr->info;
		i++;

		printf("%c x=%d y=%d\n",ptr->info,ptr->x,ptr->y);

		if (ptr->r!=null)
			push(ptr->r);
		if (ptr->l!=null)
			push(ptr->l);
	}
	coordsmax = i;
	printf("count=%d\n",coordsmax);
}


void print_stack()
{
	top = head->next;
	while (!stackempty())
	{
		printf("%p\n",top->ptr);
		top=top->next;
	}
}

void visit(struct node *ptr)
{
	ptr->x=++x;
	ptr->y=y;
}

void traverse(struct node *ptr)
{
	y++;
	if (ptr!=null)
	{
		traverse(ptr->r);
		visit(ptr);
		traverse(ptr->l);
	}
	y--;
}

void draw_tree()
{
	int test,g_driver,g_mode,g_error,i=0;
	char string[2];
	detectgraph(&g_driver,&g_mode);
	g_mode = 1; /* Override to EGA */
	if (registerbgidriver(EGAVGA_driver)<0) exit(1);
	if (registerbgifont(triplex_font)<0) exit(1);
	initgraph(&g_driver,&g_mode,"");
	settextstyle(0,0,1);
	string[0] = coords[0].info;
	string[1] = NULL;
	setcolor(WHITE);
	outtextxy(coords[0].y*20,coords[0].x*20,string);
	push2(0);
	for (i=1;i<coordsmax;i++)
	{
		string[0] = coords[i].info;
		string[1] = NULL;
		setcolor(WHITE);
		outtextxy(coords[i].y*20,coords[i].x*20,string);
		setcolor(YELLOW);
		while (coords[i].x <= coords[head2->next->idx].x)
		{  pop2(); }
		line(coords[head2->next->idx].y*20,coords[head2->next->idx].x*20,
			 coords[i].y*20,coords[i].x*20);
		push2(i);
	}
	setcolor(LIGHTCYAN);
	outtextxy(1,201,"Original expression:");
	outtextxy(1,210,stringexp);
	outtextxy(1,230,"Postfix or \"Reverse Polish\" form:");
	outtextxy(1,239,input);
	getch();
	restorecrtmode();
}

void convert_exp()
{
	int i,j=0;
	printf("Enter a fully parenthisized math expression.\n");
	printf("Example: ((A+B)*3)\n: ");
	gets(stringexp);
	initstack2();
	for (i=0;i<strlen(stringexp);i++)
	{
		if (isalnum(stringexp[i]))
			input[j++]=stringexp[i];
		else
		if (stringexp[i]=='(')
			;
		else
		if (stringexp[i]=='*'||
			stringexp[i]=='+'||
			stringexp[i]=='-'||
			stringexp[i]=='/')
			push2((int)stringexp[i]);
		else
		if (stringexp[i]==')')
			input[j++]=(char)pop2();
	}
	input[j]=NULL;
}




