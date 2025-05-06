/*
**  STUDARRY.C
**
**  Data structures in "C" course.  Thursday night second assignment.
**
**  Michael O'Shea 393-6495
**
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <string.h>

#define STUDENT_NAME 1
#define STUDENT_ID 2
#define ADD 1
#define DELETE 2
#define UPDATE 3
#define FIND 4
#define LIST 5
#define QUIT 6

struct student {
	int student_id;
	char lastname[21];
	char firstname[16];
	char major[5];
	char phone[9];
	int age;
	int next_name;
	int next_id;
};

struct student students[100];

int first_name=0;
int first_id=0;
int prev;
int curr;

/* Memory management variables */

int freepointers[100];
int freepointers_cnt = 0;
int nextpointer = 1;

void main(void);
void add_student(void);
void update_student(void);
void delete_student(void);
void find_student(void);
int find_by_name(int,char []);
int find_by_id(int,int);
void list_students(void);
void add_to_list(int);
int get_menu_choice(void);
int get_choice(void);
void display_menu(void);
void display_record(int);
void print_list(int);
int enter_record(void);
int keytype(void);
int find_record(int);
void fill_record(int);
int mymalloc(void);
void myfree(int);

void main()
{
	int valid_choice;
	int continued = 1;

	/* get command */
	while (continued)
	{
		valid_choice = get_menu_choice();
		switch (valid_choice)
		{
			case ADD : add_student();break;
			case DELETE : delete_student();break; /* to do */
			case UPDATE : update_student();break;
			case FIND   : find_student();break;
			case LIST   : list_students();break;
			case QUIT   : continued=0;break;
			default     : break;
		}
	}
}

int get_menu_choice(void)
{
	/* display menu */

	printf("\n1  =  Add student\n");
	printf("2  =  Delete student\n");
	printf("3  =  Update student\n");
	printf("4  =  Find student\n");
	printf("5  =  List students\n");
	printf("6  =  Quit program\n");
	printf("\nType number of choice: ");

	return get_choice();
}

int get_choice(void)
{
	int choice;
	scanf("%d",&choice);
	return choice;
}

void display_record(int curr)
{
	printf("\n***  ***  ***  ***  ***  ***");
	printf("\nID       : %d",students[curr].student_id);
	printf("\nLASTNAME : %s",students[curr].lastname);
	printf("\nFIRSTNAME: %s",students[curr].firstname);
	printf("\nMAJOR    : %s",students[curr].major);
	printf("\nAGE      : %d",students[curr].age);
	printf("\nPHONE    : %s",students[curr].phone);
	printf("\n");
}

int enter_record(void)
{
	int new;
	static int id_number=0;
	id_number++;
	fflush(stdin);

	/* Allocate new record space */

	new = mymalloc();
	students[new].next_name = NULL;
	students[new].next_id = NULL;

	if (first_id==NULL)
	{
		first_id=new;
		first_name=new;
	}
	students[new].student_id = id_number;
	printf("\n*** Enter New Record ***");
	fill_record(new);
	return new;
}

void fill_record(int new)
{
	char input[81];

	fflush(stdin);

	/* Fill Fields */
	printf("\nLast name: ");
	gets(students[new].lastname);
	students[new].lastname[20]=NULL;
	printf("First name: ");
	gets(students[new].firstname);
	students[new].firstname[15]=NULL;
	printf("Major: ");
	gets(students[new].major);
	students[new].major[4]=NULL;
	printf("Age: ");
	gets(input);
	students[new].age = atoi(input);
	printf("Phone: ");
	gets(students[new].phone);
	students[new].phone[8]=NULL;
}

void add_to_list(int new)
{
	/* Insert new record by name */

	curr = first_name;
	prev = curr;
	if (new!=curr)  /* check for intialized first node */
	{
		while (strcmpi(students[new].lastname,students[curr].lastname)>0&&students[curr].next_name!=NULL)
		{
			/* go to next node */
			prev = curr;
			curr = students[curr].next_name;
		}
		if (curr==first_name)  /* first node stopped on */
		{
			if (strcmpi(students[curr].lastname,students[new].lastname)>0)
			{
				students[new].next_name=curr;
				first_name=new;
			}
			else
			{
				students[new].next_name=students[curr].next_name;
				students[curr].next_name=new;
			}
		}
		else
		{
			if (students[curr].next_name!=NULL)  /* middle node stopped on */
			{
				students[new].next_name=curr;
				students[prev].next_name=new;
			}
			else   /* end node stopped on */
			{
				if (strcmpi(students[new].lastname,students[curr].lastname)>0)
					students[curr].next_name=new;
				else
				{
					students[prev].next_name=new;
					students[new].next_name=curr;
				}
			}
		}
	}

	/* Insert new record by student id */

	curr = first_id;
	prev = curr;
	if (new!=curr)  /* check for initialized first node */
	{
		while (students[new].student_id>students[curr].student_id&&students[curr].next_id!=NULL)
		{
			prev = curr;
			curr = students[curr].next_id;
		}
		if (curr==first_id)   /* stopped on first node */
		{
			if (students[curr].student_id>students[new].student_id)
			{
				students[new].next_id=curr;
				first_id=new;
			}
			else
			{
				students[new].next_id=students[curr].next_id;
				students[curr].next_id=new;
			}
		}
		else
		{
			if (students[curr].next_id!=NULL)  /* middle node stopped on */
			{
				students[new].next_id=curr;
				students[prev].next_id=new;
			}
			else    /* end node stopped on */
			{
				if (students[new].student_id>students[curr].student_id)
					students[curr].next_id=new;
				else
				{
					students[prev].next_id=new;
					students[new].next_id=curr;
				}
			}
		}
	}
}

void list_students(void)
{
	int keychoice;
	printf("\n List Students... \n");
	print_list(keytype());
}

void find_student(void)
{
	int found;
	printf("\n Find Student... \n");
	if ((found=find_record(keytype()))==NULL)
	{}  /* no action */
	else
		display_record(found);
}
void update_student(void)
{
	int found;
	char input[80];
	printf("\n Update Student... \n");
	if ((found=find_record(keytype()))==NULL)
	{}  /* no action */
	else
	{
		display_record(found);
		strcpy(input,students[found].lastname);
		printf("Re-enter information:\n");
		fill_record(found);
		if (strcmpi(input,students[found].lastname))
		{
			printf("You must delete and re-enter the record if you\n");
			printf("want to alter the last name.  The record will be\n");
			printf("saved with the other updates intact.\n");
			strcpy(students[found].lastname,input);
		}
	}
}

void delete_student(void)
{
	int foundname;
	int foundid;
	int prevname;
	int previd;
	int keychoice;
	char input[80];
	int find_id;
	char find_name[80];
	char choice;
	printf("\n Delete Student... \n");
	fflush(stdin);
	keychoice=keytype();
	if (keychoice==STUDENT_ID)
	{
		printf("Student id: ");
		scanf("%d",&find_id);
		foundid=first_id;
		if ((foundid=find_by_id(foundid,find_id))!=NULL)
		{
			previd=prev;
			strcpy(find_name,students[foundid].lastname);
			foundname=first_name;
			foundname=find_by_name(foundname,find_name);
			prevname=prev;
			display_record(foundname);
			printf("\nDo you want to delete this (y/n)?\n");
			choice=getche();
			if (toupper(choice)=='Y')
			{
				if (first_name==foundname)
				{
					first_name=students[foundname].next_name;
				}
				else
				{
					students[prevname].next_name=students[foundname].next_name;
				}

				if (first_id==foundid)
				{
					first_id=students[foundid].next_id;
				}
				else
				{
					students[previd].next_id=students[foundid].next_id;
				}
				myfree(foundid);
				printf("\nRecord deleted..\n");
			}
			else
				printf("\nDelete aborted...\n");
		}
		else
			printf("\nDelete aborted...\n");
	}
	else
	{
		fflush(stdin);
		printf("Student last name: ");
		gets(find_name);
		if (find_name[0]==NULL)
			strcpy(find_name,"   ");
		foundname=first_name;
		if ((foundname=find_by_name(foundname,find_name))!=NULL)
		{
			prevname=prev;
			find_id=students[foundname].student_id;
			foundid=first_id;
			foundid=find_by_id(foundid,find_id);
			previd=prev;
			display_record(foundid);
			printf("\nDo you want to delete this (y/n)?\n");
			choice=getche();
			if (toupper(choice)=='Y')
			{
				if (first_name==foundname)
				{
					first_name=students[foundname].next_name;
				}
				else
				{
					students[prevname].next_name=students[foundname].next_name;
				}

				if (first_id==foundid)
				{
					first_id=students[foundid].next_id;
				}
				else
				{
					students[previd].next_id=students[foundid].next_id;
				}
				myfree(foundid);
				printf("\nRecord deleted..\n");
			}
			else
				printf("\nDelete aborted...\n");
		}
		else
			printf("\nDelete aborted...\n");
	}
}
void add_student(void)
{
	int hold;
	printf("\n Add Student... \n");
	add_to_list(hold=enter_record());
	display_record(hold);
}

int keytype(void)
{
	int key=0;
	while (key<1||key>2)
	{
		fflush(stdin);
		printf("\n1 = By name");
		printf("\n2 = By student id\n");
		printf("Choice = ");
		scanf("%d",&key);
	}
	return key;
}

void print_list(int keychoice)
{
	int cursor;
	if (keychoice==STUDENT_ID)
	{
		cursor = first_id;
		if (cursor==NULL)
		{
			printf("Sorry, empty list.  Nothing to list!\n");
			return;
		}
		display_record(cursor);
		while (students[cursor].next_id!=NULL)
		{
			cursor=students[cursor].next_id;
			display_record(cursor);
		}
	}
	else
	{
		cursor = first_name;
		if (cursor==NULL)
		{
			printf("Sorry, empty list.  Nothing to list!\n");
			return;
		}
		display_record(cursor);
		while (students[cursor].next_name!=NULL)
		{
			cursor=students[cursor].next_name;
			display_record(cursor);
		}
	}
}

int find_record(int keychoice)
{
	int cursor;
	int search_id;
	char search_name[80];
	fflush(stdin);

	if (keychoice==STUDENT_ID)
	{
		printf("Student id: ");
		scanf("%d",&search_id);
		cursor=first_id;
		cursor=find_by_id(cursor,search_id);
		return cursor;
	}
	else
	{
		printf("Student last name: ");
		gets(search_name);
		if (search_name[0]==NULL)
			strcpy(search_name,"   ");
		cursor=first_name;
		cursor=find_by_name(cursor,search_name);
		return cursor;
	}
}

int find_by_id(int cursor,int search_id)
{
		while (search_id!=students[cursor].student_id)
		{
			if (students[cursor].next_id==NULL)
			{
				prev=cursor;
				break;
			}
			prev=cursor;
			cursor=students[cursor].next_id;
		}
		if (students[cursor].next_id==NULL&&search_id!=students[cursor].student_id)
		{
			printf("\n Sorry, no record found with that ID\n");
			return NULL;
		}
		else
		{
			return cursor;
		}
}


int find_by_name(int cursor,char search_name[80])
{
		while (strcmpi(search_name,students[cursor].lastname))
		{
			if (students[cursor].next_name==NULL)
			{
				prev=cursor;
				break;
			}
			prev=cursor;
			cursor=students[cursor].next_name;
		}
		if (students[cursor].next_name==NULL&&strcmpi(search_name,students[cursor].lastname))
		{
			printf("\n Sorry, no record found with that ID\n");
			return NULL;
		}
		else
		{
			return cursor;
		}
}

/*
Simulates malloc.  I realize this does not check for exceeded memory
availablity.  This is done to reduce complexity for the assignment.

	If I did check, it would be when there were no freepointers and
	when the next available pointer was greater than 100...

*/

int mymalloc(void)
{
	int pointer;
	if (freepointers_cnt > 0)
	{
		pointer = freepointers[freepointers_cnt];
		freepointers_cnt--;
		return pointer;
	}
	else
	{
		pointer = nextpointer;
		nextpointer++;
		return pointer;
	}
}

/*
Places pointer into freepointers array for use by mymalloc
*/

void myfree(int student_pointer)
{
	freepointers_cnt++;
	freepointers[freepointers_cnt] = student_pointer;
}
