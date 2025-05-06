/* Note: Where I left off:
** Left off with being able to look at
** anything and move about freely.
** (next will be having objects affect the environment)
** (next will be adding story character objects)
** (next will be randomizing some of the responses).
** (next will be adding goal evaluation (is game done?)
**
*/


/*
** TEXTDRIV.C
**
** Mike O'Shea
**
** Used to run text adventure game objects created by WRITOBJ and WRITROOM.
**
**
*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define PLAYER 100
#define MAXGETSIZE 500
#define CLEAR 1
#define KEEP_SCREEN 0

typedef struct	{
	char name[20];
	char descfile[12];
	int number;
	int size;
	int location;
	int value;
	int visible;
	int attach;
	int attachto;
	int sizelim;
	int character;
	int solved;
		} OBJECT;

typedef struct	{
	char name[20];
	char descfile[12];
	int number;
	int sizelim;
	int north;
	int south;
	int east;
	int west;
	int up;
	int down;
	int visible;
	int directo;
	int solved;
		} ROOM;

typedef struct { char cmd[15]; } word;

OBJECT object[100];
ROOM   room[100];
word cmd_list[10];
char shell_title[41];
int objcnt = 0;
int roomcnt = 0;
int currpos;
int cmdcnt;

int main()
{
    int shell_process();
    delay(0);
    load_objects();
    load_rooms();
    currpos = room[0].number;
    reset_window();
    get_title();
    border(1,1,80,25,shell_title);
    box(1,20,80,25);
    message_window();
    cprintf(" Location: The %s",room[currpos].name);
    while (shell_process());
    system("cls");
    printf("Thanks for playing....");
    return 0;
}

shell_process()
{
    char command[121];
    int parse_command();
    put_prompt();
    getstrfld(4,21,&command);

    if (parse_command_loop(&command)==1)
	return 0;
    else
	return 1;
}

parse_command_loop(char *command)
{
    text_window();
    if (!strcmpi(command,"quit"))
	return 1;
    else
    {
	parse_command(command);
	put_prompt();
	return 0;
    }
}

parse_command(char *command)
{
    char *ptr;
    int i = 0;
    ptr = strtok(command," ");
    strcpy(cmd_list[i].cmd,ptr);
    while (ptr != NULL)
    {
	ptr = strtok(NULL," ");
	i++;
	if (ignore_word(ptr))
	    i--; /* ignoring these words */
	else
	    strcpy(cmd_list[i].cmd,ptr);
    }
    cmdcnt = i;

    for (i=0;i < cmdcnt;i++)
    {
	if (move_cmd(&cmd_list[i].cmd))
	{
	    do_move(&i);
	    display_text(700,KEEP_SCREEN,room[currpos].descfile);
	}
	else
	if (thank_you(&cmd_list[i].cmd))
	{
	    message_window();
	    cprintf("Your very welcome!");
	}
	else
	if (look_cmd(&cmd_list[i].cmd))
	{
	    do_look(&i);
	}
	else
	if (hi_there(&cmd_list[i].cmd))
	{
	    message_window();
	    cprintf("Well, hello to you too!");
	}
	else
	if (inventory(&cmd_list[i].cmd))
	{
	    do_inventory();
	}
	else
	if (get_cmd(&cmd_list[i].cmd))
	{
	    do_get(&i);
	}
	else
	if (drop_cmd(&cmd_list[i].cmd))
	{
	    do_drop(&i);
	}
    }
}

do_move(int *i)
{
    char direction[8];
    int holdpos;
    text_window();
    holdpos = currpos;
    if (!strcmpi(cmd_list[*i].cmd,"north")||
	!strcmpi(cmd_list[*i].cmd,"south")||
	!strcmpi(cmd_list[*i].cmd,"east")||
	!strcmpi(cmd_list[*i].cmd,"west")||
	!strcmpi(cmd_list[*i].cmd,"up")||
	!strcmpi(cmd_list[*i].cmd,"down"))
	/* in case of shorthand direction */
	    strcpy(direction,cmd_list[*i].cmd);
    else
	strcpy(direction,cmd_list[*i+1].cmd);

    if (!strcmpi(direction,"north"))
    {
	set_position(direction,&room[room[holdpos].north].visible,&room[holdpos].north);
    }
    else
    if (!strcmpi(direction,"south"))
    {
	set_position(direction,&room[room[holdpos].south].visible,&room[holdpos].south);
    }
    else
    if (!strcmpi(direction,"east"))
    {
	set_position(direction,&room[room[holdpos].east].visible,&room[holdpos].east);
    }
    else
    if (!strcmpi(direction,"west"))
    {
	set_position(direction,&room[room[holdpos].west].visible,&room[holdpos].west);
    }
    else
    if (!strcmpi(direction,"up"))
    {
	set_position(direction,&room[room[holdpos].up].visible,&room[holdpos].up);
    }
    else
    if (!strcmpi(direction,"down"))
    {
	set_position(direction,&room[room[holdpos].down].visible,&room[holdpos].down);
    }
    else
    {
	message_window();
	cprintf("Move where?");
	delay(1000);
    }
}

set_position(char *direction,int *visible,int *moveto)
{
    message_window();
    cprintf("Moving %s...",direction);
    delay(300);
    if (*visible == 1)
    {
	currpos = *moveto;
	cprintf(" The %s",room[currpos].name);
    }
}

do_look(int *i)
{
    char looking_at[21] = "";
    int obj = 0;
    if (cmdcnt > 1)
    {
	string_words(*i, looking_at);
	if (recognize_object(&looking_at,&obj))
	    if (object[obj].location == currpos)
		display_text(0,KEEP_SCREEN,object[obj].descfile);
	    else
	    {
		message_window();
		cprintf("You can't see the %s here.",looking_at);
		display_text(1000,KEEP_SCREEN,room[currpos].descfile);
	    }
	else
	{
	    message_window();
	    cprintf("You can't see the %s here.",looking_at);
	    display_text(1000,KEEP_SCREEN,room[currpos].descfile);
	}
    }
    else
    {
	display_text(0,KEEP_SCREEN,room[currpos].descfile);
    }
}

do_get(int *i)
{
    char getting[21] = "";
    int obj = 0;
    if (cmdcnt > 1)
    {
	string_words(*i,getting);
	if (recognize_object(&getting,&obj))
	    if (object[obj].location == currpos)
		if (object[obj].size <= MAXGETSIZE)
		{
		    message_window();
		    cprintf("You have the %s.",getting);
		    object[obj].location = 100;
		    display_text(1000,CLEAR,room[currpos].descfile);
		}
		else
		{
		    message_window();
		    cprintf("Unless your the hulk, you can't budge the %s.",getting);
		    display_text(1000,CLEAR,room[currpos].descfile);
		}
	    else
	    {
		message_window();
		cprintf("You can't get the %s here.",getting);
		display_text(1000,CLEAR,room[currpos].descfile);
	    }
	else
	{
	    message_window();
	    cprintf("You can't get a %s in this game!",getting);
	    display_text(1000,CLEAR,room[currpos].descfile);
	}
    }
    else
    {
	message_window();
	cprintf("What do you want to get?");
	display_text(1000,CLEAR,room[currpos].descfile);
    }
}

do_drop(int *i)
{
    char dropping[21] = "";
    int obj = 0;
    if (cmdcnt > 1)
    {
	string_words(*i,dropping);
	if (recognize_object(&dropping,&obj))
	    if (object[obj].location == PLAYER)
	    {
		message_window();
		cprintf("You dropped the %s.",dropping);
		object[obj].location = currpos;
		display_text(1000,CLEAR,room[currpos].descfile);
	    }
	    else
	    {
		message_window();
		cprintf("You don't have the %s.",dropping);
		display_text(1000,CLEAR,room[currpos].descfile);
	    }
	else
	{
	    message_window();
	    cprintf("You can't drop a %s in this game!",dropping);
	    display_text(1000,CLEAR,room[currpos].descfile);
	};
    }
    else
    {
	message_window();
	cprintf("What do you want to drop?");
	display_text(1000,CLEAR,room[currpos].descfile);
    };
}

do_inventory()
{
    int i,y;
    message_window();
    text_window();
    clrscr();
    cprintf("You have:\n");
    y = wherey();
    for (i=0;i<objcnt;i++)
    {
	if ((object[i].visible == 1) && (object[i].location == PLAYER))
	    if (y < 10)
	    {
		gotoxy(1,y);
		cprintf("%s",object[i].name);
		gotoxy(1,++y);
	    }
	    else
	    {
		cprintf(" -- Hit Enter For More -- ");
		getch();
		clrscr();
		y = wherey();
		gotoxy(1,y++);
		cprintf("%s",object[i].name);
		gotoxy(1,y);
	    }
    }
}

/*
** This routine accepts a delay time, clear screen flag, and filename
** for a desription file.  It is generic for room and object files
**
*/

display_text(int wait, int clear, char *filename)
{
    delay(wait);
    if (clear)
	clrscr();
    text_desc(filename);
    list_objects(currpos);
}

/*
** This routine accepts a number (i) which points to the next
** word in the command list and builds a single string of the
** following words seperated by a space.  This is usefull when
** objects with more than one word are referenced by an action
** command.  It returns the string in the passed char pointer
**
*/

string_words(int i, char *compound_word)
{
    int x;
    char trunc[21];

    for (x=i+1;x<cmdcnt;x++)
    {
	strcpy(compound_word,strncat(compound_word,&cmd_list[x],21));
	strcpy(compound_word,strncat(compound_word," ",1));
    }

    strcpy(trunc,"");
    strncat(trunc,compound_word,strlen(compound_word)-1);
    strcpy(compound_word,trunc);
}

put_prompt()
{
    command_window();
    placestr(2,21,"> ");
    gotoxy(4,21);
    clreol();
}

text_window()
{
    window(2,4,79,13);
}

message_window()
{
    window(2,14,79,19);
    gotoxy(2,14);
    clrscr();
}

command_window()
{
    window(2,21,79,24);
}

reset_window()
{
    window(1,1,80,25);
    textmode(C80);
    clrscr();
}

/*
** This creates a titled box by specifying the top left and bottom
** right coordinates and the title.
** Note: be sure that your coordinates do not exceed the
**       current text window, it will cause truncation.
**
**
*/

border(int a,int b,int c,int d,char *str)
{
    int i;
    for (i=2;i<c;i++) placechar(i,b,205);
    for (i=2;i<c;i++) placechar(i,d-1,205);
    for (i=2;i<d-1;i++) placechar(a,i,186);
    for (i=2;i<d-1;i++) placechar(c,i,186);
    placechar(a,b,201);
    placechar(c,b,187);
    placechar(a,d-1,200);
    placechar(c,d-1,188);
    placechar(a,b+2,204);
    placechar(c,b+2,185);
    for (i=2;i<c;i++) placechar(i,b+2,205);
    gotoxy((c-strlen(str))/2,b+1);
    cprintf("%s",str);
}

/* Same as border only without the Title option */

box(int a,int b,int c,int d)
{
    int i;
    for (i=2;i<c;i++) placechar(i,b,205);
    for (i=2;i<c;i++) placechar(i,d-1,205);
    for (i=2;i<d-1;i++) placechar(a,i,186);
    for (i=2;i<d-1;i++) placechar(c,i,186);
    placechar(a,b,201);
    placechar(c,b,187);
    placechar(a,d-1,200);
    placechar(c,d-1,188);
}

placechar(int a,int b,int c)
{
    gotoxy(a,b);
    cprintf("%c",c);
};

placestr(int a,int b,char *str)
{
    gotoxy(a,b);
    cprintf(str);
}

getstrfld(int a,int b,char *strin)
{
    gotoxy(a,b);
    gets(strin);
}


load_objects()
{
    FILE *in;
    char string[100];
    int i = 0;
    if ((in = fopen("OBJFILE.TXT","r")) == NULL)
	return 1;
    else
    {
	while (!feof(in))
	{
	    fgets(string,101,in);
	    if (!feof(in))
		put_objrecord(&string,&i);
	}
	objcnt = i;
	fclose(in);
	return 0;
    }
}

put_objrecord(char *string,int *i)
{
    strcpy(object[*i].name,strtok(string,";\n"));
    strcpy(object[*i].descfile,strtok(NULL,";\n"));
    object[*i].number = atoi(strtok(NULL,";\n"));
    object[*i].size  = atoi(strtok(NULL,";\n"));
    object[*i].location = atoi(strtok(NULL,";\n"));
    object[*i].value = atoi(strtok(NULL,";\n"));
    object[*i].visible = atoi(strtok(NULL,";\n"));
    object[*i].attach = atoi(strtok(NULL,";\n"));
    object[*i].attachto = atoi(strtok(NULL,";\n"));
    object[*i].sizelim = atoi(strtok(NULL,";\n"));
    object[*i].character = atoi(strtok(NULL,";\n"));
    object[*i].solved = atoi(strtok(NULL,";\n"));
    *i = *i + 1;
}

load_rooms()
{
    FILE *in;
    char string[100];
    int i = 0;
    if ((in = fopen("ROOMFILE.TXT","r")) == NULL)
	return 1;
    else
    {
	while (!feof(in))
	{
	    fgets(string,101,in);
	    if (!feof(in))
		put_rmrecord(&string,&i);
	}
	roomcnt = i;
	fclose(in);
	return 0;
    }
}

put_rmrecord(char *string,int *i)
{
    strcpy(room[*i].name,strtok(string,";\n"));
    strcpy(room[*i].descfile,strtok(NULL,";\n"));
    room[*i].number = atoi(strtok(NULL,";\n"));
    room[*i].sizelim  = atoi(strtok(NULL,";\n"));
    room[*i].north = atoi(strtok(NULL,";\n"));
    room[*i].south = atoi(strtok(NULL,";\n"));
    room[*i].east = atoi(strtok(NULL,";\n"));
    room[*i].west = atoi(strtok(NULL,";\n"));
    room[*i].up = atoi(strtok(NULL,";\n"));
    room[*i].down = atoi(strtok(NULL,";\n"));
    room[*i].visible = atoi(strtok(NULL,";\n"));
    room[*i].directo = atoi(strtok(NULL,";\n"));
    room[*i].solved = atoi(strtok(NULL,";\n"));
    *i = *i + 1;
}

get_title()
{
    FILE *title;
    if ((title = fopen("title.txt","r")) == NULL)
	strcpy(shell_title,"No Title");
    else
	fgets(shell_title,72,title);
    strcpy(shell_title,strtok(shell_title,"\n"));
    fclose(title);
}

text_desc(char *filename)
{
    FILE *in;
    char textline[81];
    int y = 0;
    text_window();
    clrscr();
    y = wherey();
    if ((in = fopen(filename,"r")) == NULL)
	return 1;
    else
    {
	fgets(textline,81,in);
	while (!feof(in))
	{
	    strcpy(textline,strtok(textline,"\n"));
	    if (!feof(in))
	    {
		if (y < 10)
		{
		    cprintf("%s",textline);
		    gotoxy(1,++y);
		}
		else
		{
		    cprintf(" -- Hit Enter For More -- ");
		    getch();
		    clrscr();
		    y = wherey();
		    gotoxy(1,y++);
		    cprintf("%s",textline);
		    gotoxy(1,y);
		}
		fgets(textline,81,in);
	    }
	}
	fclose(in);
	return 0;
    }
}

list_objects()
{
    int i,y;
    message_window();
    y = wherey();
    for (i=0;i<objcnt;i++)
    {
	if ((object[i].visible == 1) && (object[i].location == currpos))
	    if (y < 6)
	    {
		cprintf("%s",object[i].name);
		gotoxy(1,++y);
	    }
	    else
	    {
		cprintf(" -- Hit Enter For More -- ");
		getch();
		clrscr();
		y = wherey();
		gotoxy(1,y++);
		cprintf("%s",object[i].name);
		gotoxy(1,y);
	    }
    }
}

ignore_word(char *ptr)
{
    if(!strcmpi(ptr,"the")||
       !strcmpi(ptr,"at")||
       !strcmpi(ptr,"to")||
       !strcmpi(ptr,"and")||
       !strcmpi(ptr,"for")||
       !strcmpi(ptr,"here")||
       !strcmpi(ptr,"there")||
       !strcmpi(ptr,"please"))
	   return  1;
    else
	return  0;
}

move_cmd(char *ptr)
{
    if(!strcmpi(ptr,"move")||
	!strcmpi(ptr,"walk")||
	!strcmpi(ptr,"run")||
	!strcmpi(ptr,"jump")||
	!strcmpi(ptr,"climb")||
	!strcmpi(ptr,"step")||
	!strcmpi(ptr,"north")||
	!strcmpi(ptr,"south")||
	!strcmpi(ptr,"east")||
	!strcmpi(ptr,"west")||
	!strcmpi(ptr,"up")||
	!strcmpi(ptr,"down")||
	!strcmpi(ptr,"slither")||
	!strcmpi(ptr,"crawl"))
	    return 1;
    else
	return 0;
}

thank_you(char *ptr)
{
    if (!strcmpi(ptr,"thank")||
	!strcmpi(ptr,"thanks")||
	!strcmpi(ptr,"gracias")||
	!strcmpi(ptr,"fuck")||
	!strcmpi(ptr,"shit")||
	!strcmpi(ptr,"damn"))
	    return 1;
    else
	return 0;
}

hi_there(char *ptr)
{
    if (!strcmpi(ptr,"hi")||
	!strcmpi(ptr,"hello"))
	return 1;
    else
	return 0;
}

look_cmd(char *ptr)
{
    if (!strcmpi(ptr,"look")||
	!strcmpi(ptr,"watch")||
	!strcmpi(ptr,"find")||
	!strcmpi(ptr,"examine")||
	!strcmpi(ptr,"see"))
	    return 1;
    else
	return 0;
}

inventory(char *ptr)
{
    if (!strcmpi(ptr,"inv")||
	!strcmpi(ptr,"inventory")||
	!strcmpi(ptr,"have")||
	!strcmpi(ptr,"list"))
	    return 1;
    else
	return 0;
}

get_cmd(char *ptr)
{
    if (!strcmpi(ptr,"pick")||
	!strcmpi(ptr,"get")||
	!strcmpi(ptr,"grab")||
	!strcmpi(ptr,"take"))
	    return 1;
    else
	return 0;
}

drop_cmd(char *ptr)
{
    if (!strcmpi(ptr,"drop")||
	!strcmpi(ptr,"lose")||
	!strcmpi(ptr,"put"))
	    return 1;
    else
	return 0;
}

/*
** This routine accepts a pointer (ptr) to a string which is then
** compared to the object list.  If a match is found, the object
** number is passed back via the *objptr
**
*/

recognize_object(char *ptr,int *objptr)
{
    int i;
    for (i=0;i<objcnt;i++)
    {
	if (!strcmpi(ptr,object[i].name))
	{
	    *objptr = object[i].number;
	    return 1;
	}
    }
    return 0;
}


