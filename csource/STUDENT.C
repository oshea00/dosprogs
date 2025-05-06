/*
**
**  STUDENT.C   data structures in "C" course Thursday night.
**
**  First assignment.
**
**  Michael O'Shea 393-6495
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
	struct student *next_name;
	struct student *next_id;
};


struct student *first_name=NULL;
struct student *first_id=NULL;
struct student *prev;
struct student *curr;

void main(void);
void add_student(void);
void update_student(void);
void delete_student(void);
void find_student(void);
struct student *find_by_name(struct student *,char []);
struct student *find_by_id(struct student *,int);
void list_students(void);
void add_to_list(struct student*);
int get_menu_choice(void);
int get_choice(void);
void display_menu(void);
void display_record(struct student*);
void print_list(int);
struct student *enter_record(void);
int keytype(void);
struct student *find_record(int);
void fill_record(struct student*);

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

void display_record(struct student *curr)
{
	printf("\n***  ***  ***  ***  ***  ***");
	printf("\nID       : %d",curr->student_id);
	printf("\nLASTNAME : %s",curr->lastname);
	printf("\nFIRSTNAME: %s",curr->firstname);
	printf("\nMAJOR    : %s",curr->major);
	printf("\nAGE      : %d",curr->age);
	printf("\nPHONE    : %s",curr->phone);
	printf("\n");
}

struct student *enter_record(void)
{
	struct student *new;
	static int id_number=0;
	id_number++;
	fflush(stdin);

	/* Allocate new record space */

	new = (struct student *) malloc(sizeof(struct student));
	new->next_name = NULL;
	new->next_id = NULL;

	if (first_id==NULL)
	{
		first_id=new;
		first_name=new;
	}
	new->student_id = id_number;
	printf("\n*** Enter New Record ***");
	fill_record(new);
	return new;
}

void fill_record(struct student *new)
{
	char input[81];

	fflush(stdin);

	/* Fill Fields */
	printf("\nLast name: ");
	gets(new->lastname);
	new->lastname[20]=NULL;
	printf("First name: ");
	gets(new->firstname);
	new->firstname[15]=NULL;
	printf("Major: ");
	gets(new->major);
	new->major[4]=NULL;
	printf("Age: ");
	gets(input);
	new->age = atoi(input);
	printf("Phone: ");
	gets(new->phone);
	new->phone[8]=NULL;
}

void add_to_list(struct student *new)
{
	/* Insert new record by name */

	curr = first_name;
	prev = curr;
	if (new!=curr)  /* check for intialized first node */
	{
		while (strcmpi(new->lastname,curr->lastname)>0&&curr->next_name!=NULL)
		{
			/* go to next node */
			prev = curr;
			curr = curr->next_name;
		}
		if (curr==first_name)  /* first node stopped on */
		{
			if (strcmpi(curr->lastname,new->lastname)>0)
			{
				new->next_name=curr;
				first_name=new;
			}
			else
			{
				new->next_name=curr->next_name;
				curr->next_name=new;
			}
		}
		else
		{
			if (curr->next_name!=NULL)  /* middle node stopped on */
			{
				new->next_name=curr;
				prev->next_name=new;
			}
			else   /* end node stopped on */
			{
				if (strcmpi(new->lastname,curr->lastname)>0)
					curr->next_name=new;
				else
				{
					prev->next_name=new;
					new->next_name=curr;
				}
			}
		}
	}

	/* Insert new record by student id */

	curr = first_id;
	prev = curr;
	if (new!=curr)  /* check for initialized first node */
	{
		while (new->student_id>curr->student_id&&curr->next_id!=NULL)
		{
			prev = curr;
			curr = curr->next_id;
		}
		if (curr==first_id)   /* stopped on first node */
		{
			if (curr->student_id>new->student_id)
			{
				new->next_id=curr;
				first_id=new;
			}
			else
			{
				new->next_id=curr->next_id;
				curr->next_id=new;
			}
		}
		else
		{
			if (curr->next_id!=NULL)  /* middle node stopped on */
			{
				new->next_id=curr;
				prev->next_id=new;
			}
			else    /* end node stopped on */
			{
				if (new->student_id>curr->student_id)
					curr->next_id=new;
				else
				{
					prev->next_id=new;
					new->next_id=curr;
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
	struct student *found;
	printf("\n Find Student... \n");
	if ((found=find_record(keytype()))==NULL)
	{}  /* no action */
	else
		display_record(found);
}
void update_student(void)
{
	struct student *found;
	char input[80];
	printf("\n Update Student... \n");
	if ((found=find_record(keytype()))==NULL)
	{}  /* no action */
	else
	{
		display_record(found);
		strcpy(input,found->lastname);
		printf("Re-enter information:\n");
		fill_record(found);
		if (strcmpi(input,found->lastname))
		{
			printf("You must delete and re-enter the record if you\n");
			printf("want to alter the last name.  The record will be\n");
			printf("saved with the other updates intact.\n");
			strcpy(found->lastname,input);
		}
	}
}

void delete_student(void)
{
	struct student *foundname;
	struct student *foundid;
	struct student *prevname;
	struct student *previd;
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
			strcpy(find_name,foundid->lastname);
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
					first_name=foundname->next_name;
				}
				else
				{
					prevname->next_name=foundname->next_name;
				}

				if (first_id==foundid)
				{
					first_id=foundid->next_id;
				}
				else
				{
					previd->next_id=foundid->next_id;
				}
				free(foundid);
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
			find_id=foundname->student_id;
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
					first_name=foundname->next_name;
				}
				else
				{
					prevname->next_name=foundname->next_name;
				}

				if (first_id==foundid)
				{
					first_id=foundid->next_id;
				}
				else
				{
					previd->next_id=foundid->next_id;
				}
				free(foundid);
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
	struct student *hold;
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
	struct student *cursor;
	if (keychoice==STUDENT_ID)
	{
		cursor = first_id;
		if (cursor==NULL)
		{
			printf("Sorry, empty list.  Nothing to list!\n");
			return;
		}
		display_record(cursor);
		while (cursor->next_id!=NULL)
		{
			cursor=cursor->next_id;
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
		while (cursor->next_name!=NULL)
		{
			cursor=cursor->next_name;
			display_record(cursor);
		}
	}
}

struct student *find_record(int keychoice)
{
	struct student *cursor;
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

struct student *find_by_id(struct student *cursor,int search_id)
{
		while (search_id!=cursor->student_id)
		{
			if (cursor->next_id==NULL)
			{
				prev=cursor;
				break;
			}
			prev=cursor;
			cursor=cursor->next_id;
		}
		if (cursor->next_id==NULL&&search_id!=cursor->student_id)
		{
			printf("\n Sorry, no record found with that ID\n");
			return NULL;
		}
		else
		{
			return cursor;
		}
}


struct student *find_by_name(struct student *cursor,char search_name[80])
{
		while (strcmpi(search_name,cursor->lastname))
		{
			if (cursor->next_name==NULL)
			{
				prev=cursor;
				break;
			}
			prev=cursor;
			cursor=cursor->next_name;
		}
		if (cursor->next_name==NULL&&strcmpi(search_name,cursor->lastname))
		{
			printf("\n Sorry, no record found with that ID\n");
			return NULL;
		}
		else
		{
			return cursor;
		}
}

