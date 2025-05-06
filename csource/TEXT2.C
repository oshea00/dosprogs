/* Note: Where I left off:
** Left off with being able to look at
** anything and move about freely.
** (next will be picking up objects and object manipulation).
** (next will be having objects affect the environment)
** (next will be adding story character objects)
** (next will be randomizing some of the responses).
** (next will be adding goal evaluation (is game done?)
**
*/


/*
** TEXT2.C - second gen derived from basic textdric.c
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

typedef struct	{
	char name[20];
	char descfile[12];
	char number[4];
	char size[4];
	char location[4];
	char value[4];
	char visible[2];
	char attach[2];
	char attachto[4];
	char sizelim[4];
	char character[2];
	char solved[2];
		} OBJREC;

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
	char number[4];
	char sizelim[4];
	char north[4];
	char south[4];
	char east[4];
	char west[4];
	char up[4];
	char down[4];
	char visible[2];
	char directo[4];
	char solved[2];
		} ROOMREC;

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

OBJREC Ob, *Obptr;
ROOMREC Rm, *Rmptr;
OBJECT object[100];
ROOM   room[100];
word cmd_list[10];
char shell_title[41];
int objcnt = 0;
int roomcnt = 0;
int currpos,cmdcnt,turncnt,waitcnt;

int main()
{
	int shell_process();
	delay(0);
	load_objects();
	load_rooms();
	turncnt = 0;
	waitcnt = 0;
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
	    };
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
	};
	cmdcnt = i;
	for (i=0;i < cmdcnt;i++)
		if (move_cmd(&cmd_list[i].cmd))
			{
			do_move(&i);
			delay(700);
			text_desc(room[currpos].descfile,10);
			list_objects(currpos);
	/* this is where you evaluate room solution */
			}
		else if (thank_you(&cmd_list[i].cmd))
			{
			message_window();
			cprintf("Your very welcome!");
			}
		else if (look_cmd(&cmd_list[i].cmd))
			{
			do_look(&i);
			}
		else if (hi_there(&cmd_list[i].cmd))
			{
			message_window();
			cprintf("Well, hello to you too!");
			};
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
		message_window();
		cprintf("Moving %s...",direction);
		delay(300);
		currpos = room[holdpos].north;
		cprintf(" The %s",room[currpos].name);
		}
	else
	if (!strcmpi(direction,"south"))
		{
		message_window();
		cprintf("Moving %s...",direction);
		delay(300);
		currpos = room[holdpos].south;
		cprintf(" The %s",room[currpos].name);
		}
	else
	if (!strcmpi(direction,"east"))
		{
		message_window();
		cprintf("Moving %s...",direction);
		delay(300);
		currpos = room[holdpos].east;
		cprintf(" The %s",room[currpos].name);
		}
	else
	if (!strcmpi(direction,"west"))
		{
		message_window();
		cprintf("Moving %s...",direction);
		delay(300);
		currpos = room[holdpos].west;
		cprintf(" The %s",room[currpos].name);
		}
	else
	if (!strcmpi(direction,"up"))
		{
		message_window();
		cprintf("Moving %s...",direction);
		delay(300);
		currpos = room[holdpos].up;
		cprintf(" The %s",room[currpos].name);
		}
	else
	if (!strcmpi(direction,"down"))
		{
		message_window();
		cprintf("Moving %s...",direction);
		delay(300);
		currpos = room[holdpos].down;
		cprintf(" The %s",room[currpos].name);
		}
	else
	       {
	       message_window();
	       cprintf("Move where?");
	       delay(1000);
	       };
}

do_look(int *i)
{
	char looking_at[21] = "";
	char trunc[21];
	int x;
	int obj = 0;
	if (cmdcnt > 1)
	    {
	    for (x=*i+1;x<cmdcnt;x++)
		{
		strcpy(looking_at,strncat(looking_at,&cmd_list[x],21));
		strcpy(looking_at,strncat(looking_at," ",1));
		};
	    strcpy(trunc,"");
	    strncat(trunc,looking_at,strlen(looking_at)-1);
	    strcpy(looking_at,trunc);
	    if (recognize_object(&looking_at,&obj))
		if (object[obj].location == currpos)
		    text_desc(object[obj].descfile,10);
		else
		    {
		    message_window();
		    cprintf("You can't see the %s here.",looking_at);
		    delay(1000);
		    text_desc(room[currpos].descfile,10);
		    list_objects(currpos);
		    }
	    else
		{
		message_window();
		cprintf("You can't see the %s here.",looking_at);
		delay(1000);
		text_desc(room[currpos].descfile,10);
		list_objects(currpos);
		};
	    }
	else
	    {
	    text_desc(room[currpos].descfile,10);
	    list_objects(currpos);
	    };
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
	OBJREC obj;
	int i = 0;
	if ((in = fopen("OBJFILE.TXT","r")) == NULL)
		return 1;
	else
	{
		while (!feof(in))
		  {
		    fgets(obj.name,21,in);
		    fgets(obj.descfile,13,in);
		    fgets(obj.number,5,in);
		    fgets(obj.size,5,in);
		    fgets(obj.location,5,in);
		    fgets(obj.value,5,in);
		    fgets(obj.visible,3,in);
		    fgets(obj.attach,3,in);
		    fgets(obj.attachto,5,in);
		    fgets(obj.sizelim,5,in);
		    fgets(obj.character,3,in);
		    fgets(obj.solved,3,in);
		    if (!feof(in))
			put_objrecord(&obj,&i);
		  };
		objcnt = i;
		fclose(in);
		return 0;
	};
}

put_objrecord(OBJREC *obj,int *i)
{
    strcpy(Ob.name,strtok(obj->name,"\n"));
    strcpy(Ob.descfile,strtok(obj->descfile,"\n"));
    strcpy(Ob.number,strtok(obj->number,"\n"));
    strcpy(Ob.size,strtok(obj->size,"\n"));
    strcpy(Ob.location,strtok(obj->location,"\n"));
    strcpy(Ob.value,strtok(obj->value,"\n"));
    strcpy(Ob.visible,strtok(obj->visible,"\n"));
    strcpy(Ob.attach,strtok(obj->attach,"\n"));
    strcpy(Ob.attachto,strtok(obj->attachto,"\n"));
    strcpy(Ob.sizelim,strtok(obj->sizelim,"\n"));
    strcpy(Ob.character,strtok(obj->character,"\n"));
    strcpy(Ob.solved,strtok(obj->solved,"\n"));
    strcpy(object[*i].name,Ob.name);
    strcpy(object[*i].descfile,Ob.descfile);
    object[*i].number = atoi(Ob.number);
    object[*i].size  = atoi(Ob.size);
    object[*i].location = atoi(Ob.location);
    object[*i].value = atoi(Ob.value);
    object[*i].visible = atoi(Ob.visible);
    object[*i].attach = atoi(Ob.attach);
    object[*i].attachto = atoi(Ob.attachto);
    object[*i].sizelim = atoi(Ob.sizelim);
    object[*i].character = atoi(Ob.character);
    object[*i].solved = atoi(Ob.solved);
    *i = *i + 1;
}

load_rooms()
{
	FILE *in;
	ROOMREC rm;
	int i = 0;
	if ((in = fopen("ROOMFILE.TXT","r")) == NULL)
		return 1;
	else
	{
		while (!feof(in))
		  {
		    fgets(rm.name,21,in);
		    fgets(rm.descfile,13,in);
		    fgets(rm.number,5,in);
		    fgets(rm.sizelim,5,in);
		    fgets(rm.north,5,in);
		    fgets(rm.south,5,in);
		    fgets(rm.east,5,in);
		    fgets(rm.west,5,in);
		    fgets(rm.up,5,in);
		    fgets(rm.down,5,in);
		    fgets(rm.visible,3,in);
		    fgets(rm.directo,5,in);
		    fgets(rm.solved,3,in);
		    if (!feof(in))
			put_rmrecord(&rm,&i);
		  };
		roomcnt = i;
		fclose(in);
		return 0;
	};
}

put_rmrecord(ROOMREC *rm,int *i)
{
    strcpy(Rm.name,strtok(rm->name,"\n"));
    strcpy(Rm.descfile,strtok(rm->descfile,"\n"));
    strcpy(Rm.number,strtok(rm->number,"\n"));
    strcpy(Rm.sizelim,strtok(rm->sizelim,"\n"));
    strcpy(Rm.north,strtok(rm->north,"\n"));
    strcpy(Rm.south,strtok(rm->south,"\n"));
    strcpy(Rm.east,strtok(rm->east,"\n"));
    strcpy(Rm.west,strtok(rm->west,"\n"));
    strcpy(Rm.up,strtok(rm->up,"\n"));
    strcpy(Rm.down,strtok(rm->down,"\n"));
    strcpy(Rm.visible,strtok(rm->visible,"\n"));
    strcpy(Rm.directo,strtok(rm->directo,"\n"));
    strcpy(Rm.solved,strtok(rm->solved,"\n"));
    strcpy(room[*i].name,Rm.name);
    strcpy(room[*i].descfile,Rm.descfile);
    room[*i].number = atoi(Rm.number);
    room[*i].sizelim  = atoi(Rm.sizelim);
    room[*i].north = atoi(Rm.north);
    room[*i].south = atoi(Rm.south);
    room[*i].east = atoi(Rm.east);
    room[*i].west = atoi(Rm.west);
    room[*i].up = atoi(Rm.up);
    room[*i].down = atoi(Rm.down);
    room[*i].visible = atoi(Rm.visible);
    room[*i].directo = atoi(Rm.directo);
    room[*i].solved = atoi(Rm.solved);
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

text_desc(char *filename,int limit)
{
	FILE *in;
	char textline[81];
	int y = 0;
	int ch;
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
			if (y < limit)
			    {
			    cprintf("%s",textline);
			    gotoxy(1,++y);
			    }
			else
			    {
			    cprintf(" -- Hit Enter For More -- ");
			    ch = getch();
			    clrscr();
			    y = wherey();
			    gotoxy(1,y++);
			    cprintf("%s",textline);
			    gotoxy(1,y);
			    };
			fgets(textline,81,in);
			};
		  };
		fclose(in);
		return 0;
	};
}

list_objects(int pos)
{
	int i,ch,y;
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
		    ch = getch();
		    clrscr();
		    y = wherey();
		    gotoxy(1,y++);
		    cprintf("%s",object[i].name);
		    gotoxy(1,y);
		    };
	};
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

recognize_object(char *ptr,int *objptr)
{
	int i;
	for (i=0;i<objcnt;i++)
		{
		if (!strcmpi(ptr,object[i].name))
			{
			*objptr = i;
			return 1;
			};
		};
	return 0;
}

